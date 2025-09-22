#pragma once
class RenderCursor
{
public:
	int spacingX = 0;
	int spacingY = 0;

	int posX = 0;
	int posY = 0;

	void SetStartPos(int posx, int posy)
	{
		_posX = posx;
		_posY = posy;
	}

	void Reset()
	{
		posX = _posX + spacingX;
		posY = _posY + spacingY;
	}

	void MoveX(int x, int h)
	{
		posX += x + spacingX;
		if (h > _currentRowHeight)
			_currentRowHeight = h;
	}

	void NextRow()
	{
		posX = _posX + spacingX;
		posY += _currentRowHeight + spacingY;
		_currentRowHeight = 0;
	}

private:
	int _posX = 0;
	int _posY = 0;
	int _currentRowHeight = 0;
};