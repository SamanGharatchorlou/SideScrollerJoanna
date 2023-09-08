#include "pch.h"
#include "TileMapDebugMenu.h"

#include "ThirdParty/imgui-master/imgui.h"
#include "Debugging/ImGui/ImGuiHelpers.h"

#include "ECS/EntityCoordinator.h"
#include "System/Window.h"
#include "Scene/SceneParsing/SceneBuilder.h"

static bool s_displayRect = false;
static bool s_displaygridLines = true;
StringBuffer32 s_selectedMap = "";

void DebugMenu::DoTileMapDebugMenu(ECS::TileMap& component)
{
	ImGui::Checkbox("Display Grid Lines", &s_displaygridLines);
	if (s_displaygridLines)
	{
		if (component.tileMap.layers.size() > 0)
		{
			VectorF window_size = GameData::Get().window->size();
			VectorF map_size = component.tileMap.mapSize;
			VectorF size_ratio = window_size / map_size;

			const SceneTileMapping& map = component.tileMap;
			const SceneTileMapping::Layer& layer = map.layers.front();
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

				debugDrawRectOutline(rect, colour);
			}
		}
	}

	if (ImGui::BeginCombo("Select Map", s_selectedMap.c_str()))
	{
		std::vector<BasicString> maps = FileManager::Get()->allFilesInFolder(FileManager::Maps);
		for (u32 i = 0; i < maps.size(); i++) 
		{
			if(FileManager::HasExt(maps[i].c_str(), ".xml")) 
			{
				StringBuffer32 name = FileManager::Get()->getItemNameAndExt( maps[i].c_str());
                if (ImGui::Selectable(name.c_str(), false)) 
				{
                   s_selectedMap = name.c_str();
				}
			}
		}
		ImGui::EndCombo();
	}

	if(ImGui::ActiveButton("Build Map", !s_selectedMap.empty())) 
	{
		SceneBuilder::BuildTileMap(s_selectedMap.c_str(), component.tileMap);
	}
}