#include "pch.h"
#include "TileMapSystem.h"

#include "GameSource/ECS/Components.h"
#include "ECS/EntityCoordinator.h"
#include "Graphics/RenderManager.h"

namespace ECS
{
	void TileMapSystem::Update(float dt)
	{
		EntityCoordinator* ecs = GameData::Get().ecs;

		for (Entity entity : entities)
		{
			const TileMap& tile_map = ecs->GetComponent(TileMap, entity);

			for (u32 i = 0; i < tile_map.tileMap.layers.size(); i++)
			{
				const TileMapLayers::Layer& layer = tile_map.tileMap.layers[i];
				const TileSet& tile_set = layer.tileset;
				VectorF tile_size = tile_set.tileSize.toFloat();
				for (u32 tile = 0; tile < layer.tilesetIndexes.size(); tile++)
				{
					TileSet::Index index = layer.tilesetIndexes[i];
					if (index.isZero())
						continue;

					VectorF top_left = (tile_set.tileSize * index).toFloat();
					RectF subRect(top_left, tile_size);

					Index map_index = IndexToMapIndex(i, tile_set.tileSize);
					VectorF pos = (map_index * tile_map.tileMap.tileSize.toInt()).toFloat();

					RectF rect(pos, tile_map.tileMap.tileSize.toFloat());

					RenderPack pack(tile_set.texture, rect, layer.render_layer);
					pack.subRect = subRect;

					GameData::Get().renderManager->AddRenderPacket(pack);
				}
			}
		}
	}
}