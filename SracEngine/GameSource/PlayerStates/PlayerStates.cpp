#include "pch.h"
#include "PlayerStates.h"

#include "Input/InputManager.h"
#include "Animations/CharacterStates.h"

#include "ECS/Components.h"
#include "ECS/EntityCoordinator.h"

#define CreatePlayerState(type) type##State(actionStack, ActionState::type##)

// Idle
// ---------------------------------------------------------
void IdleState::Update(float dt)
{
	const InputManager* input = InputManager::Get();
	VectorF direction;

	ActionState active = actionStack->Top().action;

	// Run
	if (input->isPressed(Button::Right))
	{
		actionStack->Push(CreatePlayerState(Run));
		direction += VectorF(1.0f, 0.0f);		
	}
	if (input->isPressed(Button::Left))
	{
		actionStack->Push(CreatePlayerState(Run));
		direction += VectorF(-1.0f, 0.0f);
	}

	// Jump
	if (input->isPressed(Button::Space))
	{
		actionStack->Push(CreatePlayerState(Run));
		direction += VectorF(-1.0f, 0.0f);
	}

	// Light Attack
	if (input->isCursorPressed(Cursor::ButtonType::Left))
	{
		actionStack->Push(CreatePlayerState(LightAttack));
	}
}

// Run
// ---------------------------------------------------------
void RunState::Update(float dt)
{
	const InputManager* input = InputManager::Get();
	VectorF direction;

	ActionState active = actionStack->Top().action;

	// Back to Idle
	bool still_moving = input->isHeld(Button::Right) || input->isHeld(Button::Left);
	if (!still_moving)
	{
		actionStack->Pop();
	}

	// Jump
	if (input->isPressed(Button::Space))
	{
		actionStack->Push(CreatePlayerState(Run));
		direction += VectorF(-1.0f, 0.0f);
	}
}

// Jump
// ---------------------------------------------------------
void JumpState::init()
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	ECS::Velocity* velocity = ecs->GetComponent(Velocity, entity);
	velocity->speed += VectorF(0.0f, 10.0f);
}

void JumpState::Update(float dt)
{
	const InputManager* input = InputManager::Get();
	VectorF direction;

	ActionState active = actionStack->Top().action;
	
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	ECS::Velocity* velocity = ecs->GetComponent(Velocity, entity);
}

// Fall
// ---------------------------------------------------------
void FallState::Update(float dt)
{
	const InputManager* input = InputManager::Get();
	VectorF direction;

	ActionState active = actionStack->Top().action;
	
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	
	ECS::Velocity* velocity = ecs->GetComponent(Velocity, entity);
}