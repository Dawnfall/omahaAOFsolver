#pragma once

#include "Core.h"
#include "Position.h"
#include "Data/SolverParams.h"
#include "Data/WeightedHand.h"

using Ranges = std::unordered_map<std::string, std::vector<WeightedHand>>;

class Solution
{
public:
	unsigned int PlayerCount;
	float BbSize;
	float SbSize;
	float Margin;
	std::array<uint8_t, 3> Flop;
	unsigned int Iters;
	unsigned int PerHandIters;
	std::unordered_map<std::string, std::vector<WeightedHand>> AllRanges;

	Solution() {}

	Solution(const SolverParams& solParams, Ranges& ranges) :
		PlayerCount(solParams.totalPlayers),
		BbSize(solParams.bb),
		SbSize(solParams.sb),
		Margin(solParams.margin),
		Flop(solParams.flop),
		Iters(solParams.repeatIterCount),
		PerHandIters(solParams.perHandIterCount),
		AllRanges(std::move(ranges))
	{
	}

	std::vector<WeightedHand>* GetRange(const std::string& action)
	{
		auto res = AllRanges.find(action);
		if (res != AllRanges.end())
			return &(res->second);
		return nullptr;
	}

	std::string GetName()const
	{
		phevaluator::Card c0(Flop[0]);
		phevaluator::Card c1(Flop[1]);
		phevaluator::Card c2(Flop[2]);

		return c0.describeCard() + c1.describeCard() + c2.describeCard() + "_" + std::to_string(PlayerCount) + "p";
	}

	std::string GetRangeName(const std::string& action)const
	{
		if (PlayerCount == 2)
		{
			if (action == "")
				return "SB";
			if (action == "A")
				return "BB vs SB";
		}
		if (PlayerCount == 3)
		{
			if (action == "")
				return "BU";
			if (action == "A")
				return "SB vs BU";
			if (action == "AA")
				return "BB vs BU+SB";
			if (action == "AF")
				return "BB vs BU";
			if (action == "F")
				return "SB";
			if (action == "FA")
				return "BB vs SB";
		}
		if (PlayerCount == 4)
		{
			if (action == "")
				return "CO";
			if (action == "A")
				return "BU vs CO";
			if (action == "AA")
				return "SB vs CO+BU";
			if (action == "AAA")
				return "BB vs CO+BU+SB";
			if (action == "AAF")
				return "BB vs CO+BU";
			if (action == "AF")
				return "SB vs CO";
			if (action == "AFA")
				return "BB vs CO+SB";
			if (action == "AFF")
				return "BB vs CO";
			if (action == "F")
				return "BU";
			if (action == "FA")
				return "SB vs BU";
			if (action == "FAA")
				return "BB vs BU+SB";
			if (action == "FAF")
				return "BB vs BU";
			if (action == "FF")
				return "SB";
			if (action == "FFA")
				return "BB vs SB";
		}
		return "Invalid name";
	}

	static void Serialize(const std::string& filePath, const Solution& solution);
	static std::shared_ptr<Solution> Deserialize(const std::string& filePath);

};