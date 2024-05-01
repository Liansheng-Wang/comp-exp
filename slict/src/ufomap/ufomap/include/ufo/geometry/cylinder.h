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

#ifndef UFO_GEOMETRY_CYLINDER_H
#define UFO_GEOMETRY_CYLINDER_H

#include <ufo/geometry/point.h>

namespace ufo::geometry
{
struct Cylinder {
	Point start;
	Point end;
	float radius = 0;

	constexpr Cylinder() noexcept = default;

	constexpr Cylinder(Point const& start, Point const& end, float radius) noexcept
	    : start(start), end(end), radius(radius)
	{
	}

	constexpr bool operator==(Cylinder const& rhs) const noexcept
	{
		return rhs.start == start && rhs.end == end && rhs.radius == radius;
	}

	constexpr bool operator!=(Cylinder const& rhs) const noexcept
	{
		return !(*this == rhs);
	}

	constexpr Point min() const noexcept
	{
		// FIXME: Make correct
		return Point(std::min(start.x, end.x) - radius, std::min(start.y, end.y) - radius,
		             std::min(start.z, end.z) - radius);
	}

	constexpr Point max() const noexcept
	{
		// FIXME: Make correct
		return Point(std::max(start.x, end.x) + radius, std::max(start.y, end.y) + radius,
		             std::max(start.z, end.z) + radius);
	}
};
}  // namespace ufo::geometry

#endif  // UFO_GEOMETRY_CYLINDER_H
