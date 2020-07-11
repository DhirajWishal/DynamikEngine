// Copyright 2020 Dhiraj Wishal
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _DYNAMIK_ANIMATED_MESH_COMPONENT_H
#define _DYNAMIK_ANIMATED_MESH_COMPONENT_H

#include "Objects/Animation.h"
#include "Objects/AnimNodeGraph.h"
#include "Core/Components/RenderableComponents/StaticMeshComponent.h"

namespace Dynamik
{
	/*
	 Dynamik Animated Mesh Component

	 This component stores all the relevant data to store an animation.
	 This component contains,
	 - Skinned Mesh (DMKStaticMeshComponent)
	 - Bone Mesh (DMKAnimation)

	 An animated mesh component is of 3 sub components.
	 - mesh components (static)
	 - node graph (skeletons/ bones)
	 - animations
	*/
	class DMK_API DMKAnimatedMeshComponent {
	public:
		DMKAnimatedMeshComponent() {}
		~DMKAnimatedMeshComponent() {}

		/* Skinned Mesh Component */
		DMKStaticMeshComponent skinnedMesh;

		/* Animations */
		ARRAY<DMKAnimation> animations;

		/* Animation Node Map */
		std::unordered_map<STRING, UI32> nodeMap;

		/* Animation Node Information */
		ARRAY<AAnimNodeInfo> nodeInfos;

		/* Animation Node Data */
		ARRAY<AAnimNodeData> nodeData;

		/* Animation Node Graph */
		DMKAnimNodeGraph nodeGraph;
	};
}

#endif // !_DYNAMIK_ANIMATED_MESH_COMPONENT_H