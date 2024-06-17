#pragma once

#include "Core/TypeDefs.h"
#include "Core/Vector2D.h"
#include "Core/Rect.h"
#include "Core/stack.h"

#include "Core/Physics/Fisics.h"
#include "Animations/Animator.h"

class Texture;
enum class ActionState;

namespace ECS
{
	struct Transform
	{
		COMPONENT_TYPE(Transform)

		// rect of the actual object
		RectF rect;
		VectorF targetCenterPosition;

		SDL_RendererFlip flip;
	};

	struct Sprite
	{
		COMPONENT_TYPE(Sprite)

		// used to derive the render rect from the transform rect
		RectF relativeRenderRect;
		RectF subRect;
		Texture* texture = nullptr;
		u32 renderLayer = 0;
	};

	struct CharacterState
	{
		COMPONENT_TYPE(CharacterState)

		enum Direction { Left, Right, Up, Down, Count };

		VectorI movementDirection;
		VectorI facingDirection;

		ActionState action = ActionState::None;

		bool isRunning = false;
		bool canChange = true;
	};

	struct Animation
	{
		COMPONENT_TYPE(Animation)

		Animator animator;
	};

	struct Pathing
	{
		COMPONENT_TYPE(Pathing)

		Entity target = ECS::EntityInvalid;

		VectorI currentStart;
		VectorI currentTarget;

		std::vector<VectorI> path;
	};

	struct Damage
	{
		COMPONENT_TYPE(Damage)

		float value = -1;
	};

	struct Health
	{
		COMPONENT_TYPE(Health)

		float maxHealth;
		float currentHealth;

		std::vector<Entity> ignoredDamaged;

		void ApplyDamage(const Damage& damage);
	};

	// ----------------------------------------------------------------------
	// helpers
	static u64 archetypeBit(ECS::Component::Type type)
	{
		return (u64)1 << type;
	}

#define ArcheBit(compType) archetypeBit(ECS::compType::type())
}