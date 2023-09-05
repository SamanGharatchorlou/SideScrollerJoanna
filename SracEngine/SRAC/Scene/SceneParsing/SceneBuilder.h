#pragma once

struct TileMapConfig;

struct MapLayer
{
	std::vector<u32> tildIds;
	StringMap32 attributes;
};

struct TileMap
{
	StringMap32 attributes;

	StringMap32 tilesetAttributes;
	StringMap32 tilesetImage;

	std::vector<MapLayer> layers;
};


namespace SceneBuilder
{
	void ReadScene(const char* mapName);
};