#include "pch.h"
#include "PlayerStates.h"

#include "Input/InputManager.h"
#include "Animations/CharacterStates.h"

#include "ECS/Components.h"
#include "ECS/EntityCoordinator.h"

#define CreatePlayerState(type) type##State(playerController, ActionState::type##)

void PlayerState::PopSelf(ActionState action)
{
	PlayerState* state = &playerController->actions.Top();
	playerController->actions.Pop();

	playerController->statePool.returnObject(state, action);
}

void PlayerState::Push(ActionState action)
{
	PlayerState* state = playerController->statePool.getObject(action);
	state->SetBaseParameters(playerController, action);
	playerController->actions.Push(state);
}


// Idle
// ---------------------------------------------------------
void IdleState::Update(float dt)
{
	const InputManager* input = InputManager::Get();

	// Run
	if (input->isPressed(Button::Right) || input->isPressed(Button::Left))
	{
		Push(ActionState::Run);
	}

	//// Jump
	//if (input->isPressed(Button::Space))
	//{
	//	actionStack->Push(CreatePlayerState(Run));
	//	direction += VectorF(-1.0f, 0.0f);
	//}

	// Light Attack
	if (input->isCursorPressed(Cursor::ButtonType::Left))
	{
		Push(ActionState::LightAttack);
	}

	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	if(ECS::Velocity* velocity = ecs->GetComponent(Velocity, playerController->entity))
	{
		velocity->speed.set(0.0f, 0.0f);
	}

	if(ECS::CharacterState* cs = ecs->GetComponent(CharacterState, playerController->entity))
	{
		if(!cs->OnFloor())
		{
			Push(ActionState::Fall);
		}
	}
}

// Run
// ---------------------------------------------------------
void RunState::Update(float dt)
{
	const InputManager* input = InputManager::Get();
	VectorF direction = playerController->movementDirection;

	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	if(ECS::Velocity* velocity = ecs->GetComponent(Velocity, playerController->entity))
	{
		velocity->speed += direction * velocity->acceleration * dt;
		velocity->speed = velocity->speed.clamp(velocity->maxSpeed * -1.0f, velocity->maxSpeed);
	}

	// Back to Idle
	bool still_moving = input->isHeld(Button::Right) || input->isHeld(Button::Left);
	if (!still_moving)
	{
		PopSelf(ActionState::Run);
	}

	if(ECS::CharacterState* cs = ecs->GetComponent(CharacterState, playerController->entity))
	{
		if(!cs->OnFloor())
		{
			Push(ActionState::Fall);
		}
	}


	//// Jump
	//if (input->isPressed(Button::Space))
	//{
	//	playerController->actions.Push(CreatePlayerState(Jump));
	//	direction += VectorF(-1.0f, 0.0f);
	//}
}

// Jump
// ---------------------------------------------------------
void JumpState::init()
{
	//ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	//ECS::Velocity* velocity = ecs->GetComponent(Velocity, entity);
	//velocity->speed += VectorF(0.0f, 10.0f);
}

void JumpState::Update(float dt)
{
	//const InputManager* input = InputManager::Get();
	//VectorF direction;

	//ActionState active = actionStack->Top().action;
	//
	//ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	//ECS::Velocity* velocity = ecs->GetComponent(Velocity, entity);
}
//
// 
// Fall
// ---------------------------------------------------------
void FallState::Update(float dt)
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	ECS::CharacterState* cs = ecs->GetComponent(CharacterState, playerController->entity);
	if(cs->OnFloor())
	{
		PopSelf(ActionState::Fall);
	}
}

// LightAttack
// ---------------------------------------------------------
void LightAttackState::Update(float dt)
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	ECS::CharacterState* cs = ecs->GetComponent(CharacterState, playerController->entity);
	if(cs->canChange && playerController->actions.stateFrames > 1)
	{
		PopSelf(ActionState::LightAttack);
	}

}

template<class T>
T* GetNewThing(int count, int& out_size)
{
	out_size = sizeof(T);
	return new T[count];
}

#define ActionStateCase(action) case ActionState::action: \
								state = GetNewThing<action##State>(count, outSize); \
								break;

PlayerState* PlayerStatePool::createNewObjects(ActionState type, int count, int& outSize) const
{
	PlayerState* state = nullptr;

	switch (type)
	{
	ActionStateCase(Idle)
	ActionStateCase(Run)
	ActionStateCase(LightAttack)
	ActionStateCase(Fall)
	ActionStateCase(Jump)

	case ActionState::Count:
	case ActionState::None:
	default:
		DebugPrint(Warning, "Invalid effect type %d, could not create %d objects\n", type, count);
		break;
	}

	return state;
}