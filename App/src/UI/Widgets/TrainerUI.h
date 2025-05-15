#pragma once

#include "WidgetUI.h"
#include "LabelUI.h"
#include "ButtonUI.h"
#include "Constants.h"
#include "RenderUtils.h"

#include "Core.h"

class TrainerUI :public WidgetUI
{
public:
	URef<ButtonUI> m_resetButton;
	URef<LabelUI> m_scoreLabel;
	std::array<URef<LabelUI>, 4> m_handLabels;
	URef<ButtonUI> m_foldButton;
	URef<ButtonUI> m_aiButton;
	URef<LabelUI> m_resultLabel;

	Hand hand;
	float ev;

	unsigned int correctCount = 0;
	unsigned int totalCount = 0;
	bool isCurrentActive = false;

	TrainerUI(RenderCursor& cursor, int w, int h);

	void SetNewHand();

	void Clear();

	void ScoreCurrent(bool guess);

	void ResetScore()
	{
		totalCount = 0;
		correctCount = 0;

		UpdateScoreLabel();
		m_resultLabel->hide();
	}


	RenderCursor GetStartCursor() override;

private:
	void UpdateScoreLabel()
	{
		m_scoreLabel->SetText(std::to_string(correctCount) + " / " + std::to_string(totalCount));
	}
};