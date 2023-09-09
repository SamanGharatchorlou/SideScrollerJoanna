#pragma once

#include "ECS/Components.h"
#include "ECS/Components/Collider.h"

namespace DebugMenu 
{
	bool UsingPlaylist();

	ECS::Component::Type DoTransformDebugMenu(ECS::Entity& entity);
	//void DoSpriteDebugMenu(ECS::Entity& entity);
	//void DoPlayerInputDebugMenu(ECS::Entity& entity);
	//void DoCharacterStateDebugMenu(ECS::Entity& entity);
	//void DoVelocityDebugMenu(ECS::Entity& entity);
	//void DoMovementPhysicsDebugMenu(ECS::Entity& entity);
	ECS::Component::Type DoAnimationDebugMenu(ECS::Entity& entity);
	ECS::Component::Type DoTileMapDebugMenu(ECS::Entity& entity);
	ECS::Component::Type DoColliderDebugMenu(ECS::Entity& entity);
}