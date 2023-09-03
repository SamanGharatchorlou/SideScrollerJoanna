#pragma once
#include "pch.h"
#include "MovementSystem.h"

#include "GameSource/ECS/Components.h"
#include "ECS/EntityCoordinator.h"

namespace ECS
{
	void MovementSystem::Update(float dt)
	{
		EntityCoordinator* ecs = GameData::Get().ecs;

		for (Entity entity : entities)
		{
			const CharacterState& state = ecs->GetComponent(CharacterState, entity);
			Transform& transform = ecs->GetComponent(Transform, entity);
			Velocity& velocity = ecs->GetComponent(Velocity, entity);
			MovementPhysics& physics = ecs->GetComponent(MovementPhysics, entity);

			VectorF acceleration = velocity.acceleration * state.movementDirection;
			VectorF maxSpeed = velocity.maxSpeed;

			if (state.action == ActionState::Walk)
			{
				//todo: this direction thing is wrong
				// also dont want this, we want almost instant stop to turn and move the other way
				VectorF nextSpeed = velocity.speed + (acceleration * dt);
				velocity.speed = nextSpeed.clamp(maxSpeed * -1.0f, maxSpeed);
			}
			else if (state.action == ActionState::Run)
			{
				//todo: this direction thing is wrong
				// also dont want this, we want almost instant stop to turn and move the other way
				float speed_factor = 3.0f;
				maxSpeed *= speed_factor;
				acceleration *= speed_factor;

				VectorF nextSpeed = velocity.speed + (acceleration * dt);
				velocity.speed = nextSpeed.clamp(maxSpeed * -1.0f, maxSpeed);
			}

			if (state.facingDirection.x < 0)
			{
				transform.flip = SDL_FLIP_HORIZONTAL;
			}
			else if(state.facingDirection.x > 0)
			{
				transform.flip = SDL_FLIP_NONE;
			}

			float drag = physics.physics.mDragFactor;
			if (state.action == ActionState::Idle && state.previousAction == ActionState::Run)
			{
				if(state.inTransition)
					drag *= 0.2;
			}
				

			velocity.speed = physics.physics.applyDrag(velocity.speed, maxSpeed, acceleration, drag);
			transform.baseRect = transform.baseRect.Translate(velocity.speed);
		}
	}
}