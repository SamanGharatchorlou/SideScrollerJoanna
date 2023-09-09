#include "pch.h"
#include "ComponentDebugMenu.h"

#include "ECS/EntityCoordinator.h"
#include "System/Window.h"

#include "ThirdParty/imgui-master/imgui.h"
#include "Debugging/ImGui/ImGuiHelpers.h"

static bool s_displaygridLines = true;
StringBuffer32 s_selectedMap = "";

ECS::Component::Type DebugMenu::DoTileMapDebugMenu(ECS::Entity& entity)
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	ECS::Component::Type type = ECS::Component::TileMap;

    if (ecs->HasComponent(entity, type))
    {
		if (ImGui::CollapsingHeader(ECS::ComponentNames[type]))
		{
            ECS::TileMap& tile_map = ecs->GetComponent(TileMap, entity);

			ImGui::Checkbox("Display Grid Lines", &s_displaygridLines);
			if (s_displaygridLines && tile_map.tileMap.tileLayers.size() > 0)
			{
				const SceneTileMapping& map = tile_map.tileMap;

				VectorF window_size = GameData::Get().window->size();
				VectorF map_size = map.mapSize;
				VectorF size_ratio = window_size / map_size;

				const SceneTileMapping::TileLayer& layer = map.tileLayers.front();
				VectorF tile_size = map.tileSize * size_ratio;

				for (u32 i = 0; i < layer.tileMapping.size(); i++)
				{
					VectorI map_index = IndexToMapIndex(i, map.tileCount);
					VectorF pos = map_index.toFloat() * tile_size;
					RectF rect(pos, tile_size);

					Colour colour = Colour::Blue;

					if(map_index.x == 0 ||  map_index.x == layer.tileCount.x - 1)
						colour = Colour::Green;
				
					if(map_index.y == 0 ||  map_index.y == layer.tileCount.y - 1)
						colour = Colour::Green;

					if(map_index.isZero() || map_index == (layer.tileCount - 1)) 
						colour = Colour::Purple;

					DebugDraw::RectOutline(rect, colour);
				}
			}

			if (ImGui::BeginCombo("Select Map", s_selectedMap.c_str()))
			{
				std::vector<BasicString> maps = FileManager::Get()->allFilesInFolder(FileManager::Maps);
				for (u32 i = 0; i < maps.size(); i++) 
				{
					if(!FileManager::HasExt(maps[i].c_str(), ".xml"))
						continue;

					StringBuffer32 name = FileManager::Get()->getItemNameAndExt( maps[i].c_str());
					if (ImGui::Selectable(name.c_str(), false)) 
					{
						s_selectedMap = name.c_str();
					}
				}
				ImGui::EndCombo();
			}

			if(ImGui::ActiveButton("Build Map", !s_selectedMap.empty())) 
			{
				SceneBuilder::BuildTileMap(s_selectedMap.c_str(), tile_map.tileMap);
			}
		}
	}

	return type;
}