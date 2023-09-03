#pragma once

#include "Map.h"

class StringMap32 : public Map<StringBuffer32, StringBuffer32>
{
public:
	StringMap32() { }

	void fillAtributes(const XMLNode& node);
	void fillValues(const XMLNode& node);

	bool getBool(const char* key) const;
	float getFloat(const char* key) const;
	int getInt(const char* key) const;
	VectorF getVector(const char* x, const char* y) const;
};