#pragma once

#include <string>
#include "FL/Fl_Group.H"
#include "FL/Fl_Box.H"
#include "WidgetUI.h"

class LabelUI :public WidgetUI
{
public:
	std::string m_label;

	Fl_Box* m_box;

	LabelUI(RenderCursor& cursor, int w, int h, const std::string& text) :
		WidgetUI(cursor, w, h), m_label(text)
	{
		m_box = new Fl_Box(m_posX, m_posY, m_width, m_height, m_label.c_str());
	}
	~LabelUI()
	{
		if (m_box)
		{
			if (m_box->parent())
			{
				m_box->parent()->remove(m_box);
				if (m_box->parent())
					int a = 4;
			}
			delete m_box;
		}
	}

	void SetText(const std::string& newText)
	{
		m_label = newText;
		m_box->label(m_label.c_str());
		m_box->redraw();
	}

	void SetColor(unsigned int r, unsigned int g, unsigned int b)
	{
		SetColor(fl_rgb_color(r, g, b));
	}

	void SetColor(Fl_Color color)
	{
		m_box->box(FL_FLAT_BOX);
		m_box->color(color);
		m_box->redraw();
	}

};