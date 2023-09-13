#pragma once

#include "SRAC/Game/GameStates/State.h"
#include "Input/InputManager.h"

struct PlayerState : public State
{


	void ToWalkState()
	{

	}
};

class IdleState : public PlayerState
{

	void slowUpdate(float dt) override
	{
		const InputManager* input = InputManager::Get();

		// Walk
		if (input->isHeld(Button::Right))
		{
			ToWalkState();
			direction += VectorF(1.0f, 0.0f);

			// to walk
		}
		if (input->isHeld(Button::Left))
		{
			ToWalkState();
			direction += VectorF(-1.0f, 0.0f);
		}

		// Run
		if (input->isHeld(Button::Shift) && actionState == ActionState::Walk)
		{
			actionState = ActionState::Run;
		}
	}
};

class WalkState : public PlayerState
{

}