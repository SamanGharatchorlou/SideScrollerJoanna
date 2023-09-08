#pragma once

#include "Core/TypeDefs.h"

#define ENTITY_LOGGING 1

namespace ECS
{
	constexpr u32 MaxEntityCount = 5000;

	using Entity = u32;		// simple id for each entity
	using Archetype = u64;	// bitwise encoding for every component type (max 64)

	#define ArchetypeInvalid -1;

	struct Component
	{
		enum Type : u64
		{
			Velocity,
			Transform,
			Sprite,
			PlayerInput,
			CharacterState,
			Physics,
			Animation,
			TileMap,
			Collider,

			Count
		};
	};
}