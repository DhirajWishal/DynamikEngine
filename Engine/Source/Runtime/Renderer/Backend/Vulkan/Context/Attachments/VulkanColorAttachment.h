#pragma once
#ifndef _DYNAMIK_VULKAN_COLOR_ATTACHMENT_H
#define _DYNAMIK_VULKAN_COLOR_ATTACHMENT_H

/*
 Author:    Dhiraj Wishal
 Date:      28/05/2020
*/
#include "../VulkanFrameBufferAttachment.h"

namespace Dynamik
{
    namespace Backend
    {
        /*
         Vulkan Color Attachment for the Dynamik RBL
        */
        class DMK_API VulkanColorAttachment : public VulkanFrameBufferAttachment {
        public:
            VulkanColorAttachment() {}
            ~VulkanColorAttachment() {}

            void initialize(const VulkanDevice& vDevice, const VulkanQueue& vQueue, VulkanFrameBufferAttachmentInitInfo initInfo) override;
        };
    }
}

#endif // !_DYNAMIK_VULKAN_COLOR_ATTACHMENT_H
