#pragma once
#include "Core.h"

struct SolverParams
{
	unsigned int totalPlayers;
	Board_F flop;
	float sb;
	float bb;
	float stackSize = 5.0f;
	float margin = 0.0f;

	unsigned int threadCount = 1;
	unsigned int chunkSize = 100;
	unsigned int perHandIterCount = 100;
	unsigned int repeatIterCount = 1;
	const char* folderPath;
};