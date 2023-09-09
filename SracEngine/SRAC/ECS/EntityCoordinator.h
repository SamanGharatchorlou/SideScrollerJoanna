#pragma once

#include "ComponentManager.h"
#include "SystemManager.h"
#include "EntityManager.h"


#if ENTITY_LOGGING
#define CreateEntity(name) CreateNewEntity(name)
#else
#define CreateEntity(name) CreateNewEntity()
#endif

namespace ECS
{
	struct EntityCoordinator
	{
		template<class T>
		void RegisterComponent(Component::Type type, u32 reserve_size) { components.Register<T>(type, reserve_size); }

		template<class T>
		void RegisterSystem(Archetype type) { systems.Register<T>(type); }

		Entity CreateNewEntity() { return entities.CreateEntityId(); }
#if ENTITY_LOGGING
		Entity CreateNewEntity(const char* name) { return entities.CreateEntityWithName(name); }
		Entity FindEntity(const char* name) { return entities.FindEntity(name); }
#endif

		bool IsAlive(Entity entity) const { return entities.GetAchetype(entity) != ArchetypeInvalid; }

		template<class T>
		void AddComponent(Entity entity, const T& component, Component::Type type)
		{
			components.AddComponent<T>(entity, component, type);
			entities.AddComponent(entity, type);

			Archetype archetype = entities.GetAchetype(entity);
			systems.EntityAddType(entity, archetype);
		}

		template<class T>
		void RemoveComponent(Entity entity, const T& component, Component::Type type)
		{
			components.RemoveComponent<T>(entity, component, type);
			entities.RemoveComponent(entity, type);

			Archetype archetype = entities.GetAchetype(entity);
			systems.EntityRemoveType(entity, type);
		}

		template<class T>
		T& GetComponent(Entity entity, Component::Type type) { return components.GetComponent<T>(entity, type); }

		template<class T>
		ComponentArray<T>& GetComponents(Component::Type type) { return *static_cast<ComponentArray<T>*>(components.componentArrays[type]); }

		bool HasComponent(Entity entity, Component::Type type) 
		{ 
			return entities.HasComponent(entity, type); 
		}

		void UpdateSystems(float dt)
		{
			for (size_t i = 0; i < systems.entSystems.size(); i++)
			{
				systems.entSystems[i]->Update(dt);
			}
		}

		EntityManager entities;
		ComponentManager components;
		SystemManager systems;
	};

#define RegisterComponent(compType, reserve) RegisterComponent<ECS::compType>(ECS::compType::type(), reserve)
#define AddComponent(compType, entity, component) AddComponent<ECS::compType>(entity, component, ECS::compType::type())
#define GetComponent(compType, entity) GetComponent<ECS::compType>(entity, ECS::compType::type())
}

