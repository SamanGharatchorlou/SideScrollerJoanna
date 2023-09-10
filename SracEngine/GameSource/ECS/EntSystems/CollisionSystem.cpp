#include "pch.h"
#include "CollisionSystem.h"

#include "ECS/Components/Collider.h"
#include "GameSource/ECS/Components.h"
#include "ECS/EntityCoordinator.h"
#include "Graphics/RenderManager.h"

#include "ECS/ComponentArray.h"

namespace ECS
{
	void CollisionSystem::Update(float dt)
	{
		EntityCoordinator* ecs = GameData::Get().ecs;

		ComponentArray<Collider>& colliders =  ecs->GetComponents<Collider>(Component::Type::Collider);
		std::vector<Collider>& collider_list = colliders.components;
		u32 count = collider_list.size();

		// first we need to update the collider position with where the entity wants to be
		for (Entity entity : entities)
		{
			Collider& collider = ecs->GetComponent(Collider, entity);
			Transform& transform = ecs->GetComponent(Transform, entity);

			// ignore static colliders, they dont move
			if(HasFlag(collider.mFlags, Collider::Flags::Static))
				continue;

			collider.mRect.SetCenter(transform.targetCenterPosition);
		}

		for (Entity entity : entities)
		{
			Collider& this_collider = ecs->GetComponent(Collider, entity);
			Transform& transform = ecs->GetComponent(Transform, entity);
			//Velocity& velocity = ecs->GetComponent(Velocity, entity);

			// ignore static colliders, we check against them, but not from them
			if(HasFlag(this_collider.mFlags, Collider::Flags::Static))
				continue;

			u32 index = colliders.GetComponentIndex(entity);

			for (u32 i = 0; i < count; i++) 
			{
				// ignore ourself
				if(i == index)
					continue;

				 Collider& that_collider = collider_list[i];
				 if(this_collider.intersects(that_collider)) 
				 {
					 VectorF velocity = transform.targetCenterPosition - transform.baseRect.Center();

					 VectorF allowed_velocity;

					 const RectF horizontal_rect = transform.baseRect.Translate(VectorF(velocity.x, 0.0f));
					 const bool can_move_horizontally = !that_collider.intersects(horizontal_rect);
					 if (can_move_horizontally)
						 allowed_velocity.x = velocity.x;

					 const RectF vertical_rect = transform.baseRect.Translate(VectorF(0.0f, velocity.y));
					 const bool can_move_vertically = !that_collider.intersects(vertical_rect);
					 if (can_move_vertically)
						 allowed_velocity.y = velocity.y;

					 // todo: the colliders are getting stuck verically
					 // i.e. bottom of the player collider is lower than the top of the floor collider???
					 if (ecs->HasComponent(entity, ECS::Component::CharacterState))
					 {
						 CharacterState& state = ecs->GetComponent(CharacterState, entity);
						 state.onFloor = !can_move_vertically;
					 }

					 transform.targetCenterPosition = transform.baseRect.Center() + allowed_velocity;
				 }
            }
		}
	}
}