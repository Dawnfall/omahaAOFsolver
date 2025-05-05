
#include "Core.h"
#include "Solver/Solver.h"
#include "Solver/PokerUtils.h"

void TestCompareHands()
{
	//Hand h1{ Card("Js"), Card("Qs"), Card("4s"), Card("7d") };
	//Hand h2{ Card("2c"), Card("8c"), Card("9h"), Card("Ac") };

	//Board_F flop{ Card("Ad"),Card("Ah"),Card("3h") };
	//Board_TR turnAndRiver{ Card("5s"),Card("Qh") };

	//float res = PokerUtils::CompareHands({ h1,h2 }, flop, turnAndRiver);
	//std::cout << "Hand1 eq: " << res << "" << std::endl;
}

void TestHandVsHand()
{
	//Hand h1{ (int8_t)Card("Js"),(int8_t)Card("Qs"),(int8_t)Card("4s"),(int8_t)Card("7d") };
	//Hand h2{ Card("2c"),Card("8c"),Card("9h"),Card("Ac") };

	//Board_F flop{ Card("Ad"),Card("5s"),Card("Qh") };

	//float res = Solver::CalculateEquity(sit.GetHands(), sit.GetBoard(), sit.GetRemovedCards());
	//std::cout << "Hand1 eq: " << res << "" << std::endl;
}



int main(int argc, char** argv)
{
	TestCompareHands();
}