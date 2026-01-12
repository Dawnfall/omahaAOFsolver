#include "SolverTabUI.h"
#include "UI/RenderCursor.h"

#include "Application.h"

SolverTabUI::SolverTabUI()
{
	RenderCursor cursor;
	cursor.spacingX = 10;
	cursor.spacingY = 10;
	cursor.SetStartPos(0, Constants::tabsHeight);
	cursor.Reset();
	cursor.NextRow();

	Fl_Group* solverTab = new Fl_Group(0, Constants::tabsHeight, Constants::windowWidth, Constants::windowHeight - Constants::tabsHeight, "Solver");
	solverTab->box(FL_BORDER_BOX);
	solverTab->begin();

	//*************
	// Board
	boardLabel = std::make_unique<LabelUI>(cursor, 50, Constants::el_height, Constants::board_label);
	boardInput = std::make_unique<InputUI>(cursor, 75, Constants::el_height, Constants::board_default);
	cursor.NextRow();

	//*************
	// Blinds
	sbLabel = std::make_unique<LabelUI>(cursor, 50, Constants::el_height, Constants::sb_label);
	sbInput = std::make_unique<InputUI>(cursor, 30, Constants::el_height, Constants::sb_default);
	bbLabel = std::make_unique<LabelUI>(cursor, 50, Constants::el_height, Constants::bb_label);
	bbInput = std::make_unique<InputUI>(cursor, 30, Constants::el_height, Constants::bb_default);
	cursor.NextRow();

	//*************
	// Margin
	marginLabel = std::make_unique<LabelUI>(cursor, 50, Constants::el_height, Constants::margin_label);
	maginInput = std::make_unique<InputUI>(cursor, 30, Constants::el_height, Constants::margin_default);
	cursor.NextRow();

	////********
	//// Players
	playersLabel = std::make_unique<LabelUI>(cursor, 50, Constants::el_height, Constants::players_label);
	totalPlayerLabels[0] = std::make_unique<LabelUI>(cursor, 10, Constants::el_height, Constants::player2_label);
	totalPlayerRadioButtons[0] = std::make_unique<RadioButtonUI>(cursor, Constants::el_height, Constants::el_height, true);
	totalPlayerLabels[1] = std::make_unique<LabelUI>(cursor, 10, Constants::el_height, Constants::player3_label);
	totalPlayerRadioButtons[1] = std::make_unique<RadioButtonUI>(cursor, Constants::el_height, Constants::el_height, false);
	totalPlayerLabels[2] = std::make_unique<LabelUI>(cursor, 10, Constants::el_height, Constants::player4_label);
	totalPlayerRadioButtons[2] = std::make_unique<RadioButtonUI>(cursor, Constants::el_height, Constants::el_height, false);
	cursor.NextRow();

	//*************
	// Solve Button
	solveButton = std::make_unique<ButtonUI>(cursor, 50, Constants::el_height, Constants::solveButton_label);
	cursor.NextRow();

	cursor.SetStartPos(300, 100);
	cursor.Reset();
	infoLabel = std::make_unique<LabelUI>(cursor, 200, 200, "No solution");
	infoLabel->box(FL_BORDER_BOX);
	cursor.NextRow();

	solverTab->end();
	solverTab->show();


	solveButton->callback([](Fl_Widget* widget, void* userData) {
		Application::GetInstance()->RunSolver();
		}, nullptr);

	Application::GetInstance()->OnSolveStart.AddListener([this]() {
		infoLabel->SetText("Solving...");
		});

	Application::GetInstance()->OnSolveFinish.AddListener([this]() {
		if (Application::GetInstance()->m_solverResult)
			UpdateResultsInfo();
		});
}

void SolverTabUI::UpdateResultsInfo()const
{
	if (Application::GetInstance()->m_solverResult)
	{
		auto result = Application::GetInstance()->m_solverResult.get();
		constexpr const char* info = R"(
Board: {}
Players: {}
Duration: {}s"
)";
		infoLabel->SetText(
			std::format(info,
				CardArrayToString<3>(result->Solution->Flop),
				result->Solution->PlayerCount,
				Utils::FormatDoubleToNDecimal(result->Duration, 2))
		);
	}

}

