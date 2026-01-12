#pragma once
#include "Core.h"

#include "Utils.h"
#include "Data/SolverParams.h"
#include "Data/SolverResult.h"
#include "Data/SolverData.h"
#include "RandomGenerator.h"

namespace Solver
{
	inline void Init()
	{
		PokerUtils::wrs_3Std_z95 = PokerUtils::GenerateMinWinrates<1000>(3, PokerUtils::z_95);
		PokerUtils::wrs_5Std_z95 = PokerUtils::GenerateMinWinrates<1000>(5, PokerUtils::z_95);
		PokerUtils::wrs_7Std_z95 = PokerUtils::GenerateMinWinrates<1000>(7, PokerUtils::z_95);
		PokerUtils::wrs_10Std_z95 = PokerUtils::GenerateMinWinrates<1000>(10, PokerUtils::z_95);
		PokerUtils::wrs_12Std_z95 = PokerUtils::GenerateMinWinrates<1000>(12, PokerUtils::z_95);
		PokerUtils::wrs_15Std_z95 = PokerUtils::GenerateMinWinrates<1000>(15, PokerUtils::z_95);

		PokerUtils::wrs_3Std_z99 = PokerUtils::GenerateMinWinrates<1000>(3, PokerUtils::z_99);
		PokerUtils::wrs_5Std_z99 = PokerUtils::GenerateMinWinrates<1000>(5, PokerUtils::z_99);
		PokerUtils::wrs_7Std_z99 = PokerUtils::GenerateMinWinrates<1000>(7, PokerUtils::z_99);
		PokerUtils::wrs_10Std_z99 = PokerUtils::GenerateMinWinrates<1000>(10, PokerUtils::z_99);
		PokerUtils::wrs_12Std_z99 = PokerUtils::GenerateMinWinrates<1000>(12, PokerUtils::z_99);
		PokerUtils::wrs_15Std_z99 = PokerUtils::GenerateMinWinrates<1000>(15, PokerUtils::z_99);
	}


	float CalcBbEvForAiHand(size_t nodeIndex, const std::vector<uint8_t>& range, const std::vector<float>& evs, const SolverParams& solParams, SolverData& solverData, const std::unordered_set<uint8_t>& removedCards, const std::vector<size_t>& hands, Board_TR& turnAndRiver, RandomGenerator& randGen, size_t currRepeat);

	//deal turn and river and calculate equity for first player
	float CalculateEquityOnFlop(const std::vector<size_t>& handIndices, const std::vector<uint8_t>& ranges, const Board_F& flop, Board_TR& turnAndRiver, const std::unordered_set<uint8_t> removedCards, RandomGenerator& randGen);
	
	//calculates equity of first player
	float CalcEqOfHand1(const std::vector<size_t>& handIndices, const std::vector<uint8_t>& handRange, const Board_F& flop, const std::array<uint8_t, 2>& turnAndRiver);

	std::tuple<std::vector<uint8_t>, std::vector<float>> BuildRangeAndEvs(const SolverParams& solParams, SolverData& solverData);//, std::minstd_rand& randGen);

	void SolveAsync(const SolverParams& solverParams, std::function<void(const SolverParams&, SolverResult)> callback);

	void ExecuteThreadTask(const SolverParams& solverParams, SolverData& solverData,
		const std::vector<uint8_t>& range, std::vector<float>& evs);

	// returns an index of random hand in range, removing used cards
	inline int GetRandomHand(const std::vector<uint8_t>& range, const std::unordered_set<uint8_t>& removedCards, RandomGenerator& randGen)
	{
		while (true)
		{
			unsigned int randHand = randGen.GetRandomHandFromRange();
			if (
				!removedCards.contains(range[randHand * 4]) &&
				!removedCards.contains(range[randHand * 4 + 1]) &&
				!removedCards.contains(range[randHand * 4 + 2]) &&
				!removedCards.contains(range[randHand * 4 + 3]
				))
				return randHand;
		}
		return -1;
	}

	// Returns a random hand based on range probabilities and removed cards
	inline int ForcePickHand(const std::vector<uint8_t>& range, const std::vector<float>& evs, size_t rangeIndex, int totalRanges, const std::unordered_set<uint8_t>& removedCards, RandomGenerator& randGen, bool isAI)
	{
		int randHandIndex;
		for (int i = 0; i < 50; i++)
		{
			randHandIndex = GetRandomHand(range, removedCards, randGen);
			if ((isAI && evs[randHandIndex * totalRanges + rangeIndex] >= 0) ||
				(!isAI && evs[randHandIndex * totalRanges + rangeIndex] <= 0))
			{
				return randHandIndex;
			}
		}
		return randHandIndex;
	}
}