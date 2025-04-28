#pragma once

#include "Core.h"
#include "Data/WeightedHand.h"

class HandFilter
{
	std::unordered_set<phevaluator::Card> matchedCards;
	std::unordered_set<uint8_t> matchedRanks;

	std::vector<WeightedHand> ApplyFilter(const std::vector<WeightedHand>& inRange)
	{
		std::vector<WeightedHand> outRange;

		for (auto& weHand : inRange)
		{

		}

		return outRange;
	}

	bool CheckMatch(const Hand& hand)
	{
		if (
			matchedCards.contains(hand[0]) ||
			matchedCards.contains(hand[1]) ||
			matchedCards.contains(hand[2]) ||
			matchedCards.contains(hand[3])
			)
			return true;
		return false;
	}

	bool CheckRank(const Hand& hand)
	{
		
	}

};