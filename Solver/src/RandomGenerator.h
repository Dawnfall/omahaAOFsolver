#pragma once
#include "Core.h"
#include "Solver/PokerUtils.h"

class RandomGenerator
{
public:
	RandomGenerator():
		randGen(std::random_device{}()),
		rangeDist(0, PokerUtils::rangeSize - 1),
		deckDist(0, 51),
		evRandDist(-5.0f, 5.0f)
	{}

	uint8_t GetRandomCardFromDeck()
	{
		return deckDist(randGen);
	}
	float GetRandomEv()
	{
		return static_cast<float>(evRandDist(randGen));
	}
	unsigned int GetRandomHandFromRange()
	{
		return rangeDist(randGen);
	}

private:
	std::minstd_rand randGen;
	std::uniform_int_distribution<> rangeDist;
	std::uniform_int_distribution<> deckDist;
	std::uniform_real_distribution<> evRandDist;;





};