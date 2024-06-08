#include "pch.h"
#include "PlayerControllerSystem.h"

#include "GameSource/ECS/Components.h"
#include "ECS/EntityCoordinator.h"
#include "Input/InputManager.h"
#include "ECS/Components/PlayerController.h"
#include "ECS/Components/Physics.h"

namespace ECS
{
	void PlayerControllerSystem::Update(float dt)
	{
		EntityCoordinator* ecs = GameData::Get().ecs;
		InputManager* input = InputManager::Get();
;
		for (Entity entity : entities)
		{
			PlayerController& player_controller = ecs->GetComponentRef(PlayerController, entity);
			CharacterState& state = ecs->GetComponentRef(CharacterState, entity);

			Transform& transform = ecs->GetComponentRef(Transform, entity);
			Physics& physics = ecs->GetComponentRef(Physics, entity);

			// update the transform with where we wanted to move last frame 
			// it may have been changed by the collision system
			transform.rect.SetCenter(transform.targetCenterPosition);

			if(player_controller.actions.HasAction())
				player_controller.actions.Top().Update(dt);
			else
				player_controller.PushState(ActionState::Idle);

			player_controller.actions.ProcessStateChanges();
			state.action = player_controller.actions.Top().action;

			// Movement Direction
			player_controller.hasMovementInput = input->isHeld(Button::Right) || input->isHeld(Button::Left) || 
				input->isHeld(Button::Up) || input->isHeld(Button::Down);

			int horizontal_direction = input->isHeld(Button::Right) - input->isHeld(Button::Left);
			int vertical_direction = input->isHeld(Button::Down) - input->isHeld(Button::Up);

			player_controller.movementDirection = VectorI(horizontal_direction, vertical_direction);

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
			player_controller.isRunning = input->isHeld(Button::Shift);

			// where we're trying to move to
			transform.targetCenterPosition = transform.rect.Translate(physics.speed).Center();
		}
	} 
}


/*
// reset a bunch of states here
state.jumpped = false;

if (state.onFloor)
{
	// default
	actionState = ActionState::Idle;

	// Walk
	if (input->isHeld(Button::Right))
	{
		actionState = ActionState::Walk;
		direction += VectorF(1.0f, 0.0f);
	}
	if (input->isHeld(Button::Left))
	{
		actionState = ActionState::Walk;
		direction += VectorF(-1.0f, 0.0f);
	}

	// Run
	if (input->isHeld(Button::Shift) && actionState == ActionState::Walk)
	{
		actionState = ActionState::Run;
	}

	// To Idle
	if (direction.isZero() && velocity.speed.x < velocity.maxSpeed.x * 0.05)
	{
		actionState = ActionState::Idle;
	}

	// Jump
	if (input->isPressed(Button::Space))
	{
		actionState = ActionState::Jump;
		state.jumpped = true;
		//direction += VectorF(0.0f, 1.0f);
	}

	// Attacks
	if (input->isCursorPressed(Cursor::ButtonType::Left))
	{
		actionState = ActionState::LightAttack;
	}
}
else
{
	if (velocity.speed.y < 0.0f)
	{
		actionState = ActionState::Jump;
	}
	else
	{
		// default
		actionState = ActionState::Fall;
	}
}
*/

