#include "pch.h"
#include "PlayerControllerSystem.h"

#include "GameSource/ECS/Components/Components.h"
#include "ECS/EntityCoordinator.h"
#include "Input/InputManager.h"
#include "ECS/Components/PlayerController.h"
#include "ECS/Components/Physics.h"

#include "Characters/Spawner.h"
#include "Characters/Player/PlayerCharacter.h"

namespace ECS
{
	void PlayerControllerSystem::Update(float dt)
	{
		EntityCoordinator* ecs = GameData::Get().ecs;
		InputManager* input = InputManager::Get();
;
		for (Entity entity : entities)
		{
			PlayerController& pc = ecs->GetComponentRef(PlayerController, entity);
			CharacterState& state = ecs->GetComponentRef(CharacterState, entity);
			Transform& transform = ecs->GetComponentRef(Transform, entity);
			Physics& physics = ecs->GetComponentRef(Physics, entity);

			// update the transform with where we wanted to move last frame 
			// it may have been changed by the collision system
			transform.rect.SetCenter(transform.targetCenterPosition);

			if(pc.actions.HasAction())
			{
				CharacterAction* character_state = &pc.actions.Top();
				character_state->Update(dt);

				if( character_state->action == ActionState::Death )
				{
					Player::DeathState* death_state = static_cast<Player::DeathState*>(character_state);
					if(death_state->can_respawn)
					{
						spawnPlayer = true;
						continue;
					}
				}
				else
				{
					if(Health* health = ecs->GetComponent(Health, entity))
					{
						if(health->currentHealth <= 0.0f)
						{
							pc.PopState();
							pc.PushState(ActionState::Death);
						}
					}
				}
			}
			else
				pc.PushState(ActionState::Idle);

			pc.actions.ProcessStateChanges();
			state.action = pc.actions.Top().action;

			// Movement Direction
			pc.hasMovementInput = input->isHeld(Button::Right) || input->isHeld(Button::Left) || 
				input->isHeld(Button::Up) || input->isHeld(Button::Down);

			int horizontal_direction = input->isHeld(Button::Right) - input->isHeld(Button::Left);
			int vertical_direction = input->isHeld(Button::Down) - input->isHeld(Button::Up);

			state.movementDirection = VectorI(horizontal_direction, vertical_direction);

			// if we're moving use the speed so we always face the direction we're moving
			if(!physics.speed.isZero())
			{
				float x_mag = physics.speed.x * physics.speed.x;
				float y_mag = physics.speed.y * physics.speed.y;

				if(x_mag > y_mag)
				{
					state.facingDirection.x = physics.speed.x > 0.0f ? 1 : -1;
					state.facingDirection.y = 0;
				}
				else if(x_mag < y_mag)
				{
					state.facingDirection.y = physics.speed.y > 0.0f ? 1 : -1;
					state.facingDirection.x = 0;
				}
			}

			// Running
			state.isRunning = input->isHeld(Button::Shift);

			// where we're trying to move to
			transform.targetCenterPosition = transform.rect.Translate(physics.speed).Center();
		}

		if(spawnPlayer)
		{
			if(entities.size() != 0)
			{
				ecs->entities.KillEntity(entities.front());
			}

			PlayerSpawn::Spawn();
			spawnPlayer = false;
		}
	} 
}

