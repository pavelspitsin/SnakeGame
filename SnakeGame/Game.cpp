#include "stdafx.h"
#include "Game.h"
#include "Snake.h"
#include "Food.h"
#include "GameInfo.h"
#include "Resource.h"


Game::Game(const GameInfo* gameInfo)
{
	_hGridPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));

	_hLogoFont = CreateFont(
		100,					// Устанавливает высоту шрифта или символа
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

	_hScoreFont = CreateFont(
		40,					// Устанавливает высоту шрифта или символа
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



	_need_spawn_new_food = true;
	_is_running = false;

	_gameInfo = gameInfo;
	_snake = NULL;
	_food = NULL;

	_score = 0;
}


Game::~Game()
{
	Uninitialization();

	if (_hGridPen)
	{
		DeletePen(_hGridPen);
		_hGridPen = NULL;
	}

	if (_hLogoFont)
	{
		DeleteFont(_hLogoFont);
		_hLogoFont = NULL;
	}

	if (_hScoreFont)
	{
		DeleteFont(_hScoreFont);
		_hScoreFont = NULL;
	}
}


void Game::Uninitialization()
{
	if (_snake) { delete _snake; _snake = NULL; }
	if (_food) { delete _food; _food = NULL; }
}

void Game::Initialization()
{
	Uninitialization();

	this->_food = new Food();
	this->_snake = new Snake(Settings::SNAKE_START_POSITION, Settings::SNAKE_STEP_TIME, Settings::SNAKE_START_ROTATION);
}

void Game::Update(DWORD deltaTime)
{
	if (_is_running)
	{
		if (!_is_game_over)
		{
			this->_food->Update(_gameInfo, _need_spawn_new_food);
			_need_spawn_new_food = false;

			this->_snake->Update(_gameInfo, deltaTime, _food);
		}
	}
}

void Game::Draw()
{
	Clear();
	
	if (_is_running)
	{
		if (_is_game_over)
		{
			DrawGameOverLogo();
			DrawScore();
		}
		else
		{
			_food->Draw(_gameInfo);
			_snake->Draw(_gameInfo);
			DrawGrid();
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
		_is_game_over = true;
		break;
	case ID_FOOD_ATE:
		_need_spawn_new_food = true;
		_score++;
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
	_is_game_over = false;
	_is_running = true;
	_need_spawn_new_food = true;
	_score = 0;
	Initialization();
}

void Game::Stop()
{
	_is_running = false;
}


void Game::Clear()
{
	SelectObject(_gameInfo->hdc, GetStockObject(DC_BRUSH));
	PatBlt(_gameInfo->hdc, 0, 0, _gameInfo->nMaxXScreen, _gameInfo->nMaxYScreen, PATCOPY);
}

void Game::DrawGrid()
{
	HGDIOBJ prevPen = SelectObject(_gameInfo->hdc, _hGridPen);

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
	UINT prevAlignText = SetTextAlign(_gameInfo->hdc, VTA_CENTER);

	HGDIOBJ defaultFont = SelectObject(_gameInfo->hdc, _hLogoFont);
	TCHAR text[256];
	swprintf_s(text, 256, L"Snake");

	int x = _gameInfo->rect_width / 2;
	int y = _gameInfo->rect_height / 2 - 100;

	TextOut(_gameInfo->hdc, x, y, text, wcslen(text));

	SetTextAlign(_gameInfo->hdc, prevAlignText);
	SelectObject(_gameInfo->hdc, defaultFont);
}


void Game::DrawGameOverLogo()
{
	UINT prevAlignText = SetTextAlign(_gameInfo->hdc, VTA_CENTER);
	COLORREF prevColor = SetTextColor(_gameInfo->hdc, RGB(255, 0, 0));

	HGDIOBJ defaultFont = SelectObject(_gameInfo->hdc, _hLogoFont);
	TCHAR text[256];
	swprintf_s(text, 256, L"Game Over");

	int x = _gameInfo->rect_width / 2;
	int y = _gameInfo->rect_height / 2 - 100;

	TextOut(_gameInfo->hdc, x, y, text, wcslen(text));

	SetTextAlign(_gameInfo->hdc, prevAlignText);
	SetTextColor(_gameInfo->hdc, prevColor);
	SelectObject(_gameInfo->hdc, defaultFont);
}


void Game::DrawScore()
{
	UINT prevAlignText = SetTextAlign(_gameInfo->hdc, VTA_CENTER);
	HGDIOBJ defaultFont = SelectObject(_gameInfo->hdc, _hScoreFont);

	TCHAR text[256];
	swprintf_s(text, 256, L"Score : %d", _score);

	int x = _gameInfo->rect_width / 2;
	int y = 10;

	TextOut(_gameInfo->hdc, x, y, text, wcslen(text));

	SetTextAlign(_gameInfo->hdc, prevAlignText);
	SelectObject(_gameInfo->hdc, defaultFont);
}
