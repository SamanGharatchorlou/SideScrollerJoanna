#include "pch.h"
#include "Animator.h"

#include "GameSource/Configs.h"
#include "Debugging/ImGui/Components/ComponentDebugMenu.h"

Animator::Animator() :
	mAnimationIndex(0), 
	mFrameIndex(0),
	mTime(0.0f),
	mLoops(0),
	mState(TimeState::Stopped) { }

Animator::Animator(AnimationConfig* config)
{
	mFrameIndex = 0;
	mAnimationIndex = 0;
	mTime = 0.0f;
	mLoops = 0;
	mState = TimeState::Running;

	AddAnimations(config);
}

void Animator::AddAnimations(AnimationConfig* config)
{
	const u32 animation_count = config->animations.size();

	mAnimations.reserve(mAnimations.size() + animation_count);

	for (u32 i = 0; i < animation_count; i++)
	{
		const Animation& anim = config->animations[i];
		mAnimations.push_back(config->animations[i]);
	}
}

VectorF Animator::getAnimationSubRect() const
{
	const Animation& animation = mAnimations[mAnimationIndex];

	int index = animation.startIndex + mFrameIndex;
	VectorI bounaries = animation.spriteSheet.boundaries;

	int y = 0;
	int x = 0;
	while (index >= bounaries.x)
	{
		y++;
		index -= bounaries.x;
	}
	x = index;

	return animation.spriteSheet.frameSize * Vector2D<int>(x, y).toFloat();
}

bool Animator::selectAnimation(ActionState action)
{
	if (DebugMenu::UsingPlaylist())
		return false;

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
			return true;
		}
	}

	DebugPrint(Warning, "Animation state %d not present in animator", (int)action);
	return false;
}

bool Animator::selectAnimation(ActionState action, VectorI direction)
{
	if (DebugMenu::UsingPlaylist())
		return false;

	for (int i = 0; i < mAnimations.size(); i++)
	{
		if (mAnimations[i].action == action && mAnimations[i].direction == direction)
		{
#if FRAME_CAPTURING
			mActions.push(action);
			if (mActions.size() > maxActionLength)
				mActions.popFront();
#endif

			mAnimationIndex = i;
			mFrameIndex = 0;
			mLoops = 0;
			return true;
		}
	}

	return selectAnimation(action);
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


const Animation* Animator::getAnimation(ActionState action, VectorI direction) const
{
	for (int i = 0; i < mAnimations.size(); i++)
	{
		if (mAnimations[i].action == action && mAnimations[i].direction == direction)
		{
			return &mAnimations[i];
		}
	}

	DebugPrint(Warning, "No animation with action %d and direction %f,%f", action, direction.x, direction.y);
	return nullptr;
}

void Animator::stop()
{
	mTime = 0.0f;
	mState = TimeState::Stopped;
}

bool Animator::RunActive(float dt)
{
	if (mState == TimeState::Running)
		mTime += dt;

	const Animation& animation = mAnimations[mAnimationIndex];
	bool progress_frame = mTime >= animation.frameTime;
	int next_frame = mFrameIndex + 1;

	if (progress_frame)
	{
		mFrameIndex = next_frame % animation.frameCount;
		mTime = 0.0f;

		if (mFrameIndex == 0)
		{
			mLoops++;
			return true;
		}
	}

	return false;
}

void Animator::Update(float dt)
{
	if (DebugMenu::UsingPlaylist())
		return;

	RunActive(dt);
}