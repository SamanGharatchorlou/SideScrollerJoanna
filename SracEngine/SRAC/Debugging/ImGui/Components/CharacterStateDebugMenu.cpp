#include "pch.h"
#include "ComponentDebugMenu.h"

#include "ECS/EntityCoordinator.h"
#include "Debugging/ImGui/ImGuiHelpers.h"

#include "Animations/CharacterStates.h"

ECS::Component::Type DebugMenu::DoCharacterStateDebugMenu(ECS::Entity& entity)
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	ECS::Component::Type type = ECS::Component::CharacterState;

	if (ecs->HasComponent(entity, type))
	{
		if (ImGui::CollapsingHeader(ECS::ComponentNames[type]))
		{
			ECS::CharacterState& cs = ecs->GetComponentRef(CharacterState, entity);
			if (ImGui::TreeNode("Component Data"))
			{
				ImGui::Text("Current State: %s", actionToString(cs.Action()).c_str());
				ImGui::Text("Previous State: %s", actionToString(cs.actions.Previous().action).c_str());

				ImGui::VectorText("Movement Direction", cs.movementDirection);
				ImGui::VectorText("Facing Direction", cs.facingDirection);

				ImGui::Checkbox("In Transition", &cs.inTransition);
				ImGui::Checkbox("Can Change", &cs.canChange);
				ImGui::Checkbox("On Floor", &cs.onFloor);

				ImGui::TreePop();
			}
		}
	}

	return type;
}