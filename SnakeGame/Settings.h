#pragma once

class Settings
{
public:
	static const int WINDOW_WIDTH = 600;							// Ширина окна
	static const int WINDOW_HEIGHT = 600;							// Высота окна

	static const int CELLS_WIDTH_NUMBER = 25;						// Кол-во ячеек в шиирну
	static const int CELLS_HEIGHT_NUMBER = 20;						// Кол-во ячеек в высоту
	static const int MARGIN_TOP = 80;								// Отсутп от верха окна до ячеек (для очков)

	static const float SNAKE_STEP_TIME;								// See Settings.cpp
	static const Position SNAKE_START_POSITION;						// See Settings.cpp
	static const Rotation SNAKE_START_ROTATION = Rotation::RIGHT;	// Стартовый поворот змейки
};


