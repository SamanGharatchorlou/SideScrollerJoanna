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

			// update the transform with where we wanted to move last frame 
			// it may have been changed by the collision system
			transform.baseRect.SetCenter(transform.targetCenterPosition);


			// new frame calculations
			velocity.currAcceleration = velocity.acceleration * state.movementDirection;
			VectorF maxSpeed = velocity.maxSpeed;


			if (state.action == ActionState::Walk)
			{
				// todo: this direction thing is wrong
				// also dont want this, we want almost instant stop to turn and move the other way
			}
			else if (state.action == ActionState::Run)
			{
				//todo: this direction thing is wrong
				// also dont want this, we want almost instant stop to turn and move the other way
				const float speed_factor = 3.0f;
				maxSpeed *= speed_factor;
				velocity.currAcceleration *= speed_factor;
			}

			if (state.action == ActionState::Jump)
			{
				// do the actual jump
				if (state.jumpped)
				{
					const float speed_factor = 5.0f;
					maxSpeed *= speed_factor;

					velocity.speed -= VectorF(0.0f, maxSpeed.y);
				}
			}

			if(physics.applyGravity) 
			{
				velocity.currAcceleration += VectorF(0.0f, 10.0f);

				if (state.action == ActionState::Fall)
					velocity.currAcceleration += VectorF(0.0f, 10.0f);
				else if (state.IsJumping())
				{
					velocity.currAcceleration -= VectorF(0.0f, 5.0f);
				}

				velocity.currAcceleration.clamp(velocity.maxAcceleration * -1.0f, velocity.maxAcceleration);
			}

			VectorF nextSpeed = velocity.speed + (velocity.currAcceleration * dt);
			velocity.speed = nextSpeed.clamp(maxSpeed * -1.0f, maxSpeed);

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
					drag *= 0.2f;
			}
				
			// set a new speed, we can set this again next frame
			velocity.speed = physics.physics.applyDrag(velocity.speed, maxSpeed, velocity.currAcceleration, drag);
			transform.targetCenterPosition = transform.baseRect.Translate(velocity.speed).Center();

			int a = 4;
		}
	}
}