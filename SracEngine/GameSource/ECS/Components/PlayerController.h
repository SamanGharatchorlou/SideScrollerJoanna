#pragma once

#include "Core/stack.h"
#include "CharacterStates/PlayerStates.h"

namespace ECS
{
	struct PlayerController
	{
		COMPONENT_TYPE(PlayerController)

		Player::StatePool statePool;

		ActionStack<CharacterAction> actions;

		bool hasMovementInput;

		ActionState Action() const { return actions.Top().action; }

		void PushState(ActionState state);
		void PopState();
	};
}