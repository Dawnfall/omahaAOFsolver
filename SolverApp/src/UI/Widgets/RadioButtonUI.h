#pragma once

#include "FL/Fl_Radio_Round_Button.H"
#include "FL/Fl_Radio_Button.H"
#include "UI/RenderCursor.h"

class RadioButtonUI :public Fl_Round_Button
{
public:
	RadioButtonUI(RenderCursor& cursor, int w, int h, bool isOn, Fl_Callback* cb = nullptr, void* ptr = nullptr) :
		Fl_Round_Button(cursor.posX, cursor.posY, w, h)
	{
		type(FL_RADIO_BUTTON);
		if (isOn)
			setonly();

		callback(cb, ptr);
		cursor.MoveX(w, h);
	}

	~RadioButtonUI()
	{
		if (parent())
			parent()->remove(this);
	}

	bool IsOn()
	{
		return value();
	}
};
