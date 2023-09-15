#include "pch.h"
#include "TileMapSystem.h"

#include "GameSource/ECS/Components.h"
#include "ECS/EntityCoordinator.h"
#include "Graphics/RenderManager.h"
#include "System/Window.h"

namespace ECS
{
	void TileMapSystem::Update(float dt)
	{
		EntityCoordinator* ecs = GameData::Get().ecs;

		for (Entity entity : entities)
		{
			const TileMap& tile_map = ecs->GetComponentRef(TileMap, entity);

			const VectorF window_size = GameData::Get().window->size();
			const VectorF map_size = tile_map.tileMap.mapSize;
			const VectorF size_ratio = window_size / map_size;
			
			const SceneTileMapping& map = tile_map.tileMap;
			const VectorF tile_size = map.tileSize * size_ratio;

			for (u32 l = 0; l < tile_map.tileMap.tileLayers.size(); l++)
			{
				const SceneTileMapping::TileLayer& layer = tile_map.tileMap.tileLayers[l];
				const TileSet& tile_set = layer.tileset;

				for (u32 idx = 0; idx < layer.tileMapping.size(); idx++)
				{
					VectorI index = layer.tileMapping[idx];
					if (index.isZero())
						continue;
									
					// Rect for on screen size and position
					VectorI map_index = IndexToMapIndex(idx, map.tileCount);
					VectorF pos = map_index.toFloat() * tile_size;
					RectF rect(pos, tile_size);

					// SubRect for tile map entry so we know what bit of texture to draw
					VectorF top_left = index.toFloat() * tile_set.tileSize;
					RectF subRect(top_left, tile_set.tileSize);

					RenderPack pack(tile_set.texture, rect, layer.render_layer);
					pack.subRect = subRect;

					GameData::Get().renderManager->AddRenderPacket(pack);
				}
			}
		}
	}
}