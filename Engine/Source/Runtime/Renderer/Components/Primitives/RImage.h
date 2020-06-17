// Copyright 2020 Dhiraj Wishal
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _DYNAMIK_RENDERER_IMAGE_H
#define _DYNAMIK_RENDERER_IMAGE_H

/*
 Author:	Dhiraj Wishal
 Date:		14/06/2020
*/
#include "Core/Object/Resource/Texture.h"
#include "RImageView.h"
#include "../PrimitiveTypeDefs.h"
#include "Core/Math/MathTypes.h"

namespace Dynamik
{
	class DMK_API RCoreObject;
	class DMK_API RBuffer;

	/*
	 Renderer Image Create Info
	*/
	struct DMK_API RImageCreateInfo {
		VEC3F vDimentions = VEC3F(0.0f, 0.0f, 1.0f);
		DMKTextureType imageType = DMKTextureType::DMK_TEXTURE_TYPE_2D;
		RImageUsage imageUsage = RImageUsage::IMAGE_USAGE_RENDER;
		UI32 layers = 1;
		UI32 mipLevels = 1;
		DMKFormat imageFormat = DMKFormat::DMK_FORMAT_UNDEFINED;
		RResourceMemoryType memoryType = RResourceMemoryType::RESOURCE_MEMORY_TYPE_HOST_ACCESSABLE;
		DMKSampleCount sampleCount = DMKSampleCount::DMK_SAMPLE_COUNT_1_BIT;
	};

	/*
	 Renderer Image
	*/
	class DMK_API RImage {
	public:
		RImage() {}
		virtual ~RImage() {}

		virtual void initialize(POINTER<RCoreObject> pCoreObject, RImageCreateInfo createInfo) = 0;
		virtual void copyBuffer(POINTER<RCoreObject> pCoreObject, POINTER<RBuffer> pBuffer) = 0;
		virtual void generateMipMaps(POINTER<RCoreObject> pCoreObject) = 0;
		virtual void setLayout(POINTER<RCoreObject> pCoreObject, RImageLayout newLayout) = 0;
		virtual void createImageView(POINTER<RCoreObject> pCoreObject, DMKTexture::TextureSwizzles swizzles) = 0;
		virtual void terminate(POINTER<RCoreObject> pCoreObject) = 0;

		virtual void setData(POINTER<RCoreObject> pCoreObject, UI64 uSize, UI64 offset, VPTR data) = 0;
		/*
		 Automatically maps memory
		*/
		virtual VPTR getData(POINTER<RCoreObject> pCoreObject, UI64 uSize, UI64 offset) { return nullptr; }
		virtual void unmapMemory(POINTER<RCoreObject> pCoreObject) = 0;

		VEC3F extent = VEC3F(0.0f);
		POINTER<RImageView> pImageView;
		DMKTextureType type = DMKTextureType::DMK_TEXTURE_TYPE_2D;
		RImageUsage usage = RImageUsage::IMAGE_USAGE_RENDER;
		UI32 size = 0;
		UI32 availabeMipLevels = 0;
		UI32 mipLevel = 0;
		UI32 layers = 0;
		DMKFormat format = DMKFormat::DMK_FORMAT_UNDEFINED;
		RImageLayout layout = RImageLayout::IMAGE_LAYOUT_UNDEFINED;
	};
}

#endif // !_DYNAMIK_RENDERER_IMAGE_H
