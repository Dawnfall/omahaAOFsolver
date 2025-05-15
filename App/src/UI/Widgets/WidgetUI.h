#pragma once
#include "UI/RenderCursor.h"

class WidgetUI
{
public:
	int m_posX;
	int m_posY;
	int m_width;
	int m_height;

	WidgetUI(RenderCursor& cursor, int w, int h) :
		m_posX(cursor.posX), m_posY(cursor.posY), m_width(w), m_height(h)
	{
		cursor.MoveX(w, h);
	}

	virtual RenderCursor GetStartCursor()
	{
		RenderCursor cursor;
		cursor.SetStartPos(m_posX, m_posY);
		cursor.Reset();

		return cursor;
	}
};