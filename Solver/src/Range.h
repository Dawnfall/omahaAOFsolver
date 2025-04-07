#pragma once
#include "Core.h"
#include "Situation.h"

class WeightedHand
{
public:
	WeightedHand(const Hand& h, float prob) :
		Hand(h),
		AIProb(prob)
	{
	}
	Hand Hand;
	float AIProb;
};

class Range
{
public:
	std::vector<WeightedHand> Hands;

	const WeightedHand& GetRandomWeightedHand(Situation& sit)const;
	const Hand* GetRandomHand(Situation& sit)const;

	static Range WithRemoved(const std::vector<Card>& removedCards);
	static Range FullRange();
	static Range FullRandom();
	static Range FromHands(const std::vector<Hand>& hands);
private:
	Range() {}
};