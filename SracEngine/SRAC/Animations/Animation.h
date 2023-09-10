#pragma once

class Texture;
enum class ActionState;

struct SpriteSheet
{
	Texture* sprite;
	VectorF frameSize;
	VectorF sizeFactor;
	VectorI boundaries;
};

struct Animation
{
	SpriteSheet spriteSheet;

	ActionState action = (ActionState)0;
	int startIndex = 0;
	int frameCount = 0;

	int minLoops = 0;
	float frameTime = 0.0f;
};