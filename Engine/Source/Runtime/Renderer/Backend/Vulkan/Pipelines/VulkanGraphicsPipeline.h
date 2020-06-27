// Copyright 2020 Dhiraj Wishal
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _DYNAMIK_VULKAN_GRAPHICS_PIPELINE_H
#define _DYNAMIK_VULKAN_GRAPHICS_PIPELINE_H

/*
 Author:	Dhiraj Wishal
 Date:		17/06/2020
*/
#include "Renderer/Components/RPipelineObject.h"
#include "Core/Types/Array.h"
#include <vulkan/vulkan.h>

namespace Dynamik
{
	namespace Backend
	{
		/*
		 Vulkan Graphics Pipeline
		*/
		class DMK_API VulkanGraphicsPipeline : public RPipelineObject {
			struct DMK_API VDescriptor {
				VkDescriptorSetLayout layout = VK_NULL_HANDLE;
				VkDescriptorPool pool = VK_NULL_HANDLE;
				VkDescriptorSet set = VK_NULL_HANDLE;
			};

		public:
			VulkanGraphicsPipeline() {}
			~VulkanGraphicsPipeline() {}

			virtual void initialize(RCoreObject* pCoreObject, RPipelineCreateInfo createInfo, RPipelineUsage usage) override final;
			virtual void terminate(RCoreObject* pCoreObject) override final;

			operator VkPipelineLayout() const;
			operator VkPipeline() const;

			ARRAY<VDescriptor> descriptors;

			VkPipelineLayout layout = VK_NULL_HANDLE;
			VkPipeline pipeline = VK_NULL_HANDLE;
		};
	}
}

#endif // !_DYNAMIK_VULKAN_GRAPHICS_PIPELINE_H