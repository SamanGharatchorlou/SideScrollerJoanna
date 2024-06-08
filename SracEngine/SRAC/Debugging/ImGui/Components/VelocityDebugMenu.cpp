#include "pch.h"
#include "ComponentDebugMenu.h"

#include "ECS/EntityCoordinator.h"
#include "Debugging/ImGui/ImGuiHelpers.h"
#include "ECS/Components/Physics.h"
#include "ECS/Components/PlayerController.h"

ECS::Component::Type DebugMenu::DoPhysicsDebugMenu(ECS::Entity& entity)
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	ECS::Component::Type type = ECS::Component::Physics;

	if (ecs->HasComponent(entity, type))
	{
		if (ImGui::CollapsingHeader(ECS::ComponentNames[type]))
		{
			ECS::Physics& physics = ecs->GetComponentRef(Physics, entity);
			if (ImGui::TreeNode("Component Data"))
			{
				ImGui::VectorText("Speed", physics.speed);
				ImGui::VectorText("Max Speed", physics.maxSpeed);
				ImGui::VectorText("Acceleration", physics.acceleration);

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Display"))
			{
				if (const ECS::Transform* transform = ecs->GetComponent(Transform, entity))
				{
					if (ECS::PlayerController* player_controller = ecs->GetComponent(PlayerController, entity))
					{
						if (!physics.speed.isZero())
						{
							VectorF A = transform->rect.Center();
							VectorF B = A + physics.speed * player_controller->movementDirection.toFloat() * 10.0f;

							//if (physics.speed > physics.maxSpeed * 0.8f)
							//	int a = 4;

							DebugDraw::Line(A, B, Colour::Green);
						}
					}


				}

				ImGui::TreePop();
			}
		}
	}

	return type;
}