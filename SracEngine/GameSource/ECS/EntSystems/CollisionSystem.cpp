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
		u32 count = collider_list.size();

		// first we need to update the collider position with where the entity wants to be
		for (Entity entity : entities)
		{
			Collider& collider = ecs->GetComponentRef(Collider, entity);
			collider.mRuntimeFlags = 0;

			// ignore static colliders, they dont move
			if(HasFlag(collider.mFlags, Collider::Flags::Static))
				continue;
			
			Transform& transform = ecs->GetComponentRef(Transform, entity);

			// this doesnt seem to make a difference, seems like it should
			//collider.mRect.SetCenter(transform.targetCenterPosition);
			collider.mRect.SetCenter(transform.rect.Center());
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

			u32 index = colliders.GetComponentIndex(entity);
			Transform& transform = ecs->GetComponentRef(Transform, entity);
			CharacterState* character_state = ecs->GetComponent(CharacterState, entity);

			for (u32 i = 0; i < count; i++) 
			{
				// ignore ourself
				if(i == index)
					continue;

				 Collider& that_collider = collider_list[i];
				 if(this_collider.intersects(that_collider)) 
				 {
					 VectorF velocity = transform.targetCenterPosition - transform.rect.Center();

					 VectorF allowed_velocity;

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
					 
					 transform.targetCenterPosition = transform.rect.Center() + allowed_velocity;

					 // even rolling it back doenst work either
					 this_collider.mRect.SetCenter(transform.targetCenterPosition);

					 if (character_state)
					 {
						 // set the velocity here
						 if (!can_move_horizontally)
						 {
							 character_state->restrictMovement[CharacterState::Left] = true;
							 character_state->restrictMovement[CharacterState::Right] = true;
						 }

						 if (!can_move_vertically)
						 {
							 character_state->restrictMovement[CharacterState::Up] = true;
							 character_state->restrictMovement[CharacterState::Down] = true;
						 }

						 // also need to set acceleraton
						 // make a velcity reset function matey
					 }
				 }
            }
		}
	}
}