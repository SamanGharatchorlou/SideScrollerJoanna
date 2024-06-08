#pragma once
#include "pch.h"
#include "PhysicsSystem.h"

#include "ECS/Components/Physics.h"
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

			if(physics.applyGravity)
			{
				VectorF speed = physics.speed;
				speed = speed + VectorF(0.0f, 1.0f) * dt;
				physics.speed = speed.clamp(physics.maxSpeed * -1.0f, physics.maxSpeed);
			}

			//physics.ApplyDrag(0.9f);
		}
	}
}
