#pragma once


#include "Core.h"
#include "Solver.h"
#include "Utils.h"

class Deck
{
public:
	Deck() :
		currentSize(52)
	{
		for (int i = 0; i < 52; ++i)
		{
			cards[i] = i;
			cardIndexMap[i] = i;
		}
	}

	void add(int card)
	{
		if (cardIndexMap.find(card) == cardIndexMap.end())
		{
			cards[currentSize] = card;
			cardIndexMap[card] = currentSize;
			++currentSize;
		}
	}

	void Remove(int card)
	{
		auto it = cardIndexMap.find(card);
		if (it != cardIndexMap.end())
		{
			int index = it->second;
			int lastCard = cards[currentSize - 1];
			cards[index] = lastCard;
			cardIndexMap[lastCard] = index;
			--currentSize;
			cardIndexMap.erase(it);
		}
	}

	int getRandom()
	{
		auto& randGen = Utils::GetRandomGen();
		std::uniform_int_distribution<> dis(0, currentSize - 1);
		int randomIndex = dis(randGen);
		int card = cards[randomIndex];
		Remove(card);
		return card;
	}

	std::array<int, 4> dealFourCards()
	{
		std::array<int, 4> hand;
		for (int i = 0; i < 4; ++i)
		{
			hand[i] = getRandom();
			Remove(hand[i]);
		}
		return hand;
	}

	void print() const
	{
		for (int i = 0; i < currentSize; ++i)
		{
			std::cout << cards[i] << " ";
		}
		std::cout << "\n";
	}

private:
	std::array<int, 52> cards;
	std::unordered_map<int, int> cardIndexMap; // Maps card ID to its index in the array
	int currentSize = 0; // Tracks the current number of elements in the array
};