#pragma once

class Texture;
struct TileMap;


struct TileMapConfig
{
	struct Layer
	{
		std::vector<u32> tildIds;
		StringMap32 attributes;
	};

	StringMap32 attributes;

	StringMap32 tilesetAttributes;
	StringMap32 tilesetImage;

	std::vector<Layer> layers;
};


struct TileSet
{
	Texture* texture;

	VectorF mapSize;
	VectorF tileSize;
	VectorI tileCount;
};

struct SceneTileMapping
{
	struct Layer
	{
		u32 render_layer;

		VectorI tileCount;
		TileSet tileset;

		std::vector<VectorI> tileMapping;

	};

	VectorF mapSize;
	VectorF tileSize;
	VectorI tileCount;

	std::vector<Layer> layers;
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