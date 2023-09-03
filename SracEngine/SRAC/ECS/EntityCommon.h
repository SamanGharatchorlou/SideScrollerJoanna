#pragma once

#include "Core/TypeDefs.h"

namespace ECS
{
	constexpr u32 MaxEntityCount = 5000;

	using Entity = u32;		// simple id for each entity
	using Archetype = u64;	// bitwise encoding for every component type (max 64)

	struct Component
	{
		enum Type : u64
		{
			Position,
			Velocity,
			Transform,
			Sprite,
			PlayerInput,
			CharacterState,
			Physics,
			Animation,

			Count
		};
	};
}