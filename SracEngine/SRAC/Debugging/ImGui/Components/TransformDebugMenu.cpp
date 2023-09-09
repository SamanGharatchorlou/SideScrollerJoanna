#include "pch.h"
#include "ComponentDebugMenu.h"

#include "ECS/EntityCoordinator.h"
#include "ECS/Components.h"
#include "ThirdParty/imgui-master/imgui.h"
#include "Debugging/ImGui/ImGuiHelpers.h"

bool s_displayRect = false;
bool s_displaySizedRect = false;

ECS::Component::Type DebugMenu::DoTransformDebugMenu(ECS::Entity& entity)
{
    ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	ECS::Component::Type type = ECS::Component::Transform;

    if (ecs->HasComponent(entity, type))
    {
		if (ImGui::CollapsingHeader(ECS::ComponentNames[type]))
		{
			ECS::Transform& transform = ecs->GetComponent(Transform, entity);
			if (ImGui::TreeNode("Component Data"))
			{
				ImGui::DisplayRect(transform.baseRect);

				ImGui::VectorText("Trans Size Factor", transform.sizeFactor);

				if (ImGui::Button("Vertical Flip"))
				{
					transform.flip = transform.flip == SDL_FLIP_HORIZONTAL ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
				}

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Display"))
			{
				ImGui::Checkbox("Display Base Rect", &s_displayRect);
				if (s_displayRect)
				{
					DebugDraw::RectOutline(transform.baseRect, Colour::Green);
				}

				ImGui::Checkbox("Display Size Factored Rect", &s_displaySizedRect);
				if (s_displaySizedRect)
				{
					RectF renderRect = transform.baseRect;
					if (!transform.sizeFactor.isZero())
					{
						VectorF center = renderRect.Center();
						renderRect.SetSize(renderRect.Size() * transform.sizeFactor);
						renderRect.SetCenter(center);
					}

					DebugDraw::RectOutline(renderRect, Colour::Red);
				}

				ImGui::TreePop();
			}
		}
	}

	return type;
}