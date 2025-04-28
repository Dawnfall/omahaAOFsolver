#pragma once
#include "Core.h"
#include "Utils.h"

class WeightedHand
{
public:
	WeightedHand(const Hand& hand, float ev) :
		H(hand),
		Ev(ev)
	{
	}

	bool DoPick()const
	{
		return Ev > 0;
	}

	std::string ToString()
	{
		return HandToString(H) + "Ev: " + std::to_string(Ev) + "\n";
	}

	float Ev;
	Hand H;
};

