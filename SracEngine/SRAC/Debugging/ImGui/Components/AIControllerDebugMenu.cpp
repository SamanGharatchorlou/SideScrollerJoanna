#include "pch.h"
#include "ComponentDebugMenu.h"

#include "ECS/Components/AIController.h"
#include "ECS/EntityCoordinator.h"
#include "Debugging/ImGui/ImGuiHelpers.h"

static bool s_enemyCanMove = false;
static bool s_lockCanMove = false;

bool& EnemyCanMove() 
{ 
	return s_enemyCanMove; 
}

ECS::Component::Type DebugMenu::DoAIControllerDebugMenu(ECS::Entity& entity)
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	ECS::Component::Type type = ECS::Component::AIController;

	if (ecs->HasComponent(entity, type))
	{
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
			ImGui::PopID();

			if (ImGui::TreeNode("Restrict Enemy Movement"))
			{
				ImGui::Checkbox("Lock Movement", &s_lockCanMove);
				s_enemyCanMove = s_lockCanMove;

				ImGui::SameLine();
				if(ImGui::Button("Allow movement"))
				{
					s_enemyCanMove = true;
				}

				ImGui::TreePop();
			}
		}
		
		ImGui::PopID();
	}

	return type;
}