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

#ifndef UFO_MAP_POINT_H
#define UFO_MAP_POINT_H

// UFO
#include <ufo/map/color/color.h>
#include <ufo/map/semantic/semantic.h>
#include <ufo/math/vector3.h>

// STL

namespace ufo::map
{
using coord_t = float;
using Point3 = math::Vector3<coord_t>;

struct Point3Color : public Point3, public RGBColor {
	constexpr Point3Color() = default;

	constexpr Point3Color(Point3 const& point, RGBColor const& color = RGBColor())
	    : Point3(point), RGBColor(color)
	{
	}

	constexpr Point3Color(Point3 const& point, RGBColorType red, RGBColorType green,
	                      RGBColorType blue)
	    : Point3(point), RGBColor(red, green, blue)
	{
	}

	constexpr Point3Color(coord_t x, coord_t y, coord_t z,
	                      RGBColor const& color = RGBColor())
	    : Point3(x, y, z), RGBColor(color)
	{
	}

	constexpr Point3Color(coord_t x, coord_t y, coord_t z, RGBColorType red,
	                      RGBColorType green, RGBColorType blue)
	    : Point3(x, y, z), RGBColor(red, green, blue)
	{
	}
};

struct Point3Semantic : public Point3, public SemanticPair {
	constexpr Point3Semantic() = default;

	constexpr Point3Semantic(Point3 const& point,
	                         SemanticPair const& semantic = SemanticPair())
	    : Point3(point), SemanticPair(semantic)
	{
	}

	constexpr Point3Semantic(Point3 const& point, semantic_label_t label = 0,
	                         semantic_value_t value = 0)
	    : Point3(point), SemanticPair(label, value)
	{
	}

	constexpr Point3Semantic(coord_t x, coord_t y, coord_t z,
	                         SemanticPair const& semantic = SemanticPair())
	    : Point3(x, y, z), SemanticPair(semantic)
	{
	}

	constexpr Point3Semantic(coord_t x, coord_t y, coord_t z, semantic_label_t label = 0,
	                         semantic_value_t value = 0)
	    : Point3(x, y, z), SemanticPair(label, value)
	{
	}
};

struct Point3ColorSemantic : public Point3Color, public SemanticPair {
	constexpr Point3ColorSemantic() = default;

	constexpr Point3ColorSemantic(Point3 const& point, RGBColor const& color = RGBColor(),
	                              SemanticPair const& semantic = SemanticPair())
	    : Point3Color(point, color), SemanticPair(semantic)
	{
	}

	constexpr Point3ColorSemantic(Point3 const& point, RGBColorType red, RGBColorType green,
	                              RGBColorType blue, semantic_label_t label = 0,
	                              semantic_value_t value = 0)
	    : Point3Color(point, red, green, blue), SemanticPair(label, value)
	{
	}

	constexpr Point3ColorSemantic(coord_t x, coord_t y, coord_t z,
	                              RGBColor const& color = RGBColor(),
	                              SemanticPair const& semantic = SemanticPair())
	    : Point3Color(x, y, z, color), SemanticPair(semantic)
	{
	}

	constexpr Point3ColorSemantic(coord_t x, coord_t y, coord_t z, RGBColorType red,
	                              RGBColorType green, RGBColorType blue,
	                              semantic_label_t label = 0, semantic_value_t value = 0)
	    : Point3Color(x, y, z, red, green, blue), SemanticPair(label, value)
	{
	}
};

}  // namespace ufo::map

#endif  // UFO_MAP_POINT_H