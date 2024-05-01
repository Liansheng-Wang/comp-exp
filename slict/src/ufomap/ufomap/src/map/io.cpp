/*!
 * UFOMap: An Efficient Probabilistic 3D Mapping Framework That Embraces the Unknown
 *
 * @author Daniel Duberg (dduberg@kth.se)
 * @see https://github.com/UnknownFreeOccupied/ufomap
 * @version 1.0
 * @date 2022-05-13
 *
 * @copyright Copyright (c) 2022, Daniel Duberg, KTH Royal Institute of Technology
 *
 * BSD 3-Clause License
 *
 * Copyright (c) 2022, Daniel Duberg, KTH Royal Institute of Technology
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *     list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// UFO
#include <ufo/map/io.h>

// STL
#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>

// LZ4 compression
#include <lz4.h>
#include <lz4hc.h>

namespace ufo::map
{
bool isUFOMapFile(std::filesystem::path const& filename)
{
	std::ifstream file;
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	file.imbue(std::locale());
	file.open(filename, std::ios_base::in | std::ios_base::binary);

	return isUFOMapFile(file);
}

bool isUFOMapFile(std::istream& in_stream)
{
	std::string line;
	std::getline(in_stream, line);
	return 0 == line.compare(0, FileHeader::FILE_HEADER.length(), FileHeader::FILE_HEADER);
}

FileHeader readHeader(std::filesystem::path const& filename)
{
	std::ifstream file;
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	file.imbue(std::locale());
	file.open(filename, std::ios_base::in | std::ios_base::binary);

	return readHeader(file);
}

FileHeader readHeader(std::istream& in_stream)
{
	if (!isUFOMapFile(in_stream)) {
		throw std::runtime_error("Trying to read non-UFOMap file");
	}

	FileHeader header;
	in_stream.read(reinterpret_cast<char*>(&header.major), sizeof(header.major));
	in_stream.read(reinterpret_cast<char*>(&header.minor), sizeof(header.minor));
	in_stream.read(reinterpret_cast<char*>(&header.patch), sizeof(header.patch));

	uint8_t compressed;
	in_stream.read(reinterpret_cast<char*>(&compressed), sizeof(compressed));
	header.compressed = compressed & 1U;

	in_stream.read(reinterpret_cast<char*>(&header.resolution), sizeof(header.resolution));
	in_stream.read(reinterpret_cast<char*>(&header.depth_levels),
	               sizeof(header.depth_levels));

	return header;
}

void writeHeader(std::ostream& out_stream, FileOptions const& options)
{
	out_stream << FileHeader::FILE_HEADER << '\n';
	out_stream.write(reinterpret_cast<char const*>(&FileHeader::CURRENT_MAJOR),
	                 sizeof(FileHeader::CURRENT_MAJOR));
	out_stream.write(reinterpret_cast<char const*>(&FileHeader::CURRENT_MINOR),
	                 sizeof(FileHeader::CURRENT_MINOR));
	out_stream.write(reinterpret_cast<char const*>(&FileHeader::CURRENT_PATCH),
	                 sizeof(FileHeader::CURRENT_PATCH));

	uint8_t compressed = options.compressed ? 1U : 0U;
	out_stream.write(reinterpret_cast<char const*>(&compressed), sizeof(compressed));

	out_stream.write(reinterpret_cast<char const*>(&options.resolution),
	                 sizeof(options.resolution));
	out_stream.write(reinterpret_cast<char const*>(&options.depth_levels),
	                 sizeof(options.depth_levels));
}

bool compressData(std::istream& in_stream, std::ostream& out_stream,
                  uint64_t uncompressed_data_size, int acceleration_level,
                  int compression_level)
{
	static constexpr uint64_t max = std::numeric_limits<int32_t>::max();

	uint64_t begin = 0;
	uint64_t end = std::min(max, uncompressed_data_size);
	int const max_dst_size = LZ4_compressBound(end);
	auto data = std::make_unique<char[]>(end);
	auto compressed_data = std::make_unique<char[]>(max_dst_size);
	while (begin != end) {
		int32_t num_data = end - begin;

		// Compress data
		in_stream.read(data.get(), num_data);

		int32_t compressed_data_size =
		    (0 >= compression_level)
		        ? LZ4_compress_fast(data.get(), compressed_data.get(), num_data, max_dst_size,
		                            acceleration_level)
		        : LZ4_compress_HC(data.get(), compressed_data.get(), num_data, max_dst_size,
		                          compression_level);

		// Check if compression successful
		if (0 <= compressed_data_size) {
			// Write amount of data to output stream
			out_stream.write(reinterpret_cast<char*>(&compressed_data_size),
			                 sizeof(compressed_data_size));
			// Write compressed data to output stream
			out_stream.write(compressed_data.get(), compressed_data_size);
		} else {
			return false;
		}

		// Move to next
		begin = end;
		uint64_t const diff = uncompressed_data_size - end;
		end += std::min(max, diff);
	}

	return true;
}

bool decompressData(std::istream& in_stream, std::ostream& out_stream,
                    uint64_t uncompressed_data_size)
{
	auto regen_buffer = std::make_unique<char[]>(uncompressed_data_size);
	size_t cur = 0;
	while (in_stream.good() && cur < uncompressed_data_size) {
		// Get size of compressed data
		int32_t compressed_data_size;
		in_stream.read(reinterpret_cast<char*>(&compressed_data_size),
		               sizeof(compressed_data_size));

		// Decompress data
		auto compressed_data = std::make_unique<char[]>(compressed_data_size);
		in_stream.read(compressed_data.get(), compressed_data_size);
		int const decompressed_size =
		    LZ4_decompress_safe(compressed_data.get(), regen_buffer.get() + cur,
		                        compressed_data_size, uncompressed_data_size - cur);
		cur += decompressed_size;

		// Check if decompression successful
		if (0 > decompressed_size) {
			return false;
		}
	}

	// Write decompressed data to output stream
	out_stream.write(regen_buffer.get(), uncompressed_data_size);

	return !in_stream.fail();
}

bool decompressData(std::istream& in_stream, std::ostream& out_stream,
                    uint64_t uncompressed_data_size, uint64_t& compressed_data_size)
{
	auto regen_buffer = std::make_unique<char[]>(uncompressed_data_size);
	size_t cur = 0;
	while (in_stream.good() && cur < uncompressed_data_size) {
		// Get size of compressed data
		int32_t cur_compressed_data_size;
		in_stream.read(reinterpret_cast<char*>(&cur_compressed_data_size),
		               sizeof(cur_compressed_data_size));

		compressed_data_size += cur_compressed_data_size;

		// Decompress data
		auto compressed_data = std::make_unique<char[]>(cur_compressed_data_size);
		in_stream.read(compressed_data.get(), cur_compressed_data_size);
		int const decompressed_size =
		    LZ4_decompress_safe(compressed_data.get(), regen_buffer.get() + cur,
		                        cur_compressed_data_size, uncompressed_data_size - cur);
		cur += decompressed_size;

		// Check if decompression successful
		if (0 > decompressed_size) {
			return false;
		}
	}

	// Write decompressed data to output stream
	out_stream.write(regen_buffer.get(), uncompressed_data_size);

	return !in_stream.fail();
}
}  // namespace ufo::map
