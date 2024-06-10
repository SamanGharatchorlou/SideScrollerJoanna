#pragma once

#include "Core/ObjectPool.h"
#include "SRAC/Game/GameStates/State.h"
#include "Animations/CharacterStates.h"
#include "CharacterAction.h"

namespace Enemy
{
	struct StatePool : public ObjectPool<CharacterAction, ActionState>
	{
		CharacterAction* createNewObjects(ActionState type, int count, int& outSize) const override;
	};

	struct IdleState : public CharacterAction
	{
		void Update(float dt) override;
	};

	struct RunState : public CharacterAction
	{
		void Update(float dt) override;
	};
}