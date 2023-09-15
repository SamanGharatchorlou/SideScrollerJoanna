#pragma once

#include "SRAC/Game/GameStates/State.h"
#include "Core/Stack.h"
#include "Animations/CharacterStates.h"

#define DECLARE_CONSTRUCTOR(type) \
 type##State(ActionStack<PlayerState>* _actionState, ActionState _state) : PlayerState(_actionState,_state) { }

struct PlayerState : public State
{
	PlayerState() : entity(ECS::EntityInvalid), action(ActionState::None), actionStack(nullptr)  { }

	PlayerState(ActionStack<PlayerState>* _actionState, ActionState _state) :
		actionStack(_actionState), action(_state) { }

	inline PlayerState& operator = (const PlayerState& state) { 
		entity = state.entity; actionStack = state.actionStack; action = state.action; return *this; 
	}

	ActionStack<PlayerState>* actionStack;
	ECS::Entity entity;
	ActionState action;
};

struct IdleState : public PlayerState
{
	void Update(float dt) override;
};

struct RunState : public PlayerState
{
	DECLARE_CONSTRUCTOR(Run);
	void Update(float dt) override;
};

struct JumpState : public PlayerState
{
	DECLARE_CONSTRUCTOR(Jump);

	void init() override;
	void Update(float dt) override;
};

struct FallState : public PlayerState
{
	DECLARE_CONSTRUCTOR(Fall);

	void Update(float dt) override;
};

struct LightAttackState : public PlayerState
{
	DECLARE_CONSTRUCTOR(LightAttack);

	void Update(float dt) override;
};