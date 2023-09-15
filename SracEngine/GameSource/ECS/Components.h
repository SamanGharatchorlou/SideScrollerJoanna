#pragma once

#include "Core/TypeDefs.h"
#include "Core/Vector2D.h"
#include "Core/Rect.h"

#include "ECS/EntityCommon.h"
#include "Physics/Physics.h"
#include "Animations/Animator.h"
#include "Scene/SceneParsing/SceneBuilder.h"

#include "SRAC/Game/GameStates/StateMachine.h"
#include "SRAC/Game/GameStates/State.h"

#include "Core/Stack.h"
#include "PlayerStates/PlayerStates.h"

class Texture;
enum class ActionState;

namespace ECS
{
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

	struct Velocity
	{
		VectorF speed;
		VectorF maxSpeed;	// config

		//VectorF currAcceleration;
		VectorF acceleration;
		VectorF maxAcceleration; // config
	};

	struct PlayerController
	{
		Velocity velocity;
		ActionStack<PlayerState> actions;

		VectorF movementDirection;
		VectorF facingDirection;

		// true on the frame the jump started
		//bool jumpped = false;
		bool onFloor = false;

		ActionState Action() const { return actions.Top().action; }
		bool IsJumping() const { return !onFloor && Action() != ActionState::Fall; }

		static ECS::Component::Type type() { return ECS::Component::PlayerController; }
	};

	struct CharacterState
	{
		bool onFloor = false;
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

		ActionState action;
		bool inTransition = false;
		bool canChange = true;

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