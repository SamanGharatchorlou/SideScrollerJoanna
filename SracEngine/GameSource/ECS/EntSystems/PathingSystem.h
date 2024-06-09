#pragma once

#include "ECS/SystemManager.h"

namespace ECS
{
	class PathingSystem : public EntitySystem
	{
	public:
		PathingSystem(Archetype type) : EntitySystem(type) { }

		void Update(float dt) override;

		static std::vector<VectorI> FindPath(VectorF startPosition, VectorF endPosition);
	};
}