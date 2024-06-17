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
		ECS::CharacterState& state = ecs->GetComponentRef(CharacterState, entity);

		if (ECS::Physics* physics = ecs->GetComponent(Physics, ai.entity))
		{
			// apply walk speed
			physics->maxSpeed = VectorF(3.0f, 3.0f);
			physics->ApplyMovement(state.movementDirection.toFloat(), dt);
			physics->ApplyDrag(state.movementDirection.toFloat(), 0.9f);
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

		const RectF& character_rect = transform.rect;
		const RectF boundary_rect = ECS::AnimationSystem::GetRenderRect(entity);

		RectF collider_rect;
		
		const VectorI direction = state.movementDirection;
		if(!direction.isZero())
		{
			if( direction.x == -1 || direction.x == 1 )
			{
				const float right = character_rect.Center().x;
				const float left = boundary_rect.LeftCenter().x;
				const float width = right - left;
				const float height = transform.rect.Width() * 1.5f;
				const VectorF size(width, height);
				collider_rect.SetSize(size);

				if( direction.x == 1 )
					collider_rect.SetLeftCenter(character_rect.BotCenter());
				else
					collider_rect.SetRightCenter(character_rect.BotCenter());
			}
			else if( direction.y == -1 || direction.y == 1 )
			{
				const float top = character_rect.TopCenter().y;
				const float bottom = boundary_rect.TopCenter().y;
				const float height = top - bottom;
				const float width = transform.rect.Width() * 1.5f;
				const VectorF size(width, height);
				collider_rect.SetSize(size);

				if( direction.y == 1 )
					collider_rect.SetTopCenter(character_rect.BotCenter());
				else
					collider_rect.SetBotCenter(character_rect.TopCenter());
			}
		}

		attackCollider = ecs->CreateEntity("Enemy Attack Collider");

		// Transform
		ECS::Transform& attack_transform = ecs->AddComponent(Transform, attackCollider);
		attack_transform.rect = collider_rect;
		attack_transform.targetCenterPosition = collider_rect.Center(); 

		// Collider
		ECS::Collider& collider = ecs->AddComponent(Collider, attackCollider);
		collider.mRect = collider_rect;

		// Damage
		ECS::Damage& damage = ecs->AddComponent(Damage, attackCollider);
		damage.value = 10;
		
		// dont damage our self
		ECS::Health& health = ecs->GetComponentRef(Health, entity);
		health.ignoredDamaged.push_back(attackCollider);
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
}