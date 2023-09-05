#pragma once

#include "ECS/SystemManager.h"

namespace ECS
{
	class MovementSystem : public EntitySystem
	{
	public:
		MovementSystem(Archetype type) : EntitySystem(type) { }

		void Update(float dt) override;
	};
}