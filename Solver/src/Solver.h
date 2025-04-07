#pragma once

#include "Core.h"
#include "Situation.h"

namespace Solver
{
	const int totalHandVsHandIter = 5000;
	const int totalHandVsRangeIter = 5000;
	const float errorThreshold = 0.05f;

	float CompareHands(const std::vector<Hand>& hands, Board board);
	float CompareHands(const Hand& hand1, const Hand& hand2, Board& board);
	float CompareHands(const Hand& hand1, const Hand& hand2, const Hand& hand3, Board& board);
	float CompareHands(const Hand& hand1, const Hand& hand2, const Hand& hand3, const Hand& hand4, Board& board);

	float CalculateEquity(Situation& sit, float errorThreshold, int maxIter);
};