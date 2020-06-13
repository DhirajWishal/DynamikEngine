// Copyright 2020 Dhiraj Wishal
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _DYNAMIK_VULKAN_SWAP_CHAIN_H
#define _DYNAMIK_VULKAN_SWAP_CHAIN_H

/*
 Author:    Dhiraj Wishal
 Date:      25/05/2020
*/
#include "../Common/VulkanDevice.h"
#include "../Common/VulkanQueue.h"
#include "../Common/VulkanViewport.h"

#include "../Primitives/VulkanImageView.h"

namespace Dynamik
{
    namespace Backend
    {
        struct DMK_API VulkanSwapChainSupportDetails {
            VkSurfaceCapabilitiesKHR capabilities = {};
            ARRAY<VkSurfaceFormatKHR> formats = {};
            ARRAY<VkPresentModeKHR> presentModes = {};
        };

        /*
         Vulkan Swap Chain for the Dynamik RBL
        */
        class DMK_API VulkanSwapChain {
        public:
            VulkanSwapChain() {}
            ~VulkanSwapChain() {}

            static VulkanSwapChainSupportDetails querySwapChainSupport(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface);

            void initialize(const VulkanDevice& vDevice, const VulkanQueue& vQueue, VulkanViewport vViewport);
            void terminate(const VulkanDevice& vDevice);

            operator VkSwapchainKHR() const;

            ARRAY<VulkanImage> images;
            ARRAY<VulkanImageView> imageViews;
            VkSwapchainKHR swapChain = VK_NULL_HANDLE;
            VkFormat format = VK_FORMAT_UNDEFINED;
            VkExtent2D extent = { 0, 0 };
            VulkanViewport myViewport;

        private:
            void _initializeImageViews(const VulkanDevice& vDevice);
        };
    }
}

#endif // !_DYNAMIK_VULKAN_SWAP_CHAIN_H
