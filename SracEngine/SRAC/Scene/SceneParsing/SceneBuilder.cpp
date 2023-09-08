#include "pch.h"
#include "SceneBuilder.h"

#include "Graphics/TextureManager.h"
#include "System/Files/FileManager.h"
#include "System/Files/ConfigManager.h"
#include "Configs.h"

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
			MapLayerConfig map_layer;
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


void SceneBuilder::BuildTileMap(const char* mapName, TileMapLayers& map_layers)
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

	Texture* tileset_texture = tm->getTexture(config.tilesetImage.at("source"), FileManager::Maps);

	TileSet tileset;
	tileset.texture = tileset_texture;
	tileset.size = config.tilesetImage.getVectorI("width", "height");
	tileset.tileSize = config.tilesetAttributes.getVectorI("tilewidth", "tileheight");

	for (u32 l = 0; l < config.layers.size(); l++)
	{
		MapLayerConfig& layer_config = config.layers[l];
		const u32 tile_count = layer_config.tildIds.size();

		TileMapLayers::Layer map_layer;
		map_layer.tileset = tileset;
		map_layer.tilesetIndexes.reserve(tile_count);
		map_layer.render_layer = layer_config.attributes.getInt("name");

		for (u32 i = 0; i < tile_count; i++)
		{
			int id = layer_config.tildIds[i];

			// yes int divide so i floor the value
			int row = id / tileset.tileSize.x;
			int column = id % tileset.tileSize.x;

			if (id > 0)
				int a = 4;

			TileSet::Index index(column, row);
			map_layer.tilesetIndexes.push_back(index);
		}

		map_layers.layers.push_back(map_layer);
	}

	//map_layers.tileSize = config.attributes.getVectorI("tilewidth", "tileheight");

	GameSettingsConfig* gs = ConfigManager::Get()->getConfig<GameSettingsConfig>("GameSettings");


	const VectorF screenSize = gs->settings.getVectorF("Width", "Height");

	VectorF tile_size = config.attributes.getVectorF("tilewidth", "tileheight");
	float map_width = tile_size.x * config.attributes.getFloat("width");
	float map_height = tile_size.y * config.attributes.getFloat("height");

	const VectorF mapSize = VectorF(map_width, map_height);
	VectorF ratio = screenSize / mapSize;

	map_layers.tileSize = tile_size * ratio;
}