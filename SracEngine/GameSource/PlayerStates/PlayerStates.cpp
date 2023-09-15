#include "pch.h"
#include "PlayerStates.h"

#include "Input/InputManager.h"
#include "Animations/CharacterStates.h"

#define CreatePlayerState(type) type##State(actionStack, ActionState::type##)

void IdleState::slowUpdate(float dt)
{
	const InputManager* input = InputManager::Get();
	VectorF direction;

	ActionState active = actionStack.Top().state;

	// Run
	if (input->isPressed(Button::Right))
	{
		actionStack.Push(CreatePlayerState(Run));
		direction += VectorF(1.0f, 0.0f);		
	}
	if (input->isPressed(Button::Left))
	{
		actionStack.Push(CreatePlayerState(Run));
		direction += VectorF(-1.0f, 0.0f);
	}

	// Jump
	if (input->isPressed(Button::Space))
	{
		actionStack.Push(CreatePlayerState(Run));
		direction += VectorF(-1.0f, 0.0f);
	}

	// Light Attack
	if (input->isCursorPressed(Cursor::ButtonType::Left))
	{
		actionStack.Push(CreatePlayerState(LightAttack));
	}
}

void RunState::slowUpdate(float dt)
{
	const InputManager* input = InputManager::Get();
	VectorF direction;

	ActionState active = actionStack.Top().state;

	// Back to Idle
	bool still_moving = input->isHeld(Button::Right) || input->isHeld(Button::Right);
	if (!still_moving)
	{
		actionStack.Pop();
	}

	// Jump
	if (input->isPressed(Button::Space))
	{
		actionStack.Push(CreatePlayerState(Run));
		direction += VectorF(-1.0f, 0.0f);
	}
}

void JumpState::slowUpdate(float dt)
{
	const InputManager* input = InputManager::Get();
	VectorF direction;

	ActionState active = actionStack.Top().state;


}