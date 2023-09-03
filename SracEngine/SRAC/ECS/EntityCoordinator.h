#pragma once

#include "ComponentManager.h"
#include "SystemManager.h"
#include "EntityManager.h"

namespace ECS
{
	class EntityCoordinator
	{
	public:
		template<class T>
		void RegisterComponent(Component::Type type) { components.Register<T>(type); }

		template<class T>
		void RegisterSystem(Archetype type) { systems.Register<T>(type); }

		Entity CreateEntity() { return entities.CreateEntity(); }

		template<class T>
		void AddComponent(Entity entity, const T& component, Component::Type type)
		{
			components.AddComponent<T>(entity, component, type);
			systems.EntityAddType(entity, type);
		}

		template<class T>
		void RemoveComponent(Entity entity, const T& component, Component::Type type)
		{
			components.RemoveComponent<T>(entity, component, type);
			systems.EntityRemoveType(entity, type);
		}

		template<class T>
		T& GetComponent(Entity entity, Component::Type type) { return components.GetComponent<T>(entity, type); }

		void UpdateSystems(float dt)
		{
			for (size_t i = 0; i < systems.entSystems.size(); i++)
			{
				systems.entSystems[i]->Update(dt);
			}
		}

	private:
		EntityManager entities;
		ComponentManager components;
		SystemManager systems;
	};

#define RegisterComponent(compType) RegisterComponent<ECS::compType>(ECS::compType::type())
#define AddComponent(compType, entity, component) AddComponent<ECS::compType>(entity, component, ECS::compType::type())
#define GetComponent(compType, entity) GetComponent<ECS::compType>(entity, ECS::compType::type())
}

