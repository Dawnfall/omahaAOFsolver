#pragma once

#include "Core.h"
#include "Node.h"

#include "Data/WeightedHand.h"
#include "Data/SolverParams.h"
#include "Data/ThreadData.h"
#include "Data/Solution.h"

using Nodes = std::unordered_map<std::string, std::shared_ptr<Node>>;
using Ranges = std::unordered_map<std::string, std::vector<WeightedHand>>;

namespace Solver
{
	void SolveAsync(const SolverParams& solverParams, std::function<void(const SolverParams&, std::shared_ptr<Solution>)> callback);
	void FillTurnAndRiver(Board_TR& turnAndRiver, const std::unordered_set<uint8_t>& removedCards);
	float CalculateEquity(const std::vector<Hand>& hands, const Board_F& flop, Board_TR& turnAndRiver, const std::unordered_set<uint8_t> removedCards);
	float CalcBbEvForAiHand(const Node& node, const SolverParams& solParams, const std::unordered_set<uint8_t>& removedCards, const std::vector<Hand>& hands, Board_TR& turnAndRiver, float potSize);
	void ProcessRangeChunk(Node* node, int start, int end, const SolverParams& solParams, const std::unordered_set<uint8_t>& removedCards);
	Nodes BuildTree(const SolverParams& params);
	std::shared_ptr<Node> CreateNode(Node* fromNode, Position pos, const std::string& action, const std::unordered_set<uint8_t>& removedCards, const SolverParams& solParams, Nodes& nodes);
};

//void ThreadTask(ThreadData& data, const SolverParams& solParams, Nodes& tree);
//void Solver::ThreadTask(ThreadData& data, const SolverParams& solParams, Solution& sol)
//{
//	for (int i = 0; i < solParams.repeatIterations; ++i)
//	{
//		for (auto& node : sol.Nodes)
//		{
//			while (true)
//			{
//				if (data.handIndex >= node.second->range.Hands.size())
//					break;
//
//				int start = data.handIndex.fetch_add(solParams.chunkSize);
//				int end = std::min(start + solParams.chunkSize, static_cast<int>(node.second->range.Hands.size()));
//				ProcessRangeChunk(node.second.get(), start, end, &sol, solParams);
//
//				data.syncPoint.arrive_and_wait();
//			}
//		}
//	}
//}