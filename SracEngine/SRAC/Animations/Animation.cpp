#include "pch.h"
#include "Animation.h"


RectF SpriteSheet::GetRelativeRenderRect() const
{
	VectorF pos = objectPos / frameSize;
	VectorF size = frameSize / objectSize;
	return RectF(pos, size);
}