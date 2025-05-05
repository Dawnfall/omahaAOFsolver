#pragma once
#include "Core.h"
#include "Solution.h"

struct SolverResult
{
	bool IsSuccess = false;
	double Duration = 0;
	std::shared_ptr<Solution> Solution = nullptr;
};