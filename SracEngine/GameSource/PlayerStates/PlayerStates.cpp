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

void PlayerState::Replace(ActionState action)
{
	DebugPrint(PriorityLevel::Log, "replacing player action state: %s | %d states left", actionToString(action).c_str(), playerController->statePool.size(action));
	PopSelf();
	Push(action);
}


// Idle
// ---------------------------------------------------------
void IdleState::Update(float dt)
{
	// Walk
	if(playerController->hasMovementInput)
	{
		playerController->PushState(ActionState::Walk);
	}

	// Slash Attack
	InputManager* input = InputManager::Get();
	if (input->isCursorPressed(Cursor::ButtonType::Left))
	{
		Push(ActionState::SlashAttack);
	}

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

	// Slash Attack
	InputManager* input = InputManager::Get();
	if (input->isCursorPressed(Cursor::ButtonType::Left))
	{
		Push(ActionState::SlashAttack);
	}

	// Dodge
	if (input->isPressed(Button::Space, c_inputBuffer))
	{
		Push(ActionState::Dodge);
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

	// Slash Attack
	InputManager* input = InputManager::Get();
	if (input->isCursorPressed(Cursor::ButtonType::Left, c_inputBuffer))
	{
		Push(ActionState::SlashAttack);
	}

	// Dodge
	if (input->isPressed(Button::Space, c_inputBuffer))
	{
		Push(ActionState::Dodge);
	}
}

// DodgeState
// ---------------------------------------------------------
void DodgeState::Init()
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	
	if (ECS::Physics* physics = ecs->GetComponent(Physics, playerController->entity))
	{
		float amplitude = 10.0f;
		physics->speed = playerController->movementDirection.toFloat() * amplitude;
	}
}

void DodgeState::Update(float dt)
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	ECS::Animation& animation = ecs->GetComponentRef(Animation, playerController->entity);

	ActionState action = animation.animator.activeAction();
	ASSERT(action == ActionState::Dodge, "Not the Slash Attack State");

	if (ECS::Physics* physics = ecs->GetComponent(Physics, playerController->entity))
	{
		physics->ApplyDrag(VectorF::zero(), 0.08f);
	}

	if(animation.animator.finished())
	{
		PopSelf();
	}
}


// LightAttack
// ---------------------------------------------------------
void SlashAttackState::Init()
{	
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	
	if (ECS::Physics* physics = ecs->GetComponent(Physics, playerController->entity))
	{
		float amplitude = 1.0f;
		physics->speed = playerController->movementDirection.toFloat() * amplitude;
	}
}

void SlashAttackState::Update(float dt)
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	
	if (ECS::Physics* physics = ecs->GetComponent(Physics, playerController->entity))
	{
		physics->ApplyDrag(VectorF::zero(), 0.1f);
	}
	
	ECS::Animation& animation = ecs->GetComponentRef(Animation, playerController->entity);
	ActionState action = animation.animator.activeAction();
	ASSERT(action == ActionState::SlashAttack, "Not the Slash Attack State");

	if(animation.animator.finished())
	{
		PopSelf();
	}

	if(animation.animator.lastFrame())
	{
		// input buffer
		InputManager* input = InputManager::Get();
		if (input->isCursorPressed(Cursor::ButtonType::Left))
		{
			animation.animator.restart();
			Replace(action);
		}
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
	ActionStateCase(Walk)
	ActionStateCase(Run)
	ActionStateCase(Dodge)
	ActionStateCase(SlashAttack)
	case ActionState::Count:
	case ActionState::None:
	default:
		DebugPrint(Warning, "Invalid effect type %d, could not create %d objects\n", type, count);
		break;
	}

	return state;
}