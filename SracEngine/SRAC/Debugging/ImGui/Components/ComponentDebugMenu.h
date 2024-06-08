#pragma once

#include "ECS/Components.h"
#include "ECS/Components/Collider.h"

namespace DebugMenu 
{
	bool UsingPlaylist();
	bool DisplayRenderRect(ECS::Entity& entity);

	ECS::Component::Type DoTransformDebugMenu(ECS::Entity& entity);
	ECS::Component::Type DoSpriteDebugMenu(ECS::Entity& entity);
	ECS::Component::Type DoCharacterStateDebugMenu(ECS::Entity& entity);
	ECS::Component::Type DoPhysicsDebugMenu(ECS::Entity& entity);
	ECS::Component::Type DoAnimationDebugMenu(ECS::Entity& entity);
	ECS::Component::Type DoTileMapDebugMenu(ECS::Entity& entity);
	ECS::Component::Type DoColliderDebugMenu(ECS::Entity& entity);
	ECS::Component::Type DoPlayerControllerDebugMenu(ECS::Entity& entity);
}