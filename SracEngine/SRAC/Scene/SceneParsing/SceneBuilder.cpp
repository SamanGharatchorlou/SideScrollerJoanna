#include "pch.h"
#include "SceneBuilder.h"

#include "System/Files/FileManager.h"
#include "System/Files/ConfigManager.h"
#include "Configs.h"

int fast_atoi(const char* str)
{
	int val = 0;
	while (*str) {
		val = val * 10 + (*str++ - '0');
	}
	return val;
}

static void ReadTileMap(XMLParser& parser, TileMap& tile_map)
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
			MapLayer map_layer;
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


void SceneBuilder::ReadScene(const char* mapName)
{
	FileManager* fm = FileManager::Get();
	BasicString full_path = FileManager::Get()->findFile(FileManager::Maps, mapName);

	if (full_path.empty())
		return;

	XMLParser* parser = new XMLParser;
	parser->parseXML(full_path.c_str());
	
	TileMap tile_map;
	ReadTileMap(*parser, tile_map);

	const char* tileset_source = tile_map.tilesetAttributes.at("source").c_str();

	std::vector<BasicString> files = fm->allFilesInFolder(FileManager::Maps);

	BasicString tileset_config = fm->findFile(FileManager::Maps, tileset_source);

	parser->reload(tileset_config.c_str());
	

	tile_map.tilesetAttributes.fillAtributes(parser->rootNode());
	tile_map.tilesetImage.fillAtributes(parser->rootChild("image"));

	//ConfigManager::Get()->Parse< TileMapConfig>(tileMap);

	int a = 4;
}