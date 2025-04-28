#pragma once

#include <FL/Fl_Button.H>
#include <string>

#include "WidgetUI.h"
#include "UI/RenderCursor.h"

class ButtonUI :public WidgetUI
{
public:
	std::string m_label;

	Fl_Button* m_button;
	Fl_Callback* m_callback;
	void* m_ptr;

	ButtonUI(RenderCursor& cursor, int w, int h, const std::string& label, Fl_Callback* callback, void* ptr) :
		WidgetUI(cursor, w, h), m_label(label), m_callback(callback), m_ptr(ptr)
	{
		m_button = new Fl_Button(m_posX, m_posY, m_width, m_height, m_label.c_str());
		m_button->callback(callback, ptr);
	}
};
