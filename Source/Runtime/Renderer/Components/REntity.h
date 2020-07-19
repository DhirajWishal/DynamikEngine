// Copyright 2020 Dhiraj Wishal
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _DYNAMIK_RENDERER_ENTITY_H
#define _DYNAMIK_RENDERER_ENTITY_H

#include "RMeshObject.h"

namespace Dynamik
{
	/*
	 Renderer Entity
	*/
	class DMK_API REntity {
	public:
		REntity() = default;
		virtual ~REntity() = default;

		ARRAY<RMeshObject> meshObjects;
	};
}

#endif // !_DYNAMIK_RENDERER_ENTITY_H
