#include "pch.h"
#include "SceneBuilder.h"

#include "Graphics/TextureManager.h"
#include "System/Files/FileManager.h"
#include "System/Files/ConfigManager.h"
#include "Configs.h"
#include "System/Window.h"

#include "ECS/Components.h"

int fast_atoi(const char* str)
{
	int val = 0;
	while (*str) {
		val = val * 10 + (*str++ - '0');
	}
	return val;
}

static void ReadTileMap(XMLParser& parser, TileMapConfig& tile_map)
{
	const XMLNode map_node = parser.rootNode();

	tile_map.attributes.fillAtributes(map_node);

	const XMLNode tileset_node = map_node.child("tileset");
	tile_map.tilesetAttributes.fillAtributes(tileset_node);

	XMLNode layer_node = map_node.child();
	while (layer_node)
	{
		if (StringCompare(layer_node.name(), "layer"))
		{
			TileMapConfig::Layer map_layer;
			map_layer.attributes.fillAtributes(layer_node);

			const char* data_string = layer_node.child("data").value();

			u32 value = 0;
			bool is_value = false;
			while (*data_string != '\0')
			{
				// we dont know what we might find so make sure it just valid numbers
				if (*data_string < '0' || *data_string > '9')
				{
					data_string++;

					if (is_value)
					{
						map_layer.tildIds.push_back(value);
					}

					is_value = false;
					value = 0;
					continue;
				}

				is_value = true;
				value = value * 10 + (*data_string++ - '0');
			}

			tile_map.layers.push_back(map_layer);
		}

		layer_node = layer_node.next();
	}
}


void SceneBuilder::BuildTileMap(const char* mapName, SceneTileMapping& tile_mapping)
{
	TextureManager* tm = TextureManager::Get();
	FileManager* fm = FileManager::Get();

	BasicString full_path = FileManager::Get()->findFileEtx(FileManager::Maps, mapName);

	if (full_path.empty())
		return;

	XMLParser* parser = new XMLParser;
	parser->parseXML(full_path.c_str());
	
	TileMapConfig config;
	ReadTileMap(*parser, config);

	const char* tileset_source = config.tilesetAttributes.at("source").c_str();
	BasicString tileset_config = fm->findFileEtx(FileManager::Maps, tileset_source);
	parser->reload(tileset_config.c_str());
	
	config.tilesetAttributes.fillAtributes(parser->rootNode());
	config.tilesetImage.fillAtributes(parser->rootChild("image"));

	// base map data - sizes and counts
	tile_mapping.tileCount = config.attributes.getVectorI("width", "height");
	tile_mapping.tileSize = config.attributes.getVectorF("tilewidth", "tileheight");
	tile_mapping.mapSize = tile_mapping.tileCount.toFloat() * tile_mapping.tileSize;

	// tilset set data - texture, 
	Texture* tileset_texture = tm->getTexture(config.tilesetImage.at("source"), FileManager::Maps);
	TileSet tileset;
	tileset.texture = tileset_texture;
	tileset.mapSize = config.tilesetImage.getVectorF("width", "height");
	tileset.tileSize = config.tilesetAttributes.getVectorF("tilewidth", "tileheight");

	VectorF tile_count = (tileset.mapSize / tileset.tileSize) + VectorF(0.5f, 0.5f);
	tileset.tileCount = tile_count.toInt();

	// convert a map index into a tilset index
	for (u32 l = 0; l < config.layers.size(); l++)
	{
		TileMapConfig::Layer& layer_config = config.layers[l];
		const u32 tile_count = layer_config.tildIds.size();

		SceneTileMapping::Layer map_layer;
		map_layer.tileset = tileset;
		map_layer.tileMapping.reserve(tile_count);
		map_layer.render_layer = layer_config.attributes.getInt("name");
		map_layer.tileCount = layer_config.attributes.getVectorI("width", "height");

		for (u32 i = 0; i < tile_count; i++)
		{
			VectorI index(-1, -1);
			if(layer_config.tildIds[i] > 0)
			{
				// provides a tile id, so -1 to turn that bad boi into an index
				int idx = layer_config.tildIds[i] - 1;

				// int divide to floor the value
				int row = idx / tileset.tileCount.x;
				int column = idx % tileset.tileCount.x;

				index = VectorI(column, row);
			}
			
			map_layer.tileMapping.push_back(index);
		}

		tile_mapping.layers.push_back(map_layer);
	}
}