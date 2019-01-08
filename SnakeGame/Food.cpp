#include "stdafx.h"
#include "Food.h"
#include "GameInfo.h"


Food::Food()
{
	_color = RGB(255, 0, 0);
}


Food::~Food()
{
}

void Food::Update(const GameInfo* gameInfo, bool need_spawn_new_food)
{
	if (need_spawn_new_food)
	{
		_position.x = rand() % gameInfo->cells_width_number;
		_position.y = rand() % gameInfo->cells_height_number;
	}
}

void Food::Draw(const GameInfo* gameInfo)
{
	HGDIOBJ prevBrush = NULL;
	HGDIOBJ prevPen = NULL;
	HBRUSH hBrush = CreateSolidBrush(this->_color);
	HPEN hPen = CreatePen(PS_SOLID, 1, this->_color);
	prevBrush = SelectObject(gameInfo->hdc, hBrush);
	prevPen = SelectObject(gameInfo->hdc, hPen);

	int margin_width = gameInfo->margin_width;
	int margin_height_top = gameInfo->margin_height_top;
	int margin_height_bottob = gameInfo->margin_height_bottom;
	int cell_size = gameInfo->cell_size;

	// Draw head
	int x = _position.x * cell_size + margin_width;
	int y = _position.y * cell_size + margin_height_top;

	Rectangle(gameInfo->hdc, x, y, x + cell_size, y + cell_size);

	SelectObject(gameInfo->hdc, prevBrush);
	SelectObject(gameInfo->hdc, prevPen);
	DeleteBrush(hPen);
	DeleteBrush(hBrush);
}

Position Food::GetPosition() const
{
	return _position;
}
