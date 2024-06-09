#include "pch.h"
#include "PlayerController.h"

using namespace ECS;

void PlayerController::PushState(ActionState action)
{
	DebugPrint(PriorityLevel::Log, "Pushing player action state: %s | %d states left", actionToString(action).c_str(), statePool.size(action));
	
	PlayerState* state = statePool.getObject(action);
	state->entity = entity;
	state->action = action;

	actions.Push(state);
}

void PlayerController::PopState()
{
	PlayerState* state = &actions.Top();
	actions.Pop();
	statePool.returnObject(state, state->action);

	DebugPrint(PriorityLevel::Log, "Returning player action state: %s | %d states left", actionToString(state->action).c_str(), statePool.size(state->action));
}