#pragma once

#include "Core.h"
#include "Data/WeightedHand.h"
#include "Position.h"


class Node
{
public:
	Node(Position position,const std::string& action, std::vector<WeightedHand>& hands);
	Node(Position position,const std::string& action, const std::unordered_set<uint8_t>& removedCards);

	Position Pos;
	std::string Action;
	std::vector<WeightedHand> Hands;

	Node* FromNode = nullptr;
	std::shared_ptr<Node> AINode = nullptr;
	std::shared_ptr<Node> FoldNode = nullptr;

	const WeightedHand& GetRandomWeightedHand(const std::unordered_set<uint8_t>& removedCards)const;
	const Hand ForcePickHand(const std::unordered_set<uint8_t>& removedCards)const;
};

