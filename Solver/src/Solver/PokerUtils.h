#pragma once

#include "Core.h"
#include "Position.h"
#include "Data/SolverParams.h"
#include "Utils.h"


namespace PokerUtils
{
	inline const unsigned int rangeSize = 211876;

	//***************
	// Random

	inline std::uniform_int_distribution<> rangeDist(0, rangeSize - 1);
	inline std::uniform_int_distribution<> deckDist(0, 51);
	inline std::uniform_real_distribution<> evRandDist(-5.0f, 5.0f);

	inline uint8_t GetRandomCardFromDeck(std::minstd_rand& randGen)
	{
		return deckDist(randGen);
	}
	inline float GetRandomEv(std::minstd_rand& randGen)
	{
		return static_cast<float>(evRandDist(randGen));
	}
	inline unsigned int GetRandomHandFromRange(std::minstd_rand& randGen)
	{
		return rangeDist(randGen);
	}

	//*****************
	//*****************
	// winrate error confidence ; TODO: could be made constexpr

	const float z_95 = 1.64485f;
	const float z_99 = 2.576f;

	inline std::array<float, 1000> wrs_3Std_z95;
	inline std::array<float, 1000> wrs_5Std_z95;
	inline std::array<float, 1000> wrs_7Std_z95;
	inline std::array<float, 1000> wrs_10Std_z95;
	inline std::array<float, 1000> wrs_12Std_z95;
	inline std::array<float, 1000> wrs_15Std_z95;

	inline std::array<float, 1000> wrs_3Std_z99;
	inline std::array<float, 1000> wrs_5Std_z99;
	inline std::array<float, 1000> wrs_7Std_z99;
	inline std::array<float, 1000> wrs_10Std_z99;
	inline std::array<float, 1000> wrs_12Std_z99;
	inline std::array<float, 1000> wrs_15Std_z99;


	const std::array<std::array<float, 1000>*, 2> minWrs2p_z95 = { &wrs_3Std_z95,&wrs_5Std_z95 };
	const std::array<std::array<float, 1000>*, 6> minWrs3p_z95 = { &wrs_5Std_z95,&wrs_3Std_z95,&wrs_7Std_z95,&wrs_5Std_z95,&wrs_5Std_z95,&wrs_10Std_z95 };
	const std::array<std::array<float, 1000>*, 14> minWrs4p_z95 = { &wrs_7Std_z95,&wrs_5Std_z95,&wrs_10Std_z95,&wrs_3Std_z95,&wrs_7Std_z95,&wrs_7Std_z95,&wrs_12Std_z95,&wrs_5Std_z95,&wrs_5Std_z95,&wrs_10Std_z95,&wrs_5Std_z95,&wrs_10Std_z95,&wrs_10Std_z95,&wrs_15Std_z95 };

	const std::array<std::array<float, 1000>*, 2> minWrs2p_z99 = { &wrs_3Std_z99,&wrs_5Std_z99 };
	const std::array<std::array<float, 1000>*, 6> minWrs3p_z99 = { &wrs_5Std_z99,&wrs_3Std_z99,&wrs_7Std_z99,&wrs_5Std_z99,&wrs_5Std_z99,&wrs_10Std_z99 };
	const std::array<std::array<float, 1000>*, 14> minWrs4p_z99 = { &wrs_7Std_z99,&wrs_5Std_z99,&wrs_10Std_z99,&wrs_3Std_z99,&wrs_7Std_z99,&wrs_7Std_z99,&wrs_12Std_z99,&wrs_5Std_z99,&wrs_5Std_z99,&wrs_10Std_z99,&wrs_5Std_z99,&wrs_10Std_z99,&wrs_10Std_z99,&wrs_15Std_z99 };

	template<size_t N>
	inline std::array<float, N> GenerateMinWinrates(float stdDevPerHand, float z)
	{
		std::array<float, N> reqWinrates{ -1 };
		for (size_t n = 1; n <= N - 1; ++n)
		{
			float stdError = stdDevPerHand / std::sqrtf(n);
			reqWinrates[n] = z * stdError;
		}
		return reqWinrates;
	}

