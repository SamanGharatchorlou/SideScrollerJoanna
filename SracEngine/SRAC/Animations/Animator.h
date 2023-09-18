#pragma once

#include "Graphics/Tileset.h"
#include "Animation.h"
#include "CharacterStates.h"

struct AnimationConfig;

#define FRAME_CAPTURING 0

class Animator
{
public:
	Animator();
	Animator(AnimationConfig* config);
	void AddAnimations(AnimationConfig* config);

	void Update(float dt);

	const Animation* getAnimation(ActionState action) const;
	const Animation& activeAnimation() const { return mAnimations[mAnimationIndex]; }
	VectorF getAnimationSubRect() const;

	void selectAnimation(ActionState state);

	void start() { mState = TimeState::Running; }
	void pause() { mState = TimeState::Paused; }
	void stop();

	bool RunActive(float dt);

	bool isRunning() const { return mState == TimeState::Running; }
	bool isStarted() const { return mState == TimeState::Running || mState == TimeState::Paused; }

	float frameTime() const { return mAnimations[mAnimationIndex].frameTime; }
	VectorF frameSize() const { return mAnimations[mAnimationIndex].spriteSheet.frameSize; }

	ActionState activeAction() const { return mAnimations[mAnimationIndex].action; }
	Texture* activeSpriteSheet() const { return mAnimations[mAnimationIndex].spriteSheet.sprite; }
	bool lastFrame() const { return mFrameIndex == mAnimations[mAnimationIndex].frameCount - 1; }
	bool canChange() const { return mLoops >= mAnimations[mAnimationIndex].minLoops; }


	std::vector<Animation> mAnimations;

#if FRAME_CAPTURING
	Queue<ActionState> mActions;
	u32 maxActionLength = 10;
	u32 frameCaptureIndex = 0;
#endif

	VectorF mBaseSize;
	u32 mAnimationIndex;
	u32 mFrameIndex;
	
	TimeState mState;

	int mLoops;
	float mTime;
};
