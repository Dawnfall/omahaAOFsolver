#include "Node.h"
#include "Solver.h"

Node::Node(Position position, const std::string& action, std::vector<WeightedHand>& hands) :
	Pos(position),
	Hands(std::move(hands)),
	Action(action)
{
}

Node::Node(Position position, const std::string& action, const std::unordered_set<uint8_t>& removedCards) :
	Pos(position),
	Action(action)
{
	auto& randGen = Utils::GetRandomGen();
	std::uniform_real_distribution<float> dis(-1.0f, 1.0f);

	int i0, i1, i2, i3;
	for (i0 = 51; i0 >= 3; --i0)
	{
		if (removedCards.contains(i0))
			continue;
		for (i1 = i0 - 1; i1 >= 2; --i1)
		{
			if (removedCards.contains(i1))
				continue;
			for (i2 = i1 - 1; i2 >= 1; --i2)
			{
				if (removedCards.contains(i2))
					continue;
				for (i3 = i2 - 1; i3 >= 0; --i3)
				{
					if (removedCards.contains(i3))
						continue;
					Hands.emplace_back(Hand{ (uint8_t)i0, (uint8_t)i1,(uint8_t)i2, (uint8_t)i3 }, dis(randGen));
				}
			}
		}
	}
}

// returns a random weighted hand in range, removing used cards
const WeightedHand& Node::GetRandomWeightedHand(const std::unordered_set<uint8_t>& removedCards)const
{
	auto& randGen = Utils::GetRandomGen();
	std::uniform_int_distribution<> dis(0, (int)Hands.size() - 1);

	while (true)//TODO
	{
		int i = dis(randGen);
		if (!Utils::ContainsAny(Hands[i].H, removedCards))
			return Hands[i];
	}
	return Hands[0]; //TODO... never here
}


// returns a random hand based on range probabilites and removed cards
const Hand Node::ForcePickHand(const std::unordered_set<uint8_t>& removedCards)const
{
	while (true)
	{
		WeightedHand weHand = GetRandomWeightedHand(removedCards);
		if (weHand.DoPick())
			return weHand.H;
	}
	return Hand();
}
