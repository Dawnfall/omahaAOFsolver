#pragma once
#include "Core.h"
#include "Situation.h"

namespace Utils
{
	std::mt19937& GetRandomGen();

	bool Validate(const Hand& hand, Situation& sit);

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
};
