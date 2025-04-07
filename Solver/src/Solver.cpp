#include "Solver.h"
#include "Deck.h"

float Solver::CompareHands(const std::vector<Hand>& hands, Board board)
{
	if (hands.size() == 0 || hands.size() == 1)
		return -1;

	std::vector<Rank> ranks;
	ranks.reserve(hands.size());
	for (const auto& hand : hands)
	{
		Rank r = evaluate_plo4_cards(board[0], board[1], board[2], board[3], board[4], hand[0], hand[1], hand[2], hand[3]);
		ranks.emplace_back(r);
	}

	int equalCount = 1;
	for (int i = 1; i < ranks.size(); ++i)
	{
		if (ranks[i] > ranks[0])
			return 0.0f;
		if (ranks[i] == ranks[0])
			equalCount += 1;
	}
	return 1.0f / equalCount;
}
float Solver::CompareHands(const Hand& hand1, const Hand& hand2, Board& board)
{
	Rank r1 = evaluate_plo4_cards(board[0], board[1], board[2], board[3], board[4], hand1[0], hand1[1], hand1[2], hand1[3]);
	Rank r2 = evaluate_plo4_cards(board[0], board[1], board[2], board[3], board[4], hand2[0], hand2[1], hand2[2], hand2[3]);

	if (r1 > r2)
		return 1.0f;
	if (r1 == r2)
		return 0.5f;
	return 0.0f;
}
float Solver::CompareHands(const Hand& hand1, const Hand& hand2, const Hand& hand3, Board& board)
{
	Rank r1 = evaluate_plo4_cards(board[0], board[1], board[2], board[3], board[4], hand1[0], hand1[1], hand1[2], hand1[3]);
	Rank r2 = evaluate_plo4_cards(board[0], board[1], board[2], board[3], board[4], hand2[0], hand2[1], hand2[2], hand2[3]);
	Rank r3 = evaluate_plo4_cards(board[0], board[1], board[2], board[3], board[4], hand3[0], hand3[1], hand3[2], hand3[3]);

	if (r2 > r1 || r3 > r1)
		return 0.0f;
	int splitCount = 1 + (r1 == r2) + (r1 == r3);
	return 1.0f / splitCount;
}
float Solver::CompareHands(const Hand& hand1, const Hand& hand2, const Hand& hand3, const Hand& hand4, Board& board)
{
	Rank r1 = evaluate_plo4_cards(board[0], board[1], board[2], board[3], board[4], hand1[0], hand1[1], hand1[2], hand1[3]);
	Rank r2 = evaluate_plo4_cards(board[0], board[1], board[2], board[3], board[4], hand2[0], hand2[1], hand2[2], hand2[3]);
	Rank r3 = evaluate_plo4_cards(board[0], board[1], board[2], board[3], board[4], hand3[0], hand3[1], hand3[2], hand3[3]);
	Rank r4 = evaluate_plo4_cards(board[0], board[1], board[2], board[3], board[4], hand4[0], hand4[1], hand4[2], hand4[3]);

	if (r2 > r1 || r3 > r1 || r4 > r1)
		return 0.0f;
	int splitCount = 1 + (r1 == r2) + (r1 == r3) + (r1 == r4);
	return 1.0f / splitCount;
}

float Solver::CalculateEquity(Situation& sit, float errorThreshold, int maxIter)
{
	if (sit.GetHands().size() == 0)
		return -1;
	if (sit.GetHands().size() == 1)
		return 1.0f;

	Deck deck;

	for (auto& hand : sit.GetHands())
		for (auto& card : hand)
			deck.Remove(card);
	for (auto& card : sit.GetBoard())
		deck.Remove(card);

	int boardSize = sit.GetBoard().size();
	float totalWin = 0.0f;
	float totalWinSquared = 0.0f;
	int iter = 1;
	for (; iter < maxIter; iter++)
	{
		for (int i = boardSize; i < 5; ++i)
			sit.GetBoard().emplace_back(deck.getRandom());
		float win = CompareHands(sit.GetHands(), sit.GetBoard());
		totalWin += win;
		totalWinSquared += win * win;
		for (int i = boardSize; i < 5; ++i)
			deck.add(sit.GetBoard()[i]);
		sit.GetBoard().erase(sit.GetBoard().begin() + boardSize, sit.GetBoard().end());

		// Calculate mean and standard error
		float mean = totalWin / iter;
		float variance = (totalWinSquared / iter) - (mean * mean);
		float standardError = sqrt(variance / iter);

		// Check if the standard error is below the threshold
		if (standardError < errorThreshold)
			break;
	}

	return totalWin / iter;
}




