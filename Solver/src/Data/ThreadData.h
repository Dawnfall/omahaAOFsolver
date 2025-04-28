#pragma once
#include "Core.h"

class ThreadData
{
public:
	ThreadData(unsigned int threadCount) :
		handIndex(0),
		syncPoint(threadCount)
	{
	}

	std::barrier<> syncPoint;
	std::atomic<int> handIndex;
};