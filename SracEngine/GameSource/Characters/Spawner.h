#pragma once

namespace ECS
{
	struct  TileMap;
}

namespace PlayerSpawn
{
	ECS::Entity Spawn();
	ECS::Entity Spawn(VectorF spawn_pos);
}

namespace EnemySpawn
{
	ECS::Entity Spawn(ECS::TileMap& map);
}