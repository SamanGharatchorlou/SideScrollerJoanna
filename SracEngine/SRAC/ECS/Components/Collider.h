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
			HitEnemyOnly = 1 << 5,

			IgnoreAll = 1 << 6,
			IgnoreCollisions = 1 << 7
		};

		enum RuntimeFlags 
		{ 
			RestrictLeft	= 1 << 0, 
			RestrictRight	= 2 << 0, 
			RestrictUp		= 3 << 0, 
			RestrictDown	= 4 << 0
		};
		
		virtual bool intersects(const Collider& collider) const;
		virtual bool intersects(const RectF& rect) const;
		bool contains(VectorF position) const;
	
		static bool test1DOverlap(float minA, float maxA, float minB, float maxB);

		const RectF& GetRect() const;
		void SetRect(const RectF& rect);

		//void SetCenter(const VectorF& pos);

		//void SetPosition(const RectF& rect, VectorF& target);
		void RollBackPosition();
		void RollForwardPosition();

	#if TRACK_COLLISIONS
		virtual void renderCollider();	
	#endif
		
		u32 mFlags = 0;
		
		VectorF allowedMovement;

		VectorF mForward;
		VectorF mBack;
		VectorF posOffset;

	private:
		// always need to use the transform to set this position and offset by posOffset
		RectF mRect;
	};
}