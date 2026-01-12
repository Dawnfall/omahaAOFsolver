#include "ViewerTabUI.h"

#include "Core.h"
#include "Application.h"
#include "UI/RenderCursor.h"

ViewerTabUI::ViewerTabUI()
{
	Fl_Group* viewerTab = new Fl_Group(0, Constants::tabsHeight, Constants::windowWidth, Constants::windowHeight - Constants::tabsHeight, "Viewer");
	viewerTab->hide();

	RenderCursor cursor;
	cursor.SetStartPos(10, 35);
	cursor.spacingX = 10;
	cursor.spacingY = 10;
	cursor.Reset();

	boardCards[0] = std::make_unique<LabelUI>(cursor, Constants::boardCardX, Constants::boardCardY, "");
	boardCards[0]->SetColor(1, 1, 1);
	boardCards[1] = std::make_unique<LabelUI>(cursor, Constants::boardCardX, Constants::boardCardY, "");
	boardCards[1]->SetColor(1, 1, 1);
	boardCards[2] = std::make_unique<LabelUI>(cursor, Constants::boardCardX, Constants::boardCardY, "");
	boardCards[2]->SetColor(1, 1, 1);
	loadSolutionButton = std::make_unique<ButtonUI>(cursor, Constants::loadButtonX, Constants::loadButtonY, "Load");
	cursor.NextRow();
	scrollView = std::make_unique<ScrollViewUI>(cursor, Constants::scrollViewX, Constants::scrollViewY);
	cursor.NextRow();

	rangeTotalLabel = std::make_unique<LabelUI>(cursor, Constants::scrollViewX, Constants::rangeTotalLabelY, "Total:");
	cursor.NextRow();

	filterInput = std::make_unique<InputUI>(cursor, Constants::scrollViewX, Constants::input_height, "");
	filterInput->when(FL_WHEN_ENTER_KEY);
	cursor.NextRow();

	cursor.SetStartPos(400, 35);
	cursor.Reset();

	m_nodesTitleLabel = std::make_unique<LabelUI>(cursor, 50, Constants::el_height, "Nodes");
	cursor.NextRow();
	nodeSelector = std::make_unique<NodeSelectorUI>(cursor, 90, 30, viewerTab);

	cursor.SetStartPos(700, 35);
	cursor.spacingX = 10;
	cursor.spacingY = 10;
	cursor.Reset();
	trainer = std::make_unique<TrainerUI>(cursor, 500, 300);

	viewerTab->end();

	filterInput->callback([](Fl_Widget* widget, void* userData) {
		Fl_Input* input = static_cast<Fl_Input*>(widget);

		const char* text = input->value();
		URef<FilterNode> handFilter = HandFilter::ParseFilter(text);
		if (handFilter)
		{
			input->color(FL_WHITE);
			Application::GetInstance()->SetFilter(std::move(handFilter));
		}
		else
		{
			input->color(FL_RED);
		}
		}, nullptr);

	loadSolutionButton->callback([](Fl_Widget* widget, void* userData) {
		Application* app = Application::GetInstance();
		if (app)
		{
			std::string startFolder;
			char folder[256];
			if (!app->window.optionsTab->GetSaveFolder(folder, sizeof(folder)))
			{
				startFolder = "";
			}
			startFolder = folder;
			RenderUtils::OpenFileDialog(startFolder, "sol", [app](const std::string& filePath) {
				app->LoadSolution(filePath);
				});
		}
		}, nullptr);

	Application::GetInstance()->OnRangeChange.AddListener([this]() {
		RefreshRange();
		});
	Application::GetInstance()->OnFilterChange.AddListener([this]() {
		RefreshRange();
		});
	Application::GetInstance()->OnSolutionChange.AddListener([this, viewerTab]() {
		Application* app = Application::GetInstance();
		if (app->m_solution)
		{
			phevaluator::Card c0(app->m_solution->Flop[0]);
			phevaluator::Card c1(app->m_solution->Flop[1]);
			phevaluator::Card c2(app->m_solution->Flop[2]);

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
		RefreshRange();
		});
}

void ViewerTabUI::RefreshRange()const
{
	std::vector<URef<IDrawable>> scrollItems;

	Application* app = Application::GetInstance();
	float aiCount = 0;

	float avgEv = 0;
	for (int i = 0; i < app->filteredHands.size(); i++)
	{
		const Hand& hand = app->filteredHands[i];
		float ev = app->filteredEvs[i];

		avgEv += ev;
		if (ev > 0)
			aiCount += 1;

		scrollItems.emplace_back(std::make_unique<HandEvItemUI>(hand, ev));
	}
	scrollView->UpdateItems(std::move(scrollItems));

	avgEv /= app->filteredHands.size();
	std::string rangePercentStr = Utils::FormatFloatToNDecimal(app->filteredHands.size() * 100.0f / PokerUtils::rangeSize, 1);
	std::string aiPercentStr = Utils::FormatFloatToNDecimal(aiCount * 100.0f / app->filteredHands.size(), 1);
	std::string avgEvStr = Utils::FormatFloatToNDecimal(avgEv, 2);
	rangeTotalLabel->SetText("Range %:  " + rangePercentStr + "   AI %:  " + aiPercentStr+ "   EV:  "+ avgEvStr);
}
