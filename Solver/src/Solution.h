#pragma once

#include "Core.h"
#include "Position.h"

class Node;

class Solution
{
public:
	Solution(unsigned int playerCount, float bbSize, float sbSize, const std::array<Card, 3>& flop) :
		PlayerCount(playerCount), BbSize(bbSize), SbSize(sbSize), Flop(flop)
	{
		RemovedCards.insert(Flop.begin(), Flop.end());
		BuildSolution();
	}

	std::shared_ptr<Node> GetNode(const std::string& action)
	{
		auto res = Nodes.find(action);
		if (res != Nodes.end())
			return res->second;
		return nullptr;
	}

	unsigned int PlayerCount;
	float BbSize;
	float SbSize;
	std::unordered_map<std::string, std::shared_ptr<Node>> Nodes;
	std::unordered_set<int> RemovedCards;
	std::array<Card, 3> Flop;

private:
	void BuildSolution();
	std::shared_ptr<Node> CreateNode(Node* fromNode, Position pos, const std::string& action);
};