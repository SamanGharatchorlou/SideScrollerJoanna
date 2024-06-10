#pragma once

#include "Scene/SceneParsing/SceneBuilder.h"

namespace ECS
{
	struct TileMap
	{
		Map::SceneTileMapping tileMap;

		static ECS::Component::Type type() { return ECS::Component::TileMap; }

		static VectorF GetMapSizeRatio();
		static const TileMap* GetActive();
	};
}