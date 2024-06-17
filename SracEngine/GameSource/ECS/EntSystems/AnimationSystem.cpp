#pragma once
#include "pch.h"
#include "AnimationSystem.h"

#include "ECS/Components/Components.h"
#include "ECS/EntityCoordinator.h"
#include "Debugging/ImGui/Components/ComponentDebugMenu.h"
#include "ECS/Components/PlayerController.h"
#include "Debugging/ImGui/ImGuiMainWindows.h"

namespace ECS
{
	void AnimationSystem::Update(float dt)
	{
		EntityCoordinator* ecs = GameData::Get().ecs;

		for (Entity entity : entities)
		{
			// debug break point
			if(DebugMenu::GetSelectedEntity() == entity)
				int a = 4;

			CharacterState& state = ecs->GetComponentRef(CharacterState, entity);
			Transform& transform = ecs->GetComponentRef(Transform, entity);
			Animation& animation = ecs->GetComponentRef(Animation, entity);

			bool using_playlist = DebugMenu::UsingPlaylist(entity);

			Animator& animator = animation.animator;

			if(!using_playlist)
				animator.RunActive(dt);
			
			Sprite& sprite = ecs->GetComponentRef(Sprite, entity);

			VectorF pos = animator.getAnimationSubRect();
			sprite.subRect = RectF(pos, animator.FrameSize());

			if (using_playlist)
				continue;

			// select the next action
			ActionState action = state.action;
			bool is_flipped = false;

			// todo: perhaps rename this or put it into a namespace?
			const ::Animation* anim = animator.getAnimation(action, state.facingDirection, is_flipped);
			const ::Animation* active_anim = animator.activeAnimation();
			
			transform.flip = is_flipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

			if ( anim && anim != active_anim )
			{
				animator.selectAnimation(*anim);

				const SpriteSheet& ss = animator.getSpritesheet(*anim);
				sprite.relativeRenderRect = ss.GetRelativeRenderRect();
				sprite.texture = ss.texture;
			}
		}
	}

	RectF AnimationSystem::GetRenderRect(ECS::Entity entity)
	{
		EntityCoordinator* ecs = GameData::Get().ecs;
		if(const ECS::Sprite* sprite = ecs->GetComponent(Sprite, entity))
		{
			if(const ECS::Transform* transform = ecs->GetComponent(Transform, entity))
			{
				VectorF size = transform->rect.Size() * sprite->relativeRenderRect.Size();
				VectorF pos = transform->rect.TopLeft() - sprite->relativeRenderRect.TopLeft() * size;
				return RectF(pos, size);
			}
		}

		return RectF();
	}
}