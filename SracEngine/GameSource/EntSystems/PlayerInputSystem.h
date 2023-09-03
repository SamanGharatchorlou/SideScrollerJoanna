#pragma once

#include "ECS/SystemManager.h"

namespace ECS
{
	class PlayerInputSystem : public EntitySystem
	{
	public:
		PlayerInputSystem(Archetype type) : EntitySystem(type) { }

		void Update(float dt) override;
	};
}