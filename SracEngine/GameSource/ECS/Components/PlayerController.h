#pragma once

#include "PlayerStates/PlayerStates.h"

namespace ECS
{
	struct PlayerController
	{
		PlayerStatePool statePool;

		ActionStack<PlayerState> actions;

		VectorI movementDirection;
		VectorF facingDirection;
		bool hasMovementInput;
		bool isRunning;

		Entity entity;

		ActionState Action() const { return actions.Top().action; }
		void PushState(ActionState state);


		static ECS::Component::Type type() { return ECS::Component::PlayerController; }
	};
}