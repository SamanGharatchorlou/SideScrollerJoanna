#pragma once

#include "EntityCommon.h"

namespace ECS
{
	struct EntityManager
	{
		EntityManager() : entityIdIndex(0)
		{
			memset(archetypes, 0, sizeof(Archetype) * MaxEntityCount);
		}

		Entity CreateEntity()
		{
			Entity entityId = entityIdIndex;
			entityIdIndex++;
			return entityId;
		}

		// todo: not currently using these... i assume i do, but when will i need the archetype array
		bool HasComponent(Entity entity, Component::Type component) const
		{
			return archetypes[entity] & ((u64)1 << component);
		}

		Archetype GetAchetype(Entity entity) const
		{
			return archetypes[entity];
		}

		Archetype archetypes[MaxEntityCount];
		Entity entityIdIndex;
	};
}

