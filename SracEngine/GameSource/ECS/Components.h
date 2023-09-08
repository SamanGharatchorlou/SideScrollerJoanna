#pragma once

#include "Core/TypeDefs.h"
#include "Core/Vector2D.h"
#include "Core/Rect.h"

#include "ECS/EntityCommon.h"
#include "Physics/Physics.h"
#include "Animations/Animator.h"
#include "Scene/SceneParsing/SceneBuilder.h"

class Texture;
enum class ActionState;

namespace ECS
{
	struct Velocity
	{
		VectorF speed;
		VectorF maxSpeed;

		VectorF acceleration;
		VectorF maxAcceleration;

		static ECS::Component::Type type() { return ECS::Component::Velocity; }
	};

	struct Transform
	{
		RectF baseRect;
		VectorF targetCenterPosition;

		VectorF sizeFactor;
		SDL_RendererFlip flip;
		static ECS::Component::Type type() { return ECS::Component::Transform; }
	};

	struct Sprite
	{
		RectF subRect;
		Texture* texture;
		u32 renderLayer;
		static ECS::Component::Type type() { return ECS::Component::Sprite; }
	};

	// todo: an unused component really, could i just set the type somewhere and not event bother
	// with adding to its component array
	struct PlayerInput
	{
		static ECS::Component::Type type() { return ECS::Component::PlayerInput; }
	};

	struct CharacterState
	{
		ActionState previousAction;
		ActionState action;
		VectorF movementDirection;
		VectorF facingDirection;

		bool inTransition = false;
		bool canChange = true;

		static ECS::Component::Type type() { return ECS::Component::CharacterState; }
	};

	struct MovementPhysics
	{
		Physics physics;
		bool applyGravity;

		static ECS::Component::Type type() { return ECS::Component::Physics; }
	};

	struct Animation
	{
		Animator animator;
		std::vector<Texture*> spriteSheets;

		static ECS::Component::Type type() { return ECS::Component::Animation; }
	};

	struct TileMap
	{
		SceneTileMapping tileMap;

		static ECS::Component::Type type() { return ECS::Component::TileMap; }
	};

	// ----------------------------------------------------------------------
	// helpers
	static u64 archetypeBit(ECS::Component::Type type)
	{
		return (u64)1 << type;
	}

#define ArcheBit(compType) archetypeBit(ECS::compType::type())
}