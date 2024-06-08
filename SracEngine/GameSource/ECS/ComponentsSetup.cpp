#include "pch.h"
#include "ComponentsSetup.h"

#include "ECS/EntityCoordinator.h"
#include "GameSource/ECS/Components.h"
#include "ECS/Components/Collider.h"

#include "ECS/SystemManager.h"
#include "EntSystems/RenderSystem.h"
#include "EntSystems/PlayerControllerSystem.h"
#include "EntSystems/PhysicsSystem.h"
#include "EntSystems/AnimationSystem.h"
#include "EntSystems/TileMapSystem.h"
#include "EntSystems/CollisionSystem.h"
#include "ECS/Components/PlayerController.h"
#include "ECS/Components/Physics.h"

void ECS::RegisterAllComponents()
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;

	ecs->RegisterComponent(Transform, 32);
	ecs->RegisterComponent(Sprite, 32);
	ecs->RegisterComponent(CharacterState, 1);
	ecs->RegisterComponent(PlayerController, 32);
	ecs->RegisterComponent(Physics, 32);
	ecs->RegisterComponent(Animation, 8);
	ecs->RegisterComponent(TileMap, 4);
	ecs->RegisterComponent(Collider, 32);
}

void ECS::RegisterAllSystems()
{
	EntityCoordinator* ecs = GameData::Get().ecs;

	// RenderSystem
	Signature renderSignature = /*ArcheBit(Transform) |*/ ArcheBit(Sprite);
	ecs->RegisterSystem<RenderSystem>(renderSignature);

	// PlayerInputSystem
	Signature playerInputSignature = ArcheBit(PlayerController) | ArcheBit(CharacterState) | ArcheBit(Transform) | ArcheBit(Physics);
	ecs->RegisterSystem<PlayerControllerSystem>(playerInputSignature);

	// PhysicsSystem
	Signature physicsSignature = ArcheBit(Transform) | ArcheBit(CharacterState) | ArcheBit(Physics);
	ecs->RegisterSystem<PhysicsSystem>(physicsSignature);

	// AnimationSystem
	Signature animationSignature = ArcheBit(Sprite) | ArcheBit(Animation) | ArcheBit(CharacterState) | ArcheBit(Transform);
	ecs->RegisterSystem<AnimationSystem>(animationSignature);

	// Map
	Signature mapTileSignature = ArcheBit(TileMap);
	ecs->RegisterSystem<TileMapSystem>(mapTileSignature);

	// Collisions
	Signature collisionSignature = ArcheBit(Collider) | ArcheBit(Transform);
	ecs->RegisterSystem<CollisionSystem>(collisionSignature);
}