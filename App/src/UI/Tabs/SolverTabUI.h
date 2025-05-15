#pragma once
#include "Core.h"
#include "UI/Widgets/LabelUI.h"
#include "UI/Widgets/InputUI.h"
#include "UI/Widgets/RadioButtonUI.h"
#include "UI/Widgets/ButtonUI.h"
#include "Utils.h"
#include "Constants.h"

class SolverTabUI
{
public:
	SolverTabUI();

	//Solver
	URef<LabelUI> infoLabel;

	URef<LabelUI> boardLabel;
	URef<InputUI> boardInput;

	URef<LabelUI> sbLabel;
	URef<InputUI> sbInput;

	URef<LabelUI> marginLabel;
	URef<InputUI> maginInput;

	URef<LabelUI> bbLabel;
	URef<InputUI> bbInput;

	URef<LabelUI> playersLabel;
	std::array<URef<LabelUI>, 3> totalPlayerLabels;
	std::array<URef<RadioButtonUI>, 3> totalPlayerRadioButtons;

	URef<ButtonUI> solveButton;

	// Solver
	int GetPlayerCount()const
	{
		if (totalPlayerRadioButtons[0]->IsOn())
			return 2;
		else if (totalPlayerRadioButtons[1]->IsOn())
			return 3;
		return 4;
	}
	bool GetBoard(std::array<uint8_t, 3>& flop)const
	{
		std::string board = boardInput->GetText(); // (boardInput->GetText() != nullptr) ? std::string(boardInput->m_input->value()) : std::string();
		if (board.size() != 6)
			return false;
		if (!(
			Constants::rankSet.contains(board[0]) && Constants::rankSet.contains(board[2]) && Constants::rankSet.contains(board[4]) &&
			Constants::suitSet.contains(board[1]) && Constants::suitSet.contains(board[3]) && Constants::suitSet.contains(board[5])
			))
			return false;

		phevaluator::Card c0 = phevaluator::Card(board.substr(0, 2));
		phevaluator::Card c1 = phevaluator::Card(board.substr(2, 2));
		phevaluator::Card c2 = phevaluator::Card(board.substr(4, 2));

		flop[0] = static_cast<uint8_t>((int)c0);
		flop[1] = static_cast<uint8_t>((int)c1);
		flop[2] = static_cast<uint8_t>((int)c2);
		return true;
	}
	bool GetBlinds(float& sb, float& bb)const
	{
		return Utils::SafeCharToFloat(sbInput->GetText().c_str(), sb) && Utils::SafeCharToFloat(bbInput->GetText().c_str(), bb);
	}

	// Margin
	bool GetMargin(float& margin)const
	{
		return Utils::SafeCharToFloat(maginInput->GetText().c_str(), margin);
	}

	void UpdateResultsInfo()const;
};