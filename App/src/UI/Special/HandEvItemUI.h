#pragma once

#include "Core.h"
#include "Utils.h"
#include "RenderUtils.h"

#include "FL/fl_draw.H"
#include "UI/IDrawable.h"
#include "Constants.h"

class HandEvItemUI :public IDrawable
{
public:
	int cardSpacing = 2;
	int firstCardOffset = 5;
	int evSpacing = 100;

	std::array<std::string, 5> m_labels; //4 for ranks,1 for ev
	std::array<Fl_Color, 5> m_colors; //4 for ranks,1 for ev
	std::array<int, 5> textWidths; //4 for ranks, 1 for ev
	std::array<int, 5> textHeights; //4 for ranks , 1 for ev

	HandEvItemUI(const Hand& hand, float ev)
	{
		phevaluator::Card c0(hand[0]);
		phevaluator::Card c1(hand[1]);
		phevaluator::Card c2(hand[2]);
		phevaluator::Card c3(hand[3]);

		for (int i = 0; i < 4; i++)
		{
			phevaluator::Card ci(hand[i]);
			m_colors[i] = RenderUtils::GetCardColor(ci);
			m_labels[i] = ci.describeCard().substr(0, 1);
			textWidths[i] = static_cast<int>(fl_width(m_labels[i].c_str()));
			textHeights[i] = fl_height();
		}

		m_labels[4] = Utils::FormatFloatToNDecimal(ev, 2);
		m_colors[4] = (ev < 0) ? FL_RED : FL_GREEN;
		textWidths[4] = static_cast<int>(fl_width(m_labels[4].c_str()));
		textHeights[4] = fl_height();
	}

	void Draw(int x, int y, int w, int h)const override
	{
		fl_color(FL_BLACK);
		fl_rect(x, y, w, h);

		int nextPosX = x + firstCardOffset;
		for (int i = 0; i < 4; i++)
		{
			fl_color(m_colors[i]);
			fl_rectf(nextPosX, y, Constants::scrollCardLen, Constants::scrollitemHeight);
			fl_color(FL_BLACK);
			fl_rect(nextPosX, y, Constants::scrollCardLen, Constants::scrollitemHeight);
			fl_draw(
				m_labels[i].c_str(),
				nextPosX + (Constants::scrollCardLen - textWidths[i]) / 2,
				y + (Constants::scrollitemHeight + textHeights[i]) / 2 - fl_descent()
			);
			nextPosX += cardSpacing + Constants::scrollCardLen;
		}
		nextPosX += evSpacing;
		fl_color(m_colors[4]);
		fl_rectf(nextPosX, y, Constants::scollItemEvLen, Constants::scrollitemHeight);
		fl_color(FL_BLACK);
		fl_rect(nextPosX, y, Constants::scollItemEvLen, Constants::scrollitemHeight);
		fl_draw(
			m_labels[4].c_str(),
			nextPosX + (Constants::scollItemEvLen - textWidths[4]) / 2,
			y + (Constants::scrollitemHeight + textHeights[4]) / 2 - fl_descent()
		);
	}
};