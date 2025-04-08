#pragma once

#include "Core.h"
#include "Situation.h"
#include "Solution.h"
#include "Node.h"

namespace Solver
{
	bool ValidateHand(const Hand& hand, const std::unordered_set<int>& removedCards);

	float CompareHands(const std::vector<Hand>& hands, Board board);
	float CompareHands(const Hand& hand1, const Hand& hand2, Board& board);
	float CompareHands(const Hand& hand1, const Hand& hand2, const Hand& hand3, Board& board);
	float CompareHands(const Hand& hand1, const Hand& hand2, const Hand& hand3, const Hand& hand4, Board& board);

	float CalculateEquity(const std::vector<Hand>& hands, Board& board, const std::unordered_set<int> removedCards);

	float CalcBbEvForAiHand(const Node& node,const std::unordered_set<int>& removedCards,const std::vector<Hand>& hands, Board& board, float potSize);

	Solution Solve(int totalPlayers, float bb, float sb, const std::array<Card, 3>& flop);


	float CalcPotAfterAI(Position pos, float pot);
	float CalcAIProfit(float ev, float potSize);
	float GetFoldLoss(Position pos);
};