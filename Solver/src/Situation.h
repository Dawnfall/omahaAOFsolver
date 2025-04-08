#pragma once
#include "Core.h"
#include "Position.h"
#include "Utils.h"

const float AISize = 5.0f;
const float Sb = 0.5f;
const float Bb = 1.0f;

class Situation
{
public:
	void PrintHand(const Hand& hand)
	{
		std::cout << hand[0] << " " << hand[1] << " " << hand[2] << " " << hand[3] << "\n";
	}

	void AddHand(const Hand& hand, Position playerPosition)
	{
		m_hands.emplace_back(hand);
		m_removedCards.insert(hand.begin(), hand.end());

		if (playerPosition == Position::SB)
			m_potSize += AISize - Sb;
		else if (playerPosition == Position::BB)
			m_potSize += AISize - Bb;
		else
			m_potSize += AISize;
	}
	void SetFlop(Card c1, Card c2, Card c3)
	{
		m_board[0] = c1;
		m_board[1] = c2;
		m_board[2] = c3;

		m_removedCards.insert(c1);
		m_removedCards.insert(c2);
		m_removedCards.insert(c3);
	}
	void AddDeadCards(const std::vector<Card>& deadCards)
	{
		m_removedCards.insert(deadCards.begin(), deadCards.end());
	}
	void ClearToPlayer()
	{
		for (int i = 1; i < m_hands.size(); i++)
		{
			for (auto& c : m_hands[i])
				m_removedCards.erase(c);
		}
		if (m_hands.size() > 1)
		{
			m_hands.erase(m_hands.begin() + 1, m_hands.end());
		}
	}

	Board& GetBoard() { return m_board; }
	std::vector<Hand>& GetHands() { return m_hands; }
	std::unordered_set<int>& GetRemovedCards() { return m_removedCards; };

	const Board& GetConstBoard()const { return m_board; }
	float GetPotSize() { return m_potSize; }
	float GetProfit(float ev)
	{
		return ev * GetPotSize() - AISize;
	} 

	void Print()
	{
		if (m_board.size() > 0)
			PrintBoard();
		if (m_removedCards.size() > 0)
			PrintRemovedCards();
		PrintHands();
	}
	void PrintBoard()
	{
		std::cout << "Board:" << " ";
		for (const Card& card : m_board)
		{
			std::cout << card.describeCard() << " ";
		}
		std::cout << std::endl;
	}
	void PrintHands()
	{
		for (int i = 0; i < m_hands.size(); i++)
		{
			for (const Card& card : m_hands[i])
				std::cout << card.describeCard() << " ";
			std::cout << "\n";
		}
	}
	void PrintRemovedCards()
	{
		std::cout << "Dead cards: ";
		for (const Card& c : m_removedCards)
			std::cout << c.describeCard() << " ";

		std::cout << "\n";
	}
private:
	float m_potSize=Sb+Bb;
	Board m_board;
	std::vector<Hand> m_hands;
	std::unordered_set<int> m_removedCards;
};