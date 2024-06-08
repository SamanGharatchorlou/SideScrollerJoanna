#include "pch.h"
#include "PlayerController.h"

using namespace ECS;


void PlayerController::PushState(ActionState action)
{
	DebugPrint(PriorityLevel::Log, "Pushing player action state: %s | %d states left", actionToString(action).c_str(), statePool.size(action));
	PlayerState* state = statePool.getObject(action);
	state->SetBaseParameters(this, action);
	actions.Push(state);
}