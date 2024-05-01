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

#ifndef UFO_MAP_OCCUPANCY_NODE_H
#define UFO_MAP_OCCUPANCY_NODE_H

// UFO
#include <ufo/map/color/color_node.h>
#include <ufo/map/octree/octree_node.h>
#include <ufo/map/semantic/semantic_node.h>
#include <ufo/map/types.h>

// STL
#include <cstdint>
#include <iostream>
#include <type_traits>

namespace ufo::map
{
template <typename OccupancyType = float>
struct OccupancyNode {
	OccupancyType occupancy;

	constexpr bool operator==(OccupancyNode const& rhs) const
	{
		return rhs.occupancy == occupancy;
	}

	constexpr bool operator!=(OccupancyNode const& rhs) const { return !(*this == rhs); }
};

using OccupancyNodeSmall = OccupancyNode<uint8_t>;

struct OccupancyTimeNode {
	time_step_t occupancy : 8;
	time_step_t time_step : 24;

	constexpr bool operator==(OccupancyTimeNode const& rhs) const
	{
		return rhs.occupancy == occupancy && rhs.time_step == time_step;
	}

	constexpr bool operator!=(OccupancyTimeNode const& rhs) const
	{
		return !(*this == rhs);
	}
};

template <typename OccupancyType>
struct OccupancyColorNode : OccupancyNode<OccupancyType>, ColorNode {
	constexpr bool operator==(OccupancyColorNode const& rhs) const
	{
		return OccupancyNode<OccupancyType>::operator==(rhs) && ColorNode::operator==(rhs);
	}

	constexpr bool operator!=(OccupancyColorNode const& rhs) const
	{
		return !(*this == rhs);
	}
};

template <typename OccupancyType, typename SemanticType, size_t SemanticValueWidth>
struct OccupancySemanticNode : SemanticNode<SemanticType, SemanticValueWidth>,
                               OccupancyNode<OccupancyType> {
	constexpr bool operator==(OccupancySemanticNode const& rhs) const
	{
		return OccupancyNode<OccupancyType>::operator==(rhs) &&
		       SemanticNode<SemanticType, SemanticValueWidth>::operator==(rhs);
	}

	constexpr bool operator!=(OccupancySemanticNode const& rhs) const
	{
		return !(*this == rhs);
	}
};

struct OccupancyTimeColorNode : OccupancyTimeNode, ColorNode {
	constexpr bool operator==(OccupancyTimeColorNode const& rhs) const
	{
		return OccupancyTimeNode::operator==(rhs) && ColorNode::operator==(rhs);
	}

	constexpr bool operator!=(OccupancyTimeColorNode const& rhs) const
	{
		return !(*this == rhs);
	}
};

template <typename SemanticType, size_t SemanticValueWidth>
struct OccupancyTimeSemanticNode : SemanticNode<SemanticType, SemanticValueWidth>,
                                   OccupancyTimeNode {
	constexpr bool operator==(OccupancyTimeSemanticNode const& rhs) const
	{
		return OccupancyTimeNode::operator==(rhs) &&
		       SemanticNode<SemanticType, SemanticValueWidth>::operator==(rhs);
	}

	constexpr bool operator!=(OccupancyTimeSemanticNode const& rhs) const
	{
		return !(*this == rhs);
	}
};

template <typename OccupancyType, typename SemanticType, size_t SemanticValueWidth>
struct OccupancyColorSemanticNode : SemanticNode<SemanticType, SemanticValueWidth>,
                                    OccupancyColorNode<OccupancyType> {
	constexpr bool operator==(OccupancyColorSemanticNode const& rhs) const
	{
		return OccupancyColorNode<OccupancyType>::operator==(rhs) &&
		       SemanticNode<SemanticType, SemanticValueWidth>::operator==(rhs);
	}

	constexpr bool operator!=(OccupancyColorSemanticNode const& rhs) const
	{
		return !(*this == rhs);
	}
};

template <typename SemanticType, size_t SemanticValueWidth>
struct OccupancyTimeColorSemanticNode : SemanticNode<SemanticType, SemanticValueWidth>,
                                        OccupancyTimeColorNode {
	constexpr bool operator==(OccupancyTimeColorSemanticNode const& rhs) const
	{
		return OccupancyTimeColorNode::operator==(rhs) &&
		       SemanticNode<SemanticType, SemanticValueWidth>::operator==(rhs);
	}

	constexpr bool operator!=(OccupancyTimeColorSemanticNode const& rhs) const
	{
		return !(*this == rhs);
	}
};
}  // namespace ufo::map

#endif  // UFO_MAP_OCCUPANCY_NODE_H