	inline float GetMinWinrate(size_t rangeIndex, int totalPlayers, size_t n, bool is99Confidence)
	{
		switch (totalPlayers)
		{
		case 2:
			return (is99Confidence) ? (*minWrs2p_z99[rangeIndex])[n] : (*minWrs2p_z95[rangeIndex])[n];
		case 3:
			return (is99Confidence) ? (*minWrs3p_z99[rangeIndex])[n] : (*minWrs3p_z95[rangeIndex])[n];
		default:
			return (is99Confidence) ? (*minWrs4p_z99[rangeIndex])[n] : (*minWrs4p_z95[rangeIndex])[n];
		}
	}

	//******************
	//******************
	// Hand Iters
	inline size_t maxRepeatIters = 6;

	inline std::tuple<size_t, bool> GetHandItersAndIs99Confidence(size_t currRepeat)
	{
		if (currRepeat < 2)
			return { 10,false };
		if (currRepeat < 4)
			return { 250,false };
		if (currRepeat < 5)
			return{ 500,false };
		return { 1000,true };
	}

	//******************
	// Range Info

	const std::array<Position, 2> nodePositions2p = { Position::SB,Position::BB };
	const std::array<Position, 6> nodePositions3p = { Position::BU,Position::SB,Position::SB,Position::BB,Position::BB,Position::BB };
	const std::array<Position, 14> nodePositions4p = {
		Position::CO,
		Position::BU,Position::BU,
		Position::SB,Position::SB,Position::SB,Position::SB,
		Position::BB,Position::BB,Position::BB,Position::BB,Position::BB,Position::BB,Position::BB };

	const std::array<std::string, 2> nodeNames2p = { "SB","BB vs SB" };
	const std::array<std::string, 6> nodeNames3p = { "BU","SB","SB vs BU","BB vs SB","BB vs BU","BB vs BU+SB" };
	const std::array<std::string, 14> nodeNames4p = {
	"CO",
	"BU","BU vs CO",
	"SB","SB vs BU","SB vs CO","SB vs CO+BU",
	"BB vs SB","BB vs BU","BB vs BU+SB","BB vs CO","BB vs CO+SB","BB vs CO+BU","BB vs CO+BU+SB" };

	inline const std::string& GetRangeName(size_t i, int totalPlayers)
	{
		switch (totalPlayers)
		{
		case 2:
			return nodeNames2p[i];
		case 3:
			return nodeNames3p[i];
		default:
			return nodeNames4p[i];
		}
	}
	inline Position GetRangePosition(size_t rangeIndex, int totalPlayers)
	{
		switch (totalPlayers)
		{
		case 2:
			return nodePositions2p[rangeIndex];
		case 3:
			return nodePositions3p[rangeIndex];
		default:
			return nodePositions4p[rangeIndex];
		}
	}

	//******************
	//******************
	// Range tree

	const std::array<int, 2> fromRanges2p = { -1,0 };
	const std::array<int, 6> fromRanges3p = { -1,0,0,1,2,2 };
	const std::array<int, 14> fromRanges4p = { -1,0,0,1,1,2,2,3,4,4,5,5,6,6 };

	const std::vector<std::vector<size_t>> allFromAIRanges2p = { {},{0} };
	const std::vector<std::vector<size_t>> allFromAIRanges3p = { {},{},{0},{1},{0},{0,2} };
	const std::vector<std::vector<size_t>> allFromAIRanges4p = { {},{},{0},{},{1},{0},{0,2},{3},{1},{1,4},{0},{0,5},{0,2},{0,2,6} };

	const std::vector<std::vector<size_t>> allFromFoldRanges2p = { {},{} };
	const std::vector<std::vector<size_t>> allFromFoldRanges3p = { {},{0},{},{0},{2},{} };
	const std::vector<std::vector<size_t>> allFromFoldRanges4p = { {},{0},{},{0,1},{0},{2},{},{0,1},{0,4},{0},{2,5},{2},{6},{} };

	const std::array<int, 2> toAIRange2p = { 1,-1 };
	const std::array<int, 6> toAIRange3p = { 2,3,5,-1,-1,-1 };
	const std::array<int, 14> toAIRange4p = { 2,4,6,7,9,11,13,-1,-1,-1,-1,-1,-1,-1 };

	const std::array<int, 2> toFoldRange2p = { -1,-1 };
	const std::array<int, 6> toFoldRange3p = { 1,-1,4,-1,-1,-1 };
	const std::array<int, 14> toFoldRange4p = { 1,3,5,-1,8,10,12,-1,-1,-1,-1,-1,-1,-1 };

