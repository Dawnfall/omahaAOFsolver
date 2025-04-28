#include "WindowPanel.h"
#include "RenderUtils.h"

#include "Constants.h"
#include "Application.h"

int WindowPanel::Render(Application* app)
{
	Fl_Window* window = new Fl_Window(Constants::windowWidth, Constants::windowHeight, Constants::window_title.c_str());

	Fl_Tabs* tabs = new Fl_Tabs(0, 0, Constants::windowWidth, Constants::windowHeight);
	tabs->box(FL_BORDER_BOX);

	RenderSolverPanel(app);
	RenderViewerPanel(app);
	RenderOptionsPanel(app);

	tabs->end();

	window->end();
	window->show();

	return 0;
}

void WindowPanel::RenderSolverPanel(Application* app)
{
	m_cursor.spacingX = 10;
	m_cursor.spacingY = 10;
	m_cursor.SetStartPos(0, Constants::tabsHeight);
	m_cursor.Reset();
	m_cursor.NextRow();

	Fl_Group* solverTab = new Fl_Group(0, Constants::tabsHeight, Constants::windowWidth, Constants::windowHeight - Constants::tabsHeight, "Solver");
	solverTab->box(FL_BORDER_BOX);
	solverTab->begin();

	//*************
	// Board
	boardLabel = std::make_unique<LabelUI>(m_cursor, 50, Constants::el_height, Constants::board_label);
	boardInput = std::make_unique<InputUI>(m_cursor, 75, Constants::el_height, "Kc5d3d");
	m_cursor.NextRow();

	//*************
	// Blinds
	sbLabel = std::make_unique<LabelUI>(m_cursor, 50, Constants::el_height, Constants::sb_label);
	sbInput = std::make_unique<InputUI>(m_cursor, 30, Constants::el_height, Constants::sb_default);
	bbLabel = std::make_unique<LabelUI>(m_cursor, 50, Constants::el_height, Constants::bb_label);
	bbInput = std::make_unique<InputUI>(m_cursor, 30, Constants::el_height, Constants::bb_default);
	m_cursor.NextRow();

	//*************
	// Margin
	marginLabel = std::make_unique<LabelUI>(m_cursor, 50, Constants::el_height, Constants::margin_label);
	maginInput = std::make_unique<InputUI>(m_cursor, 30, Constants::el_height, Constants::margin_default);
	m_cursor.NextRow();

	////********
	//// Players
	playersLabel = std::make_unique<LabelUI>(m_cursor, 50, Constants::el_height, Constants::players_label);
	totalPlayerLabels[0] = std::make_unique<LabelUI>(m_cursor, 10, Constants::el_height, Constants::player2_label);
	totalPlayerRadioButtons[0] = std::make_unique<RadioButtonUI>(m_cursor, Constants::el_height, Constants::el_height, true);
	totalPlayerLabels[1] = std::make_unique<LabelUI>(m_cursor, 10, Constants::el_height, Constants::player3_label);
	totalPlayerRadioButtons[1] = std::make_unique<RadioButtonUI>(m_cursor, Constants::el_height, Constants::el_height, false);
	totalPlayerLabels[2] = std::make_unique<LabelUI>(m_cursor, 10, Constants::el_height, Constants::player4_label);
	totalPlayerRadioButtons[2] = std::make_unique<RadioButtonUI>(m_cursor, Constants::el_height, Constants::el_height, false);
	m_cursor.NextRow();

	//***********
	// Iterations

	repeatIterLabel = std::make_unique<LabelUI>(m_cursor, 50, Constants::el_height, Constants::iterCount_label);
	repeatIterInput = std::make_unique<InputUI>(m_cursor, 40, Constants::el_height, Constants::repeatIter_default);

	perHandIterLabel = std::make_unique<LabelUI>(m_cursor, 50, Constants::el_height, Constants::perHandIter_label);
	perHandIterInput = std::make_unique<InputUI>(m_cursor, 40, Constants::el_height, Constants::perHandIter_default);
	m_cursor.NextRow();


	//*************
	// Solve Button
	solveButton = std::make_unique<ButtonUI>(m_cursor, 50, Constants::el_height, Constants::solveButton_label, solve_button_callback, app);
	m_cursor.NextRow();

	m_cursor.SetStartPos(300, 100);
	m_cursor.Reset();
	infoLabel = std::make_unique<LabelUI>(m_cursor, 200, 200, "No solution");
	infoLabel->m_box->box(FL_BORDER_BOX);
	m_cursor.NextRow();

	solverTab->end();
	solverTab->show();

	app->OnSolveStart.AddListener([this]() {
		infoLabel->SetText("Solving...");
		});

	app->OnSolveFinish.AddListener([this](Solution* sol) {
		if (sol)
			infoLabel->SetText(std::format("Iterations: {}\nper Hand: {}\ntime: {}s", sol->Iters, sol->PerHandIters,Application::GetInstance()->duration.count()));
		else
			infoLabel->SetText("No solution");
		});
}

void WindowPanel::RenderViewerPanel(Application* app)
{
	Fl_Group* viewerTab = new Fl_Group(0, Constants::tabsHeight, Constants::windowWidth, Constants::windowHeight - Constants::tabsHeight, "Viewer");
	viewerTab->hide();

	m_cursor.SetStartPos(10, 35);
	m_cursor.spacingX = 10;
	m_cursor.spacingY = 10;
	m_cursor.Reset();

	boardCards[0] = std::make_unique<LabelUI>(m_cursor, Constants::boardCardX, Constants::boardCardY, "");
	boardCards[0]->SetColor(1, 1, 1);
	boardCards[1] = std::make_unique<LabelUI>(m_cursor, Constants::boardCardX, Constants::boardCardY, "");
	boardCards[1]->SetColor(1, 1, 1);
	boardCards[2] = std::make_unique<LabelUI>(m_cursor, Constants::boardCardX, Constants::boardCardY, "");
	boardCards[2]->SetColor(1, 1, 1);
	loadSolutionButton = std::make_unique<ButtonUI>(m_cursor, Constants::loadButtonX, Constants::loadButtonY, "Load", load_button_callback, app);
	m_cursor.NextRow();
	scrollView = std::make_unique<ScrollViewUI>(m_cursor, Constants::scrollViewX, Constants::scrollViewY, app);
	m_cursor.NextRow();
	rangeTotalLabel = std::make_unique<LabelUI>(m_cursor, Constants::scrollViewX, Constants::rangeTotalLabelY, "Total:");
	m_cursor.NextRow();
	filterInput = std::make_unique<InputUI>(m_cursor, Constants::scrollViewX, Constants::input_height, "");

	m_cursor.SetStartPos(400, 35);
	m_cursor.spacingX = 10;
	m_cursor.spacingY = 10;
	m_cursor.Reset();

	m_nodesTitleLabel = std::make_unique<LabelUI>(m_cursor, 50, Constants::el_height, "Nodes");
	m_cursor.NextRow();
	nodeSelector = std::make_unique<NodeSelectorUI>(m_cursor, 90, 30, app, viewerTab);

	m_cursor.SetStartPos(600, 35);
	m_cursor.spacingX = 10;
	m_cursor.spacingY = 10;
	m_cursor.Reset();
	trainer = std::make_unique<TrainerUI>(m_cursor, 500, 300);

	app->OnSolutionChange.AddListener([this, viewerTab](Solution* sol) {
		if (sol)
		{
			phevaluator::Card c0(sol->Flop[0]);
			phevaluator::Card c1(sol->Flop[1]);
			phevaluator::Card c2(sol->Flop[2]);

			boardCards[0]->SetText(c0.describeCard().substr(0, 1));
			boardCards[0]->SetColor(RenderUtils::GetCardColor(c0));
			boardCards[1]->SetText(c1.describeCard().substr(0, 1));
			boardCards[1]->SetColor(RenderUtils::GetCardColor(c1));
			boardCards[2]->SetText(c2.describeCard().substr(0, 1));
			boardCards[2]->SetColor(RenderUtils::GetCardColor(c2));
		}
		else
		{
			boardCards[0]->SetText("");
			boardCards[0]->SetColor(1, 1, 1);
			boardCards[1]->SetText("");
			boardCards[1]->SetColor(1, 1, 1);
			boardCards[2]->SetText("");
			boardCards[2]->SetColor(1, 1, 1);
		}
		scrollView->RefreshRange(nullptr);
		nodeSelector->Refresh(sol, viewerTab, range_radio_callback);
		});

	app->OnRangeChange.AddListener([this](const std::vector<WeightedHand>* range) {
		//TODO...apply filter
		float aiCount = 0;
		for (auto& weHand : *range)
			if (weHand.Ev > 0)
				aiCount += 1;

		rangeTotalLabel->SetText("Total:       " + Utils::FormatFloatToNDecimal(aiCount / range->size(), 2));

		trainer->SetNewHand();
		scrollView->RefreshRange(range);
		});

	viewerTab->end();
}

void WindowPanel::RenderOptionsPanel(Application* app)
{
	m_cursor.spacingX = 10;
	m_cursor.spacingY = 10;
	m_cursor.SetStartPos(0, Constants::tabsHeight);
	m_cursor.Reset();
	m_cursor.NextRow();

	Fl_Group* optionsTab = new Fl_Group(0, 25, Constants::windowWidth, Constants::windowHeight - 20, "Options");
	m_cursor.SetStartPos(0, 25);
	m_cursor.Reset();
	optionsTab->hide();

	//***********
	// Save

	saveDirLabel = std::make_unique <LabelUI>(m_cursor, 50, Constants::el_height, Constants::saveDir_label);
	saveInput = std::make_unique<InputUI>(m_cursor, 150, Constants::el_height, Constants::saveFolder_default);
	m_cursor.NextRow();

	//***********
	// Threads and chunks

	numThreadsLabel = std::make_unique <LabelUI>(m_cursor, 50, Constants::el_height, Constants::numThreads_label);
	threadInput = std::make_unique<InputUI>(m_cursor, 40, Constants::el_height, Constants::threadCount_default);

	chunkSizeLabel = std::make_unique <LabelUI>(m_cursor, 50, Constants::el_height, Constants::chunkSize_label);
	chunkInput = std::make_unique<InputUI>(m_cursor, 40, Constants::el_height, Constants::chunkSize_default);
	m_cursor.NextRow();

	optionsTab->end();

}

void WindowPanel::range_radio_callback(Fl_Widget* widget, void* userData)
{
	std::string* action = static_cast<std::string*>(userData);
	Application::GetInstance()->SetRange(*action);
}

void WindowPanel::solve_button_callback(Fl_Widget* widget, void* userData)
{
	Application* window = static_cast<Application*>(userData);
	window->RunSolver();
}

// Static member function to handle the button callback
void WindowPanel::load_button_callback(Fl_Widget* widget, void* userData)
{
	Application* app = static_cast<Application*>(userData);
	if (app)
	{
		std::string startFolder = "";
		if (!app->window.GetSaveFolder(startFolder))
		{
			startFolder = "";
		}
		RenderUtils::OpenFileDialog(startFolder.c_str(), "sol", [app](const std::string& filePath) {
			app->LoadSolution(filePath);
			});
	}
}