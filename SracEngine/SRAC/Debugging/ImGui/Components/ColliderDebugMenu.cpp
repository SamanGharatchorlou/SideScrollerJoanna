#include "pch.h"
#include "ComponentDebugMenu.h"

#include "ECS/EntityCoordinator.h"
#include "Debugging/ImGui/ImGuiHelpers.h"

ECS::Component::Type DebugMenu::DoColliderDebugMenu(ECS::Entity& entity)
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	ECS::Component::Type type = ECS::Component::Collider;

	if(ecs->HasComponent(entity, type)) 
    {
		if (ImGui::CollapsingHeader(ECS::ComponentNames[type]))
		{
			ECS::Collider& collider = ecs->GetComponentRef(Collider, entity);
			if (ImGui::TreeNode("Component Data"))
			{
				RectF rect = collider.mRect;
				ImGui::DisplayRect(rect);

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Display"))
			{
				RectF rect = collider.mRect;
				DebugDraw::RectFill(rect, Colour::Purple);

				ImGui::TreePop();
			}
		}
	}

	return type;
}