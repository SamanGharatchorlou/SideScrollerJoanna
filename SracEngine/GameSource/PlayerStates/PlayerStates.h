#pragma once

#include "SRAC/Game/GameStates/State.h"
#include "Core/Stack.h"
#include "Animations/CharacterStates.h"

#include "Core/ObjectPool.h"

namespace ECS { struct PlayerController; }

struct PlayerState : public State
{
	PlayerState() : action(ActionState::None), playerController(nullptr)  { }

	inline PlayerState& operator = (const PlayerState& state) { 
		playerController = state.playerController; action = state.action; return *this; 
	}

	void SetBaseParameters(ECS::PlayerController* _playerController, ActionState _state) { playerController = _playerController; action = _state;  }
	
	void Push(ActionState action);
	void PopSelf(ActionState action);


	ECS::PlayerController* playerController;
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

struct RunState : public PlayerState
{
	void Update(float dt) override;
};

struct JumpState : public PlayerState
{
	void init() override;
	void Update(float dt) override;
};

struct FallState : public PlayerState
{
	void Update(float dt) override;
};

struct LightAttackState : public PlayerState
{
	void Update(float dt) override;
};