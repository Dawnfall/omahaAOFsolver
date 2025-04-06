#pragma once
#include "Core.h"
#include "Situation.h"

namespace Utils
{
	std::mt19937& GetRandomGen();

	bool Validate(const Hand& hand, Situation& sit);
};
