#pragma once

class Texture;

struct TileSet
{
	Texture* texture;

	VectorF mapSize;
	VectorF tileSize;
	VectorI tileCount;
};

struct SceneTileMapping
{
	struct TileLayer
	{
		u32 render_layer;
		VectorI tileCount;
		TileSet tileset;
		std::vector<VectorI> tileMapping;
	};

	struct ObjectLayer
	{
		u32 id;
		StringBuffer32 name;
		std::vector<RectF> rects;
	};

	VectorF mapSize;
	VectorF tileSize;
	VectorI tileCount;

	std::vector<TileLayer> tileLayers;
	std::vector<ObjectLayer> objectLayers;

	std::vector<u32> colliderEntities;
};

static VectorI IndexToMapIndex(u32 index, Vector2D<int> size)
{
	int row = index / size.x;
	int column = index % size.x;
	return VectorI(column, row);
}

namespace SceneBuilder
{
	void BuildTileMap(const char* mapName, SceneTileMapping& map_layers);
};