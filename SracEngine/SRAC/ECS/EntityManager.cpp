#include "pch.h"
#include "EntityManager.h"

// todo move this
#include "ECS/Components/ComponentsSetup.h"

namespace ECS
{
	void EntityManager::KillEntity(Entity entity)
	{
		RemoveAllComponents(entity);
		archetypes[entity] = ArchetypeInvalid;
	}
}