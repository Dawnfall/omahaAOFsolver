#pragma once

#include "Core.h"
#include "Position.h"
#include "Data/SolverParams.h"

namespace PokerUtils
{
	float CompareHands(const std::vector<Hand>& hands, const Board_F& flop, const std::array<uint8_t, 2>& turnAndRiver);
	float CompareHands(const Hand& hand1, const Hand& hand2, const Board_F& flop, const std::array<uint8_t, 2>& turnAndRiver);
	float CompareHands(const Hand& hand1, const Hand& hand2, const Hand& hand3, const Board_F& flop, const std::array<uint8_t, 2>& turnAndRiver);
	float CompareHands(const Hand& hand1, const Hand& hand2, const Hand& hand3, const Hand& hand4, const Board_F& flop, const std::array<uint8_t, 2>& turnAndRiver);

	inline float CalcPotAfterAI(Position pos, float pot, float sb, float bb, float aiSize)
	{
		switch (pos)
		{
		case Position::SB:
			return pot + aiSize - sb;
		case Position::BB:
			return pot + aiSize - bb;
		default:
			return 	pot + aiSize;
		}
	}
	inline float GetFoldLoss(Position pos, const SolverParams& solverParams)
	{
		switch (pos)
		{
		case Position::SB:
			return -solverParams.sb;
		case Position::BB:
			return -solverParams.bb;
		default:
			return 0;
		}
	}
	inline float CalcAIProfit(float ev, float potSize, const SolverParams& solverParams)
	{
		return ev * potSize - solverParams.stackSize + solverParams.margin;
	}
}