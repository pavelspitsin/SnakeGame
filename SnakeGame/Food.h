#pragma once

class GameInfo;

class Food
{
private:
	Position _position;			// Позиция еды
	COLORREF _color;			// Цвет еды
	HPEN _hPen;					// Перо
	HBRUSH _hBrush;				// Кисть

public:
	Food();
	~Food();
	void Update(const GameInfo* gameInfo, bool need_spawn_new_food);
	void Draw(const GameInfo* gameInfo);
	Position GetPosition() const;
};

