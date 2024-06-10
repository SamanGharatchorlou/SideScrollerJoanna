#pragma once

#include "Core/stack.h"
#include "CharacterStates/EnemyStates.h" // can try to forward decalare state

namespace ECS
{
	struct AIController
	{
		Enemy::StatePool statePool;
		ActionStack<CharacterAction> actions;
		Entity entity;

		void PushState(ActionState state);
		void PopState();

		static ECS::Component::Type type() { return ECS::Component::AIController; }
	};
}