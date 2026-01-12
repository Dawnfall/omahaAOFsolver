#pragma once
#include "RenderUtils.h"

#include "Data/Solution.h"
//#include "Data/WeightedHand.h"
#include <vector>

#include <FL/Fl_Group.H>

#include "WidgetUI.h"
#include "LabelUI.h"
#include "RadioButtonUI.h"

class NodeSelectorUI:public WidgetUI
{
public:
	NodeSelectorUI(RenderCursor& cursor, int w, int h, Fl_Group* parentgroup);
	std::vector<URef<RadioButtonUI>> nodeButtons;
	std::vector<URef<LabelUI>> nodeLabels;
	std::vector<int> rangeIndices;
	Fl_Group* m_parentGroup;

	void Refresh();

	RenderCursor GetStartCursor() override
	{
		RenderCursor cursor;
		cursor.SetStartPos(m_posX, m_posY);
		cursor.spacingX = 10;
		cursor.spacingY = 10;
		cursor.Reset();

		return cursor;
	}
};