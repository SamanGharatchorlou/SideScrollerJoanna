#include "pch.h"
#include "PlayerStates.h"

#include "Input/InputManager.h"
#include "Animations/CharacterStates.h"

#include "ECS/Components/PlayerController.h"
#include "ECS/Components/Physics.h"
#include "ECS/Components/Components.h"
#include "ECS/EntityCoordinator.h"

using namespace Player;

#define ActionStateCase(action) case ActionState::action: \
								out_size = sizeof(action##State);  \
								state = new action##State[count]; \
								break;

CharacterAction* Player::StatePool::createNewObjects(ActionState type, int count, int& out_size) const
{
	CharacterAction* state = nullptr;

	switch (type)
	{
	ActionStateCase(Idle)
	ActionStateCase(Walk)
	ActionStateCase(Run)
	ActionStateCase(Dodge)
	ActionStateCase(SlashAttack)
	ActionStateCase(ChopAttack)
	case ActionState::Count:
	case ActionState::None:
	default:
		DebugPrint(Warning, "Invalid effect type %d, could not create %d objects\n", type, count);
		break;
	}

	return state;
}

// Idle
// ---------------------------------------------------------
void IdleState::Update(float dt)
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	ECS::PlayerController& pc = ecs->GetComponentRef(PlayerController, entity);

	// Walk
	if(pc.hasMovementInput)
	{
		pc.PushState(ActionState::Walk);
	}

	// Slash Attack
	InputManager* input = InputManager::Get();
	if (input->isCursorPressed(Cursor::ButtonType::Left))
	{
		pc.PushState(ActionState::SlashAttack);
	}
	if (input->isCursorPressed(Cursor::ButtonType::Right, c_inputBuffer))
	{
		pc.PushState(ActionState::ChopAttack);
	}

	if(ECS::Physics* physics = ecs->GetComponent(Physics, pc.entity))
	{
		physics->speed.set(0.0f, 0.0f);
	}
}

// Walk
// ---------------------------------------------------------
void WalkState::Update(float dt)
{	
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	ECS::PlayerController& pc = ecs->GetComponentRef(PlayerController, entity);
	ECS::CharacterState& state = ecs->GetComponentRef(CharacterState, entity);

	if (!pc.hasMovementInput)
		pc.PopState();

	if (state.isRunning)
		pc.PushState(ActionState::Run);

	if (ECS::Physics* physics = ecs->GetComponent(Physics, pc.entity))
	{
		// apply walk speed
		physics->maxSpeed = VectorF(3.0f, 3.0f);
		physics->ApplyMovement(state.movementDirection.toFloat(), dt);
		physics->ApplyDrag(state.movementDirection.toFloat(), 0.9f);
	}

	// Slash Attack
	InputManager* input = InputManager::Get();
	if (input->isCursorPressed(Cursor::ButtonType::Left))
	{
		pc.PushState(ActionState::SlashAttack);
	}
	if (input->isCursorPressed(Cursor::ButtonType::Right, c_inputBuffer))
	{
		pc.PushState(ActionState::ChopAttack);
	}

	// Dodge
	if (input->isPressed(Button::Space, c_inputBuffer))
	{
		pc.PushState(ActionState::Dodge);
	}
}

// Run
// ---------------------------------------------------------
void RunState::Update(float dt)
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	ECS::PlayerController& pc = ecs->GetComponentRef(PlayerController, entity);
	ECS::CharacterState& state = ecs->GetComponentRef(CharacterState, entity);

	if (!state.isRunning)
		pc.PopState();

	if (ECS::Physics* physics = ecs->GetComponent(Physics, entity))
	{
		ECS::CharacterState& state = ecs->GetComponentRef(CharacterState, entity);

		// apply walk speed
		physics->maxSpeed = VectorF(5.0f, 5.0f);
		physics->ApplyMovement(state.movementDirection.toFloat(), dt);
		physics->ApplyDrag(state.movementDirection.toFloat(), 0.9f);
	}

	// Slash Attack
	InputManager* input = InputManager::Get();
	if (input->isCursorPressed(Cursor::ButtonType::Left, c_inputBuffer))
	{
		pc.PushState(ActionState::SlashAttack);
	}
	if (input->isCursorPressed(Cursor::ButtonType::Right, c_inputBuffer))
	{
		pc.PushState(ActionState::ChopAttack);
	}

	// Dodge
	if (input->isPressed(Button::Space, c_inputBuffer))
	{
		pc.PushState(ActionState::Dodge);
	}
}

// DodgeState
// ---------------------------------------------------------
void DodgeState::Init()
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;

	if (ECS::Physics* physics = ecs->GetComponent(Physics,entity))
	{
		ECS::CharacterState& state = ecs->GetComponentRef(CharacterState, entity);

		float amplitude = 10.0f;
		physics->speed = state.movementDirection.toFloat() * amplitude;
	}
}

void DodgeState::Update(float dt)
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	ECS::Animation& animation = ecs->GetComponentRef(Animation, entity);
	ECS::PlayerController& pc = ecs->GetComponentRef(PlayerController, entity);

	ActionState action = animation.animator.activeAction();
	ASSERT(action == ActionState::Dodge, "Not the Slash Attack State");

	if (ECS::Physics* physics = ecs->GetComponent(Physics, entity))
	{
		physics->ApplyDrag(VectorF::zero(), 0.08f);
	}

	if(animation.animator.finished())
	{
		pc.PopState();
	}
}


// SlashAttack
// ---------------------------------------------------------
void SlashAttackState::Update(float dt)
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	ECS::PlayerController& pc = ecs->GetComponentRef(PlayerController, entity);
	
	if (ECS::Physics* physics = ecs->GetComponent(Physics, entity))
	{
		physics->ApplyDrag(VectorF::zero(), 0.2f);
	}
	
	ECS::Animation& animation = ecs->GetComponentRef(Animation, entity);
	ActionState action = animation.animator.activeAction();
	ASSERT(action == ActionState::SlashAttack, "Not the Slash Attack State");

	if(animation.animator.finished())
	{
		pc.PopState();
	}

	if(animation.animator.lastFrame())
	{
		// input buffer
		InputManager* input = InputManager::Get();
		if (input->isCursorPressed(Cursor::ButtonType::Left))
		{
			animation.animator.restart();

			pc.PopState();
			pc.PushState(action);
		}
	}
}

// ChopAttack
// ---------------------------------------------------------
void ChopAttackState::Update(float dt)
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	ECS::PlayerController& pc = ecs->GetComponentRef(PlayerController, entity);
	
	if (ECS::Physics* physics = ecs->GetComponent(Physics, entity))
	{
		physics->ApplyDrag(VectorF::zero(), 0.05f);
	}
	
	ECS::Animation& animation = ecs->GetComponentRef(Animation, entity);
	ActionState action = animation.animator.activeAction();
	ASSERT(action == ActionState::ChopAttack, "Not the Slash Attack State");

	if(animation.animator.finished())
	{
		pc.PopState();
	}

	if(animation.animator.lastFrame())
	{
		// input buffer
		InputManager* input = InputManager::Get();
		if (input->isCursorPressed(Cursor::ButtonType::Left))
		{
			animation.animator.restart();			
			
			pc.PopState();
			pc.PushState(action);;
		}
	}
}
