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
		const InputManager* input = GameData::Get().inputManager;
;
		for (Entity entity : entities)
		{
			CharacterState& state = ecs->GetComponent(CharacterState, entity);
			Velocity& velocity = ecs->GetComponent(Velocity, entity);

			ActionState actionState;
			VectorF direction;

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
					direction += VectorF(0.0f, 1.0f);
					//state.onFloor = false;
				}

				// Attacks
				if (input->isCursorPressed(Cursor::ButtonType::Left))
				{
					actionState = ActionState::LightAttack;
				}
			}
			else
			{
				// default
				actionState = ActionState::Fall;
			}


			state.facingDirection = direction;
			state.movementDirection = direction;

			if (actionState != state.action && state.canChange)
			{
				state.previousAction = state.action;
				state.action = actionState;

				DebugPrint(Log, "Setting action %s", actionToString(actionState).c_str());
			}
		}
	}
}