	const std::array<bool, 2> isFromAINode2p = { 0,1 };
	const std::array<bool, 6> isFromAINode3p = { 0,0,1,1,0,1 };
	const std::array<bool, 14> isFromAINode4p = { 0,0,1,0,1,0,1,1,0,1,0,1,0,1 };

	inline const std::vector<size_t>& GetAllFromAiRanges(size_t rangeIndex, int totalPlayers)
	{
		switch (totalPlayers)
		{
		case 2:
			return allFromAIRanges2p[rangeIndex];
		case 3:
			return allFromAIRanges3p[rangeIndex];
		default:
			return allFromAIRanges4p[rangeIndex];
		}
	}

	inline const std::vector<size_t>& GetAllFromFoldRanges(size_t rangeIndex, int totalPlayers)
	{
		switch (totalPlayers)
		{
		case 2:
			return allFromFoldRanges2p[rangeIndex];
		case 3:
			return allFromFoldRanges3p[rangeIndex];
		default:
			return allFromFoldRanges4p[rangeIndex];
		}
	}

	//**************

	inline float GetPotAfter(size_t range, const SolverParams& solParams, bool isAI)
	{
		switch (solParams.totalPlayers)
		{
		case 2:
			switch (range)
			{
			case 1:
				switch (isAI)
				{
				case true:
					return 2 * solParams.stackSize;
				default:
					return solParams.bb;
				}
			}
		case 3:
			switch (range)
			{
			case 3:
				switch (isAI)
				{
				case true:
					return 2 * solParams.stackSize;
				default:
					return solParams.bb;
				}
			case 4:
				switch (isAI)
				{
				case true:
					return 2 * solParams.stackSize + solParams.sb;
				default:
					return solParams.bb + solParams.sb;
				}
			case 5:
				switch (isAI)
				{
				case true:
					return 3 * solParams.stackSize;
				default:
					return 2 * solParams.stackSize + solParams.bb;
				}

			}
		case 4:
			switch (range)
			{
			case 7:
				switch (isAI)
				{
				case true:
					return 2 * solParams.stackSize;
				default:
					return solParams.bb;
				}
			case 8:
				switch (isAI)
				{
				case true:
					return 2 * solParams.stackSize + solParams.bb;
				default:
					return solParams.sb + solParams.bb;
				}
			case 9:
				switch (isAI)
				{
				case true:
					return 3 * solParams.stackSize;
				default:
					return 2 * solParams.stackSize + solParams.bb;
				}
			case 10:
				switch (isAI)
				{
				case true:
					return 2 * solParams.stackSize + solParams.sb;
				default:
					return solParams.sb + solParams.bb;
				}
			case 11:
				switch (isAI)
				{
				case true:
					return 3 * solParams.stackSize;
				default:
					return 2 * solParams.stackSize + solParams.bb;
				}
			case 12:
				switch (isAI)
				{
				case true:
					return 3 * solParams.stackSize + solParams.sb;
				default:
					return 2 * solParams.stackSize + solParams.sb + solParams.bb;
				}
			case 13:
				switch (isAI)
				{
				case true:
					return 4 * solParams.stackSize;
				default:
					return 3 * solParams.stackSize + solParams.bb;
				}
			}
		}
		throw std::invalid_argument("Invalid argument provided");
	}

	inline bool GetFromRange(size_t rangeIndex, size_t& fromIndex, int totalPlayers)
	{
		if (rangeIndex == 0)
			return false;

		switch (totalPlayers)
		{
		case 2:
			fromIndex = fromRanges2p[rangeIndex];
			break;
		case 3:
			fromIndex = fromRanges3p[rangeIndex];
			break;
		default:
			fromIndex = fromRanges4p[rangeIndex];
			break;
		}
		return true;
	}

	inline bool GetToRange(size_t i, size_t& nextIndex, bool isAI, int totalPlayers)
	{
		int tempIndex;
		switch (isAI)
		{
		case true:
			switch (totalPlayers)
			{
			case 2:
				tempIndex = toAIRange2p[i];
				break;
			case 3:
				tempIndex = toAIRange3p[i];
				break;
			default:
				tempIndex = toAIRange4p[i];
				break;
			}
			break;
		default:
			switch (totalPlayers)
			{
			case 2:
				tempIndex = toFoldRange2p[i];
				break;
			case 3:
				tempIndex = toFoldRange3p[i];
				break;
			default:
				tempIndex = toFoldRange4p[i];
				break;
			}
			break;
		}
		if (tempIndex < 0)
			return false;

		nextIndex = tempIndex;
		return true;
	}

	inline bool IsFromAiRange(size_t rangeIndex, int totalPlayers)
	{
		switch (totalPlayers)
		{
		case 2:
			return isFromAINode2p[rangeIndex];
		case 3:
			return isFromAINode3p[rangeIndex];
		default:
			return isFromAINode4p[rangeIndex];
		}
	}


	inline const std::vector<size_t>& GetAllFromRanges(size_t rangeIndex, int totalPlayers)
	{
		switch (totalPlayers)
		{
		case 2:
			return allFromAIRanges2p[rangeIndex];
		case 3:
			return allFromAIRanges3p[rangeIndex];
		default:
			return allFromAIRanges4p[rangeIndex];
		}
	}


	inline unsigned int GetTotalRanges(int totalPlayers)
	{
		switch (totalPlayers)
		{
		case 4:
			return 14;
		case 3:
			return 6;
		default:
			return 2;
		}
	}

	inline float GetFoldLoss(size_t rangeIndex, const SolverParams& solParams)
	{
		switch (GetRangePosition(rangeIndex, solParams.totalPlayers))
		{
		case Position::SB:
			return -solParams.sb;
		case Position::BB:
			return -solParams.bb;
		default:
			return 0;
		}
	}

	inline float GetRangeAiSize(size_t rangeIndex, const SolverParams& solParams)
	{
		switch (solParams.totalPlayers)
		{
		case 2:
			switch (rangeIndex)
			{
			case 0:
				return solParams.stackSize - solParams.sb;
			default:
				return solParams.stackSize - solParams.bb;
			}
		case 3:
			switch (rangeIndex)
			{
			case 0:
				return solParams.stackSize;
			case 1:
			case 2:
				return solParams.stackSize - solParams.sb;
			default:
				return solParams.stackSize - solParams.bb;
			}
		default:
			switch (rangeIndex)
			{
			case 0:
			case 1:
			case 2:
				return solParams.stackSize;
			case 3:
			case 4:
			case 5:
			case 6:
				return solParams.stackSize - solParams.sb;
			default:
				return solParams.stackSize - solParams.bb;
			}
		}
	}


	inline float CalcPotAfterAI(Position pos, float pot, const SolverParams& solverParams)
	{
		switch (pos)
		{
		case Position::SB:
			return pot + solverParams.stackSize - solverParams.sb;
		case Position::BB:
			return pot + solverParams.stackSize - solverParams.bb;
		default:
			return 	pot + solverParams.stackSize;
		}
	}







	inline void Init()
	{
		wrs_3Std_z95 = PokerUtils::GenerateMinWinrates<1000>(3, PokerUtils::z_95);
		wrs_5Std_z95 = PokerUtils::GenerateMinWinrates<1000>(5, PokerUtils::z_95);
		wrs_7Std_z95 = PokerUtils::GenerateMinWinrates<1000>(7, PokerUtils::z_95);
		wrs_10Std_z95 = PokerUtils::GenerateMinWinrates<1000>(10, PokerUtils::z_95);
		wrs_12Std_z95 = PokerUtils::GenerateMinWinrates<1000>(12, PokerUtils::z_95);
		wrs_15Std_z95 = PokerUtils::GenerateMinWinrates<1000>(15, PokerUtils::z_95);

		wrs_3Std_z99 = PokerUtils::GenerateMinWinrates<1000>(3, PokerUtils::z_99);
		wrs_5Std_z99 = PokerUtils::GenerateMinWinrates<1000>(5, PokerUtils::z_99);
		wrs_7Std_z99 = PokerUtils::GenerateMinWinrates<1000>(7, PokerUtils::z_99);
		wrs_10Std_z99 = PokerUtils::GenerateMinWinrates<1000>(10, PokerUtils::z_99);
		wrs_12Std_z99 = PokerUtils::GenerateMinWinrates<1000>(12, PokerUtils::z_99);
		wrs_15Std_z99 = PokerUtils::GenerateMinWinrates<1000>(15, PokerUtils::z_99);
	}


}