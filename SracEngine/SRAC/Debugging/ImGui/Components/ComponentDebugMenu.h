#pragma once

#include "ECS/Components.h"
#include "ECS/Components/Collider.h"

namespace DebugMenu 
{
	bool UsingPlaylist();

	ECS::Component::Type DoTransformDebugMenu(ECS::Entity& entity);
	//ECS::Component::Type DoSpriteDebugMenu(ECS::Entity& entity);
	//ECS::Component::Type DoPlayerInputDebugMenu(ECS::Entity& entity);
	ECS::Component::Type  DoCharacterStateDebugMenu(ECS::Entity& entity);
	ECS::Component::Type DoVelocityDebugMenu(ECS::Entity& entity);
	//ECS::Component::Type DoMovementPhysicsDebugMenu(ECS::Entity& entity);
	ECS::Component::Type DoAnimationDebugMenu(ECS::Entity& entity);
	ECS::Component::Type DoTileMapDebugMenu(ECS::Entity& entity);
	ECS::Component::Type DoColliderDebugMenu(ECS::Entity& entity);
}