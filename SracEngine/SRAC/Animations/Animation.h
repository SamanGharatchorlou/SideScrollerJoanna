#pragma once

class Texture;
enum class ActionState;


struct SpriteSheet
{
	Texture* sprite;
	VectorF frameSize;
	VectorF objectSize;
	VectorF objectPos;
	VectorI boundaries;
};

struct Animation
{
	// todo: pull this out of each animation, can hold a seperate list
	SpriteSheet spriteSheet;

	ActionState action = (ActionState)0;

	VectorI direction;
	bool canFlip = false;

	int startIndex = 0;
	int frameCount = 0;

	int minLoops = 0;
	bool looping = true;

	float frameTime = 0.0f;
};