#include "pch.h"
#include "SceneBuilder.h"

#include "Graphics/TextureManager.h"
#include "System/Files/FileManager.h"
#include "System/Files/ConfigManager.h"
#include "Configs.h"
#include "System/Window.h"

#include "ECS/EntityCoordinator.h"
#include "ECS/Components.h"
#include "ECS/Components/Collider.h"

int fast_atoi(const char* str)
{
	int val = 0;
	while (*str) {
		val = val * 10 + (*str++ - '0');
	}
	return val;
}

struct TileMapConfig
{
	struct TileLayer
	{
		std::vector<u32> tildIds;
		StringMap32 attributes;
	};

	struct ObjectLayer 
	{
		StringMap32 groupAttributes;
		std::vector<StringMap32> objectAttributes;
	};

	StringMap32 attributes;

	StringMap32 tilesetAttributes;
	//StringMap32 tilesetImage;

	ObjectLayer objectLayer;

	std::vector<TileLayer> tileLayers;
};

static void FillTileSet(TileSet& tile_set, const char* tileset_name) 
{
	BasicString tileset_config = FileManager::Get()->findFileEtx(FileManager::Maps, tileset_name);
	
	XMLParser* tilseset_parser = new XMLParser;
	tilseset_parser->parseXML(tileset_config.c_str());

	StringMap32 tilsetAttributes;
	tilsetAttributes.fillAtributes(tilseset_parser->rootNode());
	tilsetAttributes.fillAtributes(tilseset_parser->rootChild("image"));

	tile_set.texture = TextureManager::Get()->getTexture(tilsetAttributes.at("source"), FileManager::Maps);;
	tile_set.mapSize = tilsetAttributes.getVectorF("width", "height");
	tile_set.tileSize = tilsetAttributes.getVectorF("tilewidth", "tileheight");

	VectorF tile_count = (tile_set.mapSize / tile_set.tileSize) + VectorF(0.5f, 0.5f);
	tile_set.tileCount = tile_count.toInt();
	
	delete tilseset_parser;
}

void SceneBuilder::BuildTileMap(const char* mapName, SceneTileMapping& tile_mapping)
{
	FileManager* fm = FileManager::Get();
	BasicString full_path = FileManager::Get()->findFileEtx(FileManager::Maps, mapName);

	if (full_path.empty())
		return;

	XMLParser* parser = new XMLParser;
	parser->parseXML(full_path.c_str());
	
	TileMapConfig config;
	const XMLNode map_node = parser->rootNode();
	
	config.attributes.fillAtributes(map_node);

	const XMLNode tileset_node = map_node.child("tileset");
	config.tilesetAttributes.fillAtributes(tileset_node);

	// base map data - sizes and counts
	tile_mapping.tileCount = config.attributes.getVectorI("width", "height");
	tile_mapping.tileSize = config.attributes.getVectorF("tilewidth", "tileheight");
	tile_mapping.mapSize = tile_mapping.tileCount.toFloat() * tile_mapping.tileSize;

	const VectorF window_size = GameData::Get().window->size();
	const VectorF size_ratio = window_size / tile_mapping.mapSize;

	XMLNode layer_node = map_node.child();
	while (layer_node)
	{
		if (StringCompare(layer_node.name(), "layer"))
		{
			TileMapConfig::TileLayer layer;
			layer.attributes.fillAtributes(layer_node);

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
						layer.tildIds.push_back(value);
					}

					is_value = false;
					value = 0;
					continue;
				}

				is_value = true;
				value = value * 10 + (*data_string++ - '0');
			}

			config.tileLayers.push_back(layer);
		}
		else if(StringCompare(layer_node.name(), "objectgroup")) 
		{
			StringMap32 attributes;
			attributes.fillAtributes(layer_node);

			SceneTileMapping::ObjectLayer object_layer;
			object_layer.id = attributes.getInt("id");
			object_layer.name = attributes.at("name");

			XMLNode object_node = layer_node.child("object");
			while(object_node) 
			{
				StringMap32 objectAttributes;
				objectAttributes.fillAtributes(object_node);

				VectorF pos = objectAttributes.getVectorF("x", "y");
				VectorF size = objectAttributes.getVectorF("width", "height");
				RectF rect(pos, size);
				rect.Scale(size_ratio);

				object_layer.rects.push_back(rect);
				
				ECS::EntityCoordinator* ecs = GameData::Get().ecs;
				ECS::Entity ent = ecs->CreateNewEntity();

				ECS::Collider collider;
				collider.mRect = rect;
				SetFlag<u32>(collider.mFlags, (u32)ECS::Collider::Static);
				ecs->AddComponent(Collider, ent, collider);

				ECS::Transform transform;
				transform.baseRect = rect;
				ecs->AddComponent(Transform, ent, transform);

				tile_mapping.colliderEntities.push_back(ent);

				tile_mapping.objectLayers.push_back(object_layer);

				object_node = object_node.next();
			}
		}

		layer_node = layer_node.next();
	}

	// tilset set data - texture, 
	const char* tileset_source = config.tilesetAttributes.at("source").c_str();
	TileSet tile_set;
	FillTileSet(tile_set, tileset_source);


	// convert a map index into a tilset index
	for (u32 l = 0; l < config.tileLayers.size(); l++)
	{
		TileMapConfig::TileLayer& layer_config = config.tileLayers[l];
		const u32 tile_count = layer_config.tildIds.size();

		SceneTileMapping::TileLayer map_layer;
		map_layer.tileset = tile_set;
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
				int row = idx / tile_set.tileCount.x;
				int column = idx % tile_set.tileCount.x;

				index = VectorI(column, row);
			}
			
			map_layer.tileMapping.push_back(index);
		}

		tile_mapping.tileLayers.push_back(map_layer);
	}

	delete parser;
}