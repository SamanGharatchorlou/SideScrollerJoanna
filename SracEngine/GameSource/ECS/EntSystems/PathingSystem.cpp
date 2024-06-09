#include "pch.h"
#include "PathingSystem.h"

#include "ECS/Components/Components.h"
#include "ECS/EntityCoordinator.h"
#include "ECS/Components/Physics.h"
#include "Game/SystemStateManager.h"
#include "GameStates/GameState.h"
#include "System/Window.h"

namespace ECS
{
	static int heuristic(VectorI from, VectorI to)
	{
		return std::abs(from.x - to.x) + std::abs(from.y - to.y);
	}
	
	using TileCost = std::pair<VectorI, int>;

	struct GreaterThanByCost
	{
		bool operator ()(TileCost left, TileCost right)
		{
			return left.second > right.second;
		};
	};

	static std::vector<VectorI> getPath(VectorI start, VectorI finish, std::vector< std::pair<VectorI, VectorI>>& visited)
	{
		std::vector<VectorI> path;
		path.push_back(finish);

		while(path.back() != start)
		{
			for( u32 i = 0; i < visited.size(); i++ )
			{
				if(visited[i].second == path.back())
				{
					path.push_back(visited[i].first);
				}
			}
		}

		return path;
	}


	std::vector<VectorI> PathingSystem::FindPath(VectorF startPosition, VectorF endPosition)
	{
		const GameState* gs = GameData::Get().systemStateManager->GetActiveState<GameState>();
		
		ECS::EntityCoordinator* ecs = GameData::Get().ecs;
		const TileMap& map = ecs->GetComponentRef(TileMap, gs->activeMap);

		VectorF window_size = GameData::Get().window->size();
		VectorF size_ratio = window_size / map.tileMap.mapSize;
		VectorF tile_size = VectorF(8.0f, 8.0f);
		VectorF world_tile_size = tile_size * size_ratio;
		
		VectorI tile_count = (map.tileMap.mapSize / tile_size).toInt();

		VectorI start_index = (startPosition / world_tile_size).toInt();
		VectorI end_index = (endPosition / world_tile_size).toInt();

		// Lowest to highest path cost queue
		
		std::priority_queue<TileCost, std::vector<TileCost>, GreaterThanByCost> frontier;
		frontier.push(TileCost(start_index, 0));

		std::vector< std::pair<VectorI, VectorI>> visited;

	#if DEBUG_CHECK
		int loopCounter = 0;
	#endif

		int pathCount = 0;

		while (!frontier.empty())
		{
			const VectorI index = frontier.top().first;
			const VectorI neighbours[4] = {
										index + VectorI(0,-1), index + VectorI( 1, 0),
										index + VectorI(0, 1), index + VectorI(-1, 0) };

			// Search all neighbours
			for (unsigned int i = 0; i < 4; i++)
			{
				const VectorI next_index = *(neighbours + i);

				bool valid = next_index.isPositive() && next_index.x < tile_count.x && next_index.y < tile_count.y;

				if (valid)
				{
					bool has_visited = false;
					for( u32 i = 0; i < visited.size(); i++ )
					{
						if(visited[i].second == next_index)
						{
							has_visited = true;
						}
					}


					if (!has_visited)
					{
						int priority = heuristic(end_index, next_index);
						frontier.push(TileCost(next_index, priority));

						visited.push_back( std::pair(index,next_index));

						if (next_index == end_index)
						{
							return getPath(start_index, end_index, visited);
							//return visited;
						}
					}
				}
			}

			//frontier.pop();

	#if DEBUG_CHECK
			if (frontier.size() > tile_count.x * tile_count.y)
			{
				return std::vector<VectorI>();
			}

			loopCounter++;
	#endif
		}

	#if DEBUG_CHECK
		int largeNumberOfLoops = 50;
		if (loopCounter > largeNumberOfLoops)
		{
			DebugPrint(Log,
				"Empty frontier: No valid path was found %d loops were made until this failed\n", loopCounter);
		}
	#endif
		
				return std::vector<VectorI>();
	}

	void PathingSystem::Update(float dt)
	{
		EntityCoordinator* ecs = GameData::Get().ecs;

 		for (Entity entity : entities)
		{
			Pathing& pathing = ecs->GetComponentRef(Pathing, entity);
			Transform& transform = ecs->GetComponentRef(Transform, entity);
			Physics& physics = ecs->GetComponentRef(Physics, entity);


		}
	}
}