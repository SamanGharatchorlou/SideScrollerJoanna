#include "pch.h"
#include "Spawner.h"

#include "Characters/Player/PlayerCharacter.h"
#include "Characters/Enemies/Enemies.h"
#include "ECS/Components/Components.h"
#include "ECS/EntityCoordinator.h"

ECS::Entity PlayerSpawn::Spawn(ECS::TileMap& map)
{
	ECS::Entity entity = Player::Create();

	VectorF spawn_pos = map.tileMap.playerSpawnArea.Center();

	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	ECS::Transform& transform = ecs->GetComponentRef(Transform, entity);
	transform.targetCenterPosition = spawn_pos;

	return entity;
}

ECS::Entity EnemySpawn::Spawn(ECS::TileMap& map)
{
	ECS::Entity entity = Enemy::Create();

	VectorF spawn_pos = map.tileMap.enemySpawnArea.Center();

	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	ECS::Transform& transform = ecs->GetComponentRef(Transform, entity);
	transform.targetCenterPosition = spawn_pos;

	return entity;
}