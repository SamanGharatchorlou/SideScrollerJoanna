#pragma once
#include "pch.h"
#include "ComponentDebugMenu.h"

#include "ECS/EntityCoordinator.h"
#include "Debugging/ImGui/ImGuiHelpers.h"
#include "Input/InputManager.h"
#include "ECS/EntSystems/PathingSystem.h"
#include "Game/SystemStateManager.h"
#include "GameStates/GameState.h"

#include "System/Window.h"

static bool s_selectStartPath = false;
static bool s_selectEndPath = false;

static VectorF s_startPosition;
static VectorF s_endPosition;

StringBuffer32 s_selectionBuffer = "Select Pathing Points";

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
				ImGui::InputVectorF("Start Point", s_startPosition);
				ImGui::InputVectorF("End Point", s_endPosition);

				if(ImGui::Button("Select Pathing Points"))
				{
					s_selectStartPath = true;
					s_selectionBuffer = "Selecting Start Point";
				}

				if(s_selectEndPath)
				{
					if (input->isCursorPressed(Cursor::ButtonType::Left))
					{
						s_endPosition = input->cursorPosition();
						s_selectStartPath = false;
						s_selectEndPath = false;
					}
				}

				if(s_selectStartPath)
				{
					if (input->isCursorPressed(Cursor::ButtonType::Left))
					{
						s_startPosition = input->cursorPosition();
						s_selectStartPath = false;
						s_selectEndPath = true;
						
						s_selectionBuffer = "Selecting End Point";
					}
				}



				if(!s_startPosition.isZero() && !s_endPosition.isZero())
				{
					const GameState* gs = GameData::Get().systemStateManager->GetActiveState<GameState>();
							
					ECS::EntityCoordinator* ecs = GameData::Get().ecs;
					const ECS::TileMap& map = ecs->GetComponentRef(TileMap, gs->activeMap);

					VectorF window_size = GameData::Get().window->size();
					VectorF size_ratio = window_size / map.tileMap.mapSize;
					VectorF tile_size = VectorF(8.0f, 8.0f);
					VectorF world_tile_size = tile_size * size_ratio;

					std::vector<VectorI> path = ECS::PathingSystem::FindPath(s_startPosition, s_endPosition);
					for( u32 i = 0; i < path.size(); i++ )
					{
						VectorF pos = path[i].toFloat() * world_tile_size;
						VectorF size = world_tile_size;
						RectF rect(pos, size);

						DebugDraw::RectFill(rect, Colour::Blue);
					}
				}

				ImGui::TreePop();
			}
		}
	}

	return type;
}