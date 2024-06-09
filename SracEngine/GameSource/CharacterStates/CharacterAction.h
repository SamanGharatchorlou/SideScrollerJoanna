#pragma once

#include "ECS/EntityCommon.h"
#include "Core/ObjectPool.h"
#include "SRAC/Game/GameStates/State.h"
#include "Animations/CharacterStates.h"

struct CharacterAction : public State
{
	CharacterAction() : action(ActionState::None), entity(ECS::EntityInvalid)  { }

	void InitState(ECS::Entity entity_id, ActionState state) { entity = entity_id; action = state; }

	ECS::Entity entity;
	ActionState action;
};