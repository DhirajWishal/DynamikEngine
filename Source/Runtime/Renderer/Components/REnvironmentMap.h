// Copyright 2020 Dhiraj Wishal
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _DYNAMIK_RENDERER_ENVIRONMENT_MAP_H
#define _DYNAMIK_RENDERER_ENVIRONMENT_MAP_H

#include "RCoreObject.h"
#include "REntity.h"

namespace Dynamik
{
	/*
	 Renderer Environment Map
	 This object contains all the necessary information needed to render the environment map provided by
	 the client application. 
	*/
	class DMK_API REnvironmentMap {
	public:
		REnvironmentMap() = default;
		virtual ~REnvironmentMap() = default;

		/* TODO */
		//virtual void createBRDF(RCoreObject* pCoreObject, UI32 dimensions, DMKFormat format) = 0;
		//virtual void createIrradianceCube(RCoreObject* pCoreObject, UI32 dimensions, DMKFormat format) = 0;
		//virtual void createPrefilteredCube(RCoreObject* pCoreObject, UI32 dimensions, DMKFormat format) = 0;

	public:		/* Vertex and Index Data */
		UI64 vertexBufferOffset = 0;
		UI64 indexBufferOffset = 0;

	public:		/* Resource Data */
		RTexture* pTexture = nullptr;
		RPipelineObject* pPipeline = nullptr;
		RBuffer* pUniformBuffer = nullptr;

	public:		/* Parent Mesh Component*/
		DMKMeshComponent* pMeshComponent = nullptr;
	};
}

#endif // !_DYNAMIK_RENDERER_ENVIRONMENT_MAP_H
