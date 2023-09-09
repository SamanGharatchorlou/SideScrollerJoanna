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
					 transform.targetCenterPosition = transform.baseRect.Center();
					 //velocity.speed.zero();

				 }
            }
		}
	}
}