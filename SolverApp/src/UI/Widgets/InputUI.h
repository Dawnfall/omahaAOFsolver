#pragma once

#include <string>
#include <FL/Fl_Input.H>
#include "UI/RenderCursor.h"

class InputUI : public Fl_Input
{
public:
	InputUI(RenderCursor& cursor, int w, int h, const std::string& text = "") :
		Fl_Input(cursor.posX, cursor.posY, w, h)
	{
		this->value(text.c_str());
		cursor.MoveX(w, h);
	}

	void SetText(const std::string& newText)
	{
		this->value(newText.c_str());
		this->redraw();
	}

	std::string GetText()
	{
		return std::string(this->value());
	}
};