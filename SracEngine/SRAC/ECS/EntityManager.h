#pragma once

#include "EntityCommon.h"

namespace ECS
{
	struct EntityManager
	{
		EntityManager() : entityIdIndex(0)
		{
			memset(archetypes, -1, sizeof(Archetype) * MaxEntityCount);
		}

		Entity CreateEntityId()
		{
			Entity entityId = entityIdIndex;
			archetypes[entityId] = 0;
			entityIdIndex++;
			return entityId;
		}

		void KillEntity()
		{
			// todo:....
			// set achetype to -1
		}


		// todo: not currently using these... i assume i do, but when will i need the archetype array
		bool HasComponent(Entity entity, Component::Type component) const
		{
			return archetypes[entity] & ((u64)1 << component);
		}

		void AddComponent(Entity entity, Component::Type component)
		{
			archetypes[entity] |= ((u64)1 << component);
		}

		void RemoveComponent(Entity entity, Component::Type component)
		{
			archetypes[entity] &= ~((u64)1 << component);
		}

		Archetype GetAchetype(Entity entity) const
		{
			return archetypes[entity];
		}

		Archetype archetypes[MaxEntityCount];
		Entity entityIdIndex;

#if ENTITY_LOGGING
		std::unordered_map<Entity, StringBuffer32> entityNames;

		Entity CreateEntityWithName(const char* name = nullptr)
		{
			const Entity entityId = CreateEntityId();

			if(name)
				entityNames[entityId] = StringBuffer32(name);

			return entityId;
		}

		Entity FindEntity(const char* name) 
		{
			for(auto iter = entityNames.begin(); iter != entityNames.end(); iter++) 
			{
				if(StringCompare(iter->second.c_str(), name)) 
				{
					return iter->first;
				}     
			}
		}
#endif
	};
}

