#include "Utils.h"

std::unique_ptr<std::mt19937> m_randGen = nullptr;

std::mt19937& Utils::GetRandomGen()
{
	if (!m_randGen)
	{
		m_randGen = std::make_unique<std::mt19937>(std::random_device{}());
	}
	return *m_randGen;
}


bool Utils::Validate(const Hand& hand, Situation& sit)
{
	for (auto& card : hand)
	{
		for (const auto& h : sit.GetHands())
			if (std::find(h.begin(), h.end(), card) != h.end())
				return false;
		if (std::find(sit.GetBoard().begin(), sit.GetBoard().end(), card) != sit.GetBoard().end())
			return false;
		if (std::find(sit.GetDeadCards().begin(), sit.GetDeadCards().end(), card) != sit.GetDeadCards().end())
			return false;
	}
	return true;
}