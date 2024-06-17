#pragma once

#include "Core/stack.h"
#include "CharacterStates/EnemyStates.h" // can try to forward decalare state

namespace ECS
{
	struct AIController
	{
		COMPONENT_TYPE(AIController)

		Enemy::StatePool statePool;
		ActionStack<CharacterAction> actions;

		void PushState(ActionState state);
		void PopState();
	};
}