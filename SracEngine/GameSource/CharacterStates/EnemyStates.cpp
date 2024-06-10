#pragma once
#include "pch.h"
#include "EnemyStates.h"

#include "SRAC/Game/GameStates/State.h"
#include "ECS/Components/Components.h"
#include "ECS/EntityCoordinator.h"
#include "ECS/Components/AIController.h"
#include "ECS/Components/Physics.h"
#include "Debugging/ImGui/ImGuiMainWindows.h"


bool& EnemyCanMove();

#define ActionStateCase(action) case ActionState::action: \
								out_size = sizeof(action##State);  \
								state = new action##State[count]; \
								break;

CharacterAction* Enemy::StatePool::createNewObjects(ActionState type, int count, int& out_size) const
{
	CharacterAction* state = nullptr;

	switch (type)
	{
	ActionStateCase(Idle)
	ActionStateCase(Run)
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
void Enemy::IdleState::Update(float dt)
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	ECS::AIController& ai = ecs->GetComponentRef(AIController, entity);
	ECS::CharacterState& state = ecs->GetComponentRef(CharacterState, entity);

	if(!state.movementDirection.isZero())
	{
		ai.PushState(ActionState::Run);
	}
}

// Run
// ---------------------------------------------------------
void Enemy::RunState::Update(float dt)
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	ECS::AIController& ai = ecs->GetComponentRef(AIController, entity);
	ECS::CharacterState& state = ecs->GetComponentRef(CharacterState, entity);

	if (ECS::Physics* physics = ecs->GetComponent(Physics, ai.entity))
	{
		// apply walk speed
		physics->maxSpeed = VectorF(3.0f, 3.0f);
		physics->ApplyMovement(state.movementDirection.toFloat(), dt);
		physics->ApplyDrag(state.movementDirection.toFloat(), 0.9f);
	}

	if(state.movementDirection.isZero())
	{
		ai.PopState();
	}
}