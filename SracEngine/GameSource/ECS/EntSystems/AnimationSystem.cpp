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
			CharacterState& state = ecs->GetComponent(CharacterState, entity);
			Transform& transform = ecs->GetComponent(Transform, entity);

			Sprite& sprite = ecs->GetComponent(Sprite, entity);
			Animation& animation = ecs->GetComponent(Animation, entity);

			Animator& animator = animation.animator;
			animator.slowUpdate(dt);

			state.inTransition = animator.inTransition();
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
					if (animator.inTransition() && animator.activeTransition().to == action)
						continue;

					if (!animator.canChange())
						continue;

					// if this a transition action change the action to the transition action
					StateTransition transition;
					transition.from = animator.activeAction();
					transition.to = action;

					const std::vector<StateTransition>& transitions = animator.mTransitions;
					for (u32 i = 0; i < transitions.size(); i++)
					{
						if (transitions[i].from == transition.from && transitions[i].to == transition.to)
						{
							action = transitions[i].transitionAction;
							animator.mTransitionIndex = i;
							break;
						}
					}

					animator.selectAnimation(action);
				}
			}

			transform.sizeFactor = animator.activeAnimation().spriteSheet.sizeFactor;
		}
	}
}