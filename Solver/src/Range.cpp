#include "Range.h"
#include "Utils.h"
#include "Situation.h"

Range Range::WithRemoved(const std::vector<Card>& removedCards)
{
	Range r;
	int i0, i1, i2, i3;
	for (i0 = 51; i0 >= 3; --i0)
	{
		if (std::find(removedCards.begin(), removedCards.end(), i0) != removedCards.end())
			continue;
		for (i1 = i0 - 1; i1 >= 2; --i1)
		{
			if (std::find(removedCards.begin(), removedCards.end(), i1) != removedCards.end())
				continue;
			for (i2 = i1 - 1; i2 >= 1; --i2)
			{
				if (std::find(removedCards.begin(), removedCards.end(), i2) != removedCards.end())
					continue;
				for (i3 = i2 - 1; i3 >= 0; --i3)
				{
					if (std::find(removedCards.begin(), removedCards.end(), i3) != removedCards.end())
						continue;
					r.Hands.emplace_back(std::array<Card, 4>{Card(i0), Card(i1), Card(i2), Card(i3)}, 1.0f);
				}
			}
		}
	}

	return r;
}

const WeightedHand& Range::GetRandomWeightedHand(Situation& sit)const
{
	auto& randGen = Utils::GetRandomGen();
	std::uniform_int_distribution<> dis(0, Hands.size() - 1);

	while (true)//TODO
	{
		int i = dis(randGen);
		if (Utils::Validate(Hands[i].Hand, sit))
			return Hands[i];
	}
	return Hands[0]; //TODO... never here

}

const Hand* Range::GetRandomHand(Situation& sit)const
{
	WeightedHand weHand = GetRandomWeightedHand(sit);

	auto& randGen = Utils::GetRandomGen();
	std::uniform_real_distribution<> dis(0.0f, 1.0f);

	float prob = dis(randGen);
	if (prob < weHand.AIProb)
		return &weHand.Hand;
	return nullptr;
}


Range Range::FullRange()
{
	Range r;
	int i0, i1, i2, i3;
	for (i0 = 51; i0 >= 3; --i0)
		for (i1 = i0 - 1; i1 >= 2; --i1)
			for (i2 = i1 - 1; i2 >= 1; --i2)
				for (i3 = i2 - 1; i3 >= 0; --i3)
				{
					r.Hands.emplace_back(std::array<Card, 4>{Card(i0), Card(i1), Card(i2), Card(i3)}, 1.0f);
				}

	return r;
}

Range Range::FullRandom()
{

	auto& randGen = Utils::GetRandomGen();
	std::uniform_real_distribution<float> dis(0.0f, 1.0f);

	Range r;
	int i0, i1, i2, i3;
	for (i0 = 51; i0 >= 3; --i0)
		for (i1 = i0 - 1; i1 >= 2; --i1)
			for (i2 = i1 - 1; i2 >= 1; --i2)
				for (i3 = i2 - 1; i3 >= 0; --i3)
				{
					r.Hands.emplace_back(std::array<Card, 4>{Card(i0), Card(i1), Card(i2), Card(i3)}, dis(randGen));
				}
	return r;
}