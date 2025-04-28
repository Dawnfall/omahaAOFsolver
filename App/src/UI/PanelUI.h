#pragma once
#include "RenderCursor.h"

class Application;
class PanelUI
{
public:
	RenderCursor m_cursor;

	virtual int Render(Application* app) = 0;
};