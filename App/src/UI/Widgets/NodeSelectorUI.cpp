#include "NodeSelectorUI.h"

#include "Application.h"

NodeSelectorUI::NodeSelectorUI(RenderCursor& cursor, int w, int h, Application* app, Fl_Group* parentGroup) :
	WidgetUI(cursor, w, h)
{

}

void NodeSelectorUI::Refresh(Solution* solution, Fl_Group* parentGroup, Fl_Callback* callback)
{
	nodeButtons.clear();
	nodeLabels.clear();

	RenderCursor thisCursor = GetStartCursor();

	if (solution)
	{
		for (auto& range : solution->AllRanges)
		{
			nodeLabels.emplace_back(std::make_unique<LabelUI>(thisCursor, 70, 20, solution->GetRangeName(range.first)));
			nodeButtons.emplace_back(std::make_unique<RadioButtonUI>(thisCursor, 20, 20, false, callback, (void*)&range.first));
			thisCursor.NextRow();

			parentGroup->add(nodeLabels.back()->m_box);
			parentGroup->add(nodeButtons.back()->m_radio);

			nodeLabels.back()->m_box->redraw();
			nodeButtons.back()->m_radio->redraw();
		}
	}
}