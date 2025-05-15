#pragma once

#include <FL/Fl_Button.H>
#include <string>

#include "UI/RenderCursor.h"

class ButtonUI :public Fl_Button
{
public:
	std::string m_label;

	ButtonUI(RenderCursor& cursor, int w, int h, const std::string& label) :
		Fl_Button(cursor.posX, cursor.posY, w,h),
		m_label(label)
	{
		this->label(m_label.c_str());
		cursor.MoveX(w, h);
	}
};
