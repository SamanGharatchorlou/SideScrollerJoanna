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

		enum RuntimeFlags 
		{ 
			RestrictLeft	= 1 << 0, 
			RestrictRight	= 2 << 0, 
			RestrictUp		= 3 << 0, 
			RestrictDown	= 4 << 0
		};
		
		static ECS::Component::Type type() { return ECS::Component::Collider; }
		
		virtual bool intersects(Collider& collider);
		virtual bool intersects(const RectF& rect) const;
		bool contains(VectorF position) const;
	
		static bool test1DOverlap(float minA, float maxA, float minB, float maxB);

	#if TRACK_COLLISIONS
		virtual void renderCollider();	
	#endif

		RectF mRect;
		VectorF mTargetCenter;

		u32 mFlags = 0;
		u32 mRuntimeFlags = 0;
	};
}