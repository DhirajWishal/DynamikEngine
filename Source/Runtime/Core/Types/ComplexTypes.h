// Copyright 2020 Dhiraj Wishal
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _DYNAMIK_COMPLEX_TYPES_H
#define _DYNAMIK_COMPLEX_TYPES_H

/*
 Author:	Dhiraj Wishal
 Date:		28/06/2020

 This file contains all the complex types used by the engine.
*/
#include "../Math/Vector/Vector2F.h"
#include "../Math/Vector/Vector4F.h"

namespace Dynamik
{
	/* Dynamik Color Component */
	typedef Vector4F DMKColorComponent;

	/* Dynamik Rectangle 2D */
	typedef Vector2F DMKRect2D;

	/* Dynamik Rectangle 3D */
	typedef Vector3F DMKRect3D;
}

#endif // !_DYNAMIK_COMPLEX_TYPES_H