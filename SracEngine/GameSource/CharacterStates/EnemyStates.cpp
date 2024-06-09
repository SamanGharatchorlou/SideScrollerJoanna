#pragma once
#include "pch.h"
#include "EnemyStates.h"

#include "SRAC/Game/GameStates/State.h"
#include "ECS/Components/Components.h"
#include "ECS/EntityCoordinator.h"


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
}