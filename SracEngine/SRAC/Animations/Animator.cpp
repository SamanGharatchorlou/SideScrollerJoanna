#include "pch.h"
#include "Animator.h"

#include "Graphics/Texture.h"
#include "GameSource/Configs.h"

#include "Input/InputManager.h"

Animator::Animator() :
	mAnimationIndex(0), mFrameIndex(0),
	mTime(0.0f),
	mState(TimeState::Stopped) { }


Animator::Animator(AnimationConfig* config)
{
	mFrameIndex = 0;
	mAnimationIndex = 0;
	mTime = 0.0f;
	mTransitionIndex = -1;
	mLoops = 0;
	transitionComplete = true;

	mState = TimeState::Running;

	AddAnimations(config);
}

void Animator::AddAnimations(AnimationConfig* config)
{
	const u32 animation_count = config->animations.size();
	const u32 transition_count = config->transitions.size();

	mAnimations.reserve(mAnimations.size() + animation_count + transition_count);
	mTransitions.reserve(mTransitions.size() + transition_count);

	for (u32 i = 0; i < animation_count; i++)
	{
		const Animation& anim = config->animations[i];
		mAnimations.push_back(config->animations[i]);
	}

	for (u32 i = 0; i < transition_count; i++)
	{
		mAnimations.push_back(config->transitions[i].animation);
		mTransitions.push_back(config->transitions[i].transition);
	}

	if (config->baseSize)
	{
		mBaseSize = config->animations.front().spriteSheet.frameSize;
	}
}

VectorF Animator::getAnimationSubRect() const
{
	const Animation& animation = mAnimations[mAnimationIndex];

	int index = animation.startIndex + mFrameIndex;

	Index bounaries = (animation.spriteSheet.sprite->originalDimentions / animation.spriteSheet.frameSize).toInt();

	int y = 0;
	int x = 0;
	while (index >= bounaries.x)
	{
		y++;
		index -= bounaries.x;
	}
	x = index;

	return animation.spriteSheet.frameSize * VectorF(x, y);
}

void Animator::selectAnimation(ActionState action)
{
	for (int i = 0; i < mAnimations.size(); i++)
	{
		if (mAnimations[i].action == action)
		{
#if FRAME_CAPTURING
			mActions.push(action);
			if (mActions.size() > maxActionLength)
				mActions.popFront();
#endif

			mAnimationIndex = i;
			mFrameIndex = 0;
			mLoops = 0;
			return;
		}
	}

	DebugPrint(Warning, "Animation state %d not present in animator", (int)action);
}

const Animation* Animator::getAnimation(ActionState action) const
{
	for (int i = 0; i < mAnimations.size(); i++)
	{
		if (mAnimations[i].action == action)
		{
			return &mAnimations[i];
		}
	}

	DebugPrint(Warning, "No animation with action %d", action);
	return nullptr;	
}

void Animator::stop()
{
	mTime = 0.0f;
	mState = TimeState::Stopped;
}

void Animator::slowUpdate(float dt)
{
	if (mState == TimeState::Running)
		mTime += dt;

	const Animation& animation = mAnimations[mAnimationIndex];
	bool progress_frame = mTime >= animation.frameTime;
	int next_frame = mFrameIndex + 1;

#if FRAME_CAPTURING
	progress_frame = false;

	const InputManager* input = GameData::Get().inputManager;
	if (input->isPressed(Button::Key::RightArrow))
	{
		next_frame = mFrameIndex + 1;

		if (next_frame >= animation.frameCount)
		{
			frameCaptureIndex++;
			frameCaptureIndex = Maths::clamp(frameCaptureIndex, (u32)0, (u32)mActions.size());
		}

		progress_frame = true;
	}

	if (input->isPressed(Button::Key::LeftArrow))
	{
		next_frame = mFrameIndex - 1;
		if (next_frame < 0)
		{
			if (mLoops > 0)
			{
				mLoops--;
			}
			else if(mActions.size() > 2)
			{
				//mActions.popBack();
				frameCaptureIndex--;
				
				selectAnimation(mActions.back());


				return;
			}

			next_frame = animation.frameCount - 1;
		}

		progress_frame = true;
	}
#endif

	if (progress_frame)
	{
		mFrameIndex = next_frame % animation.frameCount;
		mTime = 0.0f;
		
		if (mFrameIndex == 0)
			mLoops++;

		if (inTransition() && canChange())
		{
			ActionState toAction = mTransitions[mTransitionIndex].to;
			mTransitionIndex = -1;

			selectAnimation(toAction);
		}
	}
}