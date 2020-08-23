// Copyright 2020 Dhiraj Wishal
// SPDX-License-Identifier: Apache-2.0

#include "DefaultWorld.h"
#include "../Entities/Player.h"
#include "../Entities/OceanEnv.h"

void DefaultWorld::initialize()
{
	/* Add the player entity. */
	addEntity<Player>();

	/* Add the ocean environment entity. */
	auto oceanEntity = addEntity<OceanEnv>();
	oceanEntity->setCameraModule(getEntity<Player>()->getCameraModule());
}

void DefaultWorld::onUpdate(const F32 timeStep)
{
	auto playerObject = getEntity<Player>();

	if (!areEntitiesInitialized)
	{
		/* Initialize the player entity */
		playerObject->onInitializePlayer();

		/* Setup player controls */
		setupPlayerConstrols(getEntity<Player>());

		/* Submit the environment map entity. */
		submitEnvironmentToRenderer(getEntity<OceanEnv>());

		areEntitiesInitialized = true;
	}

	static B1 refresh = false;

	if (DMKEventPool::MouseButtonLeft.isPressed())
	{
		playerObject->processMouseControl(DMKEventPool::getMousePosition(), 0.1f, refresh, true);
		refresh = false;
	}

	if (DMKEventPool::MouseButtonLeft.isReleased())
		refresh = true;

	if (DMKEventPool::KeyUp.isPressed() || DMKEventPool::KeyUp.isOnRepeat())
		playerObject->addUpVector(0.0001f);

	if (DMKEventPool::KeyDown.isPressed() || DMKEventPool::KeyDown.isOnRepeat())
		playerObject->addDownVector(0.0001f);

	playerObject->updateCamera();

	getEntity<OceanEnv>()->onUpdateEnvironment();
}

void DefaultWorld::onMainWindowResize(DMKExtent2D newSize)
{
	getEntity<Player>()->setCameraViewPort(newSize);
}
