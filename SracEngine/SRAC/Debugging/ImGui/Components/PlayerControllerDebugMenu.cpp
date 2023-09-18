#include "pch.h"
#include "ComponentDebugMenu.h"

#include "ECS/EntityCoordinator.h"
#include "Debugging/ImGui/ImGuiHelpers.h"

ECS::Component::Type DebugMenu::DoPlayerControllerDebugMenu(ECS::Entity& entity)
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	ECS::Component::Type type = ECS::Component::PlayerController;

	if (ecs->HasComponent(entity, type))
	{
		if (ImGui::CollapsingHeader(ECS::ComponentNames[type]))
		{
			ECS::PlayerController& pc = ecs->GetComponentRef(PlayerController, entity);
			if (ImGui::TreeNode("Component Data"))
			{
				ImGui::Text("Current State: %s", actionToString(pc.actions.Top().action).c_str());

				ImGui::VectorText("Movement Direction", pc.movementDirection);
				ImGui::VectorText("Facing Direction", pc.facingDirection);

				ImGui::TreePop();
			}
		}
	}

	return type;
}