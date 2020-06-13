// Copyright 2020 Dhiraj Wishal
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _DYNAMIK_VULKAN_UTILITIES_H
#define _DYNAMIK_VULKAN_UTILITIES_H

/*
 Author:    Dhiraj Wishal
 Date:      25/05/2020
*/
#include "VulkanDevice.h"
#include "Object/Resource/ShaderModule.h"
#include "Object/Resource/Primitives.h"

namespace Dynamik
{
    namespace Backend 
    {
        /*
         Vulkan Utilities for the Dynamik RBL
        */
        class DMK_API VulkanUtilities {
            VulkanUtilities() {}
            ~VulkanUtilities() {}

        public:
            static VkFormat getVulkanFormat(DMKFormat format);
            static UI32 findMemoryType(UI32 typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice);
            static B1 hasStencilComponent(const VkFormat& format);
            static VkFormat findSupportedFormat(const ARRAY<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features, VkPhysicalDevice physicalDevice);
            static VkFormat findDepthFormat(const VkPhysicalDevice& physicalDevice);

            static VkDescriptorType getDescriptorType(DMKUniformType type);
            static VkShaderStageFlagBits getShaderStage(DMKShaderLocation location);
            static ARRAY<VkDescriptorSetLayoutBinding> getDescriptorSetLayoutBindings(DMKUniformBufferDescriptor descriptor);
            static ARRAY<VkDescriptorSetLayoutBinding> getDescriptorSetLayoutBindings(ARRAY<DMKShaderModule> modules);
            static ARRAY<VkDescriptorPoolSize> getDescriptorPoolSizes(DMKUniformBufferDescriptor descriptor, UI32 descriptorCount);
        
            static ARRAY<VkVertexInputAttributeDescription> VulkanUtilities::getVertexAttributeDescriptions(DMKVertexBufferDescriptor descriptor);
            static VkFormat VulkanUtilities::vertexAttributeTypeToVkFormat(DMKDataType type);
        };
    }
}

#endif // !_DYNAMIK_VULKAN_UTILITIES_H
