// Copyright 2020 Dhiraj Wishal
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "Thread/Commands/CommandQueue.h"

namespace DMK
{
	namespace GraphicsCore
	{
		using namespace GraphicsCore;

		/**
		 * Graphics Backend API enum.
		 */
		enum class GraphicsBackendAPI {
			VULKAN,
			DIRECT_X_12,
			WEB_GPU,
		};

		/**
		 * Graphics Engine Create Info structure.
		 * This structure defines how the graphics engine should be initialized.
		 */
		struct GraphicsEngineCreateInfo {
			GraphicsBackendAPI backendAPI = GraphicsBackendAPI::VULKAN;	// Backend API.

			bool enableValidation = true;	// Enable API validation.
			bool tryEnableRayTracing = false;	// Try and enable ray tracing. If not supported, compute shaders are used.
		};

		/**
		 * Graphics Engine for the Dynamik Engine.
		 * This object is the high level API of the Graphics system. All the graphics related tasks are done through
		 * this.
		 * The graphics backend (the backend thread) is managed by this object and direct commands to it shall be
		 * dont by this object by calling the GetCommandQueue() method.
		 */
		class Engine {
		public:
			Engine() {}
			~Engine() {}

			/**
			 * Initialize the Graphics Engine.
			 *
			 * @param initInfo: The initialization information structure.
			 */
			void Initialize(const GraphicsEngineCreateInfo& initInfo);

			/**
			 * Terminate the engine.
			 */
			void Terminate();

		public:
			/**
			 * Get the backend command queue.
			 *
			 * @return Thread::CommandQueue<> pointer.
			 */
			Thread::CommandQueue<THREAD_MAX_COMMAND_COUNT>* GetCommandQueue() { return &mCommandQueue; }

			/**
			 * Issue a command to the engine. This command will directly be passed to the backend.
			 *
			 * @tparam Type: The type of the command.
			 * @param initializer: The initialization values. Default is Type().
			 * @param pState: The state feedback variable pointer. Default is nullptr.
			 */
			template<class Type>
			void IssueCommand(const Type& initializer = Type(), Thread::CommandState* pState = nullptr)
			{
				GetCommandQueue()->PushCommand(initializer, pState);
			}

			/**
			 * Initialize the Graphics Backend.
			 *
			 * By enabling validation, if any invalid data were to be passed, the backend API can detect it and
			 * log it to the console. This ofcourse cost a little bit of performance. Enabling this is highly
			 * recommended for Debugging the application and when shipping the product, we recommend disabling
			 * API validation.
			 *
			 * @param enableValidation: Boolean value to enable or disable validation. Default is true.
			 */
			void InitializeBackend(bool enableValidation = true);

		private:
			std::thread mBackendThread;	// Backend thread object.
			Thread::CommandQueue<THREAD_MAX_COMMAND_COUNT> mCommandQueue;	// Backend command queue.
		};
	}
}
