#pragma once
#include "pch.h"
#include "ComponentDebugMenu.h"

#include "ECS/EntityCoordinator.h"
#include "Debugging/ImGui/ImGuiHelpers.h"
#include "Input/InputManager.h"
#include "ECS/EntSystems/PathingSystem.h"
#include "Game/SystemStateManager.h"
#include "GameStates/GameState.h"
#include "ECS/Components/TileMap.h"

#include "CharacterStates/EnemyStates.h"

#include "System/Window.h"

bool s_displayPath = false;
bool s_displayTarget = false;

ECS::Component::Type DebugMenu::DoPathingDebugMenu(ECS::Entity& entity)
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	ECS::Component::Type type = ECS::Component::Pathing;

	//if(ecs->HasComponent(entity, type)) 
    {
		if (ImGui::CollapsingHeader(ECS::ComponentNames[type]))
		{
			ECS::Pathing& pathing = ecs->GetComponentRef(Pathing, entity);
			InputManager* input = InputManager::Get();
			ImGui::PushID(entity + (int)type);

			ImGui::Checkbox("Display Path", &s_displayPath);

			if (s_displayPath)
			{
				const VectorF world_tile_size = ECS::TileMap::GetMapSizeRatio() * ECS::c_tileSize;
				std::vector<VectorI> path = pathing.path;

				for( u32 i = 0; i < path.size(); i++ )
				{
					VectorF pos = path[i].toFloat() * world_tile_size;
					VectorF size = world_tile_size;
					RectF rect(pos, size);

					DebugDraw::RectOutline(rect, Colour::Blue);
				}
			}

			
			ImGui::Checkbox("Display Target", &s_displayTarget);

			if(s_displayTarget)
			{
				if(ecs->IsAlive(pathing.target))
				{
					const ECS::Transform& target_transform = ecs->GetComponentRef(Transform, pathing.target);
					VectorF target_position = target_transform.rect.Center();
				
					const ECS::CharacterState& state = ecs->GetComponentRef(CharacterState, entity);
					const ECS::Transform& transform = ecs->GetComponentRef(Transform, entity);

					float distance_squard = (transform.rect.Center() - target_position).lengthSquared();
					float attack_range = Enemy::GetAttackRange(entity);

					VectorF looking_at = transform.rect.Center() + (state.facingDirection.toFloat() * attack_range);
					Colour colour = Colour::Green;

					if(ECS::Collider* target_collider = ecs->GetComponent(Collider, pathing.target))
					{
						if(target_collider->contains(looking_at))
						{
							colour = Colour::Red;
						}
					}
					
					DebugDraw::Point(looking_at, 10, colour);
				}
			}

			
			ImGui::PopID();
		}
	}

	return type;
}