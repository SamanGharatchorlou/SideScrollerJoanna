#pragma once

#include "SRAC/Game/GameStates/State.h"
#include "Core/Stack.h"
#include "Animations/CharacterStates.h"

#define DECLARE_CONSTRUCTOR(type) \
 type##State(ActionStack<PlayerState>& _actionState, ActionState _state) : PlayerState(_actionState,_state) { }

struct PlayerState : public State
{
	PlayerState(ActionStack<PlayerState>& _actionState, ActionState _state) :
		actionStack(_actionState), state(_state) { }

	ActionStack<PlayerState>& actionStack;
	const ActionState state;
};

struct IdleState : public PlayerState
{
	void slowUpdate(float dt) override;
};

struct RunState : public PlayerState
{
	DECLARE_CONSTRUCTOR(Run);
	void slowUpdate(float dt) override;
};

struct JumpState : public PlayerState
{
	DECLARE_CONSTRUCTOR(Jump);
	void slowUpdate(float dt) override;
};

struct LightAttackState : public PlayerState
{
	DECLARE_CONSTRUCTOR(LightAttack);
	void slowUpdate(float dt) override;
};