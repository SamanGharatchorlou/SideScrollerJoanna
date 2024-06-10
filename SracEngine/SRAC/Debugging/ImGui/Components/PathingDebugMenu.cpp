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

#include "System/Window.h"

ECS::Component::Type DebugMenu::DoPathingDebugMenu(ECS::Entity& entity)
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	ECS::Component::Type type = ECS::Component::Pathing;

	if(ecs->HasComponent(entity, type)) 
    {
		if (ImGui::CollapsingHeader(ECS::ComponentNames[type]))
		{
			ECS::Pathing& pathing = ecs->GetComponentRef(Pathing, entity);
			InputManager* input = InputManager::Get();
			if (ImGui::TreeNode("Display"))
			{
				const VectorF world_tile_size = ECS::TileMap::GetMapSizeRatio() * ECS::c_tileSize;
				std::vector<VectorI> path = pathing.path;

				for( u32 i = 0; i < path.size(); i++ )
				{
					VectorF pos = path[i].toFloat() * world_tile_size;
					VectorF size = world_tile_size;
					RectF rect(pos, size);

					DebugDraw::RectFill(rect, Colour::Blue);
				}

				ImGui::TreePop();
			}
		}
	}

	return type;
}