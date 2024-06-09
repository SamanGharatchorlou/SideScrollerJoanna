#pragma once

#include "Core/ObjectPool.h"
#include "SRAC/Game/GameStates/State.h"
#include "Animations/CharacterStates.h"
#include "CharacterAction.h"

namespace Player
{
	struct StatePool : public ObjectPool<CharacterAction, ActionState>
	{
		CharacterAction* createNewObjects(ActionState type, int count, int& outSize) const override;
	};

	struct IdleState : public CharacterAction
	{
		void Update(float dt) override;
	};

	struct WalkState : public CharacterAction
	{
		void Update(float dt) override;
	};

	struct RunState : public CharacterAction
	{
		void Update(float dt) override;
	};

	struct DodgeState : public CharacterAction
	{
		void Init() override;
		void Update(float dt) override;
	};

	struct SlashAttackState : public CharacterAction
	{
		void Update(float dt) override;
	};

	struct ChopAttackState : public CharacterAction
	{
		void Update(float dt) override;
	};
}