#pragma once

#include <string>
#include "FL/Fl_Group.H"
#include "FL/Fl_Box.H"
#include "UI/RenderCursor.h"

class LabelUI :public Fl_Box
{
public:
	std::string m_label;

	LabelUI(RenderCursor& cursor, int w, int h, const std::string& text) :
		Fl_Box(cursor.posX, cursor.posY, w, h),
		m_label(text)
	{
		this->label(m_label.c_str());
		cursor.MoveX(w, h);
	}

	~LabelUI()
	{
		if (parent())
			parent()->remove(this);
	}

	void SetText(const std::string& newText)
	{
		m_label = newText;
		this->label(m_label.c_str());
		this->redraw();
	}

	void SetColor(unsigned int r, unsigned int g, unsigned int b)
	{
		SetColor(fl_rgb_color(r, g, b));
	}

	void SetColor(Fl_Color color)
	{
		this->box(FL_FLAT_BOX);
		this->color(color);
		this->redraw();
	}

};