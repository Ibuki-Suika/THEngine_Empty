#ifndef THHASHMAP_H
#define THHASHMAP_H

#include "THObject.h"
#include <map>

namespace THEngine
{
	template<class KeyType,class ValueType>
	class Map
	{
	protected:
		std::map<KeyType, ValueType> map;

	public:
		bool Put(KeyType key, ValueType value)
		{
			std::pair<std::map<KeyType,ValueType>::iterator, bool> insert_pair;
			insert_pair = map.insert(std::pair<KeyType, ValueType>(key, value));
			if (insert_pair.second == true)
			{
				value->Retain();
				return true;
			}
			return false;
		}

		ValueType Get(KeyType key)
		{
			std::map<KeyType,ValueType>::iterator iter;
			iter = map.find(key);
			if (iter != map.end())
			{
				return iter->second;
			}
		}

		bool Remove(KeyType key)
		{
			std::map<KeyType, ValueType>::iterator iter;
			iter = map.find(key);
			if (iter != map.end())
			{
				iter->second->Release();
				iter->erase();
				return true;
			}
			return false;
		}

		void Clear()
		{
			map.clear();
		}
	};

	class MapIterator
	{

	};
}

#endif