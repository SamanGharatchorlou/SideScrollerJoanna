#pragma once

#include "ECS/EntityCommon.h"

namespace ECS
{
	struct Collider
	{
		COMPONENT_TYPE(Collider)

		enum Flags
		{
			// type
			Static = 1 << 0,
			//Damage = 1 << 1,

			// damage layer
			IsPlayer = 1 << 2,
			IsEnemy = 1 << 3,

			HitPlayerOnly = 1 << 4,
			HitEnemyOnly = 1 << 5

		};

		enum RuntimeFlags 
		{ 
			RestrictLeft	= 1 << 0, 
			RestrictRight	= 2 << 0, 
			RestrictUp		= 3 << 0, 
			RestrictDown	= 4 << 0
		};
		
		virtual bool intersects(Collider& collider);
		virtual bool intersects(const RectF& rect) const;
		bool contains(VectorF position) const;
	
		static bool test1DOverlap(float minA, float maxA, float minB, float maxB);

		void SetPosition(const RectF& rect, VectorF& target);
		void RollBackPosition();
		void RollForwardPosition();

	#if TRACK_COLLISIONS
		virtual void renderCollider();	
	#endif

		RectF mRect;
		VectorF mForward;
		VectorF mBack;;

		u32 mFlags = 0;
	};
}