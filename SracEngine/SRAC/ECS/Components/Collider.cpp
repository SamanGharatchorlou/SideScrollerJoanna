#include "pch.h"
#include "Collider.h"

#if TRACK_COLLISIONS
#include "Debugging/DebugDraw.h"
#endif

namespace ECS 
{
	bool Collider::intersects(const RectF& rect) const
	{
		const bool xOverlaps = mRect.LeftPoint() < rect.RightPoint() && mRect.RightPoint() > rect.LeftPoint();
		const bool yOverlaps = mRect.TopPoint() < rect.BotPoint() && mRect.BotPoint() > rect.TopPoint();

		return xOverlaps && yOverlaps;
	}

	bool Collider::intersects(Collider& collider)
	{ 
		const RectF& rect = collider.mRect;

		// right
		if(mRect.RightPoint() > rect.LeftPoint() && mRect.RightPoint() < rect.RightPoint())
			SetFlag<u32>(mRuntimeFlags, RuntimeFlags::RestrictRight);
		
		// left
		if(mRect.LeftPoint() < rect.RightPoint() && mRect.LeftPoint() > rect.LeftPoint())
			SetFlag<u32>(mRuntimeFlags, RuntimeFlags::RestrictLeft);

		// up
		if(mRect.TopPoint() < rect.BotPoint() && mRect.TopPoint() > rect.TopPoint())
			SetFlag<u32>(mRuntimeFlags, RuntimeFlags::RestrictUp);
		
		// down
		if(mRect.BotPoint() > rect.TopPoint() && mRect.BotPoint() < rect.BotPoint())
			SetFlag<u32>(mRuntimeFlags, RuntimeFlags::RestrictDown);

		const bool xOverlaps = mRect.LeftPoint() < rect.RightPoint() && mRect.RightPoint() > rect.LeftPoint();
		const bool yOverlaps = mRect.TopPoint() < rect.BotPoint() && mRect.BotPoint() > rect.TopPoint();

		return intersects(collider.mRect);
	}

	bool Collider::contains(VectorF position) const 
	{
		bool xOverlaps = mRect.LeftPoint() < position.x && mRect.RightPoint() > position.x;
		bool yOverlaps = mRect.TopPoint() < position.y && mRect.BotPoint() > position.y;

		return xOverlaps && yOverlaps;
	}

	bool Collider::test1DOverlap(float minA, float maxA, float minB, float maxB)
	{
		return maxA > minB && minA < maxB;
	}

	#if TRACK_COLLISIONS
	void Collider::renderCollider()
	{
		if (didHit())
		{
			debugDrawRect(scaledRect(), RenderColour::Blue);
		}
		else if (gotHit())
		{
			debugDrawRect(scaledRect(), RenderColour::Red);
		}
		else
		{
			debugDrawRect(scaledRect(), RenderColour::LightGrey);
		}
	}
	#endif
}