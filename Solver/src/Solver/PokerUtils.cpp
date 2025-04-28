#include "PokerUtils.h"

#include "Position.h"

float PokerUtils::CompareHands(const std::vector<Hand>& hands, const Board_F& flop, const std::array<uint8_t, 2>& turnAndRiver)
{
	if (hands.size() == 0 || hands.size() == 1)
		return -1;

	std::vector<Rank> ranks;
	ranks.reserve(hands.size());
	for (const auto& hand : hands)
	{
		ranks.emplace_back(evaluate_plo4_cards(flop[0], flop[1], flop[2], turnAndRiver[0], turnAndRiver[1], hand[0], hand[1], hand[2], hand[3]));
	}

	int equalCount = 1;
	for (int i = 1; i < ranks.size(); ++i)
	{
		if (ranks[i] > ranks[0])
			return 0.0f;
		if (ranks[i] == ranks[0])
			equalCount += 1;
	}
	return 1.0f / equalCount;
}
float PokerUtils::CompareHands(const Hand& hand1, const Hand& hand2, const Board_F& flop, const std::array<uint8_t, 2>& turnAndRiver)
{
	Rank r1 = evaluate_plo4_cards(flop[0], flop[1], flop[2], turnAndRiver[0], turnAndRiver[1], hand1[0], hand1[1], hand1[2], hand1[3]);
	Rank r2 = evaluate_plo4_cards(flop[0], flop[1], flop[2], turnAndRiver[0], turnAndRiver[1], hand2[0], hand2[1], hand2[2], hand2[3]);

	if (r1 > r2)
		return 1.0f;
	if (r1 == r2)
		return 0.5f;
	return 0.0f;
}
float PokerUtils::CompareHands(const Hand& hand1, const Hand& hand2, const Hand& hand3, const Board_F& flop, const std::array<uint8_t, 2>& turnAndRiver)
{
	Rank r1 = evaluate_plo4_cards(flop[0], flop[1], flop[2], turnAndRiver[0], turnAndRiver[1], hand1[0], hand1[1], hand1[2], hand1[3]);
	Rank r2 = evaluate_plo4_cards(flop[0], flop[1], flop[2], turnAndRiver[0], turnAndRiver[1], hand2[0], hand2[1], hand2[2], hand2[3]);
	Rank r3 = evaluate_plo4_cards(flop[0], flop[1], flop[2], turnAndRiver[0], turnAndRiver[1], hand3[0], hand3[1], hand3[2], hand3[3]);

	if (r2 > r1 || r3 > r1)
		return 0.0f;
	int splitCount = 1 + (r1 == r2) + (r1 == r3);
	return 1.0f / splitCount;
}
float PokerUtils::CompareHands(const Hand& hand1, const Hand& hand2, const Hand& hand3, const Hand& hand4, const Board_F& flop, const std::array<uint8_t, 2>& turnAndRiver)
{
	Rank r1 = evaluate_plo4_cards(flop[0], flop[1], flop[2], turnAndRiver[0], turnAndRiver[1], hand1[0], hand1[1], hand1[2], hand1[3]);
	Rank r2 = evaluate_plo4_cards(flop[0], flop[1], flop[2], turnAndRiver[0], turnAndRiver[1], hand2[0], hand2[1], hand2[2], hand2[3]);
	Rank r3 = evaluate_plo4_cards(flop[0], flop[1], flop[2], turnAndRiver[0], turnAndRiver[1], hand3[0], hand3[1], hand3[2], hand3[3]);
	Rank r4 = evaluate_plo4_cards(flop[0], flop[1], flop[2], turnAndRiver[0], turnAndRiver[1], hand4[0], hand4[1], hand4[2], hand4[3]);

	if (r2 > r1 || r3 > r1 || r4 > r1)
		return 0.0f;
	int splitCount = 1 + (r1 == r2) + (r1 == r3) + (r1 == r4);
	return 1.0f / splitCount;
}


