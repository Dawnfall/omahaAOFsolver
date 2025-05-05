#include "NodeSelectorUI.h"

#include "Core.h"
#include "Application.h"
#include "Solver/PokerUtils.h"

NodeSelectorUI::NodeSelectorUI(RenderCursor& cursor, int w, int h, Application* app, Fl_Group* parentGroup) :
	WidgetUI(cursor, w, h)
{

}

void NodeSelectorUI::Refresh(Fl_Group* parentGroup)
{
	nodeButtons.clear();
	nodeLabels.clear();

	RenderCursor thisCursor = GetStartCursor();

	Application* app = Application::GetInstance();
	if (app->m_solution)
	{
		int totalRanges = PokerUtils::GetTotalRanges(app->m_solution->PlayerCount);
		rangeIndices.clear(); rangeIndices.reserve(totalRanges);
		for (int rangeIndex = 0; rangeIndex < totalRanges; rangeIndex++)
		{
			rangeIndices.emplace_back(rangeIndex);
			std::shared_ptr<int> rangeIndexPtr= std::make_shared<int>(rangeIndex);

			nodeLabels.emplace_back(std::make_unique<LabelUI>(thisCursor, 70, 20, app->m_solution->GetRangeName(rangeIndex)));
			nodeButtons.emplace_back(std::make_unique<RadioButtonUI>(thisCursor, 20, 20, false));

			nodeButtons.back()->SetCallback(
				[](Fl_Widget* widget, void* userData) {
					int* rangeIndexPtr = static_cast<int*>(userData);
					Application::GetInstance()->SetNode(*rangeIndexPtr);
				}, &rangeIndices[rangeIndex]); 

			parentGroup->add(nodeLabels.back()->m_box);
			parentGroup->add(nodeButtons.back()->m_radio);

			nodeLabels.back()->m_box->redraw();
			nodeButtons.back()->m_radio->redraw();

			thisCursor.NextRow();
		}
	}
}