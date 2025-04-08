#include "Solution.h"
#include "Node.h"

void Solution::BuildSolution()
{
	Position startPos = (Position)(4 - PlayerCount);
	std::shared_ptr<Node> node = CreateNode(nullptr, startPos, "");
}

std::shared_ptr<Node> Solution::CreateNode(Node* fromNode, Position pos, const std::string& action)
{
	if (pos == Position::BB && action.size() == PlayerCount - 1 &&
		((PlayerCount == 2 && action == "F") || (PlayerCount == 3 && action == "FF") || (PlayerCount == 4 && action == "FFF")))
	{
		return nullptr;
	}

	std::shared_ptr<Node> node = std::make_shared<Node>(pos, RemovedCards);
	Nodes[action] = node;
	node->FromNode = fromNode;
	if (pos != Position::BB)
	{
		node->AINode = CreateNode(node.get(), pos + 1, action + "A");
		node->FoldNode = CreateNode(node.get(), pos + 1, action + "F");
	}
	return node;
}
