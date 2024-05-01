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

#ifndef UFO_MATH_UTIL_H
#define UFO_MATH_UTIL_H

// STL
#include <type_traits>

namespace ufo::math
{
template <typename T>
constexpr int sgn(T val) noexcept
{
	return (T(0) < val) - (val < T(0));
}

template <typename T>
constexpr T degToRad(T deg) noexcept
{
	return deg * M_PI / T(180);
}

template <typename T>
constexpr T radToDeg(T rad) noexcept
{
	return rad * T(180) / M_PI;
}

template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
constexpr T ipow(T base, int exp)
{
	if (0 == exp) {
		return 0 <= base ? T(1) : T(-1);
	}

	bool positive = 0 < exp;
	exp = 0 < exp ? exp : -exp;

	T result = base;
	while (--exp) {
		result *= base;
	}

	return positive ? result : T(1) / result;
}

template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
constexpr T ipow(T base, std::size_t exp)
{
	if (0 == exp) {
		return 1;
	}

	T result = base;
	while (--exp) {
		result *= base;
	}
	return result;
}

}  // namespace ufo::math

#endif  // UFO_MATH_UTIL_H