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

static ECS::Entity s_entWithNoPath;
bool DebugMenu::RemovePath(ECS::Entity& entity) { return s_entWithNoPath == entity; }

ECS::Component::Type DebugMenu::DoPathingDebugMenu(ECS::Entity& entity)
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	ECS::Component::Type type = ECS::Component::Pathing;

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
			const ECS::Transform& transform = ecs->GetComponentRef(Transform, entity);

			const float attack_range = Enemy::GetAttackRange(entity);

			const VectorI directions[4] = { VectorI(0,-1), VectorI( 1, 0), VectorI(0, 1), VectorI(-1, 0) };
			for( u32 i = 0; i < 4; i++ )
			{
				VectorF looking_at = transform.rect.Center() + (directions[i].toFloat() * attack_range);
					
				Colour colour = Colour::Green;

				VectorI direction;
				if(Enemy::CanAttackTarget(entity, direction))
				{
					colour = Colour::Red;
				}

				DebugDraw::Point(looking_at, 10, colour);
			}
		}

		bool is_entity = s_entWithNoPath == entity;

		if(ImGui::Checkbox("Remove entity pathing", &is_entity))
		{
			if(is_entity)
			{
				s_entWithNoPath = entity;
			}
			else
			{
				s_entWithNoPath = ECS::EntityInvalid;
			}
		}
	
		ImGui::PopID();
	}

	return type;
}