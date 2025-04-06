#pragma once

#include "Core.h"
#include "Deck.h"
#include "Range.h"
#include "Node.h"
#include "Position.h"
#include "Solution.h"

namespace Solver
{
	float CompareHands(const std::vector<Hand>& hands, Board board);
	float CompareHands(const Hand& hand1, const Hand& hand2, Board& board);
	float CompareHands(const Hand& hand1, const Hand& hand2, const Hand& hand3, Board& board);
	float CompareHands(const Hand& hand1, const Hand& hand2, const Hand& hand3, const Hand& hand4, Board& board);

	float CalculateEquity(Situation& sit, float iterCount);
	float CalcBbEvForAiHand(std::shared_ptr<Node> node, const Situation& sit);
	void BalanceNode(std::shared_ptr<Node> node, const Situation& sit);
};