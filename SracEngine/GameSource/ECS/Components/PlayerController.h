#pragma once

#include "Core/stack.h"
#include "CharacterStates/PlayerStates.h"

namespace ECS
{
	struct PlayerController
	{
		Player::StatePool statePool;

		ActionStack<CharacterAction> actions;

		bool hasMovementInput;

		Entity entity;

		ActionState Action() const { return actions.Top().action; }

		void PushState(ActionState state);
		void PopState();

		static ECS::Component::Type type() { return ECS::Component::PlayerController; }
	};
}