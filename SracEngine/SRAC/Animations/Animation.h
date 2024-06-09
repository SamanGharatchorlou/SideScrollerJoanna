#pragma once

class Texture;
enum class ActionState;


struct SpriteSheet
{
	BasicString ID;
	Texture* texture;
	VectorF frameSize;
	VectorF objectSize;
	VectorF objectPos;
	VectorI boundaries;

	// derive the render rect from the object pos + size
	RectF GetRelativeRenderRect() const;
};

struct Animation
{
	// todo: pull this out of each animation, can hold a seperate list
	SpriteSheet* spriteSheet;

	ActionState action = (ActionState)0;

	VectorI direction;
	bool canFlip = false;

	int startIndex = 0;
	int frameCount = 0;

	int minLoops = 0;
	bool looping = true;

	float frameTime = 0.0f;
};