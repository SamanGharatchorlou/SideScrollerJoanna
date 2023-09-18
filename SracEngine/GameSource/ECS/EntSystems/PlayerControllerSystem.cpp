#include "pch.h"
#include "PlayerControllerSystem.h"

#include "GameSource/ECS/Components.h"
#include "ECS/EntityCoordinator.h"
#include "Input/InputManager.h"

namespace ECS
{
	void PlayerControllerSystem::Update(float dt)
	{
		EntityCoordinator* ecs = GameData::Get().ecs;
		const InputManager* input = InputManager::Get();
;
		for (Entity entity : entities)
		{
			PlayerController& player_controller = ecs->GetComponentRef(PlayerController, entity);
			CharacterState& state = ecs->GetComponentRef(CharacterState, entity);

			Transform& transform = ecs->GetComponentRef(Transform, entity);
			Velocity& velocity = ecs->GetComponentRef(Velocity, entity);

			// update the transform with where we wanted to move last frame 
			// it may have been changed by the collision system
			transform.baseRect.SetCenter(transform.targetCenterPosition);

			player_controller.actions.ProcessStateChanges(state.canChange);
			player_controller.actions.Top().Update(dt);

			state.action = player_controller.actions.Top().action;

			// Movement Direction
			int horizontal_direction = input->isHeld(Button::Right) - input->isHeld(Button::Left);
			int vertical_direction =  input->isHeld(Button::Up) - input->isHeld(Button::Down);
			player_controller.movementDirection = VectorF(horizontal_direction, vertical_direction);

			// Facing Direction
			VectorF cursor_pos = input->getCursor()->position();
			bool cursor_on_left = transform.baseRect.Center().x >= cursor_pos.x;
			player_controller.facingDirection = cursor_on_left ? VectorF(-1.0f, 0.0f) : VectorF(1.0f, 0.0f);

			if (player_controller.facingDirection.x < 0)
			{
				transform.flip = SDL_FLIP_HORIZONTAL;
			}
			else if(player_controller.facingDirection.x > 0)
			{
				transform.flip = SDL_FLIP_NONE;
			}

			// set a new speed, we can set this again next frame
			//velocity.speed = physics.physics.applyDrag(velocity.speed, maxSpeed, velocity.acceleration, drag);
			transform.targetCenterPosition = transform.baseRect.Translate(velocity.speed).Center();
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

