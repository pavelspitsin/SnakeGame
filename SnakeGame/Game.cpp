#include "stdafx.h"
#include "Game.h"
#include "Snake.h"
#include "Food.h"
#include "GameInfo.h"
#include "Resource.h"


Game::Game(const GameInfo* gameInfo)
{
	need_spawn_new_food = true;
	is_running = false;

	_gameInfo = gameInfo;
	_snake = NULL;
	_food = NULL;

	score = 0;
}


Game::~Game()
{
	Destroy();
}


void Game::Destroy()
{
	if (_snake) { delete _snake; _snake = NULL; }
	if (_food) { delete _food; _food = NULL; }
}

void Game::Initialization()
{
	Destroy();

	this->_food = new Food();
	this->_snake = new Snake(Settings::SNAKE_START_POSITION, Settings::SNAKE_STEP_TIME, Settings::SNAKE_START_ROTATION);
}

void Game::Update(DWORD deltaTime)
{
	if (is_running)
	{
		this->_food->Update(_gameInfo, need_spawn_new_food);
		need_spawn_new_food = false;

		this->_snake->Update(_gameInfo, deltaTime, _food);
	}
}

void Game::Draw()
{
	Clear();
	
	if (is_running)
	{
		if (is_game_over)
		{
			DrawGameOverLogo();
			DrawScore();
		}
		else
		{
			_food->Draw(_gameInfo);
			_snake->Draw(_gameInfo);
			DrawCells();
			DrawScore();
		}
	}
	else
	{
		DrawLogo();
	}

	InvalidateRect(_gameInfo->hWnd, NULL, FALSE);
}

void Game::ProcessMessages(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case ID_GAME_INPUT:
	{
		switch (lParam)
		{
		case Rotation::LEFT:
			this->_snake->SetRotation(Rotation::LEFT);
			break;
		case Rotation::UP:
			this->_snake->SetRotation(Rotation::UP);
			break;
		case Rotation::RIGHT:
			this->_snake->SetRotation(Rotation::RIGHT);
			break;
		case Rotation::DOWN:
			this->_snake->SetRotation(Rotation::DOWN);
			break;
		default:
			break;
		}
		break;
	}
	case ID_GAME_OVER:
		is_game_over = true;
		break;
	case ID_FOOD_ATE:
		need_spawn_new_food = true;
		score++;
	break;
	}
}

void Game::Input_KeyDown(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case VK_LEFT:
		PostMessage(_gameInfo->hWnd, WM_GAME, ID_GAME_INPUT, Rotation::LEFT);
		break;
	case VK_UP:
		PostMessage(_gameInfo->hWnd, WM_GAME, ID_GAME_INPUT, Rotation::UP);
		break;
	case VK_RIGHT:
		PostMessage(_gameInfo->hWnd, WM_GAME, ID_GAME_INPUT, Rotation::RIGHT);
		break;
	case VK_DOWN:
		PostMessage(_gameInfo->hWnd, WM_GAME, ID_GAME_INPUT, Rotation::DOWN);
		break;
	default:
		break;
	}
}

void Game::Start()
{
	is_game_over = false;
	is_running = true;
	need_spawn_new_food = true;
	score = 0;
	Initialization();
}

void Game::Stop()
{
	is_running = false;
}


void Game::Clear()
{
	SelectObject(_gameInfo->hdc, GetStockObject(DC_BRUSH));
	PatBlt(_gameInfo->hdc, 0, 0, _gameInfo->nMaxXScreen, _gameInfo->nMaxYScreen, PATCOPY);
}

void Game::DrawCells()
{
	HPEN hRedPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	HGDIOBJ prevPen = SelectObject(_gameInfo->hdc, hRedPen);

	int margin_width = _gameInfo->margin_width;
	int margin_height_top = _gameInfo->margin_height_top;
	int width = _gameInfo->grid_width;
	int height = _gameInfo->grid_height;
	int cell_size = _gameInfo->cell_size;

	// + 1 - погрешность округления
	for (int i = 0; i < _gameInfo->grid_width + 1; ++i)
	{
		MoveToEx(_gameInfo->hdc, margin_width + i, margin_height_top, NULL);
		LineTo	(_gameInfo->hdc, margin_width + i, margin_height_top + height);
		i += cell_size - 1;
	}


	// + 1 - погрешность округления
	for (int i = 0; i < _gameInfo->grid_height + 1; ++i)
	{
		MoveToEx(_gameInfo->hdc, margin_width, margin_height_top + i, NULL);
		LineTo(_gameInfo->hdc, margin_width + width, margin_height_top + i);
		i += cell_size - 1;
	}

	SelectObject(_gameInfo->hdc, prevPen);
}


