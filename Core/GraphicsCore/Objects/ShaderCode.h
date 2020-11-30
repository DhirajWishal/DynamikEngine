// Copyright 2020 Dhiraj Wishal
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "Uniform.h"

namespace DMK
{
	namespace GraphicsCore
	{
		/**
		 * Shader Code Type enum.
		 */
		enum class ShaderCodeType : UI8 {
			UNDEFINED,
			SPIR_V,
			GLSL,
			HLSL,
		};

		/**
		 * Shader Location enum.
		 */
		enum class ShaderLocation : UI8 {
			ALL,
			VERTEX,
			TESSELLATION,
			GEOMETRY,
			FRAGMENT,
			COMPUTE,
			ALL_GRAPHICS,
			RAY_GEN,
			ANY_HIT,
			CLOSEST_HIT,
			MISS,
			INTERSECTION,
			CALLABLE,
			TASK,
			MESH
		};

		/**
		 * Shader Attribute structure.
		 * This structure contains information about a single shader attribute (input or output).
		 */
		struct ShaderAttribute {
			String mName = {};	// The name of the attribute.
			UI64 mLocation = 0;	// Location of the attribute.
			UI64 mLayerCount = 1;	// Array count.
			DataType mDataType = DataType::UNDEFINED;	// Attribute type.

			/**
			 * Get the type size using the enum.
			 *
			 * @return The size in bytes.
			 */
			UI64 GetTypeSize() const { return static_cast<UI8>(mDataType); }

			/**
			 * Get the size of the attribute in bytes.
			 *
			 * @return The size of the attribute in bytes.
			 */
			UI64 Size() const { return GetTypeSize() * mLayerCount; }
		};

		/**
		 * Shader Code object.
		 * This object stores code and will help in reflection and transpiling.
		 */
		class ShaderCode {
		public:
			/**
			 * Default constructor.
			 */
			ShaderCode() {}

			/**
			 * Default destructor.
			 */
			~ShaderCode() {}

			/**
			 * Get the shader code type.
			 *
			 * @return ShaderCodeType enum.
			 */
			ShaderCodeType GetCodeType() const { return mType; }

			/**
			 * Get the shader location.
			 *
			 * @return ShaderLocation enum.
			 */
			ShaderLocation GetLocation() const { return mLocation; }

			/**
			 * Load shader code from a file.
			 *
			 * @param pAsset: The asset path.
			 * @param mType: Shader code type.
			 * @param mLocation: The location of the shader in the pipeline.
			 */
			void LoadCode(const char* pAsset, ShaderCodeType mType, ShaderLocation mLocation);

			/**
			 * Perform reflection over the shader code.
			 * Reflection identifies the uniforms, inputs and other attributes present in the shader.
			 */
			void PerformReflection();

		public:
			std::vector<GraphicsCore::Uniform> mUniforms;	// All the uniforms the shader had.
			std::vector<GraphicsCore::ShaderAttribute> mInputAttributes;	// All the input attributes.
			std::vector<GraphicsCore::ShaderAttribute> mOutputAttributes;	// All the output attributes.

		public:
			std::vector<UI32> mShaderCode;	// Shader code vector.
			ShaderCodeType mType = ShaderCodeType::UNDEFINED;	// Shader code type.
			ShaderLocation mLocation = ShaderLocation::ALL;	// Shader location.
		};
	}
}