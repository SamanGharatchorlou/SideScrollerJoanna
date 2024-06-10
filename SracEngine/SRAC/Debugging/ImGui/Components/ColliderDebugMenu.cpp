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

				bool can_move_up = !HasFlag(collider.mRuntimeFlags, ECS::Collider::RestrictUp);
				ImGui::Checkbox("Can Move Up", &can_move_up);

				bool can_move_right = !HasFlag(collider.mRuntimeFlags, ECS::Collider::RestrictRight);
				ImGui::Checkbox("Can Move Right", &can_move_right);

				bool can_move_down = !HasFlag(collider.mRuntimeFlags, ECS::Collider::RestrictDown);
				ImGui::Checkbox("Can Move Down", &can_move_down);

				bool can_move_left = !HasFlag(collider.mRuntimeFlags, ECS::Collider::RestrictLeft);
				ImGui::Checkbox("Can Move Left", &can_move_left);

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Display"))
			{
				RectF rect = collider.mRect;
				DebugDraw::RectOutline(rect, Colour::Purple);

				ImGui::TreePop();
			}
		}
	}

	return type;
}