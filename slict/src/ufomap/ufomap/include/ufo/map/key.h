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

#ifndef UFO_MAP_KEY_H
#define UFO_MAP_KEY_H

// UFO
#include <ufo/map/types.h>

// STL
#include <immintrin.h>

#include <cstddef>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace ufo::map
{
/*!
 * @brief A key represent an octree index at a specified depth
 *
 */
class Key
{
 public:
	using key_t = uint32_t;

	constexpr Key() = default;

	constexpr Key(key_t const x, key_t const y, key_t const z, depth_t const depth)
	    : x_((x >> depth) << depth),
	      y_((y >> depth) << depth),
	      z_((z >> depth) << depth),
	      depth_(depth)
	{
	}

	constexpr Key(Key const key, depth_t const depth)
	    : x_((key.x_ >> depth) << depth),
	      y_((key.y_ >> depth) << depth),
	      z_((key.z_ >> depth) << depth),
	      depth_(depth)
	{
	}

	friend constexpr bool operator==(Key const lhs, Key const rhs) noexcept
	{
		return lhs.x_ == rhs.x_ && lhs.y_ == rhs.y_ && lhs.z_ == rhs.z_ &&
		       lhs.depth_ == rhs.depth_;
	}

	friend constexpr bool operator!=(Key const lhs, Key const rhs) noexcept
	{
		return !(lhs == rhs);
	}

	friend constexpr bool operator<(Key const lhs, Key const rhs) noexcept
	{
		return lhs.x_ < rhs.x_ || (lhs.x_ == rhs.x_ && lhs.y_ < rhs.y_) ||
		       (lhs.x_ == rhs.x_ && lhs.y_ == rhs.y_ && lhs.z_ < rhs.z_);
	}

	friend constexpr bool operator<=(Key const lhs, Key const rhs) noexcept
	{
		return lhs.x_ < rhs.x_ || (lhs.x_ == rhs.x_ && lhs.y_ < rhs.y_) ||
		       (lhs.x_ == rhs.x_ && lhs.y_ == rhs.y_ && lhs.z_ <= rhs.z_);
	}

	friend constexpr bool operator>(Key const lhs, Key const rhs) noexcept
	{
		return lhs.x_ > rhs.x_ || (lhs.x_ == rhs.x_ && lhs.y_ > rhs.y_) ||
		       (lhs.x_ == rhs.x_ && lhs.y_ == rhs.y_ && lhs.z_ > rhs.z_);
	}

	friend constexpr bool operator>=(Key const lhs, Key const rhs) noexcept
	{
		return lhs.x_ > rhs.x_ || (lhs.x_ == rhs.x_ && lhs.y_ > rhs.y_) ||
		       (lhs.x_ == rhs.x_ && lhs.y_ == rhs.y_ && lhs.z_ >= rhs.z_);
	}

	/*!
	 * @brief Get the depth of the key.
	 *
	 * @return The depth the key.
	 */
	constexpr depth_t depth() const noexcept { return depth_; }

	/*!
	 * @brief Change the depth of the key.
	 *
	 * @note This will change the x, y, z components of the key.
	 */
	constexpr void setDepth(depth_t depth) noexcept
	{
		x_ = (x_ >> depth) << depth;
		y_ = (y_ >> depth) << depth;
		z_ = (z_ >> depth) << depth;
		depth_ = depth;
	}

	constexpr key_t operator[](std::size_t idx) const { return *(&x_ + idx); }

	constexpr key_t& operator[](std::size_t idx) { return *(&x_ + idx); }

	/*!
	 * @brief Returns the x component of the key
	 *
	 * @return const key_t& The x component of the key
	 */
	constexpr key_t x() const noexcept { return x_; }

	/*!
	 * @brief Returns the y component of the key
	 *
	 * @return const key_t& The y component of the key
	 */
	constexpr key_t y() const noexcept { return y_; }

	/*!
	 * @brief Returns the z component of the key
	 *
	 * @return const key_t& The z component of the key
	 */
	constexpr key_t z() const noexcept { return z_; }

	/*!
	 * @brief Returns the x component of the key
	 *
	 * @return key_t& The x component of the key
	 */
	constexpr key_t& x() noexcept { return x_; }

	/*!
	 * @brief Returns the y component of the key
	 *
	 * @return key_t& The y component of the key
	 */
	constexpr key_t& y() noexcept { return y_; }

	/*!
	 * @brief Returns the z component of the key
	 *
	 * @return key_t& The z component of the key
	 */
	constexpr key_t& z() noexcept { return z_; }

	/*!
	 * @brief
	 *
	 */
	struct Hash {
		std::size_t operator()(Key const& key) const
		{
#if defined(__BMI2__)
			return _pdep_u64(key[0], 0x9249249249249249) |
			       _pdep_u64(key[1], 0x2492492492492492) |
			       _pdep_u64(key[2], 0x4924924924924924);
#else
			return splitBy3(key[0]) | (splitBy3(key[1]) << 1) | (splitBy3(key[2]) << 2);
#endif
		}
	};

 private:
	static uint64_t splitBy3(key_t a)
	{
#if defined(__BMI2__)
		return _pdep_u64(a, 0x9249249249249249);
#else
		uint64_t code = static_cast<uint64_t>(a) & 0x1fffff;
		code = (code | code << 32) & 0x1f00000000ffff;
		code = (code | code << 16) & 0x1f0000ff0000ff;
		code = (code | code << 8) & 0x100f00f00f00f00f;
		code = (code | code << 4) & 0x10c30c30c30c30c3;
		code = (code | code << 2) & 0x1249249249249249;
		return code;
#endif
	}

 private:
	// The key
	key_t x_ = 0;
	key_t y_ = 0;
	key_t z_ = 0;
	// The depth of the key
	depth_t depth_ = 0;
};

using KeySet = std::unordered_set<Key, Key::Hash>;
template <typename T>
using KeyMap = std::unordered_map<Key, T, Key::Hash>;

using KeyRay = std::vector<Key>;
}  // namespace ufo::map

#endif  // UFO_MAP_KEY_H