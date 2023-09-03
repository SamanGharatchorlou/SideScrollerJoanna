#pragma once

#include "Debug/Logging.h"
#include "EntityCommon.h"

namespace ECS
{
	struct EntitySystem
	{
		EntitySystem(Archetype _archetype) : archetype(_archetype) { }

		virtual void Update(float dt) = 0;

		std::vector<Entity> entities;
		Archetype archetype;
	};

	struct SystemManager
	{
		template<class T>
		void Register(Archetype type)
		{
			for (u32 i = 0; i < entSystems.size(); i++)
			{
				if (type == entSystems[i]->archetype)
				{
					const char* id = typeid(T).name();
					DebugPrint(Warning, "System %s alread registered with archetype %d", id, type);
					return;
				}
			}

			entSystems.emplace_back(new T(type));
		}

		void EntityAddType(Entity entity, Component::Type type)
		{
			const u64 signature = (u64)1 << type;
			for (u32 i = 0; i < entSystems.size(); i++)
			{
				if (entSystems[i]->archetype & signature)
				{
					for (u32 ent = 0; ent < entSystems[i]->entities.size(); i++)
					{
						// this entity is already in this system
						if (entSystems[i]->entities[ent] == entity)
							return;
					}

					entSystems[i]->entities.push_back(entity);
				}
			}
		}

		void EntityRemoveType(Entity entity, Component::Type type)
		{
			const u64 signature = (u64)1 << type;
			for (u32 i = 0; i < entSystems.size(); i++)
			{
				EntitySystem* entSystem = entSystems[i];
				if (entSystem->archetype & signature)
				{
					const u32 et_count = entSystem->entities.size();
					for (u32 et = 0; et < et_count; et++)
					{
						if (entSystem->entities[et] == entity)
						{
							// copy the back element into the to be removed entities place, then pop the back
							entSystem->entities[et] = entSystem->entities.back();
							entSystem->entities.pop_back();
						}
					}

					entSystems[i]->entities.push_back(entity);
				}
			}
		}

		std::vector<EntitySystem*> entSystems;
	};
}