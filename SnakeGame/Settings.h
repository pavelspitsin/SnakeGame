#pragma once

class Settings
{
public:
	static const int WINDOW_WIDTH = 600;							// ������ ����
	static const int WINDOW_HEIGHT = 600;							// ������ ����

	static const int CELLS_WIDTH_NUMBER = 25;						// ���-�� ����� � ������
	static const int CELLS_HEIGHT_NUMBER = 20;						// ���-�� ����� � ������
	static const int MARGIN_TOP = 80;								// ������ �� ����� ���� �� ����� (��� �����)

	static const float SNAKE_STEP_TIME;								// See Settings.cpp
	static const Position SNAKE_START_POSITION;						// See Settings.cpp
	static const Rotation SNAKE_START_ROTATION = Rotation::RIGHT;	// ��������� ������� ������
};


