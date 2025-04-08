#pragma once

#include <phevaluator.h>

#include <array>
#include <vector>
#include <memory>
#include <random>
#include <bitset>
#include <thread>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

using ui = unsigned int;
using vec_ui = std::vector<ui>;

using Card = phevaluator::Card;
using Rank = phevaluator::Rank;

using Hand = std::array<Card, 4>;
using Board = std::array<Card, 5>;

constexpr int totalHands = (52 * 51 * 50 * 49) / (4 * 3 * 2);
