#include "pch.h"
#include "TransformSystem.h"

#include "ECS/Components/Components.h"
#include "ECS/Components/ComponentCommon.h"
#include "ECS/Components/Collider.h"
#include "ECS/EntityCoordinator.h"
#include "ECS/Components/Physics.h"

namespace ECS
{
	void TransformSystem::Update(float dt)
	{
		EntityCoordinator* ecs = GameData::Get().ecs;

 		for (Entity entity : entities)
		{
			Transform& transform = ecs->GetComponentRef(Transform, entity);
			
			if(Sprite* sprite = ecs->GetComponent(Sprite, entity))
			{
				// set transform size based on the sprite
				if(sprite->renderSize.isPositive())
					transform.rect.SetSize(sprite->renderSize);
			}

			Collider* collider = ecs->GetComponent(Collider, entity);
			
			// update the transform position once its passed all collision checks
			if(collider)
			{
				//transform.rect.SetCenter(collider->mForward);

				transform.rect.Translate(collider->allowedMovement);
				collider->allowedMovement = VectorF::zero();
			}
			
			// set the target position, the place we want to move to given collisions
			if(Physics* physics = ecs->GetComponent(Physics, entity))
			{
				// where we're trying to move to
				//transform.targetCenterPosition = transform.rect.Center() + physics->speed;

				// update the collider position and roll it forwards to where we want to move
				if(collider && !HasFlag(collider->mFlags, Collider::Flags::Static))
				{
					collider->SetRect( GetColliderRect(entity) );
					collider->mBack = collider->GetRect().Center();
					collider->mForward = collider->mBack + physics->speed;

					//collider->SetPosition(transform.rect, transform.targetCenterPosition);
					collider->RollForwardPosition();
				}
			}
		}
	}
}