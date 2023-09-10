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

	bool Collider::intersects(const Collider& collider) const
	{
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