void Game::DrawLogo()
{
	int height = 100;

	HFONT font = CreateFont(
		height,					// Устанавливает высоту шрифта или символа
		0,						// Устанавливает среднюю ширину символов в шрифте
		0,						// Устанавливает угол, между вектором наклона и осью X устройства
		0,						// Устанавливает угол, между основной линией каждого символа и осью X устройства
		100,					// Устанавливает толщину шрифта в диапазоне от 0 до 1000
		0,						// Устанавливает курсивный шрифт
		0,						// Устанавливает подчеркнутый шрифт
		0,						// Устанавливает зачеркнутый шрифт
		RUSSIAN_CHARSET,		// Устанавливает набор символов
		0,						// Устанавливает точность вывода
		0,						// Устанавливает точность отсечения
		0,						// Устанавливает качество вывода
		0,						// Устанавливает ширину символов и семейство шрифта
		L"Comic Sans MS"		// устанавливает название шрифта
	);

	UINT prevAlignText = SetTextAlign(_gameInfo->hdc, VTA_CENTER);

	HGDIOBJ defaultFont = SelectObject(_gameInfo->hdc, font);
	TCHAR text[256];
	swprintf_s(text, 256, L"Snake");

	int x = _gameInfo->rect_width / 2;
	int y = _gameInfo->rect_height / 2 - height;

	TextOut(_gameInfo->hdc, x, y, text, wcslen(text));

	SetTextAlign(_gameInfo->hdc, prevAlignText);
	SelectObject(_gameInfo->hdc, defaultFont);
}

void Game::DrawGameOverLogo()
{
	int height = 100;

	HFONT font = CreateFont(
		height,					// Устанавливает высоту шрифта или символа
		0,						// Устанавливает среднюю ширину символов в шрифте
		0,						// Устанавливает угол, между вектором наклона и осью X устройства
		0,						// Устанавливает угол, между основной линией каждого символа и осью X устройства
		100,					// Устанавливает толщину шрифта в диапазоне от 0 до 1000
		0,						// Устанавливает курсивный шрифт
		0,						// Устанавливает подчеркнутый шрифт
		0,						// Устанавливает зачеркнутый шрифт
		RUSSIAN_CHARSET,		// Устанавливает набор символов
		0,						// Устанавливает точность вывода
		0,						// Устанавливает точность отсечения
		0,						// Устанавливает качество вывода
		0,						// Устанавливает ширину символов и семейство шрифта
		L"Comic Sans MS"		// устанавливает название шрифта
	);

	UINT prevAlignText = SetTextAlign(_gameInfo->hdc, VTA_CENTER);
	COLORREF prevColor = SetTextColor(_gameInfo->hdc, RGB(255, 0, 0));

	HGDIOBJ defaultFont = SelectObject(_gameInfo->hdc, font);
	TCHAR text[256];
	swprintf_s(text, 256, L"Game Over");

	int x = _gameInfo->rect_width / 2;
	int y = _gameInfo->rect_height / 2 - height;

	TextOut(_gameInfo->hdc, x, y, text, wcslen(text));

	SetTextAlign(_gameInfo->hdc, prevAlignText);
	SetTextColor(_gameInfo->hdc, prevColor);
	SelectObject(_gameInfo->hdc, defaultFont);
}

void Game::DrawScore()
{
	int height = 40;

	HFONT font = CreateFont(
		height,					// Устанавливает высоту шрифта или символа
		0,						// Устанавливает среднюю ширину символов в шрифте
		0,						// Устанавливает угол, между вектором наклона и осью X устройства
		0,						// Устанавливает угол, между основной линией каждого символа и осью X устройства
		100,					// Устанавливает толщину шрифта в диапазоне от 0 до 1000
		0,						// Устанавливает курсивный шрифт
		0,						// Устанавливает подчеркнутый шрифт
		0,						// Устанавливает зачеркнутый шрифт
		RUSSIAN_CHARSET,		// Устанавливает набор символов
		0,						// Устанавливает точность вывода
		0,						// Устанавливает точность отсечения
		0,						// Устанавливает качество вывода
		0,						// Устанавливает ширину символов и семейство шрифта
		L"Comic Sans MS"		// устанавливает название шрифта
	);

	UINT prevAlignText = SetTextAlign(_gameInfo->hdc, VTA_CENTER);

	HGDIOBJ defaultFont = SelectObject(_gameInfo->hdc, font);
	TCHAR text[256];
	swprintf_s(text, 256, L"Score : %d", score);

	int x = _gameInfo->rect_width / 2;
	int y = 10;

	TextOut(_gameInfo->hdc, x, y, text, wcslen(text));

	SetTextAlign(_gameInfo->hdc, prevAlignText);
	SelectObject(_gameInfo->hdc, defaultFont);
}
