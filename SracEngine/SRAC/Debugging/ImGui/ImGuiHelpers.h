#pragma once

#include "Core/Vector2D.h"
#include "ThirdParty/imgui-master/imgui.h"

namespace ImGui
{
	static void VectorText(const char* label, VectorF vector)
	{
		ImGui::Text("%s: %f, %f", label, vector.x, vector.y);
	}

	static void VectorText(const char* label, Vector2D<int> vector)
	{
		ImGui::Text("%s: %d, %d", label, vector.x, vector.y);
	}

	static void InputVectorF(const char* label, VectorF& vector)
	{
		ImGui::PushID(label);
		float vec[2] = { vector.x, vector.y };
		if (ImGui::InputFloat2(label, vec))
		{
			vector = VectorF(vec[0], vec[1]);
		}
		ImGui::PopID();
	}

	static bool ActiveButton(const char* label, bool isActive)
	{
		if (isActive)
		{
			ImGui::BeginDisabled();
		}

		bool did_press = ImGui::Button(label);

		if (isActive)
		{
			ImGui::EndDisabled();
		}

		return did_press;
	}
}