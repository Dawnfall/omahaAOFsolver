#pragma once
#include "Core.h"
#include "Situation.h"

class WeightedHand
{
public:
	WeightedHand(const Hand& hand, float prob) :
		m_hand(hand),
		m_prob(prob)
	{
	}

	const Hand& GetHand() const { return m_hand; }
	float GetAiProb()const { return m_prob; }
	void SetAiProb(float newProb) { m_prob = newProb; }

	bool DoPick()const
	{
		auto& randGen = Utils::GetRandomGen();
		std::uniform_real_distribution<> dis(0.0f, 1.0f);
		float prob = dis(randGen);
		return prob < m_prob;
	}

	std::string ToString()
	{
		std::string str = Utils::ToString(m_hand);
		return str + "Prob: " + std::to_string(m_prob) + "\n";
	}

private:
	Hand m_hand;
	float m_prob;
};

class Range
{
public:
	std::vector<WeightedHand> Hands;

	const WeightedHand& GetRandomWeightedHand(const std::unordered_set<int>& removedCards)const;
	const Hand ForcePickHand(const std::unordered_set<int>& removedCards)const;

	void PrintRandom(int count);

	static Range WithRemoved(const std::unordered_set<int>& removedCards);
	static Range FullRange();
	static Range FullRandom();

private:
	Range() {}
};