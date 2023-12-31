#include "pch.h"
#include "Channel.h"

#include "Audio.h"


Channel::Channel() : mIndex(-1)
{
	free();
}


bool Channel::hasPlayingState() const
{
	return mState == Playing || mState == Looping;
}


bool Channel::attenuate() const
{
	return mSource.isPositive();
}


void Channel::free()
{
	mState = Free;
	mAudio = nullptr;
	mID = NULL;
	mSource = VectorF(-1.0f, -1.0f);
	mVolume = 1.0f;
}


bool Channel::isPaused() const
{
	return mState == Paused;
}

// continue loop
void Channel::playNext()
{
	mAudio->playNext(mIndex);
}

// start loop
void Channel::loop()
{
	mState = Looping;
	mAudio->play(mIndex);
#if PRINT_PLAYING_AUDIO
	DebugPrint(Log, "%d: Looping audio: %s", mIndex, mAudio->name().c_str());
#endif
}

void Channel::play()
{
	mState = Playing;
	mAudio->play(mIndex);
#if PRINT_PLAYING_AUDIO
	DebugPrint(Log, "%d: Playing audio: %s", mIndex, mAudio->name().c_str());
#endif
}


void Channel::fadeIn(int ms)
{
	mState = Playing;
	mAudio->fadeIn(mIndex, ms);
#if PRINT_PLAYING_AUDIO
	DebugPrint(Log, "%d: Fading in audio: %s", mIndex, mAudio->name().c_str());
#endif
}


void Channel::fadeOut(int ms)
{
	mState = Playing;
	mAudio->fadeOut(mIndex, ms);
#if PRINT_PLAYING_AUDIO
	DebugPrint(Log, "%d: Fading out audio: %s", mIndex, mAudio->name().c_str());
#endif
}


void Channel::stop()
{
	mAudio->stop(mIndex);
}

void Channel::resume()
{
	mState = mPrePauseState;
	mPrePauseState = Free;
	mAudio->resume(mIndex);
}

void Channel::pause()
{
	mPrePauseState = mState;
	mState = Paused;
	mAudio->pause(mIndex);
}

bool Channel::isAudioRunning() const
{
	return mAudio->isPlaying(mIndex);
}


bool Channel::isPlaying(const Audio* audio, uintptr_t id) const
{
	return hasPlayingState() && has(audio, id);
}

bool Channel::has(const Audio* audio, uintptr_t id) const
{
	return mID == id && mAudio == audio;
}

bool Channel::canFree() const
{
	return hasPlayingState() && !isAudioRunning();
}

AudioType Channel::type() { return mAudio->type(); }