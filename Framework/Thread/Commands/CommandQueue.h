// Copyright 2020 Dhiraj Wishal
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "Command.h"
#include "Core/Types/StaticQueue.h"

#define THREAD_MAX_COMMAND_COUNT	10

namespace DMK
{
	namespace Thread
	{
		// Command queue mutex.
		static std::mutex __CommandQueueMutex = {};

		/**
		 * Command Queue for the Dynamik Engine.
		 * This object is used to store commands in a queue which is submitted to the thread.
		 *
		 * The thread will have 3 main states,
		 * - Pending: Command is issued but the thread has not picked it yet.
		 * - Executing: The command is being executed.
		 * - Result/ Exit: The command is executed and its state will be returned.
		 *
		 * @tparam CommandCount: The maximum number of commands which can be stored.
		 */
		template<UI64 CommandCount = THREAD_MAX_COMMAND_COUNT>
		class CommandQueue {
		public:
			CommandQueue() {}
			~CommandQueue() {}

			/**
			 * Push a new command to the command queue.
			 * This method will wait till the command queue has an empty slot to push the data.
			 *
			 * @tparam Type: The type of the command.
			 * @param pState: The state variable pointer.
			 */
			template<class Type>
			void PushCommand(CommandState* pState = nullptr)
			{
				// Wait till the command queue has space.
				while (mCommandQueue.Size() >= CommandCount || mLockDown);

				// Lock the queue and push the data.
				std::lock_guard<std::mutex> _lock(__CommandQueueMutex);
				mCommandQueue.Push(std::make_pair(typeid(Type).name(), new Command<Type>(Type(), pState)));
			}

			/**
			 * Push a new command to the command queue.
			 * This method will wait till the command queue has an empty slot to push the data.
			 *
			 * @tparam Type: The type of the command.
			 * @param command: The command data to be pushed with. Default is Type().
			 * @param pState: The state variable pointer.
			 */
			template<class Type>
			void PushCommand(const Type& command, CommandState* pState = nullptr)
			{
				// Wait till the command queue has space.
				while (mCommandQueue.Size() >= CommandCount);

				// Lock the queue and push the data.
				std::lock_guard<std::mutex> _lock(__CommandQueueMutex);
				mCommandQueue.Push(std::make_pair(typeid(Type).name(), new Command<Type>(std::move(command), pState)));
			}

			/**
			 * Get the next command name from the queue.
			 *
			 * @return The const char pointer name.
			 */
			const char* GetCommandName() const
			{
				// Lock the queue and get the command name.
				std::lock_guard<std::mutex> _lock(__CommandQueueMutex);
				return mCommandQueue.Get().first;
			}

			/**
			 * Get the next command from the queue.
			 *
			 * @return CommandBase pointer.
			 */
			CommandBase* GetCommand() const
			{
				// Lock the queue and get the command.
				std::lock_guard<std::mutex> _lock(__CommandQueueMutex);
				return mCommandQueue.Get().second;
			}

			/**
			 * Get the first command and pop it from the queue.
			 *
			 * @return CommandBase pointer.
			 */
			CommandBase* GetAndPop()
			{
				// Lock the queue and get the command.
				std::lock_guard<std::mutex> _lock(__CommandQueueMutex);
				return mCommandQueue.GetAndPop().second;
			}

			/**
			 * Get the number of commands stored.
			 *
			 * @return The number of commands in the command queue.
			 */
			UI64 Count() const
			{
				if (mLockDown)
					mLockDown = false;

				return mCommandQueue.Size();
			}

			/**
			 * Synchronize the threads.
			 * Once the parent thread calls to synchronize the thread, the parent thread will be held until the
			 * child has completed any command which it was working on prior to calling this method. And once the
			 * child has completed it, both the threads are released at the same time.
			 *
			 * When this method is called, once the child thread calls the Count() method, the synchronization will
			 * be reset.
			 */
			void Synchronize()
			{
				mLockDown = true;
				while (mLockDown);
			}

		private:
			StaticQueue<std::pair<const char*, CommandBase*>, CommandCount> mCommandQueue;	// Command Queue.
			mutable bool mLockDown = false;
		};
	}
}

/**
 * Macro to delete a command if it is not deleted.
 */
#define DELETE_COMMAND(command, type)		delete command->Derived<type>(), command = nullptr

 /**
  * Macro to set the command state to pending.
  */
#define SET_COMMAND_PENDING(command)		command->SetState(Thread::CommandState::PENDING)

  /**
   * Macro to set the command state to executing.
   */
#define SET_COMMAND_EXECUTING(command)		command->SetState(Thread::CommandState::EXECUTING)

   /**
	* Macro to set the command state to success.
	*/
#define SET_COMMAND_SUCCESS(command)		command->SetState(Thread::CommandState::SUCCESS)

	/**
	 * Macro to set the command state to failed.
	 */
#define SET_COMMAND_FAILED(command)			command->SetState(Thread::CommandState::FAILED)
