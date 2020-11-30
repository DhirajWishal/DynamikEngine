// Copyright 2020 Dhiraj Wishal
// SPDX-License-Identifier: Apache-2.0

#include "VulkanBackend/Common/VulkanDeviceManager.h"
#include "VulkanBackend/VulkanBackendFunction.h"

#include "GraphicsCore/Commands/CoreCommands.h"
#include "GraphicsCore/Commands/RenderTargetCommands.h"
using namespace DMK::GraphicsCore;
using namespace DMK::VulkanBackend;

#include <thread>

/** CONCEPT
 * Create a file named *.das (Dynamik Asset Serialization) which stores all the vertex data in binary.
 * This way it is easy to stream data directly to the GPU.
 *
 * Data are stored in the following format:
 * 1) Vertex data are stored.
 * 1.1) An unsigned 64 bit integer stating the number of bytes to copy.
 * 1.2) The vertex size.
 * 1.3) The vertex data.
 *
 * 2) Index data are stored.
 * 2.1) An unsigned 64 bit integer stating the number of bytes to copy.
 * 2.2) The index size.
 * 2.3) The index data.
 *
 * 3) Texture data are stored.
 * 3.1) Number (8 bit integer) of textures that are stored.
 * 3.2) 3, 32 bit, semicolon (;) separated integers representing the width, height and depth of the texture.
 * 3.3) 5, 8 bit integers containing format information.
 * 3.4) Texture data.
 * 3.5) Repeat 3.2) for a total count of 3.1).
 *
 * 4) Material  data are stored.
 * 4.1) Number (8 bit integer) containing the number of materials stored.
 * 4.2) Number of bytes to copy.
 * 4.3) The material data.
 * 4.4) Repeat 4.2) for a total count of 4.1).
 */

DMK::Vector3 mExtent = { 1280.0f, 720.0f, 1.0f };

RenderTargetAttachmentSpecification CreateSwapChainSpec()
{
	RenderTargetAttachmentSpecification spec = {};
	spec.type = RenderTargetAttachmentType::SWAP_CHAIN;

	return spec;
}

RenderTargetAttachmentSpecification CreateColorBufferSpec()
{
	RenderTargetAttachmentSpecification spec = {};
	spec.type = RenderTargetAttachmentType::COLOR_BUFFER;

	return spec;
}

RenderTargetAttachmentSpecification CreateDepthBufferSpec()
{
	RenderTargetAttachmentSpecification spec = {};
	spec.type = RenderTargetAttachmentType::DEPTH_BUFFER;

	return spec;
}

int main()
{
	DMK::Thread::CommandQueue<THREAD_MAX_COMMAND_COUNT> mCommandQueue = {};
	DMK::Thread::CommandState mCommandState = DMK::Thread::CommandState::PENDING;
	DeviceHandle mDeviceHandle = {};

	std::thread vBackendThread(VulkanBackendFunction, &mCommandQueue);

	mCommandQueue.PushCommand<Commands::InitializeBackend>(&mCommandState);
	mCommandQueue.PushCommand<Commands::CreateDevice>(Commands::CreateDevice(&mDeviceHandle), &mCommandState);

	RenderTargetHandle mRenderTargetHandle = {};
	mCommandQueue.PushCommand<Commands::CreateRenderTarget>(Commands::CreateRenderTarget(0, mExtent, { CreateSwapChainSpec(), CreateColorBufferSpec(), CreateDepthBufferSpec() }, mDeviceHandle, &mRenderTargetHandle));

	//size_t counter = 1000;
	size_t counter = std::numeric_limits<size_t>().max();
	while (counter--);

	mCommandQueue.PushCommand<Commands::DestroyRenderTarget>(Commands::DestroyRenderTarget(mRenderTargetHandle, mDeviceHandle));
	mCommandQueue.PushCommand<Commands::DestroyDevice>();
	mCommandQueue.PushCommand<Commands::TerminateBackend>();

	vBackendThread.join();
}