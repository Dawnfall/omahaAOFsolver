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

	}

	std::barrier<> syncPoint;
	std::atomic<int> handIndex;
	int totalRanges;


	//TODO: could be const expr

};