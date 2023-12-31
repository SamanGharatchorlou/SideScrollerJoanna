#pragma once

class Audio;
enum class AudioType;

class Channel
{
public:
	enum State
	{
		Free,
		Playing,
		Looping,
		Paused
	};

public:
	Channel();
	void init(int index) { mIndex = index; }

	void setAudio(const Audio* audio) { mAudio = audio; }
	const Audio* audio() const { return mAudio; }

	void play();
	void loop();
	void playNext();

	void stop();
	void resume();
	void pause();

	void fadeIn(int ms);
	void fadeOut(int ms);

	bool isPaused() const;
	bool hasPlayingState() const;
	bool isAudioRunning() const;

	bool has(const Audio* audio, uintptr_t id) const;
	bool isPlaying(const Audio* audio, uintptr_t id) const;

	bool attenuate() const;

	bool canFree() const;
	void free();

	void setIndex(int index) { mIndex = index; }
	int index() const { return mIndex; }

	AudioType type();

public:
	State mState;
	VectorF mSource;
	uintptr_t mID;

	// 0 - 1 independant of game sound volume
	float mVolume;

	State mPrePauseState;

private:
	int mIndex;
	const Audio* mAudio;
};