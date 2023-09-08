#include "pch.h"
#include "TransformDebugMenu.h"

#include "ThirdParty/imgui-master/imgui.h"
#include "Debugging/ImGui/ImGuiHelpers.h"

void DisplayRect(RectF& rect)
{
	static float rectxy1[2] = { rect.x1, rect.y1 };
	if (ImGui::InputFloat2("XY1", rectxy1))
	{
		rect.x1 = rectxy1[0];
		rect.y1 = rectxy1[1];
	}

	static float rectxy2[2] = { rect.x2, rect.y2 };
	if (ImGui::InputFloat2("XY2", rectxy2))
	{
		rect.x2 = rectxy2[0];
		rect.y2 = rectxy2[1];
	}

	VectorF center = rect.Center();
	float width = rect.Width();
	float height = rect.Height();

	static float rect_size[2] = { width, height };
	if (ImGui::InputFloat2("size", rect_size))
	{
		rect.SetSize(VectorF(rect_size[0], rect_size[1]));
		rect.SetCenter(center);
	}
}

bool s_displayRect = false;
bool s_displaySizedRect = false;

void DebugMenu::DoTransformDebugMenu(ECS::Transform& component)
{
	if (ImGui::CollapsingHeader("Transform Component"))
	{
		if (ImGui::TreeNode("Component Data"))
		{
			DisplayRect(component.baseRect);

			ImGui::VectorText("Trans Size Factor", component.sizeFactor);

			if (ImGui::Button("Vertical Flip"))
			{
				component.flip = component.flip == SDL_FLIP_HORIZONTAL ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Display"))
		{
			ImGui::Checkbox("Display Base Rect", &s_displayRect);
			if (s_displayRect)
			{
				debugDrawRectOutline(component.baseRect, Colour::Green);
			}

			ImGui::Checkbox("Display Size Factored Rect", &s_displaySizedRect);
			if (s_displaySizedRect)
			{
				RectF renderRect = component.baseRect;
				if (!component.sizeFactor.isZero())
				{
					VectorF center = renderRect.Center();
					renderRect.SetSize(renderRect.Size() * component.sizeFactor);
					renderRect.SetCenter(center);
				}

				debugDrawRectOutline(renderRect, Colour::Red);
			}

			ImGui::TreePop();
		}
	}
}