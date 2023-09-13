#include "pch.h"
#include "ComponentDebugMenu.h"

#include "ECS/EntityCoordinator.h"
#include "Debugging/ImGui/ImGuiHelpers.h"

ECS::Component::Type DebugMenu::DoVelocityDebugMenu(ECS::Entity& entity)
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	ECS::Component::Type type = ECS::Component::Velocity;

	if (ecs->HasComponent(entity, type))
	{
		if (ImGui::CollapsingHeader(ECS::ComponentNames[type]))
		{
			ECS::Velocity& velocity = ecs->GetComponent(Velocity, entity);
			if (ImGui::TreeNode("Component Data"))
			{
				ImGui::VectorText("Speed", velocity.speed);
				ImGui::VectorText("Max Speed", velocity.maxSpeed);
				ImGui::VectorText("Current Acceleration", velocity.currAcceleration);
				ImGui::VectorText("Acceleration", velocity.acceleration);
				ImGui::VectorText("Max Acceleration", velocity.maxAcceleration);

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Display"))
			{
				if (ecs->HasComponent(entity, ECS::Component::Transform))
				{
					const ECS::Transform& transform = ecs->GetComponent(Transform, entity);

					VectorF A = transform.baseRect.Center();
					VectorF B = A + velocity.speed.normalise();

					DebugDraw::Line(A, B, Colour::Green);
				}


				ImGui::TreePop();
			}
		}
	}

	return type;
}