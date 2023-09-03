#include "pch.h"
#include "StringMap.h"

void StringMap32::fillValues(const XMLNode& node)
{
	XMLNode childNode = node.child();
	while (childNode)
	{
		mData[childNode.name()] = childNode.value();
		childNode = childNode.next();
	}
}

void StringMap32::fillAtributes(const XMLNode& node)
{
	for (XMLNode::Attribute attr = node.attribute(); attr; attr = attr->next_attribute())
	{
		mData[attr->name()] = attr->value();
	}
}

bool StringMap32::getBool(const char* key) const
{
	return toBool(mData.at(key).c_str());
}

float StringMap32::getFloat(const char* key) const
{
	return  toFloat(mData.at(key).c_str());
}

int StringMap32::getInt(const char* key) const
{
	return toInt(mData.at(key).c_str());
}

VectorF StringMap32::getVector(const char* x, const char* y) const
{
	return toVector(mData.at(x).c_str(), mData.at(y).c_str());
}