#include "Solver.h"
#include "Utils.h"
#include "ThreadPool.h"
#include "Node.h"
#include "Data/WeightedHand.h"
#include "Data/Solution.h"
#include "PokerUtils.h"


void Solver::SolveAsync(const SolverParams& solverParams, std::function<void(const SolverParams&, std::shared_ptr<Solution>)> callback)
{
	// Launch a detached thread to manage the worker threads
	std::thread([solverParams, callback]() {
		Nodes tree = Solver::BuildTree(solverParams);

		std::unordered_set<uint8_t> removedCards;
		removedCards.insert(solverParams.flop.begin(), solverParams.flop.end());

		// Increase the syncPoint count by solverParams.threadCount
		ThreadData threadData(solverParams.threadCount);
		std::vector<std::thread> threads;

		// Lambda function for thread task
		auto threadTask = [&threadData, &solverParams, &tree, &removedCards]()
			{
				for (unsigned int i = 0; i < solverParams.repeatIterCount; ++i)
				{
					for (auto& node : tree)
					{
						while (true)
						{
							if (threadData.handIndex >= node.second->Hands.size())
								break;

							unsigned int start = threadData.handIndex.fetch_add(solverParams.chunkSize);
							unsigned int end = std::min(start + solverParams.chunkSize, static_cast<unsigned int>(node.second->Hands.size()));
							Solver::ProcessRangeChunk(node.second.get(), start, end, solverParams, removedCards);
						}
						threadData.syncPoint.arrive_and_wait();
						threadData.handIndex = 0;
						threadData.syncPoint.arrive_and_wait();
					}
				}
			};

		// Launch worker threads
		for (unsigned int i = 1; i < solverParams.threadCount; ++i)
		{
			threads.emplace_back(threadTask);
		}
		threadTask();

		// Join worker threads
		for (auto& thread : threads)
		{
			thread.join();
		}

		// Prepare the result
		Ranges ranges;
		for (auto& nodePair : tree)
		{
			ranges[nodePair.first] = nodePair.second->Hands;
		}

		auto solution = std::make_shared<Solution>(solverParams, ranges);

		// Invoke the callback with the solution
		if (callback)
		{
			callback(solverParams, solution);
		}
		}).detach(); // Detach the thread to allow it to run independently
}


float Solver::CalculateEquity(const std::vector<Hand>& hands, const Board_F& flop, Board_TR& turnAndRiver, const std::unordered_set<uint8_t> removedCards)
{
	if (hands.size() == 1)
		return 1.0f;

	FillTurnAndRiver(turnAndRiver, removedCards);
	return PokerUtils::CompareHands(hands, flop, turnAndRiver);
}
void Solver::FillTurnAndRiver(Board_TR& turnAndRiver, const std::unordered_set<uint8_t>& removedCards)
{
	std::uniform_int_distribution dis(0, 51);
	auto& randGen = Utils::GetRandomGen();
	while (true)
	{
		turnAndRiver[0] = dis(randGen);
		if (!removedCards.contains(turnAndRiver[0]))
			break;
	}
	while (true)
	{
		turnAndRiver[1] = dis(randGen);
		if (turnAndRiver[0] != turnAndRiver[1] && !removedCards.contains(turnAndRiver[1]))
			break;
	}
}

Nodes Solver::BuildTree(const SolverParams& solParams)
{
	Nodes nodes;

	std::unordered_set<uint8_t> removedCards;
	removedCards.insert(solParams.flop.begin(), solParams.flop.end());

	Position startPos = (Position)(4 - solParams.totalPlayers);
	std::shared_ptr<Node> node = CreateNode(nullptr, startPos, "", removedCards, solParams, nodes);

	return nodes;
}
std::shared_ptr<Node> Solver::CreateNode(Node* fromNode, Position pos, const std::string& action, const std::unordered_set<uint8_t>& removedCards, const SolverParams& solParams, Nodes& nodes)
{
	if (pos == Position::BB && action.size() == solParams.totalPlayers - 1 &&
		(
			(solParams.totalPlayers == 2 && action == "F") ||
			(solParams.totalPlayers == 3 && action == "FF") ||
			(solParams.totalPlayers == 4 && action == "FFF"))
		)
	{
		return nullptr;
	}

	std::shared_ptr<Node> node = std::make_shared<Node>(pos, action, removedCards);
	nodes[action] = node;
	node->FromNode = fromNode;
	if (pos != Position::BB)
	{
		node->AINode = CreateNode(node.get(), pos + 1, action + "A", removedCards, solParams, nodes);
		node->FoldNode = CreateNode(node.get(), pos + 1, action + "F", removedCards, solParams, nodes);
	}
	return node;
}

void Solver::ProcessRangeChunk(Node* node, int start, int end, const SolverParams& solParams, const std::unordered_set<uint8_t>& removedCards)
{
	float potSize = PokerUtils::CalcPotAfterAI(node->Pos, 1.5f, solParams.sb, solParams.bb, solParams.stackSize);
	Board_TR turnAndRiver;

	for (int i = start; i < end; ++i)
	{
		WeightedHand& weHand = node->Hands[i];
		std::vector<Hand> hands{ weHand.H };
		std::unordered_set<uint8_t> removedCardsCopy = removedCards;
		removedCardsCopy.insert(weHand.H.begin(), weHand.H.end());

		weHand.Ev = CalcBbEvForAiHand(*node, solParams, removedCardsCopy, hands, turnAndRiver, potSize) - PokerUtils::GetFoldLoss(node->Pos, solParams);
	}
}
float Solver::CalcBbEvForAiHand(const Node& node, const SolverParams& solParams, const std::unordered_set<uint8_t>& removedCards, const std::vector<Hand>& hands, Board_TR& turnAndRiver, float potSize)
{
	float bbEv = 0.0f;
	unsigned int i = 0;
	while (i++ < solParams.perHandIterCount)
	{
		std::unordered_set<uint8_t> removed = removedCards;
		std::vector<Hand> currHands = hands;
		float currPotSize = potSize;

		const Node* nextNode = &node;
		const Node* prevNode = node.FromNode;
		while (prevNode != nullptr)
		{
			if (nextNode == prevNode->AINode.get())
			{
				Hand hand = prevNode->ForcePickHand(removed);
				currHands.emplace_back(hand);
				removed.insert(hand.begin(), hand.end());
				currPotSize = PokerUtils::CalcPotAfterAI(prevNode->Pos, currPotSize, solParams.sb, solParams.bb, solParams.stackSize);
			}
			nextNode = prevNode;
			prevNode = prevNode->FromNode;
		}

		nextNode = node.AINode.get();
		while (nextNode != nullptr)
		{
			const WeightedHand& weHand = nextNode->GetRandomWeightedHand(removed);
			if (weHand.DoPick())
			{
				currHands.emplace_back(weHand.H);
				removed.insert(weHand.H.begin(), weHand.H.end());
				currPotSize = PokerUtils::CalcPotAfterAI(nextNode->Pos, currPotSize, solParams.sb, solParams.bb, solParams.stackSize);
				if (nextNode->AINode)
					nextNode = nextNode->AINode.get();
				else
					nextNode = nullptr;
			}
			else
				nextNode = nextNode->FoldNode.get();
		}

		float ev = Solver::CalculateEquity(currHands, solParams.flop, turnAndRiver, removed);
		float bbProfit = PokerUtils::CalcAIProfit(ev, currPotSize, solParams);
		bbEv += bbProfit;
	}
	return bbEv / solParams.perHandIterCount;
}





