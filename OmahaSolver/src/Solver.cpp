#include "Solver.h"

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
float Solver::CalculateEquity(Situation& sit, float iterCount)
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
	for (int iter = 0; iter < iterCount; iter++)
	{
		for (int i = boardSize; i < 5; ++i)
			sit.GetBoard().emplace_back(deck.getRandom());
		totalWin += CompareHands(sit.GetHands(), sit.GetBoard());
		for (int i = boardSize; i < 5; ++i)
			deck.add(sit.GetBoard()[i]);
		sit.GetBoard().erase(sit.GetBoard().begin() + boardSize, sit.GetBoard().end());
	}
	return totalWin / iterCount;
}

float Solver::CalcBbEvForAiHand(std::shared_ptr<Node> node, const Situation& situation)
{
	float bbEv = 0.0f;
	int iterCount = 1;
	while (iterCount-- > 0)
	{
		Situation sit = situation;
		Node* nextNode = node.get();
		Node* prevNode = node->FromNode;
		while (prevNode != nullptr)
		{
			if (nextNode == prevNode->AINode.get())
			{
				const Hand* hand = nullptr;
				while (!hand)
				{
					hand = prevNode->range.GetRandomHand(sit);
				}
				sit.AddHand(*hand, prevNode->Position);
			}
			nextNode = prevNode;
			prevNode = prevNode->FromNode;
		}

		nextNode = node->AINode.get();
		while (nextNode != nullptr)
		{
			const Hand* randHand = nextNode->range.GetRandomHand(sit);
			if (randHand)
			{
				sit.AddHand(*randHand, nextNode->Position);
				if (nextNode->AINode)
					nextNode = nextNode->AINode.get();
				else
					nextNode = nullptr;
			}
			else
				nextNode = nextNode->FoldNode.get();
		}
		float ev = Solver::CalculateEquity(sit, 1);
		float bbProfit = sit.GetProfit(ev);

		bbEv += bbProfit;
	}
	return bbEv / iterCount;
}

void Solver::BalanceNode(std::shared_ptr<Node> node, const Situation& situation)
{
	int i = 0;
	for (WeightedHand& weHand : node->range.Hands)
	{
		//std::cout << i << std::endl;
		i += 1;
		Situation sit = situation;
		sit.AddHand(weHand.Hand, node->Position);
		float newBbEv = CalcBbEvForAiHand(node, sit);
		weHand.AIProb = newBbEv;
	}
}
