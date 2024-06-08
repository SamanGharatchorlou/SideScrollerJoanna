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


void Animator::ResetOnNewAnimation()
{
	mTime = 0.0f;
	mFrameIndex = 0;
	mLoops = 0;
	mFlipped = false;
	mFinished = false;
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

			DebugPrint(PriorityLevel::Log, "Selecting animation %s (%d,%d)", actionToString(action).c_str(), mAnimations[i].direction.x, mAnimations[i].direction.y);
			mAnimationIndex = i;
			ResetOnNewAnimation();
			return true;
		}
	}

	DebugPrint(Warning, "Animation state %d not present in animator", (int)action);
	return false;
}

bool Animator::selectAnimation(const Animation& anim)
{
	if (DebugMenu::UsingPlaylist())
		return false;

	for (int i = 0; i < mAnimations.size(); i++)
	{
		if (mAnimations[i].action == anim.action)
		{
			const bool correct_direction = mAnimations[i].direction == anim.direction;
			const bool flipped_direction = mAnimations[i].canFlip && mAnimations[i].direction == anim.direction * -1;

			if(correct_direction || flipped_direction)
			{
	#if FRAME_CAPTURING
				mActions.push(action);
				if (mActions.size() > maxActionLength)
					mActions.popFront();
	#endif
			
				DebugPrint(PriorityLevel::Log, "Selecting animation %s (%d,%d)", actionToString(anim.action).c_str(), mAnimations[i].direction.x, mAnimations[i].direction.y);
				mAnimationIndex = i;
				ResetOnNewAnimation();
				mFlipped = flipped_direction;
				return true;
			}
		}
	}

	// fallback
	return selectAnimation(anim.action);
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

const Animation* Animator::getAnimation(ActionState action, VectorI direction, bool& flipped_direction) const
{
	for (int i = 0; i < mAnimations.size(); i++)
	{
		if (mAnimations[i].action == action)
		{
			const bool correct_direction = mAnimations[i].direction == direction;
			flipped_direction = mAnimations[i].canFlip && mAnimations[i].direction == direction * -1;

			if(correct_direction || flipped_direction)
				return &mAnimations[i]; 
		}
	}

	return getAnimation(action);
	//DebugPrint(Warning, "No animation with action %d and direction %f,%f", action, direction.x, direction.y);
	//return nullptr;
}

u32 Animator::getAnimationIndex(const Animation& anim)
{
	for (u32 i = 0; i < mAnimations.size(); i++)
	{
		if (mAnimations[i].action == anim.action)
		{
			const bool correct_direction = mAnimations[i].direction == anim.direction;
			const bool flipped_direction = mAnimations[i].canFlip && mAnimations[i].direction == anim.direction * -1;

			if(correct_direction || flipped_direction)
			{
				return i;
			}
		}
	}

	return -1;
}

void Animator::stop()
{
	mTime = 0.0f;
	mState = TimeState::Stopped;
}

void Animator::restart()
{
	ResetOnNewAnimation();
}

bool Animator::RunActive(float dt, bool force_loop)
{
	if (mState == TimeState::Running)
		mTime += dt;

	const Animation& animation = mAnimations[mAnimationIndex];
	bool progress_frame = mTime >= animation.frameTime;
	int next_frame = mFrameIndex + 1;

	if (progress_frame)
	{	
		// cant pass last frame if we can loop
		if(!animation.looping && lastFrame() && !force_loop)
		{
			mFinished = true;
			return false;
		}

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