#pragma once

class IDrawable
{
public:
	virtual void Draw(int posX, int poY, int w, int h)const = 0;
};