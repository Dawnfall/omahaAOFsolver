#pragma once

#include "CardCode.h"
#include "Range.h"
#include "Position.h"

class Node
{
public:
	Node(Position position) :
		range(Range::FullRandom()),
		Position(position)
	{
	}

	Position Position;
	Node* FromNode = nullptr;
	std::shared_ptr<Node> AINode = nullptr;
	std::shared_ptr<Node> FoldNode = nullptr;
	Range range;

};