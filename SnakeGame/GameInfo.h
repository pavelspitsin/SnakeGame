#pragma once
class GameInfo
{
public:
	HWND hWnd;						// ���������� ����
	HDC hdc;						// ���������� ������������ ����������
	int nMaxXScreen;				// ������ ������
	int nMaxYScreen;				// ������ ������ ������

	int rect_width;					// ������ ���� ������� ���������
	int rect_height;				// ������ ���� ������� ���������

	int grid_width;					// ������ �����
	int grid_height;				// ������ �����

	int margin_width;				// ������ �� ���� �� ����� � ������
	int margin_height_top;			// ������ �� ���� �� ����� � ������
	int margin_height_bottom;		// ������ �� ���� �� ����� � �����

	int cells_width_number;			// ���-�� ����� � ������
	int cells_height_number;		// ���-�� ����� � ������

	int cell_size;					// ������ ����� ������ (� ��������)
};

