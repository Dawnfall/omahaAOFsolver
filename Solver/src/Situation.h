#pragma once
#include "Core.h"
#include "Position.h"

const float AISize = 5.0f;

class Situation
{
public:
	Situation(float bb = 1.0f, float sb = 0.5f) :
		Bb(bb), Sb(sb), m_potSize(bb + sb)
	{
	}
	void Print()
	{
		if (m_board.size() > 0)
			PrintBoard();
		if (m_deadCards.size() > 0)
			PrintDeadCards();
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
			std::cout << toString(m_playerPositions[i]) << ":\n";
			for (const Card& card : m_hands[i])
				std::cout << card.describeCard() << " ";
			std::cout << "\n";
		}
	}
	void PrintDeadCards()
	{
		std::cout << "Dead cards: ";
		for (const Card& c : m_deadCards)
			std::cout << c.describeCard() << " ";

		std::cout << "\n";
	}

	void SetBoard(const Board& board) { m_board = board; }
	void AddHand(const Hand& hand, Position playerPosition)
	{
		m_hands.emplace_back(hand);
		m_playerPositions.emplace_back(playerPosition);

		if (playerPosition == Position::SB)
			m_potSize += AISize - Sb;
		else if (playerPosition == Position::BB)
			m_potSize += AISize - Bb;
		else
			m_potSize += AISize;
	}
	void SetDeadCards(const std::vector<Card>& deadCards) { m_deadCards = deadCards; }
	void ClearToPlayer()
	{
		if (m_hands.size() > 1)
		{
			m_hands.erase(m_hands.begin() + 1, m_hands.end());
			m_playerPositions.erase(m_playerPositions.begin() + 1, m_playerPositions.end());
		}
	}
	Board& GetBoard() { return m_board; }
	const std::vector<Card>& GetDeadCards() const { return m_deadCards; }
	const std::vector<Hand>& GetHands()const { return m_hands; }

	float GetPotSize() { return m_potSize; }
	float GetProfit(float ev)
	{
		return ev * GetPotSize() - AISize;
	}
	float GetFoldBBEv()
	{
		switch (m_playerPositions[0])
		{
		case Position::SB:
			return -Sb;
		case Position::BB:
			return -Bb;
		default:
			return 0.0f;
		}
	}

	float Bb = 1.0f;
	float Sb = 0.5f;
private:
	float m_potSize;
	Board m_board;
	std::vector<Card> m_deadCards;
	std::vector<Hand> m_hands;
	std::vector<Position> m_playerPositions;
};