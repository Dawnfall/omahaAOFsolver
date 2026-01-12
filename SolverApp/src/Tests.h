#pragma once
#include "Situation.h"
#include "Data/Solution.h"
#include "Solver/Solver.h"

namespace Test2
{
	void Run()
	{
		//SimulateNode();
		//SimulateHand();
		//TestSimulate();
		//TestHandVsRange();
		//TestHandVsHand();
		//TestCompareHands();

	}

	void SimulateNode()
	{
		Situation sit(1.0f, 0.5f, 5.0f);
		sit.AddHand({ "As","Ac","Ks","Kc" }, Position::SB);

		Solution s(4, Bb, Sb, { "3d", "8s", "9c" });
		std::shared_ptr<Node> node_sb_vs_co = s.GetNode("AF");

		float ev = Solver::CalcBbEvForAiHand(*node_sb_vs_co, sit.GetRemovedCards(), sit.GetHands(), sit.GetBoard(), sit.GetPotSize());
		std::cout << "Eq: " << ev << std::endl;
	}

	void SimulateHand()
	{
		Situation sit(1.0f, 0.5f, 5.0f);

		Solution s(3, Bb, Sb, { "Js" ,"Jd" ,"3s" });
		auto& randGen = Utils::GetRandomGen();

		std::shared_ptr<Node> node = s.GetNode("");

		while (node != nullptr)
		{
			Hand h = node->range.ForcePickHand(sit.GetRemovedCards());
			WeightedHand randomHand = node->range.GetRandomWeightedHand(sit.GetRemovedCards());
			std::uniform_real_distribution<> dis(0.0, 1.0);
			float prob = static_cast<float>(dis(randGen));

			std::cout << toString(node->Position) << ": ";
			if (prob < randomHand.GetAiProb())
			{
				std::cout << "AllIn\n";
				for (const Card& card : randomHand.GetHand())
					std::cout << card.describeCard() << " ";
				std::cout << "\n";

				sit.AddHand(randomHand.GetHand(), node->Position);
				node = node->AINode;
			}
			else
			{
				std::cout << "Fold\n";
				node = node->FoldNode;
			}
		}

		float eq1 = Solver::CalculateEquity(sit.GetHands(), sit.GetBoard(), sit.GetRemovedCards());
		std::cout << "Hand1 eq: " << eq1 << std::endl;

	}

	void TestHandVsRange()
	{
		Situation sit(1.0f, 0.5f, 5.0f);
		sit.SetFlop(Card("Ad"), Card("5s"), Card("Qh"));
		sit.AddHand({ Card("Js"),Card("Qs"),Card("4s"),Card("7d") }, Position::CO);

		Range r = Range::FullRange();

		float eq = 0.0f;
		for (int iter = 0; iter < 5000; iter++)
		{
			Situation s = sit;
			s.AddHand(r.GetRandomWeightedHand(s.GetRemovedCards()).GetHand(), Position::BB);
			eq += Solver::CalculateEquity(sit.GetHands(), sit.GetBoard(), sit.GetRemovedCards());
		}
		eq /= 5000;

		std::cout << "Hand1 eq: " << eq << "" << std::endl;
	}




}