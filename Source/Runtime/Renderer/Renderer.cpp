// Copyright 2020 Dhiraj Wishal
// SPDX-License-Identifier: Apache-2.0

#include "dmkafx.h"
#include "Renderer.h"

#include "Core/Math/MathFunctions.h"

/* Vulkan headers */
#include "Backend/Vulkan/VulkanCoreObject.h"
#include "Backend/Vulkan/Common/VulkanInstance.h"
#include "Backend/Vulkan/Common/VulkanSurface.h"
#include "Backend/Vulkan/Common/VulkanDevice.h"
#include "Backend/Vulkan/Common/VulkanQueue.h"
#include "Backend/Vulkan/Common/VulkanCommandBuffer.h"
#include "Backend/Vulkan/Common/VulkanCommandBufferManager.h"
#include "Backend/Vulkan/Common/VulkanDescriptorSetManager.h"
#include "Backend/Vulkan/Context/VulkanSwapChain.h"
#include "Backend/Vulkan/Context/VulkanRenderPass.h"
#include "Backend/Vulkan/Context/VulkanFrameBuffer.h"
#include "Backend/Vulkan/Pipelines/VulkanGraphicsPipeline.h"

namespace Dynamik
{
	using namespace Backend;

	/* ---------- CLASS DEFINITION ---------- */
	void DMKRenderer::initialize()
	{
		myCompatibility.isVulkanAvailable = glfwVulkanSupported();

		myAPI = DMKRenderingAPI::DMK_RENDERING_API_VULKAN;
		myRenderTarget = new RRenderTarget;

#ifdef DMK_DEBUG
		DMK_INFO("Entered the renderer thread!");

#endif // DMK_DEBUG

	}

	void DMKRenderer::processCommand(DMKThreadCommand* pCommand)
	{
		myCommand = Inherit<DMKRendererCommand>(pCommand);

		switch (myCommand->instruction)
		{
		case Dynamik::RendererInstruction::RENDERER_INSTRUCTION_INITIALIZE:

#ifdef DMK_DEBUG
			myCoreObject = createCore(true);

#else
			myCoreObject = createCore(false);

#endif // DMK_DEBUG
			break;
		case Dynamik::RendererInstruction::RENDERER_INSTRUCTION_CREATE_CONTEXT:
			createContext(Inherit<RendererCreateContextCommand>(myCommand)->contextType, Inherit<RendererCreateContextCommand>(myCommand)->viewport);
			break;
		case Dynamik::RendererInstruction::RENDERER_INSTRUCTION_INITIALIZE_CAMERA:
			initializeCamera(Inherit<RendererInitializeCamera>(myCommand)->pCameraModule);
			break;
		case Dynamik::RendererInstruction::RENDERER_INSTRUCTION_INITIALIZE_GAME_WORLD:
			initializeGameWorld(Inherit<RendererInitializeGameWorld>(myCommand)->pGameWorld);
			break;
		case Dynamik::RendererInstruction::RENDERER_INSTRUCTION_INITIALIZE_ENVIRONMENT_MAP:
			initializeEnvironmentMap(Inherit<RendererInitializeEnvironmentMap>(myCommand)->pEnvironmentMap);
			break;
		case Dynamik::RendererInstruction::RENDERER_INSTRUCTION_INITIALIZE_FINALS:
			initializeFinals();
			break;
		case Dynamik::RendererInstruction::RENDERER_INSTRUCTION_INITIALIZE_ENTITY:
			createEntityResources(Inherit<RendererAddEntity>(myCommand)->entity);
			break;
		case Dynamik::RendererInstruction::RENDERER_INSTRUCTION_SUBMIT_OBJECTS:
			break;
		case Dynamik::RendererInstruction::RENDERER_INSTRUCTION_DRAW_INITIALIZE:
			break;
		case Dynamik::RendererInstruction::RENDERER_INSTRUCTION_DRAW_UPDATE:
			updateInstruction();
			break;
		case Dynamik::RendererInstruction::RENDERER_INSTRUCTION_DRAW_SUBMIT:
			break;
		case Dynamik::RendererInstruction::RENDERER_INSTRUCTION_TERMINATE_FRAME:
			myCoreObject->idleCall();
			isReadyToRun = false;
			break;
		case Dynamik::RendererInstruction::RENDERER_INSTRUCTION_TERMINATE_OBJECTS:
			terminateEntities();
			break;
		case Dynamik::RendererInstruction::RENDERER_INSTRUCTION_TERMINATE:
			terminateContext();
			terminateComponents();
			break;
		case Dynamik::RendererInstruction::RENDERER_INSTRUCTION_UPDATE_OBJECTS:
			break;
		case Dynamik::RendererInstruction::RENDERER_INSTRUCTION_SET_SAMPLES:
			setSamples(Inherit<RendererSetSamplesCommand>(myCommand)->samples);
			break;
		case Dynamik::RendererInstruction::RENDERER_INSTRUCTION_SET_WINDOW_HANDLE:
			setWindowHandle(Inherit<RendererSetWindowHandleCommand>(myCommand)->windowHandle);
			break;
		case Dynamik::RendererInstruction::RENDERER_RESIZE_FRAME_BUFFER:
			resizeFrameBuffer(Inherit<RendererResizeFrameBuffer>(myCommand)->windowExtent);
			break;
		default:
			break;
		}
	}

	void DMKRenderer::onLoop()
	{
		if (!isReadyToRun)
			return;

		beginFrameInstruction();
		endFrameInstruction();
	}

	void DMKRenderer::onTermination()
	{
		DMK_INFO("Terminated the renderer thread!");
	}

	/* ---------- INTERNAL METHODS ---------- */
	void DMKRenderer::setSamples(const DMKSampleCount& samples)
	{
		mySampleCount = samples;
	}

	void DMKRenderer::setWindowHandle(const DMKWindowHandle* windowHandle)
	{
		myWindowHandle = Cast<DMKWindowHandle*>(windowHandle);
	}

	RCoreObject* DMKRenderer::createCore(B1 bEnableValidation)
	{
		switch (myAPI)
		{
		case Dynamik::DMKRenderingAPI::DMK_RENDERING_API_NONE:
			break;
		case Dynamik::DMKRenderingAPI::DMK_RENDERING_API_VULKAN:
		{
			myCoreObject = Inherit<RCoreObject>(StaticAllocator<VulkanCoreObject>::rawAllocate().get());
			myCoreObject->initialize(myWindowHandle, mySampleCount, bEnableValidation);
		}
		break;
		case Dynamik::DMKRenderingAPI::DMK_RENDERING_API_DIRECTX:
			break;
		case Dynamik::DMKRenderingAPI::DMK_RENDERING_API_OPENGL:
			break;
		default:
			DMK_ERROR_BOX("Invalid rendering API!");
			break;
		}

		myBufferFactory.setDefaults(myAPI, myCoreObject);

		return myCoreObject;
	}

	RSwapChain* DMKRenderer::createSwapChain(DMKViewport viewport, RSwapChainPresentMode presentMode)
	{
		switch (myAPI)
		{
		case Dynamik::DMKRenderingAPI::DMK_RENDERING_API_NONE:
			break;
		case Dynamik::DMKRenderingAPI::DMK_RENDERING_API_VULKAN:
		{
			mySwapChain = Inherit<RSwapChain>(StaticAllocator<VulkanSwapChain>::rawAllocate().get());
			mySwapChain->initialize(myCoreObject, viewport, presentMode);
		}
		break;
		case Dynamik::DMKRenderingAPI::DMK_RENDERING_API_DIRECTX:
			break;
		case Dynamik::DMKRenderingAPI::DMK_RENDERING_API_OPENGL:
			break;
		default:
			DMK_ERROR_BOX("Invalid rendering API!");
			break;
		}

		return mySwapChain;
	}

	RRenderPass* DMKRenderer::createRenderPass(ARRAY<RSubPasses> subPasses)
	{
		switch (myAPI)
		{
		case Dynamik::DMKRenderingAPI::DMK_RENDERING_API_NONE:
			break;
		case Dynamik::DMKRenderingAPI::DMK_RENDERING_API_VULKAN:
		{
			myRenderTarget->pRenderPass = Inherit<RRenderPass>(StaticAllocator<VulkanRenderPass>::rawAllocate().get());

			/* Attachments: Color, Depth, Swap Chain */
			myRenderTarget->pRenderPass->initialize(myCoreObject, subPasses, mySwapChain);
		}
		break;
		case Dynamik::DMKRenderingAPI::DMK_RENDERING_API_DIRECTX:
			break;
		case Dynamik::DMKRenderingAPI::DMK_RENDERING_API_OPENGL:
			break;
		default:
			DMK_ERROR_BOX("Invalid rendering API!");
			break;
		}

		return myRenderTarget->pRenderPass;
	}

	RFrameBuffer* DMKRenderer::createFrameBuffer()
	{
		switch (myAPI)
		{
		case Dynamik::DMKRenderingAPI::DMK_RENDERING_API_NONE:
			break;
		case Dynamik::DMKRenderingAPI::DMK_RENDERING_API_VULKAN:
		{
			myRenderTarget->pFrameBuffer = Inherit<RFrameBuffer>(StaticAllocator<VulkanFrameBuffer>::rawAllocate().get());
			myRenderTarget->pFrameBuffer->initialize(myCoreObject, myRenderTarget->pRenderPass, mySwapChain);
		}
		break;
		case Dynamik::DMKRenderingAPI::DMK_RENDERING_API_DIRECTX:
			break;
		case Dynamik::DMKRenderingAPI::DMK_RENDERING_API_OPENGL:
			break;
		default:
			DMK_ERROR_BOX("Invalid rendering API!");
			break;
		}

		return myRenderTarget->pFrameBuffer;
	}

	void DMKRenderer::createContext(DMKRenderContextType type, DMKViewport viewport)
	{
		myCurrentContextType = type;

		/* Initialize Swap chain */
		createSwapChain(viewport, RSwapChainPresentMode::SWAPCHAIN_PRESENT_MODE_FIFO);

		/* Initialize Render pass */
		createRenderPass(getSubPasses(myCurrentContextType));

		/* Initialize Frame buffer */
		createFrameBuffer();
	}

	RBuffer* DMKRenderer::createBuffer(const RBufferType& type, UI64 size, RResourceMemoryType memoryType)
	{
		switch (myAPI)
		{
		case Dynamik::DMKRenderingAPI::DMK_RENDERING_API_NONE:
			break;
		case Dynamik::DMKRenderingAPI::DMK_RENDERING_API_VULKAN:
		{
			RBuffer* pBuffer = StaticAllocator<VulkanBuffer>::rawAllocate();
			pBuffer->initialize(myCoreObject, type, size, memoryType);

			return pBuffer;
		}
		case Dynamik::DMKRenderingAPI::DMK_RENDERING_API_DIRECTX:
			break;
		case Dynamik::DMKRenderingAPI::DMK_RENDERING_API_OPENGL:
			break;
		default:
			break;
		}

		return nullptr;
	}

	RBuffer* DMKRenderer::createVertexBuffer(UI64 size)
	{
		return createBuffer(RBufferType::BUFFER_TYPE_VERTEX, size, RResourceMemoryType(RESOURCE_MEMORY_TYPE_DEVICE_LOCAL | RESOURCE_MEMORY_TYPE_HOST_COHERENT));
	}

	RBuffer* DMKRenderer::createIndexBuffer(UI64 size)
	{
		return createBuffer(RBufferType::BUFFER_TYPE_INDEX, size, RResourceMemoryType(RESOURCE_MEMORY_TYPE_DEVICE_LOCAL | RESOURCE_MEMORY_TYPE_HOST_COHERENT));
	}

	void DMKRenderer::copyBuffer(RBuffer* pSrcBuffer, RBuffer* pDstBuffer, UI64 size)
	{
		pDstBuffer->copy(myCoreObject, pSrcBuffer, size, 0, 0);
	}

	RTexture* DMKRenderer::createTexture(const DMKTexture* pTexture)
	{
		switch (myAPI)
		{
		case Dynamik::DMKRenderingAPI::DMK_RENDERING_API_NONE:
			break;
		case Dynamik::DMKRenderingAPI::DMK_RENDERING_API_VULKAN:
		{
			RTexture* texture = Inherit<RTexture>(StaticAllocator<VulkanTexture>::rawAllocate().get());
			texture->initialize(myCoreObject, (DMKTexture*)pTexture);
			texture->createView(myCoreObject);

			if ((pTexture->type == DMKTextureType::DMK_TEXTURE_TYPE_CUBEMAP) || (pTexture->type == DMKTextureType::DMK_TEXTURE_TYPE_CUBEMAP_ARRAY)) /* TODO */
				texture->createSampler(myCoreObject, RImageSamplerCreateInfo::createCubeMapSampler());
			else
				texture->createSampler(myCoreObject, RImageSamplerCreateInfo::createDefaultSampler());

			texture->makeRenderable(myCoreObject);

			return texture;
		}
		case Dynamik::DMKRenderingAPI::DMK_RENDERING_API_DIRECTX:
			break;
		case Dynamik::DMKRenderingAPI::DMK_RENDERING_API_OPENGL:
			break;
		default:
			break;
		}

		return nullptr;
	}

	RPipelineObject* DMKRenderer::allocatePipeline()
	{
		switch (myAPI)
		{
		case Dynamik::DMKRenderingAPI::DMK_RENDERING_API_VULKAN:
			return Inherit<RPipelineObject>(StaticAllocator<VulkanGraphicsPipeline>::rawAllocate().get());
		case Dynamik::DMKRenderingAPI::DMK_RENDERING_API_DIRECTX:
			break;
		case Dynamik::DMKRenderingAPI::DMK_RENDERING_API_OPENGL:
			break;
		default:
			break;
		}

		return nullptr;
	}

	void DMKRenderer::initializeCamera(DMKCameraModule* pCameraModule)
	{
		if (!pCameraModule)
			return;

		if (!myCameraComponent)
			myCameraComponent = StaticAllocator<RCameraComponent>::rawAllocate();

		myCameraComponent->pCameraModule = pCameraModule;
		myCameraComponent->pUniformBuffer = createBuffer(RBufferType::BUFFER_TYPE_UNIFORM, sizeof(DMKCameraMatrix));

		DMKCameraMatrix _matrix;
		_matrix.projection = DMKMathFunctions::perspective(DMKMathFunctions::radians(45.0f), mySwapChain->extent.width / mySwapChain->extent.height, 0.001f, 256.0f);
		_matrix.view = DMKMathFunctions::lookAt(pCameraModule->position, pCameraModule->position + pCameraModule->front, pCameraModule->cameraUp);
		myCameraComponent->pUniformBuffer->setData(myCoreObject, sizeof(_matrix), 0, &_matrix);
	}

	void DMKRenderer::initializeEnvironmentMap(DMKEnvironmentMap* pEnvironmentMap)
	{
		if (!pEnvironmentMap)
			return;

		/* Initialize Sky Box */
		myCurrentEnvironment.pMeshComponent = &pEnvironmentMap->skyBox;

		/* Initialize Texture */
		myCurrentEnvironment.pTexture = createTexture(pEnvironmentMap->skyBox.pTextures[0]);	/* TODO */

		/* Initialize Uniforms */
		myCurrentEnvironment.pUniformBuffer = createBuffer(RBufferType::BUFFER_TYPE_UNIFORM, sizeof(MAT4));
		myCurrentEnvironment.pUniformBuffer->setData(myCoreObject, sizeof(MAT4), 0, &pEnvironmentMap->skyBox.modelMatrix);

		/* Initialize Pipeline */
		myCurrentEnvironment.pPipeline = allocatePipeline();

		RPipelineSpecification pipelineCreateInfo = {};
		pipelineCreateInfo.shaders = pEnvironmentMap->skyBox.shaderModules;
		pipelineCreateInfo.scissorInfos.resize(1);
		pipelineCreateInfo.colorBlendInfo.blendStates = createBasicBlendStates();
		pipelineCreateInfo.multiSamplingInfo.sampleCount = myCoreObject->sampleCount;
		pipelineCreateInfo.rasterizerInfo.frontFace = RFrontFace::FRONT_FACE_CLOCKWISE;
		myCurrentEnvironment.pPipeline->initialize(myCoreObject, pipelineCreateInfo, RPipelineUsage::PIPELINE_USAGE_GRAPHICS, myRenderTarget, mySwapChain);

		/* Initialize Vertex and Index Buffers */
		myDrawCallManager.addDrawEntry(
			pEnvironmentMap->skyBox.vertexCount, pEnvironmentMap->skyBox.vertexBuffer,
			&pEnvironmentMap->skyBox.indexBuffer,
			myCurrentEnvironment.pPipeline, pEnvironmentMap->skyBox.vertexLayout);

		/* Initialize Pipeline Resources */
		/* Initialize Uniform Buffer Resources */
		ARRAY<RBuffer*> uniformBuffers = { myCurrentEnvironment.pUniformBuffer };

		if (myCameraComponent->pUniformBuffer)
			uniformBuffers.pushBack(myCameraComponent->pUniformBuffer);

		/* Initialize Texture Resources */
		ARRAY<RTexture*> textures = { myCurrentEnvironment.pTexture };

		myCurrentEnvironment.pPipeline->initializeResources(myCoreObject, uniformBuffers, textures);
	}

	void DMKRenderer::createEntityResources(DMKGameEntity* pGameEntity)
	{
		REntity entity;
		RMeshObject* meshComponent = nullptr;
		for (UI64 index = 0; index < pGameEntity->componentManager.getObjectArray<DMKStaticMeshComponent>()->size(); index++)
		{
			auto mesh = pGameEntity->componentManager.getObject<DMKStaticMeshComponent>(index);
			meshComponent = StaticAllocator<RMeshObject>::rawAllocate();
			meshComponent->pMeshComponent = mesh;

			/* Initialize Texture Data */
			for (auto pTexture : mesh->pTextures)
				meshComponent->pTextures.pushBack(createTexture(pTexture));

			/* Initialize Uniform Buffers */
			{
				/* Initialize Default Uniform */
				RBuffer* defaultUniform = createBuffer(RBufferType::BUFFER_TYPE_UNIFORM, mesh->getUniformByteSize());
				meshComponent->pUniformBuffer = defaultUniform;

				meshComponent->pUniformBuffer->setData(myCoreObject, sizeof(mesh->modelMatrix), 0, &mesh->modelMatrix);
			}

			/* Initialize Pipeline */
			meshComponent->pPipeline = allocatePipeline();

			RPipelineSpecification pipelineCreateInfo = {};
			pipelineCreateInfo.shaders = mesh->shaderModules;
			pipelineCreateInfo.scissorInfos.resize(1);
			pipelineCreateInfo.colorBlendInfo.blendStates = createBasicBlendStates();
			pipelineCreateInfo.multiSamplingInfo.sampleCount = myCoreObject->sampleCount;
			meshComponent->pPipeline->initialize(myCoreObject, pipelineCreateInfo, RPipelineUsage::PIPELINE_USAGE_GRAPHICS, myRenderTarget, mySwapChain);

			/* Initialize Pipeline Resources */
			/* Initialize Uniform Buffer Resources */
			ARRAY<RBuffer*> uniformBuffers = { meshComponent->pUniformBuffer };
			if (pGameEntity->isCameraAvailable && myCameraComponent->pUniformBuffer)
				uniformBuffers.pushBack(myCameraComponent->pUniformBuffer);

			/* Initialize Texture Resources */
			ARRAY<RTexture*> textures;
			textures.insert(meshComponent->pTextures);

			meshComponent->pPipeline->initializeResources(myCoreObject, uniformBuffers, textures);

			/* Initialize Vertex and Index Buffers */
			myDrawCallManager.addDrawEntry(
				mesh->vertexCount, mesh->vertexBuffer,
				&mesh->indexBuffer,
				meshComponent->pPipeline, mesh->vertexLayout);

			entity.pMeshObjects.pushBack(meshComponent);
		}

		myEntities.pushBack(entity);
	}

	void DMKRenderer::initializeGameWorld(DMKGameWorld* pGameWorld)
	{
		if (!pGameWorld)
			return;

		/* Initialize Environment Map */
		initializeEnvironmentMap(pGameWorld->pEnvironmentMap);

		/* Initialize Entities */
		for (auto entity : pGameWorld->entities)
			createEntityResources(entity);
	}

	void DMKRenderer::updateResources()
	{
		for (UI64 entityIndex = 0; entityIndex < myEntities.size(); entityIndex++)
		{
			for (UI64 meshIndex = 0; meshIndex < myEntities[entityIndex].pMeshObjects.size(); meshIndex++)
			{

			}
		}
	}

	void DMKRenderer::bindEnvironment(RCommandBuffer* pCommandBuffer, UI64* pFirstVertex, UI64* pFirstIndex)
	{
		pCommandBuffer->bindGraphicsPipeline(myCurrentEnvironment.pPipeline);
		pCommandBuffer->drawIndexed(*pFirstVertex, myCurrentEnvironment.indexBufferOffset, myCurrentEnvironment.pMeshComponent->indexCount, 1);

		*pFirstIndex += myCurrentEnvironment.pMeshComponent->indexCount;
		*pFirstVertex += myCurrentEnvironment.pMeshComponent->vertexCount;
	}

	void DMKRenderer::initializeCommandBuffers()
	{
		myCommandBuffers = myCommandBufferManager->allocateCommandBuffers(myCoreObject, mySwapChain->bufferCount);

		for (UI32 itr = 0; itr < myCommandBuffers.size(); itr++)
		{
			myDrawCallManager.setCommandBuffer(myCommandBuffers[itr]);
			myDrawCallManager.beginCommand();
			myDrawCallManager.bindRenderTarget(myRenderTarget, mySwapChain, itr);

			myDrawCallManager.bindDrawCalls(RDrawCallType::DRAW_CALL_TYPE_INDEX);

			myDrawCallManager.unbindRenderTarget();
			myDrawCallManager.endCommand();
		}

		isReadyToRun = true;
	}

	void DMKRenderer::initializeFinals()
	{
		myDrawCallManager.initializeBuffers(myCoreObject);

		myCommandBufferManager = Inherit<RCommandBufferManager>(StaticAllocator<VulkanCommandBufferManager>::rawAllocate().get());
		myCommandBufferManager->initialize(myCoreObject);

		initializeCommandBuffers();
	}

	void DMKRenderer::resizeFrameBuffer(DMKExtent2D windowExtent)
	{
		isReadyToRun = false;

		/* Reset Command Buffers */
		myCoreObject->idleCall();
		myCommandBufferManager->resetBuffers(myCoreObject, myCommandBuffers);

		/* Terminate The Current Context */
		{
			/* Terminate Frame Buffer */
			myRenderTarget->pFrameBuffer->terminate(myCoreObject);

			/* Terminate Render Pass */
			myRenderTarget->pRenderPass->terminate(myCoreObject);

			/* Terminate Swap Chain */
			mySwapChain->terminate(myCoreObject);
		}

		/* Create New Context */
		{
			/* Initialize View port */
			DMKViewport newViewPort;
			newViewPort.windowHandle = myWindowHandle;
			newViewPort.width = windowExtent.width;
			newViewPort.height = windowExtent.height;

			/* Initialize Swap Chain */
			mySwapChain->initialize(myCoreObject, newViewPort, RSwapChainPresentMode::SWAPCHAIN_PRESENT_MODE_FIFO);;

			/* Initialize Render Pass */
			myRenderTarget->pRenderPass->initialize(myCoreObject, getSubPasses(myCurrentContextType), mySwapChain);

			/* Initialize Frame Buffer */
			myRenderTarget->pFrameBuffer->initialize(myCoreObject, myRenderTarget->pRenderPass, mySwapChain);
		}

		/* Initialize Pipelines */
		{
			/* Initialize Environment Map Pipeline */
			myCurrentEnvironment.pPipeline->reCreate(myCoreObject, myRenderTarget, mySwapChain);

			/* Initialize Entity Pipelines */
			for (auto entity : myEntities)
				for (auto mesh : entity.pMeshObjects)
					mesh->pPipeline->reCreate(myCoreObject, myRenderTarget, mySwapChain);
		}

		/* Initialize Buffers */
		initializeCommandBuffers();
	}

	void DMKRenderer::beginFrameInstruction()
	{
		currentImageIndex = myCoreObject->prepareFrame(mySwapChain);

		if (currentImageIndex == -1)
			resizeFrameBuffer({ (F32)mySwapChain->viewPort.width, (F32)mySwapChain->viewPort.height });
	}

	void DMKRenderer::updateInstruction()
	{
		/* Update the camera component */
		if (myCameraComponent)
			myCameraComponent->pUniformBuffer->setData(myCoreObject, sizeof(DMKCameraMatrix), 0, &myCameraComponent->pCameraModule->matrix);

		//for (UI64 entityIndex = 0; entityIndex < myEntities.size(); entityIndex++)
		//{
		//	for (UI64 meshIndex = 0; meshIndex < myEntities[entityIndex].pMeshObjects.size(); meshIndex++)
		//	{
		//		myEntities[entityIndex].pMeshObjects[meshIndex]->pUniformBuffer->setData(myCoreObject, sizeof(MAT4), 0, &myEntities[entityIndex].pMeshObjects[meshIndex]->pMeshComponent->modelMatrix);
		//	}
		//}
	}

	void DMKRenderer::endFrameInstruction()
	{
		myCoreObject->submitCommand(myCommandBuffers[currentImageIndex], mySwapChain);
	}

	void DMKRenderer::terminateContext()
	{
		/* Terminate Frame Buffer */
		myRenderTarget->pFrameBuffer->terminate(myCoreObject);
		StaticAllocator<RFrameBuffer>::rawDeallocate(myRenderTarget->pFrameBuffer, 0);

		/* Terminate Render Pass */
		myRenderTarget->pRenderPass->terminate(myCoreObject);
		StaticAllocator<RRenderPass>::rawDeallocate(myRenderTarget->pRenderPass, 0);

		/* Deallocate Render Target */
		delete myRenderTarget;

		/* Terminate Swap Chain */
		mySwapChain->terminate(myCoreObject);
		StaticAllocator<RSwapChain>::rawDeallocate(mySwapChain, 0);
	}

	void DMKRenderer::terminateComponents()
	{
		/* Terminate Vertex and Index Buffers */
		myDrawCallManager.terminate(myCoreObject);

		/* Terminate Command Buffers */
		myCommandBufferManager->terminate(myCoreObject, myCommandBuffers);
		StaticAllocator<RCommandBufferManager>::rawDeallocate(myCommandBufferManager, 0);

		/* Terminate Core Object */
		myCoreObject->terminate();
		StaticAllocator<RCoreObject>::rawDeallocate(myCoreObject, 0);
	}

	void DMKRenderer::terminateEntities()
	{
		/* Terminate Camera */
		if (myCameraComponent)
		{
			myCameraComponent->pUniformBuffer->terminate(myCoreObject);
			StaticAllocator<RBuffer>::rawDeallocate(myCameraComponent->pUniformBuffer, 0);
		}

		/* Terminate Environment Map */
		{
			if (myCurrentEnvironment.pPipeline)
			{
				myCurrentEnvironment.pPipeline->terminate(myCoreObject);
				StaticAllocator<RPipelineObject>::rawDeallocate(myCurrentEnvironment.pPipeline, 0);
			}

			if (myCurrentEnvironment.pTexture)
			{
				myCurrentEnvironment.pTexture->terminate(myCoreObject);
				StaticAllocator<RTexture>::rawDeallocate(myCurrentEnvironment.pTexture, 0);
			}

			if (myCurrentEnvironment.pUniformBuffer)
			{
				myCurrentEnvironment.pUniformBuffer->terminate(myCoreObject);
				StaticAllocator<RBuffer>::rawDeallocate(myCurrentEnvironment.pUniformBuffer, 0);
			}
		}

		/* Terminate Entities */
		for (auto entity : myEntities)
		{
			for (auto mesh : entity.pMeshObjects)
			{
				for (auto texture : mesh->pTextures)
				{
					texture->terminate(myCoreObject);
					StaticAllocator<RTexture>::rawDeallocate(texture, 0);
				}

				mesh->pUniformBuffer->terminate(myCoreObject);
				StaticAllocator<RBuffer>::rawDeallocate(mesh->pUniformBuffer, 0);

				mesh->pPipeline->terminate(myCoreObject);
				StaticAllocator<RPipelineObject>::rawDeallocate(mesh->pPipeline, 0);

				StaticAllocator<RMeshObject>::rawDeallocate(mesh);
			}

			entity.pMeshObjects.clear();
		}

		myEntities.clear();
	}

	ARRAY<RColorBlendState> DMKRenderer::createBasicBlendStates()
	{
		ARRAY<RColorBlendState> blendStates;
		RColorBlendState state;
		for (UI32 index = 0; index < 1; index++)
		{
			state.colorWriteMask = (RColorComponent)0;
			state.enable = false;

			blendStates.pushBack(state);
		}

		return blendStates;
	}
	
	ARRAY<RSubPasses> DMKRenderer::getSubPasses(DMKRenderContextType contextType)
	{
		ARRAY<RSubPasses> subpasses;
		switch (contextType)
		{
		case Dynamik::DMKRenderContextType::DMK_RENDER_CONTEXT_DEFAULT:
			subpasses = { RSubPasses::SUBPASSES_COLOR, RSubPasses::SUBPASSES_DEPTH, RSubPasses::SUBPASSES_SWAPCHAIN };
			break;
		case Dynamik::DMKRenderContextType::DMK_RENDER_CONTEXT_DEFAULT_VR:
			subpasses = { RSubPasses::SUBPASSES_COLOR, RSubPasses::SUBPASSES_DEPTH, RSubPasses::SUBPASSES_SWAPCHAIN };
			break;
		case Dynamik::DMKRenderContextType::DMK_RENDER_CONTEXT_2D:
			subpasses = { RSubPasses::SUBPASSES_COLOR, RSubPasses::SUBPASSES_SWAPCHAIN };
			break;
		case Dynamik::DMKRenderContextType::DMK_RENDER_CONTEXT_3D:
			subpasses = { RSubPasses::SUBPASSES_COLOR, RSubPasses::SUBPASSES_DEPTH, RSubPasses::SUBPASSES_SWAPCHAIN };
			break;
		case Dynamik::DMKRenderContextType::DMK_RENDER_CONTEXT_DEBUG:
			subpasses = { RSubPasses::SUBPASSES_COLOR, RSubPasses::SUBPASSES_DEPTH, RSubPasses::SUBPASSES_SWAPCHAIN };
			break;
		case Dynamik::DMKRenderContextType::DMK_RENDER_CONTEXT_DEBUG_VR:
			subpasses = { RSubPasses::SUBPASSES_COLOR, RSubPasses::SUBPASSES_DEPTH, RSubPasses::SUBPASSES_SWAPCHAIN };
			break;
		default:
			DMK_ERROR_BOX("Invalid context type!");
			break;
		}

		return subpasses;
	}
}
