#include "NodeSelectorUI.h"

#include "Core.h"
#include "Application.h"
#include "PokerUtils.h"

NodeSelectorUI::NodeSelectorUI(RenderCursor& cursor, int w, int h, Fl_Group* parentGroup) :
	WidgetUI(cursor, w, h), m_parentGroup(parentGroup)
{
	Application::GetInstance()->OnSolutionChange.AddListener([this]() {
		Refresh();
		});
}

void NodeSelectorUI::Refresh()
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
			std::shared_ptr<int> rangeIndexPtr = std::make_shared<int>(rangeIndex);

			nodeLabels.emplace_back(std::make_unique<LabelUI>(thisCursor, 150, 20, app->m_solution->GetRangeName(rangeIndex)));
			nodeButtons.emplace_back(std::make_unique<RadioButtonUI>(thisCursor, 20, 20, false));

			nodeButtons.back()->callback(
				[](Fl_Widget* widget, void* userData) {
					int* rangeIndexPtr = static_cast<int*>(userData);
					Application::GetInstance()->SetRange(*rangeIndexPtr);
				}, &rangeIndices[rangeIndex]);

			m_parentGroup->add(nodeLabels.back().get());
			m_parentGroup->add(nodeButtons.back().get());

			nodeLabels.back()->redraw();
			nodeButtons.back()->redraw();

			thisCursor.NextRow();
		}
	}
}