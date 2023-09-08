#include "pch.h"
#include "TileMapDebugMenu.h"

#include "ThirdParty/imgui-master/imgui.h"
#include "Debugging/ImGui/ImGuiHelpers.h"

//#include ""

static bool s_displayRect = false;
static bool s_displaygridLines = false;

void DebugMenu::DoTileMapDebugMenu(ECS::TileMap& component)
{
	ImGui::Checkbox("Display Grid Lines", &s_displaygridLines);
	if (s_displaygridLines)
	{
		if (component.tileMap.layers.size() > 0)
		{
			const TileMapLayers::Layer& layer = component.tileMap.layers.front();
			const TileSet& tile_set = layer.tileset;
			VectorF tile_size = tile_set.tileSize.toFloat();
			for (u32 tile = 0; tile < layer.tilesetIndexes.size(); tile++)
			{
				Index map_index = IndexToMapIndex(tile, component.tileMap.tileSize.toInt());
				VectorF pos = map_index.toFloat() * component.tileMap.tileSize;

				RectF rect(pos, component.tileMap.tileSize.toFloat());

				//int last_index = 
				//Colour colour = Colour::Blue;
				//if (tile == 0)
				//	colour = Colour::Green;
				//else if (tile == layer.tilesetIndexes.size() - 1)
				//	colour = Colour::Red;

				//if(map_index.x == 0 ||  map_index.y)

				//debugDrawRectOutline(rect, colour);
			}
		}
	}
}