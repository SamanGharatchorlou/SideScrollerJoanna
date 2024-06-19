#include "pch.h"
#include "Spawner.h"

#include "Characters/Player/PlayerCharacter.h"
#include "Characters/Enemies/Enemies.h"
#include "ECS/Components/Components.h"
#include "ECS/Components/TileMap.h"
#include "ECS/EntityCoordinator.h"


#include "Game/SystemStateManager.h"
#include "GameStates/GameState.h"

ECS::Entity PlayerSpawn::Spawn()
{
	if( const GameState* gs = GameData::Get().systemStateManager->GetActiveState<GameState>() )
	{
		ECS::EntityCoordinator* ecs = GameData::Get().ecs;
		const ECS::TileMap& tile_map = ecs->GetComponentRef(TileMap, gs->activeMap);

		const VectorF spawn_pos = tile_map.tileMap.playerSpawnArea.Center();
		return PlayerSpawn::Spawn(spawn_pos);
	}
}

ECS::Entity PlayerSpawn::Spawn(VectorF spawn_pos)
{
	ECS::Entity entity = Player::Create();

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