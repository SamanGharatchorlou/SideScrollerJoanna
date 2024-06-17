#include "pch.h"
#include "Animation.h"

// do i need to take the screeb / window size into account?
RectF SpriteSheet::GetRelativeRenderRect() const
{
	VectorF pos = objectPos / frameSize;
	VectorF size = frameSize / objectSize;
	return RectF(pos, size);
}