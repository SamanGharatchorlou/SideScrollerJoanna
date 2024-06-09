#pragma once

#include "Core/TypeDefs.h"
#include "Core/Vector2D.h"
#include "Core/Rect.h"
#include "Core/stack.h"

#include "ECS/EntityCommon.h"
#include "Core/Physics/Fisics.h"
#include "Animations/Animator.h"
#include "Scene/SceneParsing/SceneBuilder.h"

#include "CharacterStates/EnemyStates.h" // can try to forward decalare state


class Texture;
enum class ActionState;

namespace ECS
{
	struct Transform
	{
		// rect of the actual object
		RectF rect;
		VectorF targetCenterPosition;

		SDL_RendererFlip flip;
		static ECS::Component::Type type() { return ECS::Component::Transform; }
	};

	struct Sprite
	{
		// used to derive the render rect from the transform rect
		RectF relativeRenderRect;
		RectF subRect;
		Texture* texture;
		u32 renderLayer;
		static ECS::Component::Type type() { return ECS::Component::Sprite; }
	};

	struct CharacterState
	{
		enum Direction { Left, Right, Up, Down, Count };

		VectorI movementDirection;
		VectorI facingDirection;

		ActionState action;

		bool isRunning;
		bool canChange = true;

		bool restrictMovement[Direction::Count] { false, false, false, false };

		bool OnFloor() const { return restrictMovement[Direction::Down]; }

		static ECS::Component::Type type() { return ECS::Component::CharacterState; }
	};

	struct Animation
	{
		Animator animator;

		static ECS::Component::Type type() { return ECS::Component::Animation; }
	};

	struct TileMap
	{
		Map::SceneTileMapping tileMap;

		static ECS::Component::Type type() { return ECS::Component::TileMap; }
	};

	struct AIController
	{
		Enemy::StatePool statePool;
		ActionStack<CharacterAction> actions;
		Entity entity;

		static ECS::Component::Type type() { return ECS::Component::AIController; }
	};

	struct Pathing
	{
		
		static ECS::Component::Type type() { return ECS::Component::Pathing; }
	};

	// ----------------------------------------------------------------------
	// helpers
	static u64 archetypeBit(ECS::Component::Type type)
	{
		return (u64)1 << type;
	}

#define ArcheBit(compType) archetypeBit(ECS::compType::type())
}