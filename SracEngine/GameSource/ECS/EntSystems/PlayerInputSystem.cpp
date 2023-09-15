#include "pch.h"
#include "PlayerInputSystem.h"

#include "GameSource/ECS/Components.h"
#include "ECS/EntityCoordinator.h"
#include "Input/InputManager.h"

namespace ECS
{
	void PlayerInputSystem::Update(float dt)
	{
		EntityCoordinator* ecs = GameData::Get().ecs;
		const InputManager* input = InputManager::Get();
;
		for (Entity entity : entities)
		{
			CharacterState& state = ecs->GetComponentRef(CharacterState, entity);
			Velocity& velocity = ecs->GetComponentRef(Velocity, entity);

			state.actions.ProcessStateChanges(state.canChange);
			state.actions.Top().Update(dt);

			// Movement Direction
			int horizontal_direction = input->isPressed(Button::Right) - input->isPressed(Button::Left);
			int vertical_direction =  input->isPressed(Button::Up) - input->isPressed(Button::Down);
			state.movementDirection = VectorF(horizontal_direction, vertical_direction);

			// Facing Direction
			if(ECS::Transform* transform = ecs->GetComponent(Transform, entity))
			{
				VectorF cursor_pos = input->getCursor()->position();
				bool cursor_on_left = transform->baseRect.Center().x >= cursor_pos.x;
				state.facingDirection = cursor_on_left ? VectorF(-1.0f, 0.0f) : VectorF(1.0f, 0.0f);
			}
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
