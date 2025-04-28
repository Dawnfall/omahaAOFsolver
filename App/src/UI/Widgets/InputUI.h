#pragma once

#include "WidgetUI.h"
#include <string>
#include <FL/Fl_Input.H>

class InputUI : public WidgetUI
{
public:
	std::string m_value;
	Fl_Input* m_input;

	InputUI(RenderCursor& cursor, int w, int h, const std::string& value = "") :
		WidgetUI(cursor, w, h), m_value(value)
	{
		m_input = new Fl_Input(m_posX,m_posY,m_width,m_height);
		m_input->value(m_value.c_str());
	}


};