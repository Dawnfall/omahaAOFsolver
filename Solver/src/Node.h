#pragma once

#include "Core.h"
#include "CardCode.h"
#include "Range.h"
#include "Position.h"
#include "Solver.h"

class Node
{
public:
	Node(Position position, const std::vector<Card>& removedCards) :
		range(Range::WithRemoved(removedCards)),
		Position(position)
	{
	}

	Position Position;
	Node* FromNode = nullptr;
	std::shared_ptr<Node> AINode = nullptr;
	std::shared_ptr<Node> FoldNode = nullptr;
	Range range;

	void BalanceNode(const Situation& situation)
	{
		int numThreads = std::thread::hardware_concurrency();
		std::vector<std::thread> threads;
		int chunkSize = static_cast<int>(range.Hands.size()) / numThreads;

		auto worker = [this, &situation](int start, int end) {
			for (int i = start; i < end; ++i)
			{
				WeightedHand& weHand = range.Hands[i];
				Situation sit = situation;
				sit.AddHand(weHand.Hand, Position);
				float newBbEv = CalcBbEvForAiHand(sit);
				weHand.AIProb = newBbEv;
			}
			};

		for (int i = 0; i < numThreads; ++i)
		{
			int start = i * chunkSize;
			int end = (i == numThreads - 1) ? static_cast<int>(range.Hands.size()) : (i + 1) * chunkSize;
			threads.emplace_back(worker, start, end);
		}

		for (auto& thread : threads)
		{
			thread.join();
		}
	}

	float CalcBbEvForAiHand(const Situation& situation)
	{
		float bbEv = 0.0f;
		int iterCount = 1;
		while (iterCount-- > 0)
		{
			Situation sit = situation;
			Node* nextNode = this;
			Node* prevNode = FromNode;
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

			nextNode = AINode.get();
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
			float ev = Solver::CalculateEquity(sit, Solver::errorThreshold, 1);
			float bbProfit = sit.GetProfit(ev);

			bbEv += bbProfit;
		}
		return bbEv / iterCount;
	}

};