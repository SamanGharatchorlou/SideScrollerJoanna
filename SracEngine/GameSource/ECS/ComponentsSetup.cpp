#include "pch.h"
#include "ComponentsSetup.h"

#include "ECS/EntityCoordinator.h"
#include "GameSource/ECS/Components.h"
#include "ECS/Components/Collider.h"

#include "ECS/SystemManager.h"
#include "EntSystems/RenderSystem.h"
#include "EntSystems/PlayerInputSystem.h"
#include "EntSystems/MovementSystem.h"
#include "EntSystems/AnimationSystem.h"
#include "EntSystems/TileMapSystem.h"
#include "EntSystems/CollisionSystem.h"

void ECS::RegisterAllComponents()
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;

	ecs->RegisterComponent(Transform, 32);
	ecs->RegisterComponent(Sprite, 32);
	ecs->RegisterComponent(PlayerInput, 1);
	ecs->RegisterComponent(CharacterState, 1);
	ecs->RegisterComponent(Velocity, 32);
	ecs->RegisterComponent(MovementPhysics, 32);
	ecs->RegisterComponent(Animation, 8);
	ecs->RegisterComponent(TileMap, 4);
	ecs->RegisterComponent(Collider, 32);
}

void ECS::RegisterAllSystems()
{
	EntityCoordinator* ecs = GameData::Get().ecs;

	// RenderSystem
	Archetype renderArchetype = ArcheBit(Transform) | ArcheBit(Sprite);
	ecs->RegisterSystem<RenderSystem>(renderArchetype);

	// PlayerInputSystem
	Archetype playerInputArchetype = ArcheBit(PlayerInput) | ArcheBit(CharacterState) | ArcheBit(Velocity);
	ecs->RegisterSystem<PlayerInputSystem>(playerInputArchetype);

	// MovementSystem
	Archetype movementArchetype = ArcheBit(Transform) | ArcheBit(Velocity) | ArcheBit(CharacterState) | ArcheBit(MovementPhysics);
	ecs->RegisterSystem<MovementSystem>(movementArchetype);

	// AnimationSystem
	Archetype animationArchetype = ArcheBit(Sprite) | ArcheBit(Animation) | ArcheBit(CharacterState) | ArcheBit(Transform);
	ecs->RegisterSystem<AnimationSystem>(animationArchetype);

	// Map
	Archetype mapTileArcheType = ArcheBit(TileMap);
	ecs->RegisterSystem<TileMapSystem>(mapTileArcheType);

	// Collisions
	Archetype collisionArcheType = ArcheBit(Collider) | ArcheBit(Transform);
	ecs->RegisterSystem<CollisionSystem>(collisionArcheType);
}