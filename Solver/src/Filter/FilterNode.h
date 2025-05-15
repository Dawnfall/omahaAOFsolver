#pragma once

#include "Core.h"


class FilterNode
{
public:
	FilterNode* parentFilter = nullptr;

	std::vector<URef<FilterNode>> childAndFilters;
	std::vector<URef<FilterNode>> childOrFilters;

	std::unordered_map<int8_t, int> suits;
	std::unordered_map<int8_t, int> ranks;
	std::unordered_set<uint8_t> includedCards;

	bool isNegated = false;

	bool IsLeaf()const
	{
		return !IsNode();
	}
	bool IsNode()const
	{
		return childAndFilters.size() > 0 || childOrFilters.size() > 0;
	}
	bool IsEmpty()const
	{
		return IsLeaf() && suits.size() == 0 && ranks.size() == 0 && includedCards.size() == 0;
	}

	FilterNode* AddChild(bool isAnd)
	{
		if (isAnd)
		{
			childAndFilters.emplace_back(std::make_unique<FilterNode>());
			childAndFilters.back()->parentFilter = this;
			return childAndFilters.back().get();
		}
		else
		{
			childOrFilters.emplace_back(std::make_unique<FilterNode>());
			childOrFilters.back()->parentFilter = this;
			return childOrFilters.back().get();
		}
	}

	bool Validate(const std::array<uint8_t, 4>& hand)const
	{
		if (IsLeaf())
			return ValidateAsLeaf(hand);
		return ValidateAsNode(hand);
	}

private:
	bool ValidateAsLeaf(const std::array<uint8_t, 4>& hand)const
	{
		for (auto suit : suits)
		{
			int count = 0;
			for (auto card : hand)
			{
				int cardSuit = card % 4;
				if (cardSuit == suit.first)
					++count;
			}
			if (count < suit.second)
				return false || isNegated;
		}
		for (auto rank : ranks)
		{
			int count = 0;
			for (auto card : hand)
			{
				int cardRank = card - card % 4;
				if (cardRank == rank.first)
					++count;
			}
			if (count < rank.second)
				return false || isNegated;
		}
		for (auto matchedCard : includedCards)
		{
			bool isFound = false;
			for (auto card : hand)
			{
				if (card == matchedCard)
				{
					isFound = true;
					break;
				}
			}
			if (!isFound)
				return false || isNegated;
		}
		return true && !isNegated;
	}
	bool ValidateAsNode(const std::array<uint8_t, 4>& hand)const
	{
		bool isSuccess = false;

		for (auto& child : childOrFilters)
		{
			if (child->Validate(hand))
			{
				isSuccess = true;
				break;
			}
		}
		if (!isSuccess)
			return false || isNegated;


		for (auto& child : childAndFilters)
		{
			if (!child->Validate(hand))
				return false || isNegated;
		}

		return true && !isNegated;
	}
};