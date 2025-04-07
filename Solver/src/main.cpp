#include "Core.h"

#include "Solver.h"
#include "Utils.h"
#include "Situation.h"
#include "Solution.h"


void BuildSolution()
{
	Situation sit;
	sit.SetBoard({ "3d","8s","9c" });

	Solution s(2, sit);
	s.Solve();
}

void SimulateNode()
{
	Situation sit;
	sit.AddHand({ "As","Ac","Ks","Kc" }, Position::SB);
	sit.SetBoard({ "3d","8s","9c" });

	Solution s(4, sit);
	std::shared_ptr<Node> node_sb_vs_co = s.GetNode("AF");

	float ev = node_sb_vs_co->CalcBbEvForAiHand(sit);
	std::cout << "Eq: " << ev << std::endl;
}

void SimulateHand()
{
	Situation sit;
	sit.SetBoard({ Card("Js"),Card("Jd"),Card("3s") });

	Solution s(3, sit);
	auto& randGen = Utils::GetRandomGen();

	std::shared_ptr<Node> node = s.GetNode("");

	while (node != nullptr)
	{
		WeightedHand randomHand = node->range.GetRandomWeightedHand(sit);

		std::uniform_real_distribution<> dis(0.0, 1.0);
		float prob = static_cast<float>(dis(randGen));

		if (prob < randomHand.AIProb)
		{
			sit.AddHand(randomHand.Hand, node->Position);
			node = node->AINode;
		}
		else
		{
			node = node->FoldNode;
		}
	}

	sit.Print();

	float eq1 = Solver::CalculateEquity(sit, Solver::errorThreshold, Solver::totalHandVsHandIter);
	std::cout << "Hand1 eq: " << eq1 << std::endl;

}

void TestHandVsRange()
{
	Hand h1{ Card("Js"),Card("Qs"),Card("4s"),Card("7d") };
	Board b = { Card("Ad"),Card("5s"),Card("Qh") };

	Situation situation;
	situation.AddHand(h1, Position::CO);
	situation.SetBoard(b);

	Range r = Range::FullRange();

	float eq = 0.0f;
	for (int iter = 0; iter < Solver::totalHandVsRangeIter; iter++)
	{
		situation.AddHand(r.GetRandomWeightedHand(situation).Hand, Position::BB);
		eq += Solver::CalculateEquity(situation, Solver::errorThreshold, 1);
		situation.ClearToPlayer();
	}
	eq /= Solver::totalHandVsRangeIter;

	std::cout << "Hand1 eq: " << eq << "" << std::endl;
}

void TestHandVsHand()
{
	Situation sit;
	sit.AddHand({ Card("Js"),Card("Qs"),Card("4s"),Card("7d") }, Position::CO);
	sit.AddHand({ Card("2c"),Card("8c"),Card("9h"),Card("Ac") }, Position::BU);
	sit.SetBoard({ Card("Ad"),Card("5s"),Card("Qh") });

	float res = Solver::CalculateEquity(sit, Solver::errorThreshold, Solver::totalHandVsHandIter);
	std::cout << "Hand1 eq: " << res << "" << std::endl;
}

void TestCompareHands()
{
	Hand h1{ Card("Js"),Card("Qs"),Card("4s"),Card("7d") };
	Hand h2{ Card("2c"),Card("8c"),Card("9h"),Card("Ac") };
	Board b = { Card("Ad"),Card("Ah"),Card("3h"),Card("5s"),Card("Qh") };
	float res = Solver::CompareHands(h1, h2, b);
	std::cout << "Hand1 eq: " << res << "" << std::endl;
}

int main()
{
	BuildSolution();
	//SimulateNode();
	//SimulateHand();
	//TestHandVsRange();
	//TestHandVsHand();
	//TestSimulate();

	return 0;


}

