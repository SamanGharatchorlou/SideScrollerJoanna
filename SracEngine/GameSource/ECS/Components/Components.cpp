#include "pch.h"
#include "Components.h"

#include "ECS/EntityCoordinator.h"

namespace ECS
{
	void Health::ApplyDamage(const Damage& damage)
	{
		EntityCoordinator* ecs = GameData::Get().ecs;

		for( u32 i = 0; i < ignoredDamaged.size(); i++ )
		{
			// ignore damage from this source
			if(ignoredDamaged[i] == damage.entity)
				return;
		}

		currentHealth -= damage.value;
		currentHealth = std::clamp(currentHealth, 0.0f, maxHealth);

		// todo: clean this list when the entity is dead
		ignoredDamaged.push_back(damage.entity);
	}
}