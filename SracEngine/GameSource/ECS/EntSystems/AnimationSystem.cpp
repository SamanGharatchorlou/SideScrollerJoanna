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
			//CharacterState& state = ecs->GetComponentRef(CharacterState, entity);
			Transform& transform = ecs->GetComponentRef(Transform, entity);

			Sprite& sprite = ecs->GetComponentRef(Sprite, entity);
			Animation& animation = ecs->GetComponentRef(Animation, entity);

			Animator& animator = animation.animator;
			animator.Update(dt);

			animation.inTransition = animator.inTransition();
			animation.canChange = animator.canChange();

			VectorF pos = animator.getAnimationSubRect();
			sprite.subRect = RectF(pos, animator.frameSize());
			sprite.texture = animator.activeSpriteSheet();

			if (!DebugMenu::UsingPlaylist())
			{
				// select the next action
				if (animation.action != animator.activeAction())
				{
					if (animator.inTransition() && animator.activeTransition().to == animation.action)
						continue;

					if (!animator.canChange())
						continue;

					// if this a transition action change the action to the transition action
					StateTransition transition;
					transition.from = animator.activeAction();
					transition.to = animation.action;

					const std::vector<StateTransition>& transitions = animator.mTransitions;
					for (u32 i = 0; i < transitions.size(); i++)
					{
						if (transitions[i].from == transition.from && transitions[i].to == transition.to)
						{
							animation.action = transitions[i].transitionAction;
							animator.mTransitionIndex = i;
							break;
						}
					}

					animator.selectAnimation(animation.action);
				}
			}

			transform.sizeFactor = animator.activeAnimation().spriteSheet.sizeFactor;
		}
	}
}