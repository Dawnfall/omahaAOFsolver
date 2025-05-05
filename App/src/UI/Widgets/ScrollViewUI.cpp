#include "ScrollViewUI.h"

#include "RenderUtils.h"
#include "Core.h"
#include "Application.h"
#include "Solver/PokerUtils.h"

ScrollViewUI::ScrollViewUI(RenderCursor& renderCursor, int w, int h, Application* app) :
	WidgetUI(renderCursor, w, h)
{
	RenderCursor thisCursor = GetStartCursor();

	scrollView = new Fl_Scroll(thisCursor.posX, thisCursor.posY, m_width, m_height);
	scrollView->type(Fl_Scroll::VERTICAL);
	scrollView->box(FL_BORDER_BOX); // Make the scroll visible
	scrollView->color(FL_WHITE);    // Set a background color
	scrollView->end();
	thisCursor.MoveX(m_width, m_height);
	thisCursor.NextRow();

}

void ScrollViewUI::RefreshRange()
{
	scrollHands.clear();
	scrollView->clear();

	RenderCursor thisCursor = GetStartCursor();
	//thisCursor.MoveX(30, 44);
	//thisCursor.NextRow();

	Application* app = Application::GetInstance();
	if (app->currentNode != -1)
	{
		for (int i = 0; i < PokerUtils::rangeSize; i++)
		{
			const auto& [hand, evRef] = app->m_solution->GetHandAndEv(app->currentNode, i);

			scrollHands.emplace_back(std::make_unique<ScrollHandItemUI>(thisCursor, 300, 30, hand,evRef));
			scrollView->add(scrollHands.back()->m_itemGroup);
			thisCursor.NextRow();
		}
		scrollView->end();
	}
	scrollView->redraw();
}



