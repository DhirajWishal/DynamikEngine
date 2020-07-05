// Copyright 2020 Dhiraj Wishal
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _DYNAMIK_DATA_TYPES_H
#define _DYNAMIK_DATA_TYPES_H

#include <cstdint>
#include <string>
#include "Core/Macros/Global.h"

/*
 Dynamik Data Types.
 This file contains all the necessary data types and enums used in the Dynamik Engine.
*/

namespace Dynamik
{
	typedef unsigned char UI8;			// unsigned 8 bit integer
	typedef unsigned short UI16;		// unsigned 16 bit integer
	typedef unsigned int UI32;			// unsigned 32 bit integer
	typedef unsigned long long UI64;	// unsigned 64 bit integer

	typedef signed char SI8;			// signed 8 bit integer
	typedef signed short SI16;			// signed 16 bit integer
	typedef signed int SI32;			// signed 32 bit integer
	typedef signed long long SI64;		// signed 64 bit integer

	typedef signed char I8;				// 8 bit integer
	typedef short I16;					// 16 bit integer
	typedef int I32;					// 32 bit integer
	typedef long long I64;				// 64 bit integer

	typedef bool B1;					// bool
	typedef float F32;					// float
	typedef double D64;					// double
	typedef long double LD64;			// long double

#ifdef DMK_USE_WIDE_CHAR
#define DMK_TEXT(text)  Ltext# 
	typedef std::wstring STRING;
	typedef wchar_t CHR;				// wide char (UTF-8)

#else
#define DMK_TEXT(text)  text
	typedef std::string STRING;
	typedef char CHR;					// char (ASCII)

#endif // DMK_USE_WIDE_CHR

	typedef void* VPTR;					// void pointer
	typedef CHR* CPTR;					// char pointer
	typedef const CHR* CCPTR;			// const char pointer
	typedef unsigned char* UCPTR;		// unsigned char pointer
	typedef const wchar_t* CWCPTR;		// const char pointer

	typedef UI8 BYTE;					// byte of data
	typedef BYTE* BPTR;					// byte pointer

	typedef unsigned char UCHR;			// unsigned char 

	/*
	 This contains all the virtual data types for the Dynamik Engine.
	*/
	enum class DMK_API DMKDataType : UI32 {
		DMK_DATA_TYPE_UNDEFINED = 0,	// Undefined type

		DMK_DATA_TYPE_BYTE = 1,			// Translates to one byte

		DMK_DATA_TYPE_I8 = 1,			// Translates to 8 bit integer
		DMK_DATA_TYPE_UI8 = 1,			// Translates to 8 bit unsigned integer
		DMK_DATA_TYPE_SI8 = 1,			// Translates to 8 bit signed integer
		DMK_DATA_TYPE_I16 = 2,			// Translates to 16 bit integer
		DMK_DATA_TYPE_UI16 = 2,			// Translates to 16 bit unsigned integer
		DMK_DATA_TYPE_SI16 = 2,			// Translates to 16 bit signed integer
		DMK_DATA_TYPE_I32 = 4,			// Translates to 32 bit integer
		DMK_DATA_TYPE_UI32 = 4,			// Translates to 32 bit unsigned integer
		DMK_DATA_TYPE_SI32 = 4,			// Translates to 32 bit signed integer
		DMK_DATA_TYPE_I64 = 8,			// Translates to 64 bit integer
		DMK_DATA_TYPE_UI64 = 8,			// Translates to 64 bit unsigned integer
		DMK_DATA_TYPE_SI64 = 8,			// Translates to 64 bit signed integer

		DMK_DATA_TYPE_F32 = 4,			// Translates to 32 bit float
		DMK_DATA_TYPE_F64 = 4,			// Translates to 64 bit float
		DMK_DATA_TYPE_D64 = 8,			// Translates to 64 bit double
		DMK_DATA_TYPE_CHR = 1,			// Translates to 8 bit char

		DMK_DATA_TYPE_PTR = 8,			// Translates to 8 bit pointer (x64 architecture)
		DMK_DATA_TYPE_VPTR = 8,			// Translates to 8 bit void pointer (x64 architecture)

		DMK_DATA_TYPE_VEC1 = 4,			// Translates to 4 bit vector (1 float)
		DMK_DATA_TYPE_VEC2 = 8,			// Translates to 8 bit vector (2 floats)
		DMK_DATA_TYPE_VEC3 = 16,		// Translates to 16 bit vector (3 floats)
		DMK_DATA_TYPE_VEC4 = 16,		// Translates to 16 bit vector (4 floats)
		DMK_DATA_TYPE_UVEC1 = 4,		// Translates to 4 bit unsigned vector (1 float)
		DMK_DATA_TYPE_UVEC2 = 8,		// Translates to 8 bit unsigned vector (2 floats)
		DMK_DATA_TYPE_UVEC3 = 16,		// Translates to 16 bit unsigned vector (3 floats)
		DMK_DATA_TYPE_UVEC4 = 16,		// Translates to 16 bit unsigned vector (4 floats)
		DMK_DATA_TYPE_SVEC1 = 4,		// Translates to 4 bit signed vector (1 float)
		DMK_DATA_TYPE_SVEC2 = 8,		// Translates to 8 bit signed vector (2 floats)
		DMK_DATA_TYPE_SVEC3 = 16,		// Translates to 16 bit signed vector (3 floats)
		DMK_DATA_TYPE_SVEC4 = 16,		// Translates to 16 bit signed vector (4 floats)
		DMK_DATA_TYPE_BVEC1 = 1,		// Translates to 4 bit boolean vector (1 float)
		DMK_DATA_TYPE_BVEC2 = 2,		// Translates to 8 bit boolean vector (2 floats)
		DMK_DATA_TYPE_BVEC3 = 3,		// Translates to 16 bit boolean vector (3 floats)
		DMK_DATA_TYPE_BVEC4 = 4,		// Translates to 16 bit boolean vector (4 floats)

		DMK_DATA_TYPE_MAT2 = 16,		// Translates to 16 bit matrix (2x2)
		DMK_DATA_TYPE_MAT3 = 36,		// Translates to 36 bit matrix (3x3)
		DMK_DATA_TYPE_MAT4 = 64,		// Translates to 64 bit matrix (4x4)
	};

	/*
	 This contains all the formats supported by the Dynamik Engine.
	 R = Red,
	 G = Green,
	 B = Blue,
	 A = Alpha
	*/
	enum class DMK_API DMKFormat : UI32 {
		DMK_FORMAT_UNDEFINED,			// Undefined format

		DMK_FORMAT_RG_4_UNORMAL_PACK8,
		DMK_FORMAT_RGBA_4_UNORMAL_PACK16,
		DMK_FORMAT_BGRA_4_UNORMAL_PACK16,
		DMK_FORMAT_R5G6B5_UNORMAL_PACK16,
		DMK_FORMAT_B5G6R5_UNORMAL_PACK16,
		DMK_FORMAT_R5G6B5A1_UNORMAL_PACK16,
		DMK_FORMAT_B5G6R5A1_UNORMAL_PACK16,
		DMK_FORMAT_A1B5G6R5_UNORMAL_PACK16,

		DMK_FORMAT_R_8_UNORMAL,
		DMK_FORMAT_R_8_SNORMAL,
		DMK_FORMAT_R_8_USCALED,
		DMK_FORMAT_R_8_SSCALED,
		DMK_FORMAT_R_8_UINT,
		DMK_FORMAT_R_8_SINT,
		DMK_FORMAT_R_8_SRGB,

		DMK_FORMAT_RG_8_UNORMAL,
		DMK_FORMAT_RG_8_SNORMAL,
		DMK_FORMAT_RG_8_USCALED,
		DMK_FORMAT_RG_8_SSCALED,
		DMK_FORMAT_RG_8_UINT,
		DMK_FORMAT_RG_8_SINT,
		DMK_FORMAT_RG_8_SRGB,

		DMK_FORMAT_RGB_8_UNORMAL,
		DMK_FORMAT_RGB_8_SNORMAL,
		DMK_FORMAT_RGB_8_USCALED,
		DMK_FORMAT_RGB_8_SSCALED,
		DMK_FORMAT_RGB_8_UINT,
		DMK_FORMAT_RGB_8_SINT,
		DMK_FORMAT_RGB_8_SRGB,

		DMK_FORMAT_BGR_8_UNORMAL,
		DMK_FORMAT_BGR_8_SNORMAL,
		DMK_FORMAT_BGR_8_USCALED,
		DMK_FORMAT_BGR_8_SSCALED,
		DMK_FORMAT_BGR_8_UINT,
		DMK_FORMAT_BGR_8_SINT,
		DMK_FORMAT_BGR_8_SRGB,

		DMK_FORMAT_RGBA_8_UNORMAL,
		DMK_FORMAT_RGBA_8_SNORMAL,
		DMK_FORMAT_RGBA_8_USCALED,
		DMK_FORMAT_RGBA_8_SSCALED,
		DMK_FORMAT_RGBA_8_UINT,
		DMK_FORMAT_RGBA_8_SINT,
		DMK_FORMAT_RGBA_8_SRGB,

		DMK_FORMAT_BGRA_8_UNORMAL,
		DMK_FORMAT_BGRA_8_SNORMAL,
		DMK_FORMAT_BGRA_8_USCALED,
		DMK_FORMAT_BGRA_8_SSCALED,
		DMK_FORMAT_BGRA_8_UINT,
		DMK_FORMAT_BGRA_8_SINT,
		DMK_FORMAT_BGRA_8_SRGB,

		DMK_FORMAT_RGBA_8_UNORMAL_PACK32,
		DMK_FORMAT_RGBA_8_SNORMAL_PACK32,
		DMK_FORMAT_RGBA_8_USCALED_PACK32,
		DMK_FORMAT_RGBA_8_SSCALED_PACK32,
		DMK_FORMAT_RGBA_8_UINT_PACK32,
		DMK_FORMAT_RGBA_8_SINT_PACK32,
		DMK_FORMAT_RGBA_8_SRGB_PACK32,

		DMK_FORMAT_A2R10G10B10_UNORMAL_PACK32,
		DMK_FORMAT_A2R10G10B10_SNORMAL_PACK32,
		DMK_FORMAT_A2R10G10B10_USCALED_PACK32,
		DMK_FORMAT_A2R10G10B10_SSCALED_PACK32,
		DMK_FORMAT_A2R10G10B10_UINT_PACK32,
		DMK_FORMAT_A2R10G10B10_SINT_PACK32,

		DMK_FORMAT_A2B10G10R10_UNORMAL_PACK32,
		DMK_FORMAT_A2B10G10R10_SNORMAL_PACK32,
		DMK_FORMAT_A2B10G10R10_USCALED_PACK32,
		DMK_FORMAT_A2B10G10R10_SSCALED_PACK32,
		DMK_FORMAT_A2B10G10R10_UINT_PACK32,
		DMK_FORMAT_A2B10G10R10_SINT_PACK32,

		DMK_FORMAT_R_16_UNORMAL,
		DMK_FORMAT_R_16_SNORMAL,
		DMK_FORMAT_R_16_USCALED,
		DMK_FORMAT_R_16_SSCALED,
		DMK_FORMAT_R_16_UINT,
		DMK_FORMAT_R_16_SINT,
		DMK_FORMAT_R_16_SF32,

		DMK_FORMAT_RG_16_UNORMAL,
		DMK_FORMAT_RG_16_SNORMAL,
		DMK_FORMAT_RG_16_USCALED,
		DMK_FORMAT_RG_16_SSCALED,
		DMK_FORMAT_RG_16_UINT,
		DMK_FORMAT_RG_16_SINT,
		DMK_FORMAT_RG_16_SF32,

		DMK_FORMAT_RGB_16_UNORMAL,
		DMK_FORMAT_RGB_16_SNORMAL,
		DMK_FORMAT_RGB_16_USCALED,
		DMK_FORMAT_RGB_16_SSCALED,
		DMK_FORMAT_RGB_16_UINT,
		DMK_FORMAT_RGB_16_SINT,
		DMK_FORMAT_RGB_16_SF32,

		DMK_FORMAT_RGBA_16_UNORMAL,
		DMK_FORMAT_RGBA_16_SNORMAL,
		DMK_FORMAT_RGBA_16_USCALED,
		DMK_FORMAT_RGBA_16_SSCALED,
		DMK_FORMAT_RGBA_16_UINT,
		DMK_FORMAT_RGBA_16_SINT,
		DMK_FORMAT_RGBA_16_SF32,

		DMK_FORMAT_R_32_UINT,
		DMK_FORMAT_R_32_SINT,
		DMK_FORMAT_R_32_SF32,

		DMK_FORMAT_RG_32_UINT,
		DMK_FORMAT_RG_32_SINT,
		DMK_FORMAT_RG_32_SF32,

		DMK_FORMAT_RGB_32_UINT,
		DMK_FORMAT_RGB_32_SINT,
		DMK_FORMAT_RGB_32_SF32,

		DMK_FORMAT_RGBA_32_UINT,
		DMK_FORMAT_RGBA_32_SINT,
		DMK_FORMAT_RGBA_32_SF32,

		DMK_FORMAT_R_64_UINT,
		DMK_FORMAT_R_64_SINT,
		DMK_FORMAT_R_64_SF32,

		DMK_FORMAT_RG_64_UINT,
		DMK_FORMAT_RG_64_SINT,
		DMK_FORMAT_RG_64_SF32,

		DMK_FORMAT_RGB_64_UINT,
		DMK_FORMAT_RGB_64_SINT,
		DMK_FORMAT_RGB_64_SF32,

		DMK_FORMAT_RGBA_64_UINT,
		DMK_FORMAT_RGBA_64_SINT,
		DMK_FORMAT_RGBA_64_SF32,

		DMK_FORMAT_B10G11R11_UFLOAT_PACK32,
		DMK_FORMAT_E5B9G9R9_UFLOAT_PACK32,

		DMK_FORMAT_D_16_UNORM,
		DMK_FORMAT_X8_D_24_UNORM_PACK32,
		DMK_FORMAT_D_32_SF32,
		DMK_FORMAT_S_8_UINT,
		DMK_FORMAT_D_16_UNORM_S_8_UINT,
		DMK_FORMAT_D_24_UNORM_S_8_UINT,
		DMK_FORMAT_D_32_SF32_S_8_UINT,

		DMK_FORMAT_BC1_RGB_UNORM_BLOCK,
		DMK_FORMAT_BC1_RGB_SRGB_BLOCK,
		DMK_FORMAT_BC1_RGBA_UNORM_BLOCK,
		DMK_FORMAT_BC1_RGBA_SRGB_BLOCK,

		DMK_FORMAT_BC2_UNORM_BLOCK,
		DMK_FORMAT_BC2_SRGB_BLOCK,
		DMK_FORMAT_BC3_UNORM_BLOCK,
		DMK_FORMAT_BC3_SRGB_BLOCK,

		DMK_FORMAT_BC4_UNORM_BLOCK,
		DMK_FORMAT_BC4_SNORM_BLOCK,

		DMK_FORMAT_BC5_UNORM_BLOCK,
		DMK_FORMAT_BC5_SNORM_BLOCK,

		DMK_FORMAT_BC6H_UFLOAT_BLOCK,
		DMK_FORMAT_BC6H_SFLOAT_BLOCK,

		DMK_FORMAT_BC7_UNORM_BLOCK,
		DMK_FORMAT_BC7_SRGB_BLOCK,

		DMK_FORMAT_ETC2_RGB_8_UNORM_BLOCK,
		DMK_FORMAT_ETC2_RGB_8_SRGB_BLOCK,

		DMK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK,
		DMK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK,

		DMK_FORMAT_ETC2_RGBA_8_UNORM_BLOCK,
		DMK_FORMAT_ETC2_RGBA_8_SRGB_BLOCK,

		DMK_FORMAT_EAC_R_11_UNORM_BLOCK,
		DMK_FORMAT_EAC_R_11_SNORM_BLOCK,

		DMK_FORMAT_EAC_RG_11_UNORM_BLOCK,
		DMK_FORMAT_EAC_RG_11_SNORM_BLOCK,

		DMK_FORMAT_ASTC_4x4_UNORM_BLOCK,
		DMK_FORMAT_ASTC_4x4_SRGB_BLOCK,

		DMK_FORMAT_ASTC_5x4_UNORM_BLOCK,
		DMK_FORMAT_ASTC_5x4_SRGB_BLOCK,

		DMK_FORMAT_ASTC_5x5_UNORM_BLOCK,
		DMK_FORMAT_ASTC_5x5_SRGB_BLOCK,

		DMK_FORMAT_ASTC_6x5_UNORM_BLOCK,
		DMK_FORMAT_ASTC_6x5_SRGB_BLOCK,

		DMK_FORMAT_ASTC_6x6_UNORM_BLOCK,
		DMK_FORMAT_ASTC_6x6_SRGB_BLOCK,

		DMK_FORMAT_ASTC_8x5_UNORM_BLOCK,
		DMK_FORMAT_ASTC_8x5_SRGB_BLOCK,

		DMK_FORMAT_ASTC_8x6_UNORM_BLOCK,
		DMK_FORMAT_ASTC_8x6_SRGB_BLOCK,

		DMK_FORMAT_ASTC_8x8_UNORM_BLOCK,
		DMK_FORMAT_ASTC_8x8_SRGB_BLOCK,

		DMK_FORMAT_ASTC_10x5_UNORM_BLOCK,
		DMK_FORMAT_ASTC_10x5_SRGB_BLOCK,

		DMK_FORMAT_ASTC_10x6_UNORM_BLOCK,
		DMK_FORMAT_ASTC_10x6_SRGB_BLOCK,

		DMK_FORMAT_ASTC_10x8_UNORM_BLOCK,
		DMK_FORMAT_ASTC_10x8_SRGB_BLOCK,

		DMK_FORMAT_ASTC_10x10_UNORM_BLOCK,
		DMK_FORMAT_ASTC_10x10_SRGB_BLOCK,

		DMK_FORMAT_ASTC_12x10_UNORM_BLOCK,
		DMK_FORMAT_ASTC_12x10_SRGB_BLOCK,

		DMK_FORMAT_ASTC_12x12_UNORM_BLOCK,
		DMK_FORMAT_ASTC_12x12_SRGB_BLOCK,
	};
}

#endif // !_DYNAMIK_DATA_TYPES_H
