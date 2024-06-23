#include "pch.h"
#include "ComponentDebugMenu.h"

#include "ECS/Components/AIController.h"
#include "ECS/EntityCoordinator.h"
#include "Debugging/ImGui/ImGuiHelpers.h"

static bool s_enemyCanMove = true;

bool EnemyCanMove() 
{ 
	return s_enemyCanMove; 
}

ECS::Component::Type DebugMenu::DoAIControllerDebugMenu(ECS::Entity& entity)
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	ECS::Component::Type type = ECS::Component::AIController;

	ImGui::PushID(entity + (int)type);
	if (ImGui::CollapsingHeader(ECS::ComponentNames[type]))
	{
		if (ImGui::TreeNode("Component Data"))
		{
			ECS::AIController& pc = ecs->GetComponentRef(AIController, entity);
			ECS::CharacterState& state = ecs->GetComponentRef(CharacterState, entity);

			ImGui::VectorText("Movement Direction", state.movementDirection);
			ImGui::VectorText("Facing Direction", state.facingDirection);

			ImGui::Text("State count: %d", pc.actions.stack.size());
			ImGui::Text("Current State: %s", actionToString(pc.actions.Top().action).c_str());

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Restrict Enemy Movement"))
		{
			ImGui::Checkbox("Allow Enemy Movement", &s_enemyCanMove);
			ImGui::TreePop();
		}
	}
		
	ImGui::PopID();

	return type;
}