#include "Range.h"
#include "Utils.h"
#include "Situation.h"
#include "Solver.h"

Range Range::WithRemoved(const std::unordered_set<int>& removedCards)
{
	Range r;
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
					r.Hands.emplace_back(Hand{i0, i1, i2, i3}, 1.0f);
				}
			}
		}
	}

	return r;
}

// returns a random weighted hand in range, removing used cards
const WeightedHand& Range::GetRandomWeightedHand(const std::unordered_set<int>& removedCards)const
{
	auto& randGen = Utils::GetRandomGen();
	std::uniform_int_distribution<> dis(0, Hands.size() - 1);

	while (true)//TODO
	{
		int i = dis(randGen);
		if (Solver::ValidateHand(Hands[i].GetHand(), removedCards))
			return Hands[i];
	}
	return Hands[0]; //TODO... never here
}

// returns a random hand based on range probabilites and removed cards
const Hand Range::ForcePickHand(const std::unordered_set<int>& removedCards)const
{
	while (true)
	{
		WeightedHand weHand = GetRandomWeightedHand(removedCards);
		if (weHand.DoPick())
			return weHand.GetHand();
	}
	return Hand();
}

void Range::PrintRandom(int count)
{
	while (count-- > 0)
	{
		auto& randGen = Utils::GetRandomGen();
		std::uniform_int_distribution<> dis(0, Hands.size() - 1);
		int randomI = dis(randGen);
		std::cout << Hands[randomI].ToString();
	}
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
					r.Hands.emplace_back(Hand{i0, i1, i2, i3}, 1.0f);
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
					r.Hands.emplace_back(Hand{i0, i1, i2, i3}, dis(randGen));
				}
	return r;
}