#pragma once

#if DEBUG_CHECK
#include "Debug/Logging.h"
#endif

class XMLNode;

template <class T, class K>
class Map
{
public:
	// Iterator
	using iterator = typename std::unordered_map<T, K>::iterator;
	iterator begin() { return mData.begin(); }
	iterator end() { return mData.end(); }

	// Const Iterator	
	using const_iterator = typename std::unordered_map<T, K>::const_iterator;
	const_iterator begin() const { return mData.begin(); }
	const_iterator end() const { return mData.end(); }


public:
	virtual ~Map() { };

	void clear() { mData.clear(); }

	K& operator [] (const T& key) { return mData[key]; }
	const K& at(const T& key) const;

	int size() const { mData.size(); }
	bool contains(const T& key) const { return mData.count(key) > 0; }
	bool isEmpty() const { return mData.size() == 0; }

	void merge(const Map<T, K>& map);

#if DEBUG_CHECK
	void log() const;
#endif

protected:
	std::unordered_map<T, K> mData;
};

#if DEBUG_CHECK
template<class T, class K>
void Map<T,K>::log() const
{
	DebugPrint(Log, "\n-String Data Map Values-");
	const_iterator iter;
	for (iter = mData.begin(); iter != mData.end(); iter++)
	{
		BasicString key(iter->first);
		BasicString value(iter->second);
		DebugPrint(Log, "Data map %s has value %s", key.c_str(), value.c_str());
	}
	DebugPrint(Log, "----------------");
}
#endif


template <class T, class K>
const K& Map<T, K>::at(const T& key) const
{
	return mData.at(key);
}


template <class T, class K>
void Map<T, K>::merge(const Map<T, K>& map)
{
	Map<T, K>::const_iterator iter;

	for (iter = map.begin(); iter != map.end(); iter++)
	{
		mData[iter->first] = iter->second;
	}
}

