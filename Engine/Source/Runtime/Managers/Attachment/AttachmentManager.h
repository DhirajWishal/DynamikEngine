#pragma once
#ifndef _DYNAMIK_ATTACHMENT_MANAGER_H
#define _DYNAMIK_ATTACHMENT_MANAGER_H

/*
 Author:    Dhiraj Wishal
 Date:      20/05/2020
*/
#include "GameLibrary/GameAssetAttachment.h"

namespace Dynamik
{
	/*
	 Dynamik Attachment Map
	 This contains the location of the stored attachment in the attachmen buffer, type of the attachment
	 and the byte size of it.
	*/
	struct DMK_API DMKAttachmentMap {
		VPTR location = nullptr;
		DMKGameAssetAttachmentType type = DMKGameAssetAttachmentType::DMK_GAME_ASSET_ATTACHMENT_TYPE_TRANSFORM;
		UI32 byteSize = 0;
	};

	/*
	 Dynamik Attachment Manager
	 Game objects use attachments to add aditional functionalities. Since these attachments are added inheritently,
	 it is not that easy to make them cache friendly. This is where the attachment manager comes to play.
	 The basic way the manager accomplishes this is by maintaining a pool of memory where all the attachment
	 data are stored. When accessed, the CPU will load multiple data stored in the pool this reducing latency.

	 Since most of the attachments are added and/ or removed by the studio, the allocation time and recompation
	 time will not affect the game runtime preformance.
	 This system is closely related to ECS (Entity Component System) but one of the core features of this is that
	 components could have functionalities.
	*/
	class DMK_API DMKAttachmentManager {
	public:
		DMKAttachmentManager();
		~DMKAttachmentManager();

		void addAttachment(POINTER<DMKGameAssetAttachment> attachment);

		const VPTR getBuffer() const;
		const ARRAY<DMKAttachmentMap> getAttachments() const;

		const POINTER<DMKAttachmentMap> begin() const noexcept;
		const POINTER<DMKAttachmentMap> end() const noexcept;

	private:
		/*
		 If already allocated, it automatically extend the heap and move the existing data.
		*/
		inline void _allocateHeap(UI32 byteSize);

		/*
		 Recreate the attachment mapping if the current mapping is out dated.
		*/
		inline void _recreateMappings();

		ARRAY<DMKAttachmentMap> myAttachmentMapping;
		VPTR myMemoryBuffer = nullptr;
		POINTER<BYTE> myNextPtr = myMemoryBuffer;
		UI32 myAllocationSize = 0;
	};
}

#endif // !_DYNAMIK_ATTACHMENT_MANAGER_H