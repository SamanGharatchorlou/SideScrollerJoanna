#pragma once

#include "Vector2D.h"

static bool hasFlag(uint32_t flag, uint32_t marker)
{
	return flag & marker;
}

template<class T>
static void setFlag(T& flag, T marker)
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

static VectorF toVector(const char* x, const char* y)
{
	float xVec = toFloat(x);
	float yVec = toFloat(y);
	return VectorF(xVec, yVec);
}