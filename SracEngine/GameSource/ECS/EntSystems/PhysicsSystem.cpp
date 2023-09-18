#pragma once
#include "pch.h"
#include "PhysicsSystem.h"

#include "GameSource/ECS/Components.h"
#include "ECS/EntityCoordinator.h"

namespace ECS
{
	void PhysicsSystem::Update(float dt)
	{
		EntityCoordinator* ecs = GameData::Get().ecs;

		for (Entity entity : entities)
		{
			const CharacterState& state = ecs->GetComponentRef(CharacterState, entity);
			Transform& transform = ecs->GetComponentRef(Transform, entity);
			Physics& physics = ecs->GetComponentRef(Physics, entity);
			Velocity& velocity = ecs->GetComponentRef(Velocity, entity);

			if(physics.applyGravity)
			{
				VectorF speed = velocity.speed;
				speed = speed + VectorF(0.0f, 1.0f) * dt;
				velocity.speed = speed.clamp(velocity.maxSpeed * -1.0f, velocity.maxSpeed);
			}
		}
	}
}
