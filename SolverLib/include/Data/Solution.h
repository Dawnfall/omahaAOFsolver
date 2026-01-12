#pragma once

#include "Core.h"
#include "Data/SolverParams.h"
#include "RandomGenerator.h"

struct Solution
{
	Solution() {}

	Solution(const SolverParams& solverParams, std::vector<uint8_t>& hands, std::vector<float> evs) :
		PlayerCount(solverParams.totalPlayers),
		BbSize(solverParams.bb),
		SbSize(solverParams.sb),
		Margin(solverParams.margin),
		Flop(solverParams.flop),
		Hands(std::move(hands)),
		Evs(std::move(evs))
	{
	}

	unsigned int PlayerCount = 0;
	float BbSize = 0;
	float SbSize = 0;
	float Margin = 0;
	std::array<uint8_t, 3> Flop{ 0,0,0 };
	std::vector<uint8_t> Hands;
	std::vector<float> Evs;

	std::string GetName()const
	{
		phevaluator::Card c0(Flop[0]);
		phevaluator::Card c1(Flop[1]);
		phevaluator::Card c2(Flop[2]);

		return c0.describeCard() + c1.describeCard() + c2.describeCard() + "_" + std::to_string(PlayerCount) + "p";
	}

	std::string GetRangeName(int rangeIndex)const;

	std::tuple<Hand, float> GetRandomHandAndEv(int rangeIndex, RandomGenerator& randGen)const;
	std::tuple<Hand, float> GetHandAndEv(int rangeIndex, int handIndex)const;

	static void Serialize(const std::string& filePath, const Solution& solution);
	static std::shared_ptr<Solution> Deserialize(const std::string& filePath);
};