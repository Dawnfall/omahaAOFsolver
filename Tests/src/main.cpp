
#include "Core.h"
#include "Solver/Solver.h"
#include "Solver/PokerUtils.h"
#include "FilterTests.h"


void TestPotSizes()
{
	SolverParams solParams;
	solParams.sb = 0.5f;
	solParams.bb = 1.0f;
	solParams.stackSize = 5.0f;

	solParams.totalPlayers = 2;

	float pot_2p_1_ai = PokerUtils::GetPotAfter(1, solParams, true);
	if (pot_2p_1_ai != 10.0f)
		std::cout << "invalid pot ";

	float pot_2p_1_fold = PokerUtils::GetPotAfter(1, solParams, false);
	if (pot_2p_1_fold != 6.0f)
		std::cout << "invalid pot ";

	solParams.totalPlayers = 3;

	float pot_3p_3_ai = PokerUtils::GetPotAfter(3, solParams, true);
	if (pot_3p_3_ai != 10.0f)
		std::cout << "invalid pot ";

	float pot_3p_3_fold = PokerUtils::GetPotAfter(3, solParams, false);
	if (pot_3p_3_fold != 6.0f)
		std::cout << "invalid pot ";

	float pot_3p_4_ai = PokerUtils::GetPotAfter(4, solParams, true);
	if (pot_3p_4_ai != 10.5f)
		std::cout << "invalid pot ";

	float pot_3p_4_fold = PokerUtils::GetPotAfter(4, solParams, false);
	if (pot_3p_4_fold != 6.5f)
		std::cout << "invalid pot ";

	float pot_3p_5_ai = PokerUtils::GetPotAfter(5, solParams, true);
	if (pot_3p_5_ai != 15.0f)
		std::cout << "invalid pot ";

	float pot_3p_5_fold = PokerUtils::GetPotAfter(5, solParams, false);
	if (pot_3p_5_fold != 11.0f)
		std::cout << "invalid pot ";

	solParams.totalPlayers = 4;

	float pot_4p_7_ai = PokerUtils::GetPotAfter(7, solParams, true);
	if (pot_4p_7_ai != 10.0f)
		std::cout << "invalid pot ";

	float pot_4p_7_fold = PokerUtils::GetPotAfter(7, solParams, false);
	if (pot_4p_7_fold != 6.0f)
		std::cout << "invalid pot ";

	float pot_4p_8_ai = PokerUtils::GetPotAfter(8, solParams, true);
	if (pot_4p_8_ai != 10.5f)
		std::cout << "invalid pot ";

	float pot_4p_8_fold = PokerUtils::GetPotAfter(8, solParams, false);
	if (pot_4p_8_fold != 6.5f)
		std::cout << "invalid pot ";

	float pot_4p_9_ai = PokerUtils::GetPotAfter(9, solParams, true);
	if (pot_4p_9_ai != 15.0f)
		std::cout << "invalid pot ";

	float pot_4p_9_fold = PokerUtils::GetPotAfter(9, solParams, false);
	if (pot_4p_9_fold != 11.0f)
		std::cout << "invalid pot ";

	float pot_4p_10_ai = PokerUtils::GetPotAfter(10, solParams, true);
	if (pot_4p_10_ai != 10.5f)
		std::cout << "invalid pot ";

	float pot_4p_10_fold = PokerUtils::GetPotAfter(10, solParams, false);
	if (pot_4p_10_fold != 6.5f)
		std::cout << "invalid pot ";

	float pot_4p_11_ai = PokerUtils::GetPotAfter(11, solParams, true);
	if (pot_4p_11_ai != 15.0f)
		std::cout << "invalid pot ";

	float pot_4p_11_fold = PokerUtils::GetPotAfter(11, solParams, false);
	if (pot_4p_11_fold != 11.0f)
		std::cout << "invalid pot ";

	float pot_4p_12_ai = PokerUtils::GetPotAfter(12, solParams, true);
	if (pot_4p_12_ai != 15.5f)
		std::cout << "invalid pot ";

	float pot_4p_12_fold = PokerUtils::GetPotAfter(12, solParams, false);
	if (pot_4p_12_fold != 11.5f)
		std::cout << "invalid pot ";

	float pot_4p_13_ai = PokerUtils::GetPotAfter(13, solParams, true);
	if (pot_4p_13_ai != 20.0f)
		std::cout << "invalid pot ";

	float pot_4p_13_fold = PokerUtils::GetPotAfter(13, solParams, false);
	if (pot_4p_13_fold != 16.0f)
		std::cout << "invalid pot ";

}

void TestFoldLoss()
{
	SolverParams solParams;
	solParams.sb = 0.5f;
	solParams.bb = 1.0f;
	solParams.stackSize = 5.0f;

	solParams.totalPlayers = 2;

	float loss_2p_0 = PokerUtils::GetFoldLoss(0, solParams);
	if (loss_2p_0 != 0.5f)
		std::cout << "invalid fold loss ";

	float loss_2p_1 = PokerUtils::GetFoldLoss(1, solParams);
	if (loss_2p_1 != 1.0f)
		std::cout << "invalid fold loss ";

	solParams.totalPlayers = 3;

	float loss_3p_0 = PokerUtils::GetFoldLoss(0, solParams);
	if (loss_3p_0 != 0)
		std::cout << "invalid fold loss ";

	float loss_3p_1 = PokerUtils::GetFoldLoss(1, solParams);
	if (loss_3p_1 != 0.5f)
		std::cout << "invalid fold loss ";

	float loss_3p_2 = PokerUtils::GetFoldLoss(2, solParams);
	if (loss_3p_2 != 0.5f)
		std::cout << "invalid fold loss ";

	float loss_3p_3 = PokerUtils::GetFoldLoss(3, solParams);
	if (loss_3p_3 != 1.0f)
		std::cout << "invalid fold loss ";

	float loss_3p_4 = PokerUtils::GetFoldLoss(4, solParams);
	if (loss_3p_4 != 1.0f)
		std::cout << "invalid fold loss ";

	float loss_3p_5 = PokerUtils::GetFoldLoss(5, solParams);
	if (loss_3p_5 != 1.0f)
		std::cout << "invalid fold loss ";


	solParams.totalPlayers = 4;

	float loss_4p_0 = PokerUtils::GetFoldLoss(0, solParams);
	if (loss_4p_0 != 0)
		std::cout << "invalid fold loss ";

	float loss_4p_1 = PokerUtils::GetFoldLoss(1, solParams);
	if (loss_4p_1 != 0)
		std::cout << "invalid fold loss ";

	float loss_4p_2 = PokerUtils::GetFoldLoss(2, solParams);
	if (loss_4p_2 != 0)
		std::cout << "invalid fold loss ";

	float loss_4p_3 = PokerUtils::GetFoldLoss(3, solParams);
	if (loss_4p_3 != 0.5f)
		std::cout << "invalid fold loss ";

	float loss_4p_4 = PokerUtils::GetFoldLoss(4, solParams);
	if (loss_4p_4 != 0.5f)
		std::cout << "invalid fold loss ";

	float loss_4p_5 = PokerUtils::GetFoldLoss(5, solParams);
	if (loss_4p_5 != 0.5f)
		std::cout << "invalid fold loss ";

	float loss_4p_6 = PokerUtils::GetFoldLoss(6, solParams);
	if (loss_4p_6 != 0.5f)
		std::cout << "invalid fold loss ";

	float loss_4p_7 = PokerUtils::GetFoldLoss(7, solParams);
	if (loss_4p_7 != 1.0f)
		std::cout << "invalid fold loss ";

	float loss_4p_8 = PokerUtils::GetFoldLoss(8, solParams);
	if (loss_4p_8 != 1.0f)
		std::cout << "invalid fold loss ";

	float loss_4p_9 = PokerUtils::GetFoldLoss(9, solParams);
	if (loss_4p_9 != 1.0f)
		std::cout << "invalid fold loss ";

	float loss_4p_10 = PokerUtils::GetFoldLoss(10, solParams);
	if (loss_4p_10 != 1.0f)
		std::cout << "invalid fold loss ";

	float loss_4p_11 = PokerUtils::GetFoldLoss(11, solParams);
	if (loss_4p_11 != 1.0f)
		std::cout << "invalid fold loss ";

	float loss_4p_12 = PokerUtils::GetFoldLoss(12, solParams);
	if (loss_4p_12 != 1.0f)
		std::cout << "invalid fold loss ";

	float loss_4p_13 = PokerUtils::GetFoldLoss(13, solParams);
	if (loss_4p_13 != 1.0f)
		std::cout << "invalid fold loss ";

}

void TestGetToRange()
{
	size_t currRange;
	int totalPlayers = 2;

	currRange = 0;
	if (PokerUtils::GetToRange(currRange, true, totalPlayers) != 1)
		std::cout << "Invalid go to!";

	if (PokerUtils::GetToRange(currRange, false, totalPlayers) != -1)
		std::cout << "Invalid go to!";

	currRange = 1;
	if (PokerUtils::GetToRange(currRange, true, totalPlayers) != -1)
		std::cout << "Invalid go to!";

	if (PokerUtils::GetToRange(currRange, false, totalPlayers) != -1)
		std::cout << "Invalid go to!";


	totalPlayers = 3;

	currRange = 0;
	if (PokerUtils::GetToRange(currRange, true, totalPlayers) != 2)
		std::cout << "Invalid go to!";

	if (PokerUtils::GetToRange(currRange, false, totalPlayers) != 1)
		std::cout << "Invalid go to!";


	currRange = 1;
	if (PokerUtils::GetToRange(currRange, true, totalPlayers) != 3)
		std::cout << "Invalid go to!";

	if (PokerUtils::GetToRange(currRange, false, totalPlayers) != -1)
		std::cout << "Invalid go to!";


	currRange = 2;
	if (PokerUtils::GetToRange(currRange, true, totalPlayers) != 5)
		std::cout << "Invalid go to!";

	if (PokerUtils::GetToRange(currRange, false, totalPlayers) != 4)
		std::cout << "Invalid go to!";


	currRange = 3;
	if (PokerUtils::GetToRange(currRange, true, totalPlayers) != -1)
		std::cout << "Invalid go to!";

	if (PokerUtils::GetToRange(currRange, false, totalPlayers) != -1)
		std::cout << "Invalid go to!";

	currRange = 4;
	if (PokerUtils::GetToRange(currRange, true, totalPlayers) != -1)
		std::cout << "Invalid go to!";

	if (PokerUtils::GetToRange(currRange, false, totalPlayers) != -1)
		std::cout << "Invalid go to!";

	currRange = 5;
	if (PokerUtils::GetToRange(currRange, true, totalPlayers) != -1)
		std::cout << "Invalid go to!";

	if (PokerUtils::GetToRange(currRange, false, totalPlayers) != -1)
		std::cout << "Invalid go to!";


	totalPlayers = 4;

	currRange = 0;
	if (PokerUtils::GetToRange(currRange, true, totalPlayers) != 2)
		std::cout << "Invalid go to!";
	if (PokerUtils::GetToRange(currRange, false, totalPlayers) != 1)
		std::cout << "Invalid go to!";

	currRange = 1;
	if (PokerUtils::GetToRange(currRange, true, totalPlayers) != 4)
		std::cout << "Invalid go to!";
	if (PokerUtils::GetToRange(currRange, false, totalPlayers) != 3)
		std::cout << "Invalid go to!";

	currRange = 2;
	if (PokerUtils::GetToRange(currRange, true, totalPlayers) != 6)
		std::cout << "Invalid go to!";
	if (PokerUtils::GetToRange(currRange, false, totalPlayers) != 5)
		std::cout << "Invalid go to!";

	currRange = 3;
	if (PokerUtils::GetToRange(currRange, true, totalPlayers) != 7)
		std::cout << "Invalid go to!";
	if (PokerUtils::GetToRange(currRange, false, totalPlayers) != -1)
		std::cout << "Invalid go to!";

	currRange = 4;
	if (PokerUtils::GetToRange(currRange, true, totalPlayers) != 9)
		std::cout << "Invalid go to!";
	if (PokerUtils::GetToRange(currRange, false, totalPlayers) != 8)
		std::cout << "Invalid go to!";

	currRange = 5;
	if (PokerUtils::GetToRange(currRange, true, totalPlayers) != 11)
		std::cout << "Invalid go to!";
	if (PokerUtils::GetToRange(currRange, false, totalPlayers) != 10)
		std::cout << "Invalid go to!";

	currRange = 6;
	if (PokerUtils::GetToRange(currRange, true, totalPlayers) != 13)
		std::cout << "Invalid go to!";
	if (PokerUtils::GetToRange(currRange, false, totalPlayers) != 12)
		std::cout << "Invalid go to!";

	currRange = 7;
	if (PokerUtils::GetToRange(currRange, true, totalPlayers) != -1)
		std::cout << "Invalid go to!";
	if (PokerUtils::GetToRange(currRange, false, totalPlayers) != -1)
		std::cout << "Invalid go to!";

	currRange = 8;
	if (PokerUtils::GetToRange(currRange, true, totalPlayers) != -1)
		std::cout << "Invalid go to!";
	if (PokerUtils::GetToRange(currRange, false, totalPlayers) != -1)
		std::cout << "Invalid go to!";


	currRange = 9;
	if (PokerUtils::GetToRange(currRange, true, totalPlayers) != -1)
		std::cout << "Invalid go to!";
	if (PokerUtils::GetToRange(currRange, false, totalPlayers) != -1)
		std::cout << "Invalid go to!";

	currRange = 10;
	if (PokerUtils::GetToRange(currRange, true, totalPlayers) != -1)
		std::cout << "Invalid go to!";
	if (PokerUtils::GetToRange(currRange, false, totalPlayers) != -1)
		std::cout << "Invalid go to!";

	currRange = 11;
	if (PokerUtils::GetToRange(currRange, true, totalPlayers) != -1)
		std::cout << "Invalid go to!";
	if (PokerUtils::GetToRange(currRange, false, totalPlayers) != -1)
		std::cout << "Invalid go to!";

	currRange = 12;
	if (PokerUtils::GetToRange(currRange, true, totalPlayers) != -1)
		std::cout << "Invalid go to!";
	if (PokerUtils::GetToRange(currRange, false, totalPlayers) != -1)
		std::cout << "Invalid go to!";

	currRange = 13;
	if (PokerUtils::GetToRange(currRange, true, totalPlayers) != -1)
		std::cout << "Invalid go to!";
	if (PokerUtils::GetToRange(currRange, false, totalPlayers) != -1)
		std::cout << "Invalid go to!";

}

int main(int argc, char** argv)
{
	RankSimpleTest();
	SuitSimpleTest();
	MixSimpleTest();
	NegateTest();
	OrTest();
	DoubleNegateTest();
	Complex1Test();
	//TestGetToRange();
	//TestPotSizes();
	//TestFoldLoss();
}