#pragma once
#include "Core.h"

namespace Utils
{
	template <typename T, std::size_t N>
	bool ContainsAny(const std::array<T, N>& array, const std::unordered_set<T>& removedItems)
	{
		for (const auto& item : array)
		{
			if (removedItems.contains(item))
				return true;
		}
		return false;
	}

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

	inline bool SafeCharToFloat(const char* str, float& res)
	{
		if (str == nullptr || *str == '\0')
			return false;

		char* endPtr = nullptr;
		errno = 0;

		res = strtof(str, &endPtr);

		if (errno == ERANGE || endPtr == str)
			return false;
		return true;
	}

	inline bool SafeCharToUint(const char* str, unsigned int& res)
	{
		if (str == nullptr || *str == '\0')
			return false;

		char* endPtr = nullptr;
		errno = 0;

		res = strtoul(str, &endPtr, 0);

		if (errno == ERANGE || endPtr == str)
			return false;
		return true;
	}

	template<typename T, typename U>
	inline bool TryGetValue(const std::unordered_map<T, U>& map, char key, int& result) {
		auto it = map.find(key);
		if (it != map.end()) {
			result = it->second;
			return true;
		}
		return false;
	}

	inline std::string FormatFloatToNDecimal(float value, unsigned int n) {
		std::ostringstream stream;
		stream << std::fixed << std::setprecision(n) << value; // Set precision to 1 decimal place
		return stream.str();
	}

	inline std::string FormatDoubleToNDecimal(double value, unsigned int n) {
		std::ostringstream stream;
		stream << std::fixed << std::setprecision(n) << value; // Set precision to 1 decimal place
		return stream.str();
	}

	template <typename T>
	inline const T& GetRandomElement(const std::vector<T>& vec) {
		if (vec.empty()) {
			throw std::runtime_error("Vector is empty");
		}

		// Create a random number generator
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, vec.size() - 1);

		// Get a random index
		int randomIndex = dis(gen);

		// Return the element at the random index
		return vec[randomIndex];
	}
};
