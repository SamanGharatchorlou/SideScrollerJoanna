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

			// ignore static colliders, they dont move
			if(HasFlag(collider.mFlags, Collider::Flags::Static))
				continue;
			
			Transform& transform = ecs->GetComponentRef(Transform, entity);

			// this doesnt seem to make a difference, seems like it should
			collider.SetPosition(transform.rect, transform.targetCenterPosition);
			collider.RollForwardPosition();
		}

		for (Entity entity : entities)
		{
			// debug break point
			if(DebugMenu::GetSelectedEntity() == entity)
				int a = 4;

			Collider& this_collider = ecs->GetComponentRef(Collider, entity);

			// ignore static colliders, we check against them, but not from them
			if(HasFlag(this_collider.mFlags, Collider::Flags::Static))
				continue;

			const u32 index = colliders.GetComponentIndex(entity);
			Transform& transform = ecs->GetComponentRef(Transform, entity);
			
			const Damage* this_damage = ecs->GetComponent(Damage, entity);

			for (u32 i = 0; i < count; i++) 
			{
				// ignore ourself
				if(i == index)
					continue;
				
				Collider& that_collider = collider_list[i];
				ECS::Entity that_entity = that_collider.entity;
				
				// ignore checking against damage colliders
				Damage* that_damage = ecs->GetComponent(Damage, that_entity);
				if(that_damage)
					continue;	

				// if this applies damage we only need to do the check if they have health
				Health* that_health = ecs->GetComponent(Health, that_entity);
				if(this_damage && !that_health)
					continue;

				if(this_collider.intersects(that_collider)) 
				{
					// Damage
					if( this_damage )
					{
						that_health->ApplyDamage(*this_damage);
					}
					// Physical
					else
					{
						VectorF velocity = transform.targetCenterPosition - transform.rect.Center();
						if (velocity.isZero())
							continue;

						VectorF allowed_velocity;

						this_collider.RollBackPosition();
						that_collider.RollBackPosition();

						bool still_interacts = this_collider.intersects(that_collider);
						if (!still_interacts)
						{
							// doesnt matter on the direction, always seems to fail NOT with static colliders though
							// only with moving colliders to be fair, maybe theres something in that
							const RectF horizontal_rect = transform.rect.Translate(VectorF(velocity.x, 0.0f));
							const bool can_move_horizontally = !that_collider.intersects(horizontal_rect);
							if (can_move_horizontally)
								allowed_velocity.x = velocity.x;

							const RectF vertical_rect = transform.rect.Translate(VectorF(0.0f, velocity.y));
							const bool can_move_vertically = !that_collider.intersects(vertical_rect);
							if (can_move_vertically)
								allowed_velocity.y = velocity.y;

							// now that we're allowed to move this much we cant also allow the other collider to do the same priorities?
							// otherwise we could both move towards each other assuming we can, but really only 1 can right?
							// so remove our roll back, we're not going that way for sure
							this_collider.mBack = transform.rect.Center() + allowed_velocity;
						}
						else
						{
							allowed_velocity = VectorF::zero();
						}
						that_collider.RollForwardPosition();

						transform.targetCenterPosition = transform.rect.Center() + allowed_velocity;
						this_collider.mRect.SetCenter(transform.targetCenterPosition);
					}
				}
            }
		}
	}
}