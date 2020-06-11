#pragma once
#ifndef _DYNAMIK_COMPONENT_MANAGER_H
#define _DYNAMIK_COMPONENT_MANAGER_H

/*
 Author:	Dhiraj Wishal
 Date:		06/06/2020
*/
#include "Macros/Global.h"
#include "Components/RenderableComponents/MeshComponent.h"
#include "ComponentArray.h"

namespace Dynamik
{
	/*
	 Component manager for the Dynamik Engine
	 This manager is set per component and holds all the supported components. Some of the main components
	 are:
	 * Mesh Component
	 * Audio Component
	*/
	class DMK_API DMKComponentManager {
	public:
		DMKComponentManager() {}
		~DMKComponentManager();

		/*
		 Register a component
		*/
		template<class COMPONENT>
		void registerComponent()
		{
			STRING componentName = typeid(COMPONENT).name();

			if (componentTypes.find(componentName).size())
			{
				DMK_ERROR_BOX("Registering component already exists!");
			}

			components[componentName] = (POINTER<IComponentArray>)StaticAllocator<DMKComponentArray<COMPONENT>>::allocate();
			componentTypes.pushBack(componentName);
		}

		/*
		 Add a component
		*/
		template<class COMPONENT>
		void addComponent(const COMPONENT& component)
		{
			getComponentArray<COMPONENT>()->myComponents.pushBack(component);
		}

		/*
		 Get the component array
		*/
		template<class COMPONENT>
		POINTER<DMKComponentArray<COMPONENT>> getComponentArray()
		{
			STRING componentName = typeid(COMPONENT).name();

			if (!componentTypes.find(componentName).size())
			{
				DMKErrorManager::logWarn(DMK_TEXT("Requested component does not exist! Creating a new component."));
				registerComponent<COMPONENT>();
			}

			return (POINTER<DMKComponentArray<COMPONENT>>)components[componentName];
		}

		/*
		 Get a component at a given index.
		 By default its 0.
		*/
		template<class COMPONENT>
		POINTER<COMPONENT> getComponent(UI64 index = 0)
		{
			return getComponentArray<COMPONENT>()->myComponents.location(index);
		}

	private:
		std::unordered_map<STRING, POINTER<IComponentArray>> components;
		ARRAY<STRING> componentTypes;
	};
}

#endif // !_DYNAMIK_COMPONENT_MANAGER_H
