#include "pch.h"
#include "AIControllerSystem.h"

#include "ECS/Components/Components.h"
#include "ECS/EntityCoordinator.h"
#include "Input/InputManager.h"
#include "ECS/Components/Physics.h"

namespace ECS
{
	using namespace ECS;

	static void PushState(AIController& aic, ActionState action)
	{
		DebugPrint(PriorityLevel::Log, "Pushing player action state: %s | %d states left", actionToString(action).c_str(), aic.statePool.size(action));
	
		CharacterAction* state = aic.statePool.getObject(action);
		state->entity = aic.entity;
		state->action = action;

		aic.actions.Push(state);
	}

	static void PopState(AIController& aic)
	{
		CharacterAction* state = &aic.actions.Top();
		aic.actions.Pop();
		aic.statePool.returnObject(state, state->action);

		DebugPrint(PriorityLevel::Log, "Returning player action state: %s | %d states left", actionToString(state->action).c_str(), aic.statePool.size(state->action));
	}

	void AIControllerSystem::Update(float dt)
	{
		EntityCoordinator* ecs = GameData::Get().ecs;

 		for (Entity entity : entities)
		{
			AIController& aic = ecs->GetComponentRef(AIController, entity);
			CharacterState& state = ecs->GetComponentRef(CharacterState, entity);
			Physics& physics = ecs->GetComponentRef(Physics, entity);
			Transform& transform = ecs->GetComponentRef(Transform, entity);

			transform.rect.SetCenter(transform.targetCenterPosition);

			if(aic.actions.HasAction())
				aic.actions.Top().Update(dt);
			else
				PushState(aic, ActionState::Idle);

			aic.actions.ProcessStateChanges();
			state.action = aic.actions.Top().action;

			
			// where we're trying to move to
			transform.targetCenterPosition = transform.rect.Translate(physics.speed).Center();
		}
	}
}