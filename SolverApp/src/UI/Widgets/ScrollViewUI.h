#pragma once

#include "Core.h"
#include "FL/Fl_Scroll.H"
#include <FL/fl_Box.h>
#include "UI/RenderCursor.h"
#include "UI/IDrawable.h"

class ScrollViewUI :public Fl_Scroll
{
	std::vector<URef<IDrawable>> m_items;
	Fl_Box* m_dummyContentBox=nullptr;
public:
	ScrollViewUI(RenderCursor& renderCursor, int w, int h);

	void UpdateItems(std::vector<URef<IDrawable>> items);

private:
	void draw()override;
};