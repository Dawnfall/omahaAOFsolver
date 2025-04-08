#include "Solver.h"
#include "Deck.h"
#include "Utils.h"
#include <cassert>

bool Solver::ValidateHand(const Hand& hand, const std::unordered_set<int>& removedCards)
{
	for (auto& card : hand)
	{
		if (removedCards.contains(card))
			return false;

		//for (const auto& h : sit.GetHands())
		//	if (std::find(h.begin(), h.end(), card) != h.end())
		//		return false;
		////if (std::find(sit.GetBoard().begin(), sit.GetBoard().end(), card) != sit.GetBoard().end())
		////	return false;
	}
	return true;
}

float Solver::CompareHands(const std::vector<Hand>& hands, Board board)
{
	if (hands.size() == 0 || hands.size() == 1)
		return -1;

	std::vector<Rank> ranks;
	ranks.reserve(hands.size());
	for (const auto& hand : hands)
	{
		ranks.emplace_back(evaluate_plo4_cards(board[0], board[1], board[2], board[3], board[4], hand[0], hand[1], hand[2], hand[3]));
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

float Solver::CalculateEquity(const std::vector<Hand>& hands, Board& board, const std::unordered_set<int> removedCards)
{
	if (hands.size() == 0)
		throw std::exception("At least two hands needed for comparison!");
	if (hands.size() == 1)
		return 1.0f;

	Deck deck(removedCards);
	deck.FillTurnAndRiver(board);
	return Solver::CompareHands(hands, board);
}

Solution Solver::Solve(int totalPlayers, float bb, float sb, const std::array<Card, 3>& flop)
{
	Solution solution(totalPlayers, bb, sb, flop);

	for (int i = 0; i < 5; i++)
	{
		std::vector<std::shared_ptr<Node>> nodesAsVec = Utils::CopyMapValuesToVector(solution.Nodes);
		std::shuffle(nodesAsVec.begin(), nodesAsVec.end(), Utils::GetRandomGen());

		for (auto& node : nodesAsVec)
		{
			std::vector<std::thread> threads;
			int chunkSize = std::ceil(node->range.Hands.size() / static_cast<float>(Utils::NumberOfThreads));

			auto worker = [&node, &solution](int first, int last) {
				float potSize = CalcPotAfterAI(node->Position, 1.5f);
				Board board = { solution.Flop[0],solution.Flop[1],solution.Flop[2],-1,-1 };
				for (int i = first; i <= last; ++i)
				{
					WeightedHand& weHand = node->range.Hands[i];
					std::vector<Hand> hands{ weHand.GetHand() };
					std::unordered_set<int> removedCards = solution.RemovedCards;
					removedCards.insert(weHand.GetHand().begin(), weHand.GetHand().end());
					float newBbEv = CalcBbEvForAiHand(*node, removedCards, hands, board, potSize);

					float foldLoss = GetFoldLoss(node->Position);
					weHand.SetAiProb((newBbEv > foldLoss) ? 1 : 0);

					//std::cout << Utils::ToString(hands[0]);
					//std::cout << "\n" << newBbEv << "\n";
				}
				};

			for (int i = 0; i < Utils::NumberOfThreads; ++i)
			{
				int first = i * chunkSize;
				int last = first + chunkSize - 1;
				last = (last >= node->range.Hands.size()) ? node->range.Hands.size() - 1 : last;
				threads.emplace_back(worker, first, last);
			}

			for (auto& thread : threads)
			{
				thread.join();
			}
		}
	}
	return solution;
}


float Solver::CalcBbEvForAiHand(const Node& node, const std::unordered_set<int>& removedCards, const std::vector<Hand>& hands, Board& board, float potSize)
{
	float bbEv = 0.0f;

	int i = 0;
	while (i < Utils::PerNodeIterCount)
	{
		++i;
		std::unordered_set<int> removed = removedCards;
		std::vector<Hand> currHands = hands;
		float currPotSize = potSize;

		const Node* nextNode = &node;
		const Node* prevNode = node.FromNode;
		while (prevNode != nullptr)
		{
			if (nextNode == prevNode->AINode.get())
			{
				Deck d(removed);
				//Hand hand = d.DealFourCards();
				Hand hand = prevNode->range.ForcePickHand(removed);
				currHands.emplace_back(hand);
				removed.insert(hand.begin(), hand.end());
				currPotSize = Solver::CalcPotAfterAI(prevNode->Position, currPotSize);
			}
			nextNode = prevNode;
			prevNode = prevNode->FromNode;
		}

		nextNode = node.AINode.get();
		while (nextNode != nullptr)
		{
			const WeightedHand& weHand = nextNode->range.GetRandomWeightedHand(removedCards);
			if (weHand.DoPick())
			{
				currHands.emplace_back(weHand.GetHand());
				removed.insert(weHand.GetHand().begin(), weHand.GetHand().end());
				currPotSize = CalcPotAfterAI(nextNode->Position, currPotSize);
				if (nextNode->AINode)
					nextNode = nextNode->AINode.get();
				else
					nextNode = nullptr;
			}
			else
				nextNode = nextNode->FoldNode.get();
		}

		float ev = Solver::CalculateEquity(currHands, board, removed);
		float bbProfit = CalcAIProfit(ev, currPotSize);
		bbEv += bbProfit;
	}
	return bbEv / Utils::PerNodeIterCount;
}

float Solver::CalcPotAfterAI(Position pos, float pot)
{
	switch (pos)
	{
	case Position::SB:
		pot += AISize - Sb;
		break;
	case Position::BB:
		pot += AISize - Bb;
		break;
	default:
		pot += AISize;
		break;
	}
	return pot;
}

float Solver::GetFoldLoss(Position pos)
{
	switch (pos)
	{
	case Position::SB:
		return -Sb;
	case Position::BB:
		return -Bb;
	default:
		return 0;
	}
}

float Solver::CalcAIProfit(float ev, float potSize)
{
	return ev * potSize - AISize;
}


