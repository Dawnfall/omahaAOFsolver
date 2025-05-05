#pragma once
#include "Core.h"


#include "Utils.h"
#include "Data/SolverParams.h"
#include "Data/SolverResult.h"
#include "Data/SolverData.h"

namespace Solver
{
	inline float CalcEqOfHand1(const std::vector<size_t>& handIndices, const std::vector<uint8_t>& handRange, const Board_F& flop, const std::array<uint8_t, 2>& turnAndRiver);

	float CalcBbEvForAiHand(size_t nodeIndex, const std::vector<uint8_t>& range, const std::vector<float>& evs, const SolverParams& solParams, SolverData& solverData, const std::unordered_set<uint8_t>& removedCards, const std::vector<size_t>& hands, Board_TR& turnAndRiver, std::minstd_rand& randGen, size_t totalPerHandIters, bool boolIs99Confidence, bool doRemoveFolded);

	inline float CalculateEquityOnFlop(const std::vector<size_t>& handIndices, const std::vector<uint8_t>& ranges, const Board_F& flop, Board_TR& turnAndRiver, const std::unordered_set<uint8_t> removedCards, std::minstd_rand& randGen);

	std::tuple<std::vector<uint8_t>, std::vector<float>> BuildRangeAndEvs(const SolverParams& solParams, SolverData& solverData);//, std::minstd_rand& randGen);

	void SolveAsync(const SolverParams& solverParams, std::function<void(const SolverParams&, SolverResult)> callback);

	// returns an index of random hand in range, removing used cards
	inline int GetRandomHand(const std::vector<uint8_t>& range, const std::unordered_set<uint8_t>& removedCards, std::minstd_rand& randGen)
	{
		while (true)
		{
			unsigned int i = PokerUtils::GetRandomHandFromRange(randGen);
			if (
				!removedCards.contains(range[i * 4]) &&
				!removedCards.contains(range[i * 4 + 1]) &&
				!removedCards.contains(range[i * 4 + 2]) &&
				!removedCards.contains(range[i * 4 + 3]
				))
				return i;
		}
		return -1;
	}

	// Returns a random hand based on range probabilities and removed cards
	inline int ForcePickHand(const std::vector<uint8_t>& range, const std::vector<float>& evs, size_t rangeIndex, int totalRanges, const std::unordered_set<uint8_t>& removedCards, std::minstd_rand& randGen, bool isAI)
	{
		while (true)
		{
			int randHandIndex = GetRandomHand(range, removedCards, randGen);
			if ((isAI && evs[randHandIndex * totalRanges + rangeIndex] >= 0) ||
				(!isAI && evs[randHandIndex * totalRanges + rangeIndex] <= 0))
			{
				return randHandIndex;
			}
		}
		return 0;
	}
}