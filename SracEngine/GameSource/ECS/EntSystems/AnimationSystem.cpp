#pragma once
#include "pch.h"
#include "AnimationSystem.h"

#include "GameSource/ECS/Components.h"
#include "ECS/EntityCoordinator.h"
#include "Debugging/ImGui/Components/ComponentDebugMenu.h"
#include "ECS/Components/PlayerController.h"

namespace ECS
{
	void AnimationSystem::Update(float dt)
	{
		EntityCoordinator* ecs = GameData::Get().ecs;

		for (Entity entity : entities)
		{
			CharacterState& state = ecs->GetComponentRef(CharacterState, entity);
			Transform& transform = ecs->GetComponentRef(Transform, entity);

			Sprite& sprite = ecs->GetComponentRef(Sprite, entity);
			Animation& animation = ecs->GetComponentRef(Animation, entity);

			Animator& animator = animation.animator;
			animator.Update(dt);

			VectorF pos = animator.getAnimationSubRect();
			sprite.subRect = RectF(pos, animator.frameSize());
			sprite.texture = animator.activeSpriteSheet();

			if (DebugMenu::UsingPlaylist())
				return;

			// select the next action
			ActionState action = state.action;
			bool is_flipped = false;

			// todo: perhaps rename this or put it into a namespace?
			const ::Animation* anim = animator.getAnimation(action, state.facingDirection, is_flipped);
			const ::Animation& active_anim = animator.activeAnimation();
			
			transform.flip = is_flipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

			if ( anim && anim != &active_anim )
			{
				animator.selectAnimation(*anim);
			}
		}
	}
}