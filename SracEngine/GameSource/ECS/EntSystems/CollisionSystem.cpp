#include "pch.h"
#include "CollisionSystem.h"

#include "ECS/Components/Collider.h"
#include "ECS/Components/Components.h"
#include "ECS/EntityCoordinator.h"
#include "Graphics/RenderManager.h"
#include "Debugging/ImGui/ImGuiMainWindows.h"

#include "ECS/ComponentArray.h"

namespace ECS
{
	static void RollForwardCollider(EntityCoordinator* ecs, ECS::Entity entity)
	{
		Collider& collider = ecs->GetComponentRef(Collider, entity);

		// ignore static colliders, they dont move
		u32 flags = Collider::Flags::Static;
		if(HasFlag(collider.mFlags, flags))
			return;

		collider.RollForwardPosition();
	}

	static bool CanCollide(EntityCoordinator* ecs, const Collider& A_collider, bool A_isDamage, bool A_isPhysical, const Collider& B_collider)
	{
		ECS::Entity B_entity = B_collider.entity;
		if(HasFlag(B_collider.mFlags, Collider::Flags::IgnoreAll))
			return false;

		const bool B_is_damage = ecs->HasComponent(Damage, B_collider.entity);
		const bool B_is_physical =  !B_is_damage;

		// ignore checking against damage colliders
		if(B_is_damage)
		{
			return false;
		}
		else if (B_is_physical)
		{
			u32 ignore_physical = Collider::Flags::IgnoreCollisions;
			if(HasFlag(A_collider.mFlags, ignore_physical))
				return false;
		}

		// if this applies damage we only need to do the check if they have health
		if(A_isDamage && !ecs->HasComponent(Health, B_entity))
			return false;

		// if physical and ignoring collisions skip
		if( A_isPhysical )
		{
			u32 ignore_physical = Collider::Flags::IgnoreCollisions;
			if(HasFlag(A_collider.mFlags, ignore_physical))
				return false;

			// if the collider we're checking against is physical but is ignoring collisions then skip
			if(B_is_physical)
			{
				if(HasFlag(B_collider.mFlags, ignore_physical))
					return false;
			}
		}

		return true;
	}

	bool CollisionSystem::DoesColliderInteract(ECS::Entity entity)
	{
		// debug break point
		if(DebugMenu::GetSelectedEntity() == entity)
			int a = 4;
		
		EntityCoordinator* ecs = GameData::Get().ecs;
		const ComponentArray<Collider>& colliders =  ecs->GetComponents<Collider>(Component::Type::Collider);
		const std::vector<Collider>& collider_list = colliders.components;

		const Collider& A_collider = ecs->GetComponentRef(Collider, entity);

		// ignore static colliders, we check against them, but not from them (or if we're ignoring all)
		u32 flags = Collider::Flags::Static | Collider::Flags::IgnoreAll;
		if(HasFlag(A_collider.mFlags, flags))
			return false;

		const u32 index = colliders.GetComponentIndex(entity);
		const bool A_is_damage = ecs->HasComponent(Damage, entity);
		const bool A_is_physical =  !A_is_damage;

		for (u32 i = 0; i < collider_list.size(); i++) 
		{
			// ignore ourself
			if(i == index)
				continue;
				
			const Collider& B_collider = collider_list[i];

			if(!CanCollide(ecs, A_collider, A_is_damage, A_is_physical, B_collider))
				continue;

			if( A_collider.intersects(B_collider) )
				return true;
        }

		return false;
	}

	void CollisionSystem::Update(float dt)
	{
		EntityCoordinator* ecs = GameData::Get().ecs;

		ComponentArray<Collider>& colliders =  ecs->GetComponents<Collider>(Component::Type::Collider);
		std::vector<Collider>& collider_list = colliders.components;
		const u32 count = collider_list.size();

		// first we need to update the collider position with where the entity wants to be
		for (Entity entity : entities)
		{
			Collider& collider = ecs->GetComponentRef(Collider, entity);
			collider.collisions.clear();

			// ignore static colliders, they dont move
			u32 flags = Collider::Flags::Static;
			if(HasFlag(collider.mFlags, flags))
				return;

			collider.RollForwardPosition();
		}

		for (Entity entity : entities)
		{
			// debug break point
			if(DebugMenu::GetSelectedEntity() == entity)
				int a = 4;

			Collider& A_collider = ecs->GetComponentRef(Collider, entity);
			A_collider.allowedMovement = A_collider.mForward - A_collider.mBack;

			// ignore static colliders, we check against them, but not from them (or if we're ignoring all)
			u32 flags = Collider::Flags::Static | Collider::Flags::IgnoreAll;
			if(HasFlag(A_collider.mFlags, flags))
				continue;

			const u32 index = colliders.GetComponentIndex(entity);
			const Damage* A_damage = ecs->GetComponent(Damage, entity);
			const bool A_is_damage = A_damage != nullptr;
			const bool A_is_physical =  !A_is_damage;

			// no point in checking anything if we're not moving
			if( A_is_physical && A_collider.allowedMovement.isZero() )
				continue;

			for (u32 i = 0; i < count; i++) 
			{
				// ignore ourself
				if(i == index)
					continue;
				
				Collider& B_collider = collider_list[i];

				if(!CanCollide(ecs, A_collider, A_is_damage, A_is_physical, B_collider))
					continue;

				if(A_collider.intersects(B_collider)) 
				{
					ECS::Entity B_entity = B_collider.entity;
					push_back_unique(A_collider.collisions, B_entity);
					push_back_unique(B_collider.collisions, entity);

					// ghost colliders just check for collisions and have no effect so dont compute anything below
					u32 flags = Collider::Flags::GhostCollider;
					if(HasFlag(A_collider.mFlags, flags))
						continue;

					// Damage
					if( A_damage )
					{
						Health* B_health = ecs->GetComponent(Health, B_entity);
						B_health->ApplyDamage(*A_damage);
					}
					// Physical
					else
					{
						VectorF velocity = A_collider.mForward - A_collider.mBack;
						if (velocity.isZero())
							continue;

						VectorF allowed_velocity;

						// roll back both colliders, then roll this one forward axis by axis to check which way it can move
						A_collider.RollBackPosition();
						B_collider.RollBackPosition();

						bool still_interacts = A_collider.intersects(B_collider);
						if (!still_interacts)
						{
							// rolled back rect
							RectF rect = A_collider.GetRect();

							// doesnt matter on the direction, always seems to fail NOT with static colliders though
							// only with moving colliders to be fair, maybe theres something in that
							const RectF horizontal_rect = rect.MoveCopy(VectorF(velocity.x, 0.0f));
							const bool can_move_horizontally = !B_collider.intersects(horizontal_rect);
							if (can_move_horizontally)
								allowed_velocity.x = velocity.x; 

							const RectF vertical_rect = rect.MoveCopy(VectorF(0.0f, velocity.y));
							const bool can_move_vertically = !B_collider.intersects(vertical_rect);
							if (can_move_vertically)
								allowed_velocity.y = velocity.y;

							// now that we're allowed to move this much we cant also allow the other collider to do the same priorities?
							// otherwise we could both move towards each other assuming we can, but really only 1 can right?
							// so remove our roll back, we're not going that way for sure
							A_collider.mBack = rect.Center() + allowed_velocity;
						}
						else
						{
							allowed_velocity = VectorF::zero();
						}

						B_collider.RollForwardPosition();

						if(allowed_velocity.x == 0)
							A_collider.allowedMovement.x = 0.0f;

						if(allowed_velocity.y == 0)
							A_collider.allowedMovement.y = 0.0f;
					}
				}
            }
		}
	}
}