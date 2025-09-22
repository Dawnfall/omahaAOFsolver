#pragma once

#include "Core.h"
#include "Position.h"
#include "Data/SolverParams.h"
#include "Utils.h"

namespace PokerUtils
{
	inline const unsigned int rangeSize = 211876;

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
	inline std::array<float, N> GenerateMinWinrates(float stdDevPerHand, float z) //threshold winrates for certain confidence that hand is above or below 0 EV.
	{
		std::array<float, N> reqWinrates{ -1 };
		for (size_t n = 1; n <= N - 1; ++n)
		{
			float stdError = stdDevPerHand / std::sqrtf(static_cast<float>(n));
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
	// Iteration Data

	inline void GetIterData(int totalPlayers, unsigned int& repeatIters,
		std::vector<unsigned int>& minHandIters, std::vector<unsigned int>& maxHandIters, std::vector<bool>& is99Conf)
	{
		switch (totalPlayers)
		{

		case 2: //less iters, more hands...7s6s5d ... cca 350s boljsa resitev
			repeatIters = 5;
			minHandIters = { 5,20,50,500,1500 };
			maxHandIters = { 10,50,100,2500,5000 };
			is99Conf = { 0,0,0,0,1 };
			break;
		case 3:
			repeatIters = 5;
			minHandIters = { 5,20,100,500,2000 };
			maxHandIters = { 10,50,250,2500,5000 };
			is99Conf = { 0,0,0,0,1 };
			break;
		default: //4
			repeatIters = 15;
			minHandIters = { 5,10,10,10,20,20,20,50,50,50,100,100,100,100,100 };
			maxHandIters = { 5,10,50,50,50,100,100,100,200,200,250,250,500,500,1000 };
			is99Conf = { 0,0,0,0,0,0,0,0,0,0,0,0,0,1,1 };
			break;
		}
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

	inline const std::vector<size_t>& GetAllFromAiRanges(size_t rangeIndex, int totalPlayers)
	{
		static const std::vector<size_t> empty{};
		static const std::vector<size_t> zero{ 0 };
		static const std::vector<size_t> one{ 1 };
		static const std::vector<size_t> zeroTwo{ 0,2 };
		static const std::vector<size_t> three{ 3 };
		static const std::vector<size_t> oneFour{ 1,4 };
		static const std::vector<size_t> zeroFive{ 0,5 };
		static const std::vector<size_t> zeroTwoSix{ 0,2,6 };

		switch (totalPlayers)
		{
		case 2:
			switch (rangeIndex)
			{
			case 0:
				return empty;
			default: //1
				return zero;
			}
		case 3:
			switch (rangeIndex)
			{
			case 0:
			case 1:
				return empty;
			case 2:
			case 4:
				return zero;
			case 3:
				return one;
			default: //5
				return zeroTwo;
			}
		default: //4 {},{},{0},{},{1},{0},{0,2},{3},{1},{1,4},{0},{0,5},{0,2},{0,2,6}
			switch (rangeIndex)
			{
			case 0:
			case 1:
			case 3:
				return empty;
			case 2:
			case 5:
			case 10:
				return zero;
			case 4:
			case 8:
				return one;
			case 6:
			case 12:
				return zeroTwo;
			case 7:
				return three;
			case 9:
				return oneFour;
			case 11:
				return zeroFive;
			default: //13
				return zeroTwoSix;
			}
		}
	}
	inline const std::vector<size_t>& GetAllFromFoldRanges(size_t rangeIndex, int totalPlayers)
	{
		static const std::vector<size_t> empty{};
		static const std::vector<size_t> zero{ 0 };
		static const std::vector<size_t> two{ 2 };
		static const std::vector<size_t> zeroOne{ 0,1 };
		static const std::vector<size_t> zeroFour{ 0,4 };
		static const std::vector<size_t> twoFive{ 2,5 };
		static const std::vector<size_t> six{ 6 };


		switch (totalPlayers)
		{
		case 2:
			return empty;
		case 3:
			switch (rangeIndex)
			{
			case 0:
			case 2:
			default: //5
				return empty;
			case 1:
			case 3:
				return zero;
			case 4:
				return two;
			}
		default:
			switch (rangeIndex)
			{
			case 0:
			case 2:
			case 6:
			default:
				return empty;
			case 1:
			case 4:
			case 9:
				return zero;
			case 3:
			case 7:
				return zeroOne;
			case 5:
			case 11:
				return two;
			case 8:
				return zeroFour;
			case 10:
				return twoFive;
			case 12:
				return six;
			}
		}
	}

	//finds next player index based on action
	inline int GetToRange(size_t range, bool isAI, int totalPlayers)
	{
		switch (totalPlayers)
		{
		case 2:
			switch (range)
			{
			case 0:
				switch (isAI)
				{
				case true:
					return 1;
				default:
					return -1;
				}
			default:
				return -1;
			}
		case 3:
			switch (range)
			{
			case 0:
				switch (isAI)
				{
				case true:
					return 2;
				default:
					return 1;
				}
			case 1:
				switch (isAI)
				{
				case true:
					return 3;
				default:
					return -1;
				}
			case 2:
				switch (isAI)
				{
				case true:
					return 5;
				default:
					return 4;
				}
			case 3:
			case 4:
			default:
				return -1;
			}
		default:
			switch (range)
			{
			case 0:
				switch (isAI)
				{
				case true:
					return 2;
				default:
					return 1;
				}
			case 1:
				switch (isAI)
				{
				case true:
					return 4;
				default:
					return 3;
				}
			case 2:
				switch (isAI)
				{
				case true:
					return 6;
				default:
					return 5;
				}
			case 3:
				switch (isAI)
				{
				case true:
					return 7;
				default:
					return -1;
				}
			case 4:
				switch (isAI)
				{
				case true:
					return 9;
				default:
					return 8;
				}
			case 5:
				switch (isAI)
				{
				case true:
					return 11;
				default:
					return 10;
				}
			case 6:
				switch (isAI)
				{
				case true:
					return 13;
				default:
					return 12;
				}
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
			case 12:
			default:
				return -1;
			}
		}
	}

	//**************
	//**************
	// Pot

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
					return solParams.stackSize + solParams.bb;
				}
			default:
				throw std::invalid_argument("Invalid argument provided");
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
					return solParams.stackSize + solParams.bb;
				}
			case 4:
				switch (isAI)
				{
				case true:
					return 2 * solParams.stackSize + solParams.sb;
				default:
					return solParams.stackSize + solParams.bb + solParams.sb;
				}
			case 5:
				switch (isAI)
				{
				case true:
					return 3 * solParams.stackSize;
				default:
					return 2 * solParams.stackSize + solParams.bb;
				}
			default:
				throw std::invalid_argument("Invalid argument provided");
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
					return solParams.stackSize + solParams.bb;
				}
			case 8:
				switch (isAI)
				{
				case true:
					return 2 * solParams.stackSize + solParams.sb;
				default:
					return solParams.stackSize + solParams.sb + solParams.bb;
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
					return solParams.stackSize + solParams.sb + solParams.bb;
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
			default:
				throw std::invalid_argument("Invalid argument provided");
			}
		}
		throw std::invalid_argument("Invalid argument provided");
	}

	inline float GetFoldLoss(size_t rangeIndex, const SolverParams& solParams)
	{
		switch (solParams.totalPlayers)
		{
		case 2:
			switch (rangeIndex)
			{
			case 0:
				return solParams.sb;
			default: //1
				return solParams.bb;
			}
		case 3:
			switch (rangeIndex)
			{
			case 0:
				return 0;
			case 1:
			case 2:
				return solParams.sb;
			default: //3 4 5
				return solParams.bb;
			}
		default: //4
			switch (rangeIndex)
			{
			case 0:
			case 1:
			case 2:
				return 0;
			case 3:
			case 4:
			case 5:
			case 6:
				return solParams.sb;
			default: //7 8 9 10 11 12 13
				return solParams.bb;
			}
		}
	}

}