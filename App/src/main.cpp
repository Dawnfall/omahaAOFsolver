#include "Core.h"

#include "Solver.h"
#include "Utils.h"
#include "Situation.h"
#include "Solution.h"


void BuildSolution()
{
	Solution s = Solver::Solve(2, Bb, Sb, { "3d", "8c", "9c" });

	auto sb = s.GetNode("");
	sb->range.PrintRandom(10);

}

void SimulateNode()
{
	Situation sit;
	sit.AddHand({ "As","Ac","Ks","Kc" }, Position::SB);

	Solution s(4, Bb, Sb, { "3d", "8s", "9c" });
	std::shared_ptr<Node> node_sb_vs_co = s.GetNode("AF");

	float ev = Solver::CalcBbEvForAiHand(*node_sb_vs_co, sit.GetRemovedCards(), sit.GetHands(), sit.GetBoard(), sit.GetPotSize());
	std::cout << "Eq: " << ev << std::endl;
}

void SimulateHand()
{
	Situation sit;

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
	Situation sit;
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

void TestHandVsHand()
{
	Situation sit;
	sit.SetFlop(Card("Ad"), Card("5s"), Card("Qh"));
	sit.AddHand({ Card("Js"),Card("Qs"),Card("4s"),Card("7d") }, Position::CO);
	sit.AddHand({ Card("2c"),Card("8c"),Card("9h"),Card("Ac") }, Position::BU);

	float res = Solver::CalculateEquity(sit.GetHands(), sit.GetBoard(), sit.GetRemovedCards());
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
	//TestSimulate();
	//TestHandVsRange();
	//TestHandVsHand();
	//TestCompareHands();

	return 0;


}

