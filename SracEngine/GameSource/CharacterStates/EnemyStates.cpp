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

	// Idle
	// ---------------------------------------------------------
	void IdleState::Update(float dt)
	{
		ECS::EntityCoordinator* ecs = GameData::Get().ecs;
		ECS::AIController& ai = ecs->GetComponentRef(AIController, entity);
		ECS::CharacterState& state = ecs->GetComponentRef(CharacterState, entity);

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

		if( ECS::Pathing* pathing = ecs->GetComponent(Pathing, entity) )
		{
			if(ecs->IsAlive(pathing->target))
			{
				const ECS::Transform& target_transform = ecs->GetComponentRef(Transform, pathing->target);
				VectorF target_position = target_transform.rect.Center();
				
				const ECS::Transform& transform = ecs->GetComponentRef(Transform, entity);

				float distance_squard = (transform.rect.Center() - target_position).lengthSquared();
				float attack_range = GetAttackRange(entity);

				VectorF looking_at = transform.rect.Center() + (state.facingDirection.toFloat() * attack_range);
				if(ECS::Collider* target_collider = ecs->GetComponent(Collider, pathing->target))
				{
					if(target_collider->contains(looking_at))
					{
						ai.PopState();
						ai.PushState(ActionState::BasicAttack);
					}
				}

				//if( (attack_range * attack_range) > distance_squard)
				//{
				//	ai.PopState();
				//	ai.PushState(ActionState::BasicAttack);
				//}
			}
		}

		if(state.movementDirection.isZero())
		{
			ai.PopState();
		}
	}

	// BasicAttack
	// ---------------------------------------------------------
	void BasicAttackState::Init()
	{	
		ECS::EntityCoordinator* ecs = GameData::Get().ecs;

		const ECS::Transform& transform = ecs->GetComponentRef(Transform, entity);
		const ECS::CharacterState& state = ecs->GetComponentRef(CharacterState, entity);

		if(ECS::Physics* physics = ecs->GetComponent(Physics, entity))
		{
			physics->speed.set(0.0f, 0.0f);
		}

		RectF collider_rect;
		
		const VectorI direction = state.facingDirection;
		if( direction.x == -1 || direction.x == 1 )
		{
			const float range = GetAttackRange(entity);
			const float height = transform.rect.Width() * 1.5f;
			const VectorF size(range, height);
			collider_rect.SetSize(size);

			if( direction.x == 1 )
				collider_rect.SetLeftCenter(transform.rect.BotCenter());
			else
				collider_rect.SetRightCenter(transform.rect.BotCenter());
		}
		else if( direction.y == -1 || direction.y == 1 )
		{
			const float range = GetAttackRange(entity);
			const float width = transform.rect.Width() * 1.5f;
			const VectorF size(width, range);
			collider_rect.SetSize(size);

			if( direction.y == 1 )
				collider_rect.SetTopCenter(transform.rect.Center());
			else
				collider_rect.SetBotCenter(transform.rect.Center());
		}

		attackCollider = CreateAttackCollider(entity, collider_rect, 60, "Enemy Attack Collider");
	}

	void BasicAttackState::Update(float dt)
	{
		ECS::EntityCoordinator* ecs = GameData::Get().ecs;
		ECS::Animation& animation = ecs->GetComponentRef(Animation, entity);
		ECS::AIController& ai = ecs->GetComponentRef(AIController, entity);
			
		if(animation.animator.finished())
		{
			ai.PopState();
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
		ECS::EntityCoordinator* ecs = GameData::Get().ecs;

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

	const float GetAttackRange(ECS::Entity entity)
	{
		ECS::EntityCoordinator* ecs = GameData::Get().ecs;

		const ECS::Transform& transform = ecs->GetComponentRef(Transform, entity);
		const ECS::CharacterState& state = ecs->GetComponentRef(CharacterState, entity);

		const RectF& character_rect = transform.rect;
		const RectF boundary_rect = ECS::AnimationSystem::GetRenderRect(entity);

		const VectorI direction = state.facingDirection;
		if( direction.x == -1 || direction.x == 1 )
		{
			const float right = character_rect.Center().x;
			const float left = boundary_rect.LeftCenter().x;
			return right - left;
		}
		else if( direction.y == -1 || direction.y == 1 )
		{
			const float top = character_rect.Center().y;
			const float bottom = boundary_rect.BotCenter().y;
			return bottom - top;
		}

		return 0;
	}
}


