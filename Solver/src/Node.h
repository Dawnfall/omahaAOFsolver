#pragma once

#include "Core.h"
#include "CardCode.h"
#include "Range.h"
#include "Position.h"
#include "Solver.h"

class Node
{
public:
	Node(Position position, const std::unordered_set<int>& removedCards) :
		range(Range::WithRemoved(removedCards)),
		Position(position)
	{
	}

	Position Position;
	Node* FromNode = nullptr;
	std::shared_ptr<Node> AINode = nullptr;
	std::shared_ptr<Node> FoldNode = nullptr;
	Range range;

};