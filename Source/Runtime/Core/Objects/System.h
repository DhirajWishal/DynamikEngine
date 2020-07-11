// Copyright 2020 Dhiraj Wishal
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _DYNAMIK_SYSTEM_H
#define _DYNAMIK_SYSTEM_H

#include "Core/Macros/Global.h"

namespace Dynamik
{
    /*
     The Dynamik Engine is made up of multiple systems interconnected to work with each other. All these systems
     are centrally managed by either the Engine or by the Studio.
     These systems are:
     * Rendering Engine
     * Physics Engine
     * Audio Engine
     * Destruction Engine
     * Game Mechanic Interpreter
     * Game Component Manager
    */
    class DMK_API DMKSystem {
    public:
        DMKSystem() {}
        virtual ~DMKSystem() {}
    };
}

#endif // !_DYNAMIK_SYSTEM_H