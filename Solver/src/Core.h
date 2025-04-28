#pragma once

#ifdef SOLVER_DLL
#define SOLVER_API __declspec(dllexport)
#else
#define SOLVER_API __declspec(dllimport)
#endif

#include <phevaluator.h>

#include <filesystem>
#include <cstdint>
#include <array>
#include <vector>
#include <memory>
#include <random>
#include <bitset>
#include <thread>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <barrier>
#include <fstream>

template <typename T>
using URef = std::unique_ptr<T>;
template <typename T>
using SRef = std::shared_ptr<T>;

//card oreder 0,1,2....
// 2c, 2d, 2h, 2s,...

//using Card = phevaluator::Card;
using Rank = phevaluator::Rank;

using Hand = std::array<uint8_t, 4>;
using Board_F = std::array<uint8_t, 3>;
using Board_TR = std::array<uint8_t, 2>;

inline const std::string CardToString(int8_t card)
{
	return phevaluator::Card(static_cast<int>(card)).describeCard();
}

inline const std::string HandToString(const Hand& hand)
{
	return CardToString(hand[0]) + CardToString(hand[1]) + CardToString(hand[2]) + CardToString(hand[3]);
}

template <std::size_t N>
inline std::string CardArrayToString(const std::array<uint8_t, N>& arr)
{
	std::string res;
	for (const auto& i : arr)
	{
		res += CardToString(i);
	}
	return res;
}

