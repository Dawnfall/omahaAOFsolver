#pragma once
#include "Core.h"
//#include "Solver/PokerUtils.h"

struct SolverParams
{
	unsigned int totalPlayers = 0;
	Board_F flop = { 0,0,0 };
	float sb = 0.0f;
	float bb = 0.0f;
	float stackSize = 5.0f;
	float margin = 0.0f;

	unsigned int threadCount = 1;
	unsigned int chunkSize = 100;
	char folder[256];
};