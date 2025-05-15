#pragma once

#include "Core.h"
#include "SolverParams.h"
#include "Utils.h"
#include "Solver/PokerUtils.h"
struct SolverData
{
	SolverData(const SolverParams& solParams) :
		handIndex(0), syncPoint(solParams.threadCount), totalRanges(PokerUtils::GetTotalRanges(solParams.totalPlayers))
	{
		PokerUtils::GetIterData(solParams.totalPlayers, repeatIters, minHandIters, maxHandIters,is99Conf);
	}

	std::barrier<> syncPoint;
	std::atomic<int> handIndex;
	unsigned int totalRanges;

	unsigned int repeatIters;
	std::vector<unsigned int> minHandIters;
	std::vector<unsigned int> maxHandIters;
	std::vector<bool> is99Conf;
};