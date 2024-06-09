#pragma once

#include "SRAC/Game/GameStates/State.h"
#include "Animations/CharacterStates.h"
#include "Core/ObjectPool.h"

struct PlayerState : public State
{
	PlayerState() : action(ActionState::None), entity(ECS::EntityInvalid)  { }

	void InitState(ECS::Entity entity_id, ActionState state) { entity = entity_id; action = state; }

	ECS::Entity entity;
	ActionState action;
};

struct PlayerStatePool : public ObjectPool<PlayerState, ActionState>
{
	PlayerState* createNewObjects(ActionState type, int count, int& outSize) const override;
};

struct IdleState : public PlayerState
{
	void Update(float dt) override;
};

struct WalkState : public PlayerState
{
	void Update(float dt) override;
};

struct RunState : public PlayerState
{
	void Update(float dt) override;
};

struct DodgeState : public PlayerState
{
	void Init() override;
	void Update(float dt) override;
};

struct SlashAttackState : public PlayerState
{
	void Update(float dt) override;
};

struct ChopAttackState : public PlayerState
{
	void Update(float dt) override;
};