#pragma once

#include "ECS/SystemManager.h"

namespace ECS
{
	class CollisionSystem : public EntitySystem
	{
	public:
		CollisionSystem(Archetype type) : EntitySystem(type) { }

		void Update(float dt) override;

		static bool DoesColliderInteract(ECS::Entity entity);
	};
}