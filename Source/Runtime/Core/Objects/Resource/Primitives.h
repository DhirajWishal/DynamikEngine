// Copyright 2020 Dhiraj Wishal
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _DYNAMIK_RESOURCE_PRIMITIVES_H
#define _DYNAMIK_RESOURCE_PRIMITIVES_H

/*
 This file contains all the primitive resources in their virtual form (Vertex, Shader, Uniform).
*/
#include "Core/Math/MathTypes.h"
#include "Core/Types/Array.h"

namespace Dynamik
{
	/* Dynamik Shader Locations */
	enum class DMK_API DMKShaderLocation {
		DMK_SHADER_LOCATION_ALL,
		DMK_SHADER_LOCATION_VERTEX,
		DMK_SHADER_LOCATION_TESSELLATION,
		DMK_SHADER_LOCATION_GEOMETRY,
		DMK_SHADER_LOCATION_FRAGMENT,
		DMK_SHADER_LOCATION_COMPUTE,
		DMK_SHADER_LOCATION_ALL_GRAPHICS,

		/* Ray Tracing (Nvidia) */
		DMK_SHADER_LOCATION_RAY_GEN,
		DMK_SHADER_LOCATION_ANY_HIT,
		DMK_SHADER_LOCATION_CLOSEST_HIT,
		DMK_SHADER_LOCATION_MISS,
		DMK_SHADER_LOCATION_INTERSECTION,
		DMK_SHADER_LOCATION_CALLABLE,

		DMK_SHADER_LOCATION_TASK,
		DMK_SHADER_LOCATION_MESH,
	};

	/* Vertex */
	/* TEMPLATED CONTAINER
	 Dynamik Engine supports multiple sizes of vertex bone information types. These types are based on the number
	 of boneIDs, boneWeights the container supports. These count ranges are: 8, 16, 32, 65, 256, 512 and 1024.
	*/
	template<const UI32 BoneCount = 8>
	struct  DMK_API VertexBoneInformation {
		/*
		 Add a bone to the container.

		 @param boneID: ID of the bone.
		 @param boneWeight:	Weight of that bone ID.
		*/
		void addBone(UI32 boneID, F32 boneWeight)
		{
			for (UI32 _itr = 0; _itr < BoneCount; _itr++)
			{
				if (boneWeights[_itr] == 0.0f)
				{
					boneIDs[_itr] = boneID;
					boneWeights[_itr] = boneWeight;

					return;
				}
			}
		}

		UI32 boneIDs[BoneCount] = { 0 };
		F32 boneWeights[BoneCount] = { 0.0f };
	};

	/* Shader input attribute types */
	enum class DMK_API DMKVertexAttributeType {
		DMK_VERTEX_ATTRIBUTE_TYPE_POSITION,						/* Position coordinates */
		DMK_VERTEX_ATTRIBUTE_TYPE_NORMAL,						/* Normal vectors */
		DMK_VERTEX_ATTRIBUTE_TYPE_COLOR_0,						/* Color coordinates 0 */
		DMK_VERTEX_ATTRIBUTE_TYPE_COLOR_1,						/* Color coordinates 1 */
		DMK_VERTEX_ATTRIBUTE_TYPE_COLOR_2,						/* Color coordinates 2 */
		DMK_VERTEX_ATTRIBUTE_TYPE_COLOR_3,						/* Color coordinates 3 */
		DMK_VERTEX_ATTRIBUTE_TYPE_TEXTURE_COORDINATES_0,		/* Texture coordinates 0 */
		DMK_VERTEX_ATTRIBUTE_TYPE_TEXTURE_COORDINATES_1,		/* Texture coordinates 1 */
		DMK_VERTEX_ATTRIBUTE_TYPE_TEXTURE_COORDINATES_2,		/* Texture coordinates 2 */
		DMK_VERTEX_ATTRIBUTE_TYPE_TEXTURE_COORDINATES_3,		/* Texture coordinates 3 */
		DMK_VERTEX_ATTRIBUTE_TYPE_TEXTURE_COORDINATES_4,		/* Texture coordinates 4 */
		DMK_VERTEX_ATTRIBUTE_TYPE_TEXTURE_COORDINATES_5,		/* Texture coordinates 5 */
		DMK_VERTEX_ATTRIBUTE_TYPE_TEXTURE_COORDINATES_6,		/* Texture coordinates 6 */
		DMK_VERTEX_ATTRIBUTE_TYPE_TEXTURE_COORDINATES_7,		/* Texture coordinates 7 */
		DMK_VERTEX_ATTRIBUTE_TYPE_UV_COORDINATES,				/* UV coordinates */
		DMK_VERTEX_ATTRIBUTE_TYPE_TANGENT,						/* Tangents */
		DMK_VERTEX_ATTRIBUTE_TYPE_BITANGENT,					/* Bi tangents */
		DMK_VERTEX_ATTRIBUTE_TYPE_INTEGRITY,					/* Integrity value */
		DMK_VERTEX_ATTRIBUTE_TYPE_BONE_ID,						/* Bone IDs */
		DMK_VERTEX_ATTRIBUTE_TYPE_BONE_WEIGHT,					/* Bone Weights */
		DMK_VERTEX_ATTRIBUTE_TYPE_CUSTOM						/* Custom */
	};

	/* Dynamik Shader Input Attribute */
	struct DMK_API DMKShaderInputAttribute {
		DMKShaderInputAttribute() = default;
		virtual ~DMKShaderInputAttribute() = default;

		DMKDataType dataType = DMKDataType::DMK_DATA_TYPE_VEC3;
		UI32 dataCount = 1;     /* Number of elements of data which is sent. Used for sending arrays. */
	};

	/* Dynamik Vertex Attribute */
	struct DMK_API DMKVertexAttribute : public DMKShaderInputAttribute {
		DMKVertexAttribute() = default;
		~DMKVertexAttribute() = default;

		DMKVertexAttributeType attributeType = DMKVertexAttributeType::DMK_VERTEX_ATTRIBUTE_TYPE_POSITION;
	};

	/* This contains all the vertex attributes */
	class DMK_API DMKVertexLayout {
	public:
		DMKVertexLayout() {}
		~DMKVertexLayout() {}

		/* Get the size of the vertex using the attributes */
		const UI64 getVertexSize() const;

		/* Vertex attributes */
		ARRAY<DMKVertexAttribute> attributes;

	public:		/* Helper methods */
		/*
		 Create a basic vertex layout.
		 This contains,
		 - Position				[Vector3F * 1]
		 - Color				[Vector3F * 1]
		 - Texture Coordinates	[Vector2F * 1]
		*/
		static DMKVertexLayout createBasic();

		/*
		 Create an animated vertex layout.
		 This contains,
		 - Position				[Vector3F * 1]
		 - Normal				[Vector3F * 1]
		 - UV					[Vector2F * 1]
		 - Color				[Vector3F * 1]
		 - boneWeights			[Float * 4]
		 - boneIDs				[unsigned int * 4]
		*/
		static DMKVertexLayout createAnimated();

	public:		/* Operators */
		B1 operator==(const DMKVertexLayout& other) const;
	};

	/*
	 Dynamik Vertex Buffer
	 This object is capable of storing all the vertexes of a given mesh.

	 The best way to load data to the vertex buffer is to first allocate the buffer with the required size,
	 and then move data at once or sequentially.
	*/
	class DMK_API DMKVertexBuffer {
	public:
		DMKVertexBuffer()
			: layout({}), dataCount(0), _allocationSize(0), pDataStore(nullptr) {}
		DMKVertexBuffer(const DMKVertexLayout& layout)
			: layout(layout), dataCount(0), _allocationSize(0), pDataStore(nullptr) {}
		DMKVertexBuffer(const DMKVertexLayout& layout, const UI64& dataCount)
			: layout(layout), dataCount(dataCount), _allocationSize(0), pDataStore(nullptr) {}
		~DMKVertexBuffer() {}

		/*
		 Initialize the buffer.
		*/
		void initialize();

		/*
		 Initialize the buffer.

		 @param dataCount: The total number of vertexes the buffer will have.
		*/
		void initialize(UI64 dataCount);

		/*
		 Initialize the buffer.

		 @param dataCount: The total number of vertexes the buffer will have.
		 @param layout: The vertex layout.
		*/
		void initialize(UI64 dataCount, DMKVertexLayout layout);

		/*
		 Set the vertex layout.

		 @param layout: The vertex layout.
		*/
		void setLayout(const DMKVertexLayout& layout);

		/*
		 Get the vertex layout.
		*/
		DMKVertexLayout getLayout() const;

		/*
		 Set the data count.

		 @param count: The data count.
		*/
		void setDataCount(const UI64& count);

		/*
		 Get the data count (size).
		*/
		UI64 size() const;

		/*
		 Get the size of a single vertex (stride).
		*/
		UI64 stride() const;

		/*
		 Get the total byte size of the buffer.
		*/
		UI64 byteSize() const;

		/*
		 Clear the buffer.
		*/
		void clear();

		/*
		 Get the data pointer.
		*/
		VPTR data() const;

		/*
		 Add data to the buffer.

		 @param source: Source pointer from which to load the data.
		 @param byteCount: Byte size to copy from the source address.
		 @param offset: The offset of the buffer to which the data are added.
		*/
		void addData(const VPTR source, const UI64& byteCount, const UI64& offset);

		/*
		 Set data to the whole size of the buffer.

		 @param source: The source address of the data.
		*/
		void setData(const VPTR source);

		/*
		 Set null to a block of data in the buffer. This is used if the layout requested data are not
		 available in the input file (asset file).

		 @param byteSize: The number of bytes to be zeroed.
		 @param offset: The offset to which the zero values are set in the buffer.
		*/
		void setNull(const UI64& byteSize, const UI64& offset);

	public:		/* Buffer Data */
		/* Vertex Layout */
		DMKVertexLayout layout = {};

		/* Data Count */
		UI64 dataCount = 0;

		/* Data Store */
		VPTR pDataStore = nullptr;

	private:
		UI64 _allocationSize = 0;
	};

	/* Constant Block */
	/* Dynamik Constant Attribute */
	struct DMK_API DMKConstantAttribute {
		DMKDataType dataType = DMKDataType::DMK_DATA_TYPE_F32;
		UI64 dataCount = 1;
	};

	/* Dynamik Constant Block Descriptor */
	struct DMK_API DMKConstantBlockDescription {
		ARRAY<DMKConstantAttribute> attributes;
		DMKShaderLocation location = DMKShaderLocation::DMK_SHADER_LOCATION_VERTEX;
		UI32 offset = 0;

		/* Block size */
		UI64 getBlockSize();
	};

	/* Uniform */
	/* Dynamik Uniform Type */
	enum class DMK_API DMKUniformType {
		DMK_UNIFORM_TYPE_UNIFORM_BUFFER,
		DMK_UNIFORM_TYPE_STORAGE_BUFFER,
		DMK_UNIFORM_TYPE_UNIFORM_BUFFER_DYNAMIC,
		DMK_UNIFORM_TYPE_STORAGE_BUFFER_DYNAMIC,
		DMK_UNIFORM_TYPE_UNIFORM_TEXEL_BUFFER,
		DMK_UNIFORM_TYPE_STORAGE_TEXEL_BUFFER,
		DMK_UNIFORM_TYPE_INPUT_ATTACHMENT,
		DMK_UNIFORM_TYPE_STORAGE_IMAGE,
		DMK_UNIFORM_TYPE_CONSTANT,
		DMK_UNIFORM_TYPE_SAMPLER_2D,
		DMK_UNIFORM_TYPE_SAMPLER_CUBE,
		DMK_UNIFORM_TYPE_SAMPLER_2D_ARRAY,
		DMK_UNIFORM_TYPE_SAMPLER_CUBE_ARRAY,
		DMK_UNIFORM_TYPE_ACCELERATION_STRUCTURE,
	};

	/* Dynamik Uniform Attribute Types */
	enum class DMK_API DMKUniformAttributeType {
		DMK_UNIFORM_ATTRIBUTE_TYPE_MODEL,
		DMK_UNIFORM_ATTRIBUTE_TYPE_VIEW,
		DMK_UNIFORM_ATTRIBUTE_TYPE_PROJECTION,
		DMK_UNIFORM_ATTRIBUTE_TYPE_CUSTOM
	};

	/* Dynamik Uniform Buffer Usage */
	enum class DMK_API DMKUniformBufferUsage {
		DMK_UNIFORM_BUFFER_USAGE_CAMERA,
		DMK_UNIFORM_BUFFER_USAGE_SAMPLER,
		DMK_UNIFORM_BUFFER_USAGE_MODEL,
		DMK_UNIFORM_BUFFER_USAGE_CUSTOM,
	};

	/* Dynamik Uniform Attribute */
	struct DMK_API DMKUniformAttribute {
		DMKDataType dataType = DMKDataType::DMK_DATA_TYPE_MAT4;
		DMKUniformAttributeType attributeType = DMKUniformAttributeType::DMK_UNIFORM_ATTRIBUTE_TYPE_MODEL;
		UI32 dataCount = 1;     /* Number of data elements that is sent to the shader. Used when sending an array. */
	};

	/*
	 Dynamik Uniform Description
	 This object contains all the necessary information to create a uniform buffer object in the Dynamik Engine.
	*/
	struct DMK_API DMKUniformDescription {
		ARRAY<DMKUniformAttribute> attributes;
		DMKUniformType type = DMKUniformType::DMK_UNIFORM_TYPE_UNIFORM_BUFFER;
		DMKShaderLocation shaderLocation = DMKShaderLocation::DMK_SHADER_LOCATION_VERTEX;
		DMKUniformBufferUsage usage = DMKUniformBufferUsage::DMK_UNIFORM_BUFFER_USAGE_CUSTOM;
		UI32 destinationBinding = 0;
		UI32 offset = 0;

		/* Get the total byte size of the uniform buffer object. */
		UI64 getUniformSize();
	};

	/* Dynamik Uniform Descriptor */
	class DMK_API DMKUniformBufferDescriptor {
	public:
		DMKUniformBufferDescriptor() {}
		~DMKUniformBufferDescriptor() {}

		/* Buffer object container */
		ARRAY<DMKUniformDescription> uniformBufferObjects;

		/* ID operator */
		I64 operator()();
	};

	/*
	 Dynamik Uniform Buffer Object
	 Uniform data are passed to the Dynamik Engine using this object.
	 */
	class DMK_API DMKUniformBufferObject {
	public:
		DMKUniformBufferObject() {}
		~DMKUniformBufferObject();

		/* Initialize the object */
		void initialize(const DMKUniformDescription& description);

		/*
		 Add data to the uniform buffer object.

		 @param data: The data to be added to the buffer.
		 @param byteSize: Size of the added data.
		 @param location: The location to which the data is bound. This location corresponds to the uniform
						  binding.
		 @param arrayIndex: Index to which the data are bound in the array. If the buffer element at the
							location does not contain an array, the index is kept 0.
		*/
		void setData(const VPTR& data, const UI32& byteSize, const UI32& location, const UI32& arrayIndex = 0U);

		/* Clear all the stored values in the buffer */
		void clear();

	private:	/* Private Data Store */
		VPTR uniformBufferStorage = nullptr;
		BYTE* nextPointer = (BYTE*)uniformBufferStorage;

	public:		/* Public Data */
		DMKUniformDescription myDescription;

	public:		/* Static Utility Functions */
		/*
		 Create a basic Camera uniform buffer object.
		*/
		static DMKUniformDescription createUniformCamera(UI32 binding = 0, DMKShaderLocation location = DMKShaderLocation::DMK_SHADER_LOCATION_VERTEX);

		/*
		 Create a basic model uniform buffer object.
		*/
		static DMKUniformDescription createUniformModel(UI32 binding = 0, DMKShaderLocation location = DMKShaderLocation::DMK_SHADER_LOCATION_VERTEX);
	};
}

#endif // !_DYNAMIK_RESOURCE_PRIMITIVES_H