#pragma once
class GameInfo
{
public:
	HWND hWnd;						// Дескриптор окна
	HDC hdc;						// Дескриптор графического устройства
	int nMaxXScreen;				// Ширина экрана
	int nMaxYScreen;				// Ширина высота экрана

	int rect_width;					// Ширина всей области рисования
	int rect_height;				// Высота всей области рисования

	int grid_width;					// Ширина сетки
	int grid_height;				// Высота сетки

	int margin_width;				// Отсутп от края до сетки в ширину
	int margin_height_top;			// Отсутп от края до сетки в сверху
	int margin_height_bottom;		// Отсутп от края до сетки в снизу

	int cells_width_number;			// Кол-во ячеек в шиирну
	int cells_height_number;		// Кол-во ячеек в высоту

	int cell_size;					// Размер одной ячейки (в пикселях)
};

