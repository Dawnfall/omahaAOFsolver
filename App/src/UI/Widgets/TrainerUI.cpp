#include "TrainerUI.h"

#include "Application.h"


TrainerUI::TrainerUI(RenderCursor& cursor, int w, int h) :
	WidgetUI(cursor, w, h)
{
	RenderCursor thisCursor = GetStartCursor();

	m_resetButton = std::make_unique<ButtonUI>(thisCursor, 30, 30, "R");
	m_scoreLabel = std::make_unique<LabelUI>(thisCursor, 100, 30, "Score:");
	thisCursor.NextRow();

	m_handLabels[0] = std::make_unique<LabelUI>(thisCursor, Constants::boardCardX, Constants::boardCardY, "");
	m_handLabels[1] = std::make_unique<LabelUI>(thisCursor, Constants::boardCardX, Constants::boardCardY, "");
	m_handLabels[2] = std::make_unique<LabelUI>(thisCursor, Constants::boardCardX, Constants::boardCardY, "");
	m_handLabels[3] = std::make_unique<LabelUI>(thisCursor, Constants::boardCardX, Constants::boardCardY, "");
	thisCursor.NextRow();

	m_foldButton = std::make_unique<ButtonUI>(thisCursor, 60, 30, "Fold"); 
	m_aiButton = std::make_unique<ButtonUI>(thisCursor, 60, 30, "All In"); 
	thisCursor.NextRow();

	m_resultLabel = std::make_unique<LabelUI>(thisCursor, 140, 30, "");

	m_foldButton->callback([](Fl_Widget* widget, void* userData) {
		TrainerUI* trainer = static_cast<TrainerUI*>(userData);
		if (trainer->isCurrentActive)
			trainer->ScoreCurrent(false);
		else
			trainer->SetNewHand();
		}, this);
	m_aiButton->callback([](Fl_Widget* widget, void* userData) {
		TrainerUI* trainer = static_cast<TrainerUI*>(userData);
		if (trainer->isCurrentActive)
			trainer->ScoreCurrent(true);
		else
			trainer->SetNewHand();
		}, this);
	m_resetButton->callback([](Fl_Widget* callback, void* userData) {
		TrainerUI* trainer = static_cast<TrainerUI*>(userData);
		trainer->ResetScore();
		trainer->SetNewHand();
		}, this);

	Application::GetInstance()->OnRangeChange.AddListener([this]()
		{
			SetNewHand();
		});

	Clear();
}

void TrainerUI::SetNewHand()
{
	RandomGenerator randGen;
	Application* app = Application::GetInstance();
	if (app->currentRange != -1)
	{
		auto [_hand, _ev] = app->m_solution->GetRandomHandAndEv(app->currentRange, randGen);

		this->hand = _hand;
		this->ev = _ev;

		phevaluator::Card c0(hand[0]);
		phevaluator::Card c1(hand[1]);
		phevaluator::Card c2(hand[2]);
		phevaluator::Card c3(hand[3]);

		m_handLabels[0]->SetColor(RenderUtils::GetCardColor(c0));
		m_handLabels[0]->SetText(c0.describeCard().substr(0, 1));
		m_handLabels[1]->SetColor(RenderUtils::GetCardColor(c1));
		m_handLabels[1]->SetText(c1.describeCard().substr(0, 1));
		m_handLabels[2]->SetColor(RenderUtils::GetCardColor(c2));
		m_handLabels[2]->SetText(c2.describeCard().substr(0, 1));
		m_handLabels[3]->SetColor(RenderUtils::GetCardColor(c3));
		m_handLabels[3]->SetText(c3.describeCard().substr(0, 1));

		m_resultLabel->SetText(Utils::FormatFloatToNDecimal(ev, 2));
		m_resultLabel->hide();
		isCurrentActive = true;
	}
}

void TrainerUI::Clear()
{
	m_handLabels[0]->SetColor(FL_BLACK);
	m_handLabels[1]->SetColor(FL_BLACK);
	m_handLabels[2]->SetColor(FL_BLACK);
	m_handLabels[3]->SetColor(FL_BLACK);

	UpdateScoreLabel();
	m_resultLabel->hide();
}

void TrainerUI::ScoreCurrent(bool guess)
{
	totalCount += 1;
	if ((ev > 0 && guess) || (ev < 0 && !guess))
	{
		correctCount += 1;
		m_resultLabel->SetColor(FL_GREEN);
	}
	else
		m_resultLabel->SetColor(FL_RED);

	UpdateScoreLabel();
	m_resultLabel->show();
	isCurrentActive = false;
}

RenderCursor TrainerUI::GetStartCursor()
{
	RenderCursor cursor;
	cursor.SetStartPos(m_posX, m_posY);
	cursor.spacingX = 10;
	cursor.spacingY = 10;
	cursor.Reset();

	return cursor;
}