// Copyright 2020 Dhiraj Wishal
// SPDX-License-Identifier: Apache-2.0

#include "dmkafx.h"
#include "Studio.h"

namespace Dynamik
{
	DMKStudio::DMKStudio()
	{
	}

	DMKStudio::~DMKStudio()
	{
	}
	
	void DMKStudio::execute()
	{
		beginFrame();
		updateFrame();
		endFrame();
	}

	void DMKStudio::beginFrame()
	{
		myGuiManager.beginFrame();
	}

	void DMKStudio::updateFrame()
	{
		myGuiManager.onUpdate();
	}

	void DMKStudio::endFrame()
	{
		myGuiManager.endFrame();
	}
}