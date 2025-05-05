#pragma once

#include "RenderUtils.h"

#include "Core.h"
#include "Data/Solution.h"

#include "WidgetUI.h"
#include "LabelUI.h"
#include "ButtonUI.h"
#include "InputUI.h"

#include "ScrollHandItemUI.h"
class Application;
class ScrollViewUI :public WidgetUI
{
public:
	ScrollViewUI(RenderCursor& renderCursor, int w, int h, Application* app);

	Fl_Scroll* scrollView = nullptr;
	std::vector<URef<ScrollHandItemUI>> scrollHands;

	void RefreshRange();

	RenderCursor GetStartCursor()override
	{
		RenderCursor cursor;
		cursor.SetStartPos(m_posX, m_posY);
		cursor.spacingX = 10;
		cursor.spacingY = 10;
		cursor.Reset();

		return cursor;
	}
};