#pragma once

#include "Core.h"
#include "Utils.h"
#include "Solver.h"

class Deck
{
public:
	Deck(const std::unordered_set<int>& removedCards)
	{
		m_count = 0;
		for (int i = 0; i < 52; ++i) {
			if (removedCards.find(i) == removedCards.end()) {
				m_cards[m_count++] = i;
			}
		}
	}

	void Add(int card)
	{
		m_cards[m_count++] = card;
	}

	void Remove(int card)
	{
		for (int i = 0; i < m_count; i++)
			if (m_cards[i] == card)
			{
				m_cards[i] = m_cards[m_count - 1];
				m_cards[m_count - 1] = card;
				m_count -= 1;
				return;
			}

	}

	void FillTurnAndRiver(Board& board)const
	{
		auto& randGen = Utils::GetRandomGen();
		std::uniform_int_distribution<> dis1(0, m_count - 1);
		std::uniform_int_distribution<> dis2(0, m_count - 2);

		int first = dis1(randGen);
		int second = dis2(randGen);

		if (second >= first) {
			second += 1;
		}

		board[3] = m_cards[first];
		board[4] = m_cards[second];
	}

	int RemoveAt(int i)
	{
		int card = m_cards[i];
		m_cards[i] = m_cards[m_count - 1];
		m_cards[m_count - 1] = card;
		m_count -= 1;
		return card;
	}

	int RemoveRandom()
	{
		auto& randGen = Utils::GetRandomGen();
		std::uniform_int_distribution<> dis(0, m_count - 1);
		int randomIndex = dis(randGen);
		return RemoveAt(randomIndex);
	}

	Hand DealFourCards()
	{
		Hand hand;
		for (int i = 0; i < 4; ++i)
		{
			hand[i] = RemoveRandom();
		}
		return hand;
	}

	void print() const
	{
		for (int i = 0; i < m_count; ++i)
		{
			std::cout << m_cards[i] << " ";
		}
		std::cout << "\n";
	}

private:
	std::array<int, 52> m_cards;
	int m_count = 0; // Tracks the current number of elements in the array
};