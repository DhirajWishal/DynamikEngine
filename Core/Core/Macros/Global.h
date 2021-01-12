// Copyright 2020 Dhiraj Wishal
// SPDX-License-Identifier: Apache-2.0

#pragma once

#define DMK_INLINE						inline
#define DMK_FORCEINLINE					__forceinline

#define BIT_SHIFT(count)				(1 << count)

#define TYPE_NAME(type)					typeid(type).name()

#define DMK_DEFINE_UI64_HANDLE(name)	enum class name : UI64 { INVALID = 0Ui64 }