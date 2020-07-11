// Copyright 2020 Dhiraj Wishal
// SPDX-License-Identifier: Apache-2.0

#include "dmkafx.h"
#include "AssimpWrapper.h"

#include "Animation/Objects/AnimNodeGraph.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Dynamik
{
	/*
	 Assimp Importer object.
	*/
	Assimp::Importer localImporter;

	/*
	 Return the correct readable size of a given container.
	*/
	template<class TYPE>
	UI64 getCorrectSize(const UI64 size)
	{
		if (size < sizeof(TYPE))
			return size;

		return sizeof(TYPE);
	}

	/*
	 Load data from an aiMatrix4x4 object to a Matrix4F object.

	 @param mat: The aiMatrix4x4 object.
	*/
	Matrix4F aiMatrixToMatrix4F(const aiMatrix4x4& mat)
	{
		return Matrix4F(
			mat.a1, mat.a2, mat.a3, mat.a4,
			mat.b1, mat.b2, mat.b3, mat.b4,
			mat.c1, mat.c2, mat.c3, mat.c4,
			mat.d1, mat.d2, mat.d3, mat.d4);
	}

	/*
	 Load data from aiVectorKey to VectorFrame object.

	 @param key: aiVectorKey object.
	*/
	AVectorFrame aiVectorKeyToVectorFrame(const aiVectorKey& key)
	{
		return AVectorFrame(key.mValue.x, key.mValue.y, key.mValue.z);
	}

	/*
	 Load data from aiQuatKey to QuatFrame object.

	 @param key: aiQuatKey object.
	*/
	AQuatFrame aiQuatKeyToQuatFrame(const aiQuatKey& key)
	{
		return AQuatFrame(key.mValue.x, key.mValue.y, key.mValue.z, key.mValue.w);
	}

	DMKStaticMeshComponent AssimpWrapper::loadMeshComponent(VPTR pAiMeshObject, const DMKVertexLayout& vertexLayout)
	{
		auto _mesh = (aiMesh*)pAiMeshObject;

		DMKStaticMeshComponent _meshComponent;
		_meshComponent.vertexBuffer.initialize(_mesh->mNumVertices, vertexLayout);
		UI64 vertexOffset = 0;
		UI64 dataSize = 0;

		for (UI32 _index = 0; _index < _mesh->mNumVertices; _index++)
		{
			for (auto attribute : vertexLayout.attributes)
			{
				dataSize = (UI64)attribute.dataType * attribute.dataCount;

				switch (attribute.attributeType)
				{
				case Dynamik::DMKVertexAttributeType::DMK_VERTEX_ATTRIBUTE_TYPE_POSITION:
					if (_mesh->HasPositions())
						_meshComponent.vertexBuffer.addData(&_mesh->mVertices[_index], getCorrectSize<aiVector3D>(dataSize), vertexOffset);
					else
						_meshComponent.vertexBuffer.setNull(dataSize, vertexOffset);
					break;

				case Dynamik::DMKVertexAttributeType::DMK_VERTEX_ATTRIBUTE_TYPE_NORMAL:
					if (_mesh->HasNormals())
						_meshComponent.vertexBuffer.addData(&_mesh->mNormals[_index], getCorrectSize<aiVector3D>(dataSize), vertexOffset);
					else
						_meshComponent.vertexBuffer.setNull(dataSize, vertexOffset);
					break;

				case Dynamik::DMKVertexAttributeType::DMK_VERTEX_ATTRIBUTE_TYPE_COLOR_0:
					if (_mesh->HasVertexColors(0))
						_meshComponent.vertexBuffer.addData(&_mesh->mColors[0][_index], getCorrectSize<aiColor4D>(dataSize), vertexOffset);
					else
						_meshComponent.vertexBuffer.setNull(dataSize, vertexOffset);
					break;

				case Dynamik::DMKVertexAttributeType::DMK_VERTEX_ATTRIBUTE_TYPE_COLOR_1:
					if (_mesh->HasVertexColors(1))
						_meshComponent.vertexBuffer.addData(&_mesh->mColors[1][_index], getCorrectSize<aiColor4D>(dataSize), vertexOffset);
					else
						_meshComponent.vertexBuffer.setNull(dataSize, vertexOffset);
					break;

				case Dynamik::DMKVertexAttributeType::DMK_VERTEX_ATTRIBUTE_TYPE_COLOR_2:
					if (_mesh->HasVertexColors(2))
						_meshComponent.vertexBuffer.addData(&_mesh->mColors[2][_index], getCorrectSize<aiColor4D>(dataSize), vertexOffset);
					else
						_meshComponent.vertexBuffer.setNull(dataSize, vertexOffset);
					break;

				case Dynamik::DMKVertexAttributeType::DMK_VERTEX_ATTRIBUTE_TYPE_COLOR_3:
					if (_mesh->HasVertexColors(3))
						_meshComponent.vertexBuffer.addData(&_mesh->mColors[3][_index], getCorrectSize<aiColor4D>(dataSize), vertexOffset);
					else
						_meshComponent.vertexBuffer.setNull(dataSize, vertexOffset);
					break;

				case Dynamik::DMKVertexAttributeType::DMK_VERTEX_ATTRIBUTE_TYPE_TEXTURE_COORDINATES_0:
					if (_mesh->HasTextureCoords(0))
						_meshComponent.vertexBuffer.addData(&_mesh->mTextureCoords[0][_index], getCorrectSize<aiVector3D>(dataSize), vertexOffset);
					else
						_meshComponent.vertexBuffer.setNull(dataSize, vertexOffset);
					break;

				case Dynamik::DMKVertexAttributeType::DMK_VERTEX_ATTRIBUTE_TYPE_TEXTURE_COORDINATES_1:
					if (_mesh->HasTextureCoords(1))
						_meshComponent.vertexBuffer.addData(&_mesh->mTextureCoords[1][_index], getCorrectSize<aiVector3D>(dataSize), vertexOffset);
					else
						_meshComponent.vertexBuffer.setNull(dataSize, vertexOffset);
					break;

				case Dynamik::DMKVertexAttributeType::DMK_VERTEX_ATTRIBUTE_TYPE_TEXTURE_COORDINATES_2:
					if (_mesh->HasTextureCoords(2))
						_meshComponent.vertexBuffer.addData(&_mesh->mTextureCoords[2][_index], getCorrectSize<aiVector3D>(dataSize), vertexOffset);
					else
						_meshComponent.vertexBuffer.setNull(dataSize, vertexOffset);
					break;

				case Dynamik::DMKVertexAttributeType::DMK_VERTEX_ATTRIBUTE_TYPE_TEXTURE_COORDINATES_3:
					if (_mesh->HasTextureCoords(3))
						_meshComponent.vertexBuffer.addData(&_mesh->mTextureCoords[3][_index], getCorrectSize<aiVector3D>(dataSize), vertexOffset);
					else
						_meshComponent.vertexBuffer.setNull(dataSize, vertexOffset);
					break;

				case Dynamik::DMKVertexAttributeType::DMK_VERTEX_ATTRIBUTE_TYPE_TEXTURE_COORDINATES_4:
					if (_mesh->HasTextureCoords(4))
						_meshComponent.vertexBuffer.addData(&_mesh->mTextureCoords[4][_index], getCorrectSize<aiVector3D>(dataSize), vertexOffset);
					else
						_meshComponent.vertexBuffer.setNull(dataSize, vertexOffset);
					break;

				case Dynamik::DMKVertexAttributeType::DMK_VERTEX_ATTRIBUTE_TYPE_TEXTURE_COORDINATES_5:
					if (_mesh->HasTextureCoords(5))
						_meshComponent.vertexBuffer.addData(&_mesh->mTextureCoords[5][_index], getCorrectSize<aiVector3D>(dataSize), vertexOffset);
					else
						_meshComponent.vertexBuffer.setNull(dataSize, vertexOffset);
					break;

				case Dynamik::DMKVertexAttributeType::DMK_VERTEX_ATTRIBUTE_TYPE_TEXTURE_COORDINATES_6:
					if (_mesh->HasTextureCoords(6))
						_meshComponent.vertexBuffer.addData(&_mesh->mTextureCoords[6][_index], getCorrectSize<aiVector3D>(dataSize), vertexOffset);
					else
						_meshComponent.vertexBuffer.setNull(dataSize, vertexOffset);
					break;

				case Dynamik::DMKVertexAttributeType::DMK_VERTEX_ATTRIBUTE_TYPE_TEXTURE_COORDINATES_7:
					if (_mesh->HasTextureCoords(7))
						_meshComponent.vertexBuffer.addData(&_mesh->mTextureCoords[7][_index], getCorrectSize<aiVector3D>(dataSize), vertexOffset);
					else
						_meshComponent.vertexBuffer.setNull(dataSize, vertexOffset);
					break;

				case Dynamik::DMKVertexAttributeType::DMK_VERTEX_ATTRIBUTE_TYPE_TANGENT:
					break;

				case Dynamik::DMKVertexAttributeType::DMK_VERTEX_ATTRIBUTE_TYPE_BITANGENT:
					break;

				case Dynamik::DMKVertexAttributeType::DMK_VERTEX_ATTRIBUTE_TYPE_UV_COORDINATES:
					break;

				case Dynamik::DMKVertexAttributeType::DMK_VERTEX_ATTRIBUTE_TYPE_INTEGRITY:
					break;

				case Dynamik::DMKVertexAttributeType::DMK_VERTEX_ATTRIBUTE_TYPE_BONE_ID:
					break;

				case Dynamik::DMKVertexAttributeType::DMK_VERTEX_ATTRIBUTE_TYPE_BONE_WEIGHT:
					break;

				case Dynamik::DMKVertexAttributeType::DMK_VERTEX_ATTRIBUTE_TYPE_CUSTOM:
					break;

				default:
					DMK_ERROR_BOX("Invalid vertex attribute!");
					break;
				}

				vertexOffset += dataSize;
			}
		}

		aiFace face;
		for (UI32 index = 0; index < _mesh->mNumFaces; index++)
		{
			face = _mesh->mFaces[index];
			for (UI32 itr = 0; itr < face.mNumIndices; itr++)
				_meshComponent.indexBuffer.pushBack(face.mIndices[itr]);
		}

		_meshComponent.indexCount = _meshComponent.indexBuffer.size();

		return _meshComponent;
	}

	DMK_FORCEINLINE void readAllNodes(aiNode* pNode, ARRAY<DMKAnimNode>* pAnimNodes, const DMKAnimNode* pParentNode)
	{
		DMKAnimNode node(0, pNode->mName.C_Str(), aiMatrixToMatrix4F(pNode->mTransformation));
		node.pParentNode = (DMKAnimNode*)pParentNode;
		pAnimNodes->pushBack(node);

		auto makeRoot = pAnimNodes->location(-1);
		for (UI32 i = 0; i < pNode->mNumChildren; i++)
			readAllNodes(pNode->mChildren[i], pAnimNodes, makeRoot);
	}

	DMK_FORCEINLINE DMKAnimKeyFrame getAnimationFrame(aiNodeAnim* pNodeAnim)
	{
		DMKAnimKeyFrame frame;

		for (UI32 transformIndex = 0; transformIndex < pNodeAnim->mNumPositionKeys; transformIndex++)
		{
			DMKAnimNodeTransform transform(
				aiVectorKeyToVectorFrame(pNodeAnim->mPositionKeys[transformIndex]),
				aiQuatKeyToQuatFrame(pNodeAnim->mRotationKeys[transformIndex]),
				aiVectorKeyToVectorFrame(pNodeAnim->mScalingKeys[transformIndex]));

			frame.timeStamp = pNodeAnim->mPositionKeys[transformIndex].mTime;
			frame.transform[pNodeAnim->mNodeName.C_Str()] = transform;
		}

		return frame;
	}

	DMKAnimation AssimpWrapper::loadAnimation(VPTR pAiAnimation, VPTR pAiRootNode)
	{
		auto pAnimation = (aiAnimation*)pAiAnimation;

		DMKAnimation animation;

		for (UI32 index = 0; index < pAnimation->mNumChannels; index++)
			animation.frames.pushBack(getAnimationFrame(pAnimation->mChannels[index]));

		animation.duration = pAnimation->mDuration;
		DMKAnimNodeGraph graph;
		readAllNodes((aiNode*)pAiRootNode, &graph.nodes, nullptr);

		return animation;
	}

	DMKAnimatedMeshComponent AssimpWrapper::loadAnimatedMeshComponent(VPTR pAiSceneObject, const DMKVertexLayout& vertexLayout, const UI32& nodesPerVertex)
	{
		auto _scene = (aiScene*)pAiSceneObject;

		if (!_scene)
		{
			DMKErrorManager::issueErrorBox("Unable to load the mesh file!");
			return DMKAnimatedMeshComponent();
		}

		UI32 totalNodes = 0;
		UI32 vertexOffset = 0;

		DMKAnimatedMeshComponent animMeshComponent;

		/* Load all nodes/ bones */
		readAllNodes(_scene->mRootNode, &animMeshComponent.nodeGraph.nodes, nullptr);

		for (UI32 _itr = 0; _itr < _scene->mNumMeshes; _itr++)
		{
			auto _mesh = _scene->mMeshes[_itr];

			animMeshComponent.nodeData = ARRAY<AAnimNodeData>(_mesh->mNumVertices);

			for (UI32 boneIndex = 0; boneIndex < _mesh->mNumBones; boneIndex++)
			{
				/* Create a new node structure to store the bone data */
				DMKAnimNode node;
				node.name = _mesh->mBones[boneIndex]->mName.data;
				_scene->mRootNode;

				if (animMeshComponent.nodeMap.find(node.name) == animMeshComponent.nodeMap.end())
				{
					node.index = totalNodes++;

					AAnimNodeInfo nodeInfo;
					animMeshComponent.nodeInfos.pushBack(nodeInfo);
				}
				else
					node.index = animMeshComponent.nodeMap[node.name];

				animMeshComponent.nodeMap[node.name] = node.index;
				animMeshComponent.nodeInfos[node.index].offset = aiMatrixToMatrix4F(_mesh->mBones[boneIndex]->mOffsetMatrix);

				for (UI32 i = 0; i < nodesPerVertex; i++)
				{
					UI32 vertexID = vertexOffset + _mesh->mBones[boneIndex]->mWeights[i].mVertexId;
					animMeshComponent.nodeData[vertexID].add(vertexID, _mesh->mBones[boneIndex]->mWeights[i].mWeight);
				}
			}

			animMeshComponent.skinnedMesh = loadMeshComponent(_mesh, vertexLayout);
			vertexOffset += animMeshComponent.skinnedMesh.vertexBuffer.size();
		}

		for (UI32 index = 0; index < _scene->mNumAnimations; index++)
			animMeshComponent.animations.pushBack(loadAnimation(_scene->mAnimations[index], _scene->mRootNode));

		return animMeshComponent;
	}

	DMKAnimatedMeshComponent AssimpWrapper::loadAnimatedModel(const STRING& file, const DMKVertexLayout& vertexLayout, const UI32& nodesPerVertex)
	{
		return loadAnimatedMeshComponent((VPTR)localImporter.ReadFile(file, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs), vertexLayout, nodesPerVertex);
	}

	ARRAY<DMKStaticMeshComponent> AssimpWrapper::loadStaticModel(const STRING& file, const DMKVertexLayout& vertexLayout)
	{
		auto _scene = localImporter.ReadFile(file, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

		if (!_scene)
		{
			DMKErrorManager::issueErrorBox("Unable to load the mesh file!");
			return ARRAY<DMKStaticMeshComponent>();
		}

		ARRAY<DMKStaticMeshComponent> myModel;
		for (UI32 _itr = 0; _itr < _scene->mNumMeshes; _itr++)
			myModel.pushBack(loadMeshComponent(_scene->mMeshes[_itr], vertexLayout));

		return myModel;
	}
}