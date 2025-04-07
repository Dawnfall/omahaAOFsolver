#pragma once

#include "Core.h"
#include "Position.h"
#include "Node.h"

class Solution
{
public:
	Solution(unsigned int playerCount, const Situation& sit) :
		Sit(sit)
	{
		Position startPos = (Position)(4 - playerCount);
		std::shared_ptr<Node> node = CreateNode(nullptr, startPos, "");
	}

	std::shared_ptr<Node> GetNode(const std::string& action)
	{
		auto res = m_nodes.find(action);
		if (res != m_nodes.end())
			return res->second;
		return nullptr;
	}

	void Solve()
	{
		for (int i = 0; i < 2; i++)
		{
			std::vector<std::shared_ptr<Node>> nodesAsVec = Utils::CopyMapValuesToVector(m_nodes);
			std::shuffle(nodesAsVec.begin(), nodesAsVec.end(), Utils::GetRandomGen());

			for (auto& node : nodesAsVec)
			{
				node->BalanceNode(Sit);
			}
		}
	}

private:

	std::shared_ptr<Node> CreateNode(Node* fromNode, Position pos, const std::string& action)
	{
		std::shared_ptr<Node> node = std::make_shared<Node>(pos, Sit.GetBoard());
		m_nodes[action] = node;
		node->FromNode = fromNode;
		if (pos != Position::BB)
		{
			node->AINode = CreateNode(node.get(), pos + 1, action + "A");
			node->FoldNode = CreateNode(node.get(), pos + 1, action + "F");
		}
		return node;
	}

	std::unordered_map<std::string, std::shared_ptr<Node>> m_nodes;
	Situation Sit;
};