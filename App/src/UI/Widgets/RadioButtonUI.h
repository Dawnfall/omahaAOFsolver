#pragma once

#include "WidgetUI.h"
#include "FL/Fl_Radio_Round_Button.H"
#include "FL/Fl_Radio_Button.H"

class RadioButtonUI :public WidgetUI
{
private:
	Fl_Callback* m_callback = nullptr;
	void* m_userData;

public:
	Fl_Round_Button* m_radio;

	RadioButtonUI(RenderCursor& cursor, int w, int h, bool isOn, Fl_Callback* callback = nullptr, void* userData = nullptr) :
		WidgetUI(cursor, w, h), m_callback(callback), m_userData(userData)
	{
		m_radio = new Fl_Round_Button(m_posX, m_posY, m_width, m_height);
		m_radio->type(FL_RADIO_BUTTON);
		if (isOn)
			m_radio->setonly();
		SetCallback(m_callback, m_userData);
	}

	~RadioButtonUI()
	{
		if (m_radio)
		{
			if (m_radio->parent())
				m_radio->parent()->remove(m_radio);
			delete m_radio;
		}
	}

	bool IsOn()
	{
		return m_radio->value();
	}

	void SetCallback(Fl_Callback* callback, void* userData)
	{
		m_callback = callback;
		m_userData = userData;

		if (m_callback)
			m_radio->callback(m_callback, m_userData);
	}
};
