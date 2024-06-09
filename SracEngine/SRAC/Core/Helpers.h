#pragma once

#include "Vector2D.h"

static bool HasFlag(uint32_t flag, uint32_t marker)
{
	return flag & marker;
}

template<class T>
static void SetFlag(T& flag, T marker)
{
	flag |= marker;
}

template<typename T>
void merge(std::vector<T>& vectorA, const std::vector<T>& vectorB)
{
	vectorA.insert(vectorA.end(), vectorB.begin(), vectorB.end());
}


//--------------------------------------------------------
// String helpers
static bool toBool(const char* string)
{
	return StringCompare(string, "true", 4);
}

static int toInt(const char* string)
{
	return atoi(string);
}

static float toFloat(const char* string)
{
	return (float)atof(string);
}

static VectorF toVectorF(const char* x, const char* y)
{
	float xVec = toFloat(x);
	float yVec = toFloat(y);
	return VectorF(xVec, yVec);
}

static Vector2D<int> toVectorI(const char* x, const char* y)
{
	int xVec = toInt(x);
	int yVec = toInt(y);
	return Vector2D<int>(xVec, yVec);
}

//--------------------------------------------------------
// Parser helpers
static VectorF attributesToVectorF(XMLNode node, const char* x, const char* y)
{
	float vec_x = toFloat(node.attribute(x)->value());
	float vec_y = toFloat(node.attribute(y)->value());
	return VectorF(vec_x, vec_y);
}

static VectorI attributesToVectorI(XMLNode node, const char* x, const char* y)
{
	int vec_x = toInt(node.attribute(x)->value());
	int vec_y = toInt(node.attribute(y)->value());
	return VectorI(vec_x, vec_y);
}


//--------------------------------------------------------
// map helpers
template<class T, class K>
static void log(const std::unordered_map<T,K>& map)
{
	DebugPrint(Log, "\n-String Data Map Values-");
	for (auto iter = map.begin(); iter != map.end(); iter++)
	{
		DebugPrint(Log, "Data map %s has value %s", iter->first.c_str(), iter->second.c_str());
	}
	DebugPrint(Log, "----------------");
}

template <class T, class K>
static void merge(std::unordered_map<T, K>& map_a, std::unordered_map<T, K>& map_b)
{
	for (auto iter = map_b.begin(); iter != map_b.end(); iter++)
	{
		map_a[iter->first] = iter->second;
	}
}