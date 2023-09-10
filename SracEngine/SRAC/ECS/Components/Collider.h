#pragma once

#include "ECS/EntityCommon.h"

namespace ECS
{
	struct Collider
	{
		enum Flags
		{
			Static = 1 << 0
		};
		
		static ECS::Component::Type type() { return ECS::Component::Collider; }
		
		virtual bool intersects(const Collider& collider) const;
		virtual bool intersects(const RectF& rect) const;
		bool contains(VectorF position) const;
	
		static bool test1DOverlap(float minA, float maxA, float minB, float maxB);

	#if TRACK_COLLISIONS
		virtual void renderCollider();	
	#endif

		RectF mRect;
		u32 mFlags;
	};
}