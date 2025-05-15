#include "ScrollViewUI.h"
#include "RenderUtils.h"
#include "Core.h"
#include "Application.h"
#include "Solver/PokerUtils.h"

#include "Filter/HandFilter.h"
#include <FL/fl_draw.H>
#include <FL/fl_Box.h>
#include <FL/fl_draw.H>

ScrollViewUI::ScrollViewUI(RenderCursor& cursor, int width, int height) :
	Fl_Scroll(cursor.posX, cursor.posY, width, height)
{
	type(Fl_Scroll::VERTICAL);
	box(FL_BORDER_BOX); // Make the scroll visible
	color(FL_WHITE);    // Set a background color

	m_dummyContentBox = new Fl_Box(0, 0, this->w(), this->h());
	m_dummyContentBox->hide();
	add(m_dummyContentBox);
	end();
	cursor.MoveX(width, height);
}

void ScrollViewUI::UpdateItems(std::vector<URef<IDrawable>> items)
{
	m_items = std::move(items);
	m_dummyContentBox->resize(0, 0, w(), m_items.size() * Constants::scrollitemHeight);
	redraw();
}

void ScrollViewUI::draw()
{
	fl_color(FL_WHITE); // or use this->color() for the widget's set color
	fl_rectf(x(), y(), w()-Fl::scrollbar_size(), h());

	Fl_Scroll::draw(); // draw scrollbars and clip area

	int first = yposition() / Constants::scrollitemHeight;
	int last = first + h() / Constants::scrollitemHeight;

	first = std::max(0, first);
	last = std::min(last, static_cast<int>(m_items.size()));

	for (int i = first; i < last; ++i)
	{
		int iy = this->y() + (i - first) * Constants::scrollitemHeight;
		m_items[i]->Draw(x(), iy, w() - Fl::scrollbar_size(), Constants::scrollitemHeight);
	}
}






