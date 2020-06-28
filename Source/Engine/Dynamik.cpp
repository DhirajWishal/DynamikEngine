// Copyright 2020 Dhiraj Wishal
// SPDX-License-Identifier: Apache-2.0

/*
 This file contains all the definitions of the Dynamik Engine interface.

 Author:    Dhiraj Wishal
 Date:      15/05/2020
*/

#include "dmkafx.h"
#include "Dynamik.h"

#include "Core/Error/ErrorManager.h"
#include "Core/Utilities/Clock.h"
#include "Core/FileSystem/FileSystem.h"
#include "Core/Memory/AutomatedMemoryManager.h"

#include "Managers/Window/WindowManager.h"

#include "Importer/Dynamik/DAI/DAIObject.h"
#include "Importer/Asset/MeshImporter.h"

#include "GameLibrary/Utilities/MeshFactory.h"
#include "Core/Object/Resource/ShaderFactory.h"

namespace Dynamik
{
	/* Default constructor */
	DMKEngine::DMKEngine(const DMKEngineInstanceDescriptor& instanceDescriptor, const DMKGamePackage* gamePackage)
		: _instanceDescription(instanceDescriptor), _gamePackage((DMKGamePackage*)gamePackage)
	{
		_clock.start();
		_gamePackage->onLoad();

		_initializeRuntimeSystems();

		DMKErrorManager::logInfo("Welcome to the Dynamik Engine!");
		auto _localPath = DMKFileSystem::getExecutablePath();
		auto _workingDirectory = DMKFileSystem::getWorkingDirectory();

		DMKMeshFactory::setWorkingDirectory(_workingDirectory);
		DMKShaderFactory::setWorkingDirectory(_workingDirectory);

		UI32 windowID = _windowManager.createWindow(_instanceDescription.windowDescription.width, _instanceDescription.windowDescription.height, _instanceDescription.windowDescription.title);
		_threadManager.issueWindowHandleCommandRT(_windowManager.getWindowHandle(windowID));

		_threadManager.issueInitializeCommandRT();
		_threadManager.issueCreateContextCommandRT(DMKRenderContextType::DMK_RENDER_CONTEXT_DEFAULT, _windowManager.createViewport(windowID, 512, 512, 0, 0));

		_gamePackage->onInit();

		_loadLevel();
	}

	/* Execute the game code */
	void DMKEngine::execute()
	{
		_gamePackage->onExecute();
		_threadManager.issueInitializeEntityCommandRT(_currentLevel->myEntities[0]);
		_threadManager.issueInitializeFinalsCommandRT();

		UI64 _itrIndex = 0;
		DMKGameEntity* _entity;

		printf("Allocation count: %u", DMKAutomatedMemoryManager::getAllocationCount());

		while (true)
		{
			_gamePackage->onBeginFrame();

			_threadManager.clearCommands();

			_windowManager.pollEvents();

			for (_itrIndex = 0; _itrIndex < _currentLevel->myEntities.size(); _itrIndex++)
			{
				_currentLevel->myEntities[_itrIndex];
				/* send entity to the physics engine */
			}

			_windowManager.clean();

			_gamePackage->onEndFrame();
		}
	}

	/* Default destructor */
	DMKEngine::~DMKEngine()
	{
		_gamePackage->onExit();

		_windowManager.terminateAll();
		_clock.end();
	}

	void DMKEngine::_initializeRuntimeSystems()
	{
		_threadManager.initializeBasicThreads();
	}

	void DMKEngine::_loadLevel()
	{
		_gamePackage->onLevelLoad(_nextLevelIndex);
		_currentLevel = _gamePackage->levels[_nextLevelIndex++];

		_currentLevel->onLoad();
		_currentLevel->initializeComponents();

		for (auto _entity : _currentLevel->myEntities)
			_entity->initialize();

		_currentLevel->initializeCameraModule();
	}
}