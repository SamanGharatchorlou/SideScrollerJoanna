#include "pch.h"
#include "PlayerStates.h"

#include "Input/InputManager.h"
#include "Animations/CharacterStates.h"

#include "ECS/Components/PlayerController.h"
#include "ECS/Components/Physics.h"
#include "ECS/Components.h"
#include "ECS/EntityCoordinator.h"

#define CreatePlayerState(type) type##State(playerController, ActionState::type##)

void PlayerState::PopSelf()
{
	PlayerState* state = &playerController->actions.Top();
	playerController->actions.Pop();

	playerController->statePool.returnObject(state, state->action);
	DebugPrint(PriorityLevel::Log, "Returning player action state: %s | %d states left", actionToString(action).c_str(), playerController->statePool.size(action));
}

void PlayerState::Push(ActionState action)
{
	DebugPrint(PriorityLevel::Log, "Pushing player action state: %s | %d states left", actionToString(action).c_str(), playerController->statePool.size(action));
	PlayerState* state = playerController->statePool.getObject(action);
	state->SetBaseParameters(playerController, action);
	playerController->actions.Push(state);
}


// Idle
// ---------------------------------------------------------
void IdleState::Update(float dt)
{
	const InputManager* input = InputManager::Get();

	// Walk
	if(playerController->hasMovementInput)
	{
		playerController->PushState(ActionState::Walk);
	}

	// Light Attack
	//if (input->isCursorPressed(Cursor::ButtonType::Left))
	//{
	//	Push(ActionState::LightAttack);
	//}

	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	if(ECS::Physics* physics = ecs->GetComponent(Physics, playerController->entity))
	{
		physics->speed.set(0.0f, 0.0f);
	}
}

// Walk
// ---------------------------------------------------------
void WalkState::Update(float dt)
{
	if (!playerController->hasMovementInput)
		PopSelf();

	if (playerController->isRunning)
		playerController->PushState(ActionState::Run);

	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	if (ECS::Physics* physics = ecs->GetComponent(Physics, playerController->entity))
	{
		// apply walk speed
		physics->maxSpeed = VectorF(3.0f, 3.0f);

		physics->ApplyMovement(playerController->movementDirection.toFloat(), dt);
		physics->ApplyDrag(playerController->movementDirection.toFloat(), 0.9f);
	}
}

// Run
// ---------------------------------------------------------
void RunState::Update(float dt)
{
	if (!playerController->isRunning)
		PopSelf();

	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	if (ECS::Physics* physics = ecs->GetComponent(Physics, playerController->entity))
	{
		// apply walk speed
		physics->maxSpeed = VectorF(5.0f, 5.0f);

		physics->ApplyMovement(playerController->movementDirection.toFloat(), dt);
		physics->ApplyDrag(playerController->movementDirection.toFloat(), 0.9f);
	}

}

// LightAttack
// ---------------------------------------------------------
void LightAttackState::Update(float dt)
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	ECS::CharacterState* cs = ecs->GetComponent(CharacterState, playerController->entity);
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
	ActionStateCase(Walk)
	ActionStateCase(Run)
	//ActionStateCase(LightAttack)
	//ActionStateCase(Fall)
	//ActionStateCase(Jump)

	case ActionState::Count:
	case ActionState::None:
	default:
		DebugPrint(Warning, "Invalid effect type %d, could not create %d objects\n", type, count);
		break;
	}

	return state;
}