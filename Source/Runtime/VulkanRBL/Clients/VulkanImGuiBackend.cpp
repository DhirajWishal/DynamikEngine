// Copyright 2020 Dhiraj Wishal
// SPDX-License-Identifier: Apache-2.0

#include "dmkafx.h"
#include "VulkanImGuiBackend.h"

#include "../Primitives/VulkanTexture.h"
#include "../VulkanUtilities.h"
#include "../Common/VulkanCommandBuffer.h"
#include "../Common/VulkanCommandBufferManager.h"

#include "Services/RuntimeSystems/AssetRegistry.h"
#include "Tools/Shader/GLSL/Compiler.h"
#include "Renderer/RUtilities.h"
#include "Core/Types/StaticArray.h"

#include <imgui.h>

namespace Dynamik
{
	namespace Backend
	{
		std::mutex __globalLock;

		void VulkanImGuiBackend::initialize()
		{
			/* Initialize Font Data */
			_initializeFontTexture();

			/* Initialize Pipeline */
			_initializePipeline();

			/* Initialize Command Buffers */
			_initializeCommandBuffers();

			/* Allocate Buffers */
			pVertexBuffer = StaticAllocator<VulkanBuffer>::rawAllocate();
			pIndexBuffer = StaticAllocator<VulkanBuffer>::rawAllocate();
		}

		void VulkanImGuiBackend::update(ImDrawData* pDrawData)
		{
			/* Check if the vertex buffer is already allocated. */
			if (!pVertexBuffer)
			{
				DMK_ERROR("The vertex buffer is not initialized! Make sure that you have called the initialize() method.");
				return;
			}

			/* Check if the index buffer is already allocated. */
			if (!pIndexBuffer)
			{
				DMK_ERROR("The index buffer is not initialized! Make sure that you have called the initialize() method.");
				return;
			}

			UI64 vertexBufferSize = pDrawData->TotalVtxCount * sizeof(ImDrawVert);
			UI64 indexBufferSize = pDrawData->TotalIdxCount * sizeof(ImDrawIdx);

			/* Return if there is no vertex or index buffer size. */
			if ((!vertexBufferSize) || (!indexBufferSize))
				return;

			/* Re create the vertex buffer if the vertex count was changed. */
			if (vertexCount != pDrawData->TotalVtxCount)
			{
				if (Inherit<VulkanBuffer>(pVertexBuffer)->buffer != VK_NULL_HANDLE)
					pVertexBuffer->terminate(pCoreObject);

				pVertexBuffer->initialize(pCoreObject, RBufferType::BUFFER_TYPE_VERTEX, vertexBufferSize);
				vertexCount = pDrawData->TotalVtxCount;
			}

			/* Re create the index buffer if the index count was changed. */
			if (indexCount != pDrawData->TotalIdxCount)
			{
				if (Inherit<VulkanBuffer>(pIndexBuffer)->buffer != VK_NULL_HANDLE)
					pIndexBuffer->terminate(pCoreObject);

				pIndexBuffer->initialize(pCoreObject, RBufferType::BUFFER_TYPE_INDEX, indexBufferSize);
				indexCount = pDrawData->TotalIdxCount;
			}

			/* Push data to the buffers */
			{
				/* Push data to the vertex buffer */
				{
					ImDrawVert* vtxDst = Cast<ImDrawVert*>(pVertexBuffer->getData(pCoreObject, vertexBufferSize, 0));

					if (vtxDst)
					{
						for (I32 index = 0; index < pDrawData->CmdListsCount; index++)
						{
							const ImDrawList* pDrawList = pDrawData->CmdLists[index];
							DMKMemoryFunctions::copyData(vtxDst, pDrawList->VtxBuffer.Data, pDrawList->VtxBuffer.size() * sizeof(ImDrawVert));
							vtxDst += pDrawList->VtxBuffer.size();
						}

						pVertexBuffer->unmapMemory(pCoreObject);
					}
					else
						DMK_INFO("Could not map!");
				}

				/* Push data to the index buffer */
				{
					ImDrawIdx* idxDst = Cast<ImDrawIdx*>(pIndexBuffer->getData(pCoreObject, indexBufferSize, 0));

					if (idxDst)
					{
						for (I32 index = 0; index < pDrawData->CmdListsCount; index++)
						{
							const ImDrawList* pDrawList = pDrawData->CmdLists[index];
							DMKMemoryFunctions::copyData(idxDst, pDrawList->IdxBuffer.Data, pDrawList->IdxBuffer.size() * sizeof(ImDrawIdx));
							idxDst += pDrawList->IdxBuffer.size();
						}

						pIndexBuffer->unmapMemory(pCoreObject);
					}
					else
						DMK_INFO("Could not map!");
				}
			}
		}

		void VulkanImGuiBackend::drawFrame()
		{
		}

		void VulkanImGuiBackend::terminate()
		{
			/* Terminate the vertex buffer */
			if (pVertexBuffer)
			{
				pVertexBuffer->terminate(pCoreObject);
				StaticAllocator<VulkanBuffer>::rawDeallocate(pVertexBuffer);
			}

			/* Terminate the index buffer */
			if (pIndexBuffer)
			{
				pIndexBuffer->terminate(pCoreObject);
				StaticAllocator<VulkanBuffer>::rawDeallocate(pIndexBuffer);
			}

			/* Terminate the pipeline */
			pPipelineObject->terminate(pCoreObject);
			StaticAllocator<VulkanGraphicsPipeline>::rawDeallocate(pPipelineObject);

			/* Terminate the Font Texture */
			pFontTexture->terminate(pCoreObject);
			StaticAllocator<VulkanTexture>::rawDeallocate(pFontTexture);

			/* Terminate command buffers */
			pCommandBufferManager->terminate(pCoreObject, pCommandBuffers);
			StaticAllocator<VulkanCommandBufferManager>::rawDeallocate(pCommandBufferManager);
		}

		void VulkanImGuiBackend::bindCommands(RCommandBuffer* pCommandBuffer)
		{
			if (!Inherit<VulkanBuffer>(pVertexBuffer)->buffer || !Inherit<VulkanBuffer>(pIndexBuffer)->buffer)
				return;

			ImGuiIO& io = ImGui::GetIO();

			auto context = ImGui::GetCurrentContext();

			VulkanCommandBuffer vCommandBuffer = *Inherit<VulkanCommandBuffer>(pCommandBuffer);

			vkCmdBindDescriptorSets(vCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, Inherit<VulkanGraphicsPipeline>(pPipelineObject)->layout, 0, 1, &Inherit<VulkanGraphicsPipeline>(pPipelineObject)->descriptor.set, 0, nullptr);
			vkCmdBindPipeline(vCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, Inherit<VulkanGraphicsPipeline>(pPipelineObject)->pipeline);

			VkViewport viewport = {};
			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.maxDepth = 1.0f;
			viewport.minDepth = 0.0f;
			viewport.width = Cast<F32>(ImGui::GetIO().DisplaySize.x);
			viewport.height = Cast<F32>(ImGui::GetIO().DisplaySize.y);
			vkCmdSetViewport(vCommandBuffer, 0, 1, &viewport);

			/* Submit constant data. */
			uniformData.scale = Vector2F(2.0f / io.DisplaySize.x, 2.0f / io.DisplaySize.y);
			uniformData.transform[0] = -1.0f - pDrawData->DisplayPos.x * uniformData.scale[0];
			uniformData.transform[1] = -1.0f - pDrawData->DisplayPos.y * uniformData.scale[1];
			vkCmdPushConstants(vCommandBuffer, Inherit<VulkanGraphicsPipeline>(pPipelineObject)->layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(uniformData), &uniformData);

			UI64 vertexOffset = 0;
			UI64 indexOffset = 0;

			/* Wait till pDrawData is submitted with draw data. */
			while (pDrawData == nullptr);
			while (pDrawData->CmdListsCount < 1);

			__globalLock.lock();

			ImVec2 clip_off = pDrawData->DisplayPos;         // (0,0) unless using multi-viewports
			ImVec2 clip_scale = pDrawData->FramebufferScale; // (1,1) unless using retina display which are often (2,2)

			StaticArray<VkDeviceSize, 1> offsets;

			vkCmdBindVertexBuffers(vCommandBuffer, 0, 1, &Inherit<VulkanBuffer>(pVertexBuffer)->buffer, offsets.data());
			vkCmdBindIndexBuffer(vCommandBuffer, Inherit<VulkanBuffer>(pIndexBuffer)->buffer, 0, sizeof(ImDrawIdx) == 2 ? VK_INDEX_TYPE_UINT16 : VK_INDEX_TYPE_UINT32);

			for (I32 index = 0; index < pDrawData->CmdListsCount; index++)
			{
				const ImDrawList* pDrawList = pDrawData->CmdLists[index];

				for (I32 itr = 0; itr < pDrawList->CmdBuffer.Size; itr++)
				{
					const ImDrawCmd* pDrawCmd = &pDrawList->CmdBuffer[itr];

					ImVec4 clip_rect;
					clip_rect.x = (pDrawCmd->ClipRect.x - clip_off.x) * clip_scale.x;
					clip_rect.y = (pDrawCmd->ClipRect.y - clip_off.y) * clip_scale.y;
					clip_rect.z = (pDrawCmd->ClipRect.z - clip_off.x) * clip_scale.x;
					clip_rect.w = (pDrawCmd->ClipRect.w - clip_off.y) * clip_scale.y;

					if (clip_rect.x < 0.0f)
						clip_rect.x = 0.0f;
					if (clip_rect.y < 0.0f)
						clip_rect.y = 0.0f;

					VkRect2D scissor = {};
					scissor.offset.x = Cast<I32>(clip_rect.x);
					scissor.offset.y = Cast<I32>(clip_rect.y);
					scissor.extent.width = Cast<I32>(clip_rect.z - clip_rect.x);
					scissor.extent.height = Cast<I32>(clip_rect.w - clip_rect.y);

					vkCmdSetScissor(vCommandBuffer, 0, 1, &scissor);
					vkCmdDrawIndexed(vCommandBuffer, pDrawCmd->ElemCount, 1, Cast<UI32>(indexOffset), Cast<UI32>(vertexOffset), 0);
					indexOffset += pDrawCmd->ElemCount;
				}

				vertexOffset += pDrawList->VtxBuffer.Size;
			}

			__globalLock.unlock();
		}

		void VulkanImGuiBackend::reCreatePipeline(RRenderTarget* pRenderTarget, DMKViewport viewport)
		{
			if (pPipelineObject)
				pPipelineObject->reCreate(pCoreObject, pRenderTarget, viewport);
		}

		void VulkanImGuiBackend::updateResources()
		{
		}

		void VulkanImGuiBackend::onRendererUpdate(const UI64 activeFrameIndex, RSwapChain* pSwapChain, RCommandBuffer* pActiveCommandBuffer)
		{
			/* Update resources prior to drawing. */
			updateResources();

			/* Bind draw calls */
			VkCommandBufferBeginInfo cmdBufferBeginInfo = {};
			cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			DMK_VULKAN_ASSERT(vkBeginCommandBuffer(Inherit<VulkanCommandBuffer>(pActiveCommandBuffer)->buffer, &cmdBufferBeginInfo), "Failed to begin recording of command buffer!");

			pActiveCommandBuffer->bindRenderTarget(pRenderTarget, pSwapChain, Cast<UI32>(activeFrameIndex), RSubpassContentType::SUBPASS_CONTENT_TYPE_SECONDARY_COMMAND_BUFFER);

			VkCommandBufferInheritanceInfo inheritanceInfo = {};
			inheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
			inheritanceInfo.pNext = VK_NULL_HANDLE;
			inheritanceInfo.renderPass = Inherit<VulkanRenderPass>(pRenderTarget->pRenderPass)->renderPass;
			inheritanceInfo.framebuffer = Inherit<VulkanFrameBuffer>(pRenderTarget->pFrameBuffer)->buffers[activeFrameIndex];

			/* Bind commands */
			{
				auto pCurrentCommandBuffer = pCommandBuffers[activeFrameIndex];

				/* Begin command buffer */
				VkCommandBufferBeginInfo beginInfo = {};
				beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				beginInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT | VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
				beginInfo.pInheritanceInfo = &inheritanceInfo;
				DMK_VULKAN_ASSERT(vkBeginCommandBuffer(Inherit<VulkanCommandBuffer>(pCurrentCommandBuffer)->buffer, &beginInfo), "Failed to begin recording of command buffer!");

				/* Bind the commands */
				bindCommands(pCurrentCommandBuffer);

				/* End the command buffer */
				DMK_VULKAN_ASSERT(vkEndCommandBuffer(Inherit<VulkanCommandBuffer>(pCurrentCommandBuffer)->buffer), "Failed to end the command buffer recording!");

				/* Execute commands */
				vkCmdExecuteCommands(Inherit<VulkanCommandBuffer>(pActiveCommandBuffer)->buffer, 1, &Inherit<VulkanCommandBuffer>(pCurrentCommandBuffer)->buffer);
			}

			pActiveCommandBuffer->unbindRenderTarget();

			pActiveCommandBuffer->end();
		}

		void VulkanImGuiBackend::_initializeFontTexture()
		{
			ImGuiIO& io = ImGui::GetIO();

			UCHR* fontData = nullptr;
			I32 width = 0, height = 0;
			io.Fonts->GetTexDataAsRGBA32(&fontData, &width, &height);
			VkDeviceSize uploadSize = Cast<I64>(width) * Cast<I64>(height) * Cast<I64>(4) * Cast<I64>(sizeof(CHR));

			pFontTexture = StaticAllocator<VulkanTexture>::rawAllocate();

			RImageCreateInfo imageCreateInfo = {};
			imageCreateInfo.vDimentions.width = Cast<F32>(width);
			imageCreateInfo.vDimentions.height = Cast<F32>(height);
			imageCreateInfo.vDimentions.depth = 1.0f;
			imageCreateInfo.mipLevels = 1;
			imageCreateInfo.layers = 1;
			imageCreateInfo.sampleCount = DMKSampleCount::DMK_SAMPLE_COUNT_1_BIT;
			imageCreateInfo.imageUsage = RImageUsage(RImageUsage::IMAGE_USAGE_RENDER | RImageUsage::IMAGE_USAGE_TRANSFER_DST);
			imageCreateInfo.imageFormat = DMKFormat::DMK_FORMAT_RGBA_8_UNORMAL;

			pFontTexture->pImage = StaticAllocator<VulkanImage>::rawAllocate();
			pFontTexture->pImage->initialize(pCoreObject, imageCreateInfo);

			pFontTexture->pImage->createImageView(pCoreObject, DMKTexture::TextureSwizzles());

			VulkanUtilities::copyDataToImage(pCoreObject, pFontTexture->pImage, fontData, Cast<UI64>(uploadSize));

			pFontTexture->makeRenderable(pCoreObject);

			pFontTexture->pSampler = StaticAllocator<VulkanImageSampler>::rawAllocate();
			pFontTexture->pSampler->initialize(pCoreObject, RImageSamplerCreateInfo::createDefaultSampler(0.0f));
		}

		void VulkanImGuiBackend::_initializePipeline()
		{
			Tools::GLSLCompiler compiler;

			auto shaderVS = compiler.getSPIRV(DMKAssetRegistry::getAsset(TEXT("SHADER_IM_GUI_UI_VERT")), DMKShaderLocation::DMK_SHADER_LOCATION_VERTEX);

			shaderVS.addInputAttribute(DMKShaderInputAttribute(DMKFormat::DMK_FORMAT_RG_32_SF32, 1));
			shaderVS.addInputAttribute(DMKShaderInputAttribute(DMKFormat::DMK_FORMAT_RG_32_SF32, 1));
			shaderVS.addInputAttribute(DMKShaderInputAttribute(DMKFormat::DMK_FORMAT_RGBA_8_UNORMAL, 1));

			shaders.pushBack(shaderVS);
			shaders.pushBack(compiler.getSPIRV(DMKAssetRegistry::getAsset(TEXT("SHADER_IM_GUI_UI_FRAG")), DMKShaderLocation::DMK_SHADER_LOCATION_FRAGMENT));

			/* Create uniform buffers */

			RPipelineSpecification pipelineSpec = {};
			pipelineSpec.shaders = shaders;
			pipelineSpec.dynamicStates = { RDynamicState::DYNAMIC_STATE_VIEWPORT, RDynamicState::DYNAMIC_STATE_SCISSOR };
			pipelineSpec.scissorInfos.resize(1);
			pipelineSpec.colorBlendInfo.blendStates = { createColorBlendState() };
			pipelineSpec.multiSamplingInfo.sampleCount = pCoreObject->sampleCount;
			pipelineSpec.rasterizerInfo.cullMode = RCullMode::CULL_MODE_NONE;
			pipelineSpec.depthStencilInfo.compareOp = RStencilCompareOp::STENCIL_COMPARE_OP_LESS_OR_EQUAL;

			pPipelineObject = StaticAllocator<VulkanGraphicsPipeline>::rawAllocate();
			pPipelineObject->initialize(pCoreObject, pipelineSpec, RPipelineUsage::PIPELINE_USAGE_GRAPHICS, pRenderTarget, DMKViewport());

			pPipelineObject->initializeResources(pCoreObject, {}, { pFontTexture });
		}

		void VulkanImGuiBackend::_initializeCommandBuffers()
		{
			pCommandBufferManager = StaticAllocator<VulkanCommandBufferManager>::allocate();
			pCommandBufferManager->initialize(pCoreObject);

			if (pCommandBuffers.size())
				pCommandBufferManager->resetBuffers(pCoreObject, pCommandBuffers);
			else
				pCommandBuffers = pCommandBufferManager->allocateCommandBuffers(pCoreObject, Cast<UI32>(Inherit<VulkanFrameBuffer>(pRenderTarget->pFrameBuffer)->buffers.size()), RCommandBufferLevel::COMMAND_BUFFEER_LEVEL_SECONDARY);
		}

		RColorBlendState VulkanImGuiBackend::createColorBlendState()
		{
			RColorBlendState blendState = {};
			blendState.enable = true;
			blendState.colorWriteMask = RColorComponent(RColorComponent::COLOR_COMPONENT_R_BIT | RColorComponent::COLOR_COMPONENT_G_BIT | RColorComponent::COLOR_COMPONENT_B_BIT | RColorComponent::COLOR_COMPONENT_A_BIT);
			blendState.srcColorBlendFactor = RColorBlendFactor::COLOR_BLEND_FACTOR_SRC_ALPHA;
			blendState.dstColorBlendFactor = RColorBlendFactor::COLOR_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			blendState.colorBlendOp = RColorBlendOp::COLOR_BLEND_OP_ADD;
			blendState.srcAlphaBlendFactor = RColorBlendFactor::COLOR_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			blendState.dstAlphaBlendFactor = RColorBlendFactor::COLOR_BLEND_FACTOR_ZERO;
			blendState.alphaBlendOp = RColorBlendOp::COLOR_BLEND_OP_ADD;

			return blendState;
		}
	}
}
