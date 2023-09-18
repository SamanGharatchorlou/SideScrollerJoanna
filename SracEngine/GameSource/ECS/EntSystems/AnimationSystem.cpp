#pragma once
#include "pch.h"
#include "AnimationSystem.h"

#include "GameSource/ECS/Components.h"
#include "ECS/EntityCoordinator.h"
#include "Debugging/ImGui/Components/ComponentDebugMenu.h"

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
			state.canChange = animator.canChange();

			VectorF pos = animator.getAnimationSubRect();
			sprite.subRect = RectF(pos, animator.frameSize());
			sprite.texture = animator.activeSpriteSheet();

			if (!DebugMenu::UsingPlaylist())
			{
				// select the next action
				ActionState action = state.action;
				if (action != animator.activeAction())
				{
					if (!animator.canChange())
						continue;

					animator.selectAnimation(action);
				}
			}

			transform.sizeFactor = animator.activeAnimation().spriteSheet.sizeFactor;
		}
	}
}