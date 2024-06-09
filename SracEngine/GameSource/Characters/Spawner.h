#pragma once

namespace ECS
{
	struct  TileMap;
}

namespace PlayerSpawn
{
	ECS::Entity Spawn(ECS::TileMap& map);
}

namespace EnemySpawn
{
	ECS::Entity Spawn(ECS::TileMap& map);
}