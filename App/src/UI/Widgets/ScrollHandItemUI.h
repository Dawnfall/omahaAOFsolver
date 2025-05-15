//#pragma once
//#include "WidgetUI.h"
//#include "LabelUI.h"
//#include "Core.h"
//#include "Utils.h"
//#include "Constants.h"
//
//class ScrollHandItemUI :public WidgetUI
//{
//public:
//	std::string text;
//
//	Fl_Group* m_itemGroup;
//	std::array<URef<LabelUI>, 4> m_hand;
//	URef<LabelUI> m_evLabel;
//
//	ScrollHandItemUI(RenderCursor& cursor, int w, int h, const Hand& hand,float ev) :
//		WidgetUI(cursor, w, h)
//	{
//		m_itemGroup = new Fl_Group(m_posX, m_posY, w, h);
//
//		RenderCursor thisCursor;
//		thisCursor.SetStartPos(m_posX, m_posY);
//		thisCursor.spacingX = 10;
//		thisCursor.spacingY = 10;
//		thisCursor.Reset();
//
//		phevaluator::Card c0(hand[0]);
//		phevaluator::Card c1(hand[1]);
//		phevaluator::Card c2(hand[2]);
//		phevaluator::Card c3(hand[3]);
//
//		m_hand[0] = std::make_unique<LabelUI>(thisCursor, Constants::scrollCardLen, Constants::scrollitemHeight, c0.describeCard().substr(0,1));
//		m_hand[0]->SetColor(RenderUtils::GetCardColor(c0));
//		m_hand[1] = std::make_unique<LabelUI>(thisCursor, Constants::scrollCardLen, Constants::scrollitemHeight, c1.describeCard().substr(0, 1));
//		m_hand[1]->SetColor(RenderUtils::GetCardColor(c1));
//		m_hand[2] = std::make_unique<LabelUI>(thisCursor, Constants::scrollCardLen, Constants::scrollitemHeight, c2.describeCard().substr(0, 1));
//		m_hand[2]->SetColor(RenderUtils::GetCardColor(c2));
//		m_hand[3] = std::make_unique<LabelUI>(thisCursor, Constants::scrollCardLen, Constants::scrollitemHeight, c3.describeCard().substr(0, 1));
//		m_hand[3]->SetColor(RenderUtils::GetCardColor(c3));
//		thisCursor.MoveX(50, 0);
//
//		m_evLabel = std::make_unique<LabelUI>(thisCursor, Constants::scollItemEvLen, Constants::scrollitemHeight, Utils::FormatFloatToNDecimal(ev,2));
//		if (ev > 0)
//			m_evLabel->SetColor(FL_GREEN);
//		else
//			m_evLabel->SetColor(FL_RED);
//		m_itemGroup->end();
//	}
//};