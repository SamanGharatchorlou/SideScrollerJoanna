#pragma once
#include "pch.h"
#include "EnemyStates.h"

#include "SRAC/Game/GameStates/State.h"
#include "ECS/Components/Components.h"
#include "ECS/EntityCoordinator.h"
#include "ECS/Components/AIController.h"
#include "ECS/Components/Physics.h"
#include "Debugging/ImGui/ImGuiMainWindows.h"
#include "ECS/EntSystems/AnimationSystem.h"
#include "ECS/Components/Collider.h"
#include "ECS/Components/ComponentCommon.h"

namespace Enemy
{
	CharacterAction* StatePool::createNewObjects(ActionState type, int count, int& out_size) const
	{
		CharacterAction* state = nullptr;

		switch (type)
		{
		ActionStateCase(Idle)
		ActionStateCase(Run)
		ActionStateCase(BasicAttack)
		ActionStateCase(Death)
		case ActionState::Count:
		case ActionState::None:
		default:
			DebugPrint(Warning, "Invalid effect type %d, could not create %d objects\n", type, count);
			break;
		}

		return state;
	}

	bool CanAttackTarget(ECS::Entity entity, VectorI& out_facing_direction)
	{
		ECS::EntityCoordinator* ecs = GameData::Get().ecs;
		if( ECS::Pathing* pathing = ecs->GetComponent(Pathing, entity) )
		{
			if(ecs->IsAlive(pathing->target))
			{
				const ECS::Transform& target_transform = ecs->GetComponentRef(Transform, pathing->target);
				VectorF target_position = target_transform.rect.Center();
				
				const ECS::Transform& transform = ecs->GetComponentRef(Transform, entity);

				float distance_squard = (transform.rect.Center() - target_position).lengthSquared();
				float attack_range = GetAttackRange(entity);

				// out of attack range
				if(distance_squard > (attack_range * attack_range))
					return false;
				
				if(ECS::Collider* target_collider = ecs->GetComponent(Collider, pathing->target))
				{
					// todo: draw the attack collider here, if its going to hit then do it

					ECS::CharacterState& state = ecs->GetComponentRef(CharacterState, entity);
					const VectorI directions[4] = { VectorI(0,-1), VectorI( 1, 0), VectorI(0, 1), VectorI(-1, 0) };
					for( u32 i = 0; i < 4; i++ )
					{
						VectorF looking_at = transform.rect.Center() + (directions[i].toFloat() * attack_range);
						if(target_collider->contains(looking_at))
						{
							out_facing_direction = directions[i];
							return true;
						}
					}
				}
			}
		}

		return false;
	}

	static bool TryAttackTarget(ECS::Entity entity)
	{
		VectorI direction;
		if(CanAttackTarget(entity, direction))
		{
			ECS::EntityCoordinator* ecs = GameData::Get().ecs;

			ECS::AIController& ai = ecs->GetComponentRef(AIController, entity);
			ai.PushState(ActionState::BasicAttack);
			
			ECS::CharacterState& state = ecs->GetComponentRef(CharacterState, entity);
			state.facingDirection = direction;

			return true;
		}

		return false;
	}

	// Idle
	// ---------------------------------------------------------
	void IdleState::Update(float dt)
	{
		ECS::EntityCoordinator* ecs = GameData::Get().ecs;
		ECS::AIController& ai = ecs->GetComponentRef(AIController, entity);
		ECS::CharacterState& state = ecs->GetComponentRef(CharacterState, entity);
		
		TryAttackTarget(entity);

		if(!state.movementDirection.isZero())
		{
			ai.PushState(ActionState::Run);
		}
	}

	// Run
	// ---------------------------------------------------------
	void RunState::Update(float dt)
	{
		ECS::EntityCoordinator* ecs = GameData::Get().ecs;
		ECS::AIController& ai = ecs->GetComponentRef(AIController, entity);
		const ECS::CharacterState& state = ecs->GetComponentRef(CharacterState, entity);

		if (ECS::Physics* physics = ecs->GetComponent(Physics, entity))
		{
			// apply walk speed
			physics->maxSpeed = VectorF(3.0f, 3.0f);
			physics->ApplyMovement(state.movementDirection.toFloat(), dt);
			physics->ApplyDrag(state.movementDirection.toFloat(), 0.9f);
		}

		TryAttackTarget(entity);

		if(state.movementDirection.isZero())
		{
			ai.PopState();
		}
	}

	// BasicAttack
	// ---------------------------------------------------------
	void BasicAttackState::Init()
	{	
		// init values
		attackCollider = ECS::EntityInvalid;

		ECS::EntityCoordinator* ecs = GameData::Get().ecs;

		const ECS::Transform& transform = ecs->GetComponentRef(Transform, entity);
		const ECS::CharacterState& state = ecs->GetComponentRef(CharacterState, entity);
		
		const RectF boundary_rect = ECS::GetRenderRect(entity);
		const RectF character_rect = ECS::GetObjectRect(entity);

		if(ECS::Physics* physics = ecs->GetComponent(Physics, entity))
		{
			physics->speed.set(0.0f, 0.0f);
		}

		RectF collider_rect;
		
		const VectorI direction = state.facingDirection;
		if( direction.x == -1 || direction.x == 1 )
		{
			const float range = GetAttackRange(entity);
			const float height = boundary_rect.Height() * 0.25f;
			const VectorF size(range, height);
			collider_rect.SetSize(size);

			if( direction.x == 1 )
				collider_rect.SetLeftCenter(character_rect.BotCenter());
			else
				collider_rect.SetRightCenter(character_rect.BotCenter());
		}
		else if( direction.y == -1 || direction.y == 1 )
		{
			const float range = GetAttackRange(entity);
			const float width = boundary_rect.Width() * 0.6f;
			const VectorF size(width, range);
			collider_rect.SetSize(size);

			if( direction.y == 1 )
			{
				const RectF character_collider_rect = ECS::GetColliderRect(entity);
				collider_rect.SetTopCenter(character_collider_rect.BotCenter());
			}
			else
				collider_rect.SetBotCenter(character_rect.Center());
		}

		attackCollider = CreateAttackCollider(entity, collider_rect, 60, "Enemy Attack Collider");
	}

	void BasicAttackState::Update(float dt)
	{
		ECS::EntityCoordinator* ecs = GameData::Get().ecs;
		ECS::Animation& animation = ecs->GetComponentRef(Animation, entity);
		ECS::AIController& ai = ecs->GetComponentRef(AIController, entity);
		
		const Animation* anim = animation.animator.activeAnimation();
		
		if(HandleAttackAnimation(entity, attackCollider))
		{
			ai.PopState();
			ai.PushState(ActionState::Idle, 1.0f);
		}
	}

	void BasicAttackState::Exit()
	{
		ECS::EntityCoordinator* ecs = GameData::Get().ecs;
		GameData::Get().ecs->entities.KillEntity(attackCollider);
	}

	// DeathState
	// ---------------------------------------------------------
	void DeathState::Init()
	{
		can_kill = false;

		ECS::EntityCoordinator* ecs = GameData::Get().ecs;

		if(ECS::Collider* collider = ecs->GetComponent(Collider, entity))
		{
			SetFlag(collider->mFlags, (u32)ECS::Collider::IgnoreAll);
		}

		if(ECS::Physics* physics = ecs->GetComponent(Physics, entity))
		{
			physics->speed.set(0.0f, 0.0f);
		}
	}

	void DeathState::Update(float dt)
	{	
		ECS::EntityCoordinator* ecs = GameData::Get().ecs;

		const ECS::Animation& animation = ecs->GetComponentRef(Animation, entity);
		if(animation.animator.finished())
		{
			can_kill = true;
		}
	}

	// helpers
	const float GetAttackRange(ECS::Entity entity)
	{
		ECS::EntityCoordinator* ecs = GameData::Get().ecs;
		
		const ECS::CharacterState& state = ecs->GetComponentRef(CharacterState, entity);
		const RectF boundary_rect = ECS::GetRenderRect(entity);
		const VectorI direction = state.facingDirection;

		if( direction.x == -1 || direction.x == 1 )
		{
			return boundary_rect.Width() * 0.5f;
		}
		else if( direction.y == -1 || direction.y == 1 )
		{
			return boundary_rect.Height() * 0.3f;
		}

		return 0;
	}
}


