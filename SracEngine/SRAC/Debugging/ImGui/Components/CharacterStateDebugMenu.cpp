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
				ImGui::Text("Current State: %s", actionToString(cs.action).c_str());

				ImGui::Checkbox("Can Change", &cs.canChange);

				bool on_floor = cs.OnFloor();
				ImGui::Checkbox("On Floor", &on_floor);

				ImGui::TreePop();
			}
		}
	}

	return type;
}