#pragma once

class Texture;
struct TileMap;

struct MapLayerConfig
{
	std::vector<u32> tildIds;
	StringMap32 attributes;
};

struct TileMapConfig
{
	StringMap32 attributes;

	StringMap32 tilesetAttributes;
	StringMap32 tilesetImage;

	std::vector<MapLayerConfig> layers;
};

struct TileSet
{
	typedef Vector2D<int> Index;

	Texture* texture;
	Vector2D<int> size;
	Vector2D<int> tileSize;
};

struct TileMapLayers
{
	struct Layer
	{
		u32 render_layer;
		TileSet tileset;
		std::vector<TileSet::Index> tilesetIndexes;
	};

	VectorF tileSize;
	std::vector<Layer> layers;
};

static Index IndexToMapIndex(u32 index, Vector2D<int> size)
{
	int row = index / size.x;
	int column = index % size.x;
	return Index(column, row);
}

namespace SceneBuilder
{
	void BuildTileMap(const char* mapName, TileMapLayers& map_layers);
};