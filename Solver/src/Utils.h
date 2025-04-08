#pragma once
#include "Core.h"

namespace Utils
{
	extern unsigned int NumberOfThreads;
	std::minstd_rand& GetRandomGen();
	const int PerNodeIterCount = 100;

	template <typename K, typename V>
	std::vector<V> CopyMapValuesToVector(const std::unordered_map<K, V>& map)
	{
		std::vector<V> vec;
		vec.reserve(map.size());
		for (const auto& pair : map)
		{
			vec.emplace_back(pair.second);
		}
		return vec;
	}

	inline std::string ToString(const Hand& hand)
	{
		std::string str = "";
		for (const Card& c : hand)
		{
			str += c.describeCard() + " ";
		}
		return str;
	}
};
