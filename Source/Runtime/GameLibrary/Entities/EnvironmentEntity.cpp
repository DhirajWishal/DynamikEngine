// Copyright 2020 Dhiraj Wishal
// SPDX-License-Identifier: Apache-2.0

#include "dmkafx.h"
#include "EnvironmentEntity.h"

#include "../Utilities/MeshFactory.h"

namespace Dynamik
{
	void DMKEnvironmentEntity::addShaderModule(const DMKShaderModule& sModule)
	{
		shaders.pushBack(sModule);
	}

	void DMKEnvironmentEntity::setShaderModules(ARRAY<DMKShaderModule> sModules)
	{
		shaders = std::move(sModules);
	}

	DMKShaderModule* DMKEnvironmentEntity::getShaderModule(I32 index)
	{
		return shaders.location(index);
	}
	
	ARRAY<DMKShaderModule> DMKEnvironmentEntity::getShaders()
	{
		return shaders;
	}
}