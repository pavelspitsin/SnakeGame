#include "stdafx.h"
#include "Snake.h"
#include "Food.h"
#include "BodyPart.h"
#include "GameInfo.h"
#include "Resource.h"

Snake::Snake(const Position snakePos, float stepTime, Rotation rotation) :
	_nextPart(NULL), _headPosition(snakePos), _stepTime(stepTime), _currentRotation(rotation), _nextRotation(rotation)
{
	_color = RGB(0, 0, 0);
	_nextStepTime = 0;


	_hBrush = CreateSolidBrush(this->_color);

	Position bodyPosition = _headPosition;
	BodyPart* prevPart = NULL;

	for (int i = 0; i < 2; ++i)
	{
		bodyPosition.x--;
		BodyPart* newPart = new BodyPart();
		newPart->position = bodyPosition;
		newPart->next = NULL;

		if (i == 0)
		{
			_nextPart = newPart;
			prevPart = newPart;
			continue;
		}

		newPart->next = NULL;
		prevPart->next = newPart;
		prevPart = newPart;
	}
}

Snake::~Snake()
{
	if (_hBrush)
	{
		DeleteBrush(_hBrush);
		_hBrush = NULL;
	}

	BodyPart* tmp;

	while (_nextPart)
	{
		tmp = _nextPart;
		_nextPart = tmp->next;
		delete tmp;
	}
}

Rotation Snake::GetRotation() const
{
	return _currentRotation;
}

void Snake::SetRotation(Rotation rotation)
{
	_nextRotation = rotation;
}

void Snake::UpdateRotation()
{
	switch (_nextRotation)
	{
	case LEFT:
		if (_currentRotation != RIGHT)
			_currentRotation = _nextRotation;
		break;
	case UP:
		if (_currentRotation != DOWN)
			_currentRotation = _nextRotation;
		break;
	case RIGHT:
		if (_currentRotation != LEFT)
			_currentRotation = _nextRotation;
		break;
	case DOWN:
		if (_currentRotation != UP)
			_currentRotation = _nextRotation;
		break;
	default:
		break;
	}
}

void Snake::Update(const GameInfo* gameInfo, DWORD deltaTime, const Food* food)
{
	_nextStepTime += deltaTime;

	if (_nextStepTime >= _stepTime * 1000)
	{
		UpdateRotation();
		Position prevHeadPosition = UpdateHeadPosition();

		bool addPart = false;

		if (_headPosition.x < 0 || _headPosition.x >= gameInfo->cells_width_number ||
			_headPosition.y < 0 || _headPosition.y >= gameInfo->cells_height_number)
		{
			PostMessage(gameInfo->hWnd, WM_GAME, ID_GAME_OVER, 0);
			return;
		}

		if (food->GetPosition() == _headPosition)
		{
			PostMessage(gameInfo->hWnd, WM_GAME, ID_FOOD_ATE, 0);
			addPart = true;
		}
		UpdatePartsPositions(prevHeadPosition, addPart);

		if (SelfAte()) // если съели себя
		{
			PostMessage(gameInfo->hWnd, WM_GAME, ID_GAME_OVER, 0);
			return;
		}

		_nextStepTime = 0;
	}
}

void Snake::Draw(const GameInfo* gameInfo)
{
	HGDIOBJ prevBrush = NULL;
	prevBrush = SelectObject(gameInfo->hdc, _hBrush);

	int margin_width = gameInfo->margin_width;
	int margin_height_top = gameInfo->margin_height_top;
	int cell_size = gameInfo->cell_size;

	// Draw head
	int x = _headPosition.x * cell_size + margin_width;
	int y = _headPosition.y * cell_size + margin_height_top;

	Rectangle(gameInfo->hdc, x, y, x + cell_size, y + cell_size);
	 

	// Draw body

	BodyPart* nextPart = _nextPart;

	while (nextPart != NULL)
	{
		x = nextPart->position.x * cell_size + margin_width;
		y = nextPart->position.y * cell_size + margin_height_top;
		Rectangle(gameInfo->hdc, x, y, x + cell_size, y + cell_size);
		nextPart = nextPart->next;
	}

	SelectObject(gameInfo->hdc, prevBrush);
}


bool Snake::SelfAte()
{
	bool result = false;
	BodyPart* nextBodyPart = this->_nextPart;

	while (nextBodyPart != NULL)
	{		
		if (nextBodyPart->position == _headPosition)
		{
			result = true;
			break;
		}
		nextBodyPart = nextBodyPart->next;
	}

	return result;
}

// Returns previous position
Position Snake::UpdateHeadPosition()
{
	Position prevPosition = _headPosition;

	switch (_currentRotation)
	{
	case LEFT:
		this->_headPosition.x--;
		break;
	case UP:
		this->_headPosition.y--;
		break;
	case RIGHT:
		this->_headPosition.x++;
		break;
	case DOWN:
		this->_headPosition.y++;
		break;
	default:
		break;
	}

	return prevPosition;
}

void Snake::UpdatePartsPositions(Position prevHeadPosition, bool addPart)
{
	Position prevPosition = prevHeadPosition;
	BodyPart* nextBodyPart = this->_nextPart;
	Position tmp;

	if (addPart)
	{
		BodyPart* newPart = new BodyPart();
		newPart->position = prevPosition;
		newPart->next = this->_nextPart;
		this->_nextPart = newPart;
		return;
	}


	while (nextBodyPart != NULL)
	{
		tmp = nextBodyPart->position;
		nextBodyPart->position = prevPosition;
		prevPosition = tmp;

		nextBodyPart = nextBodyPart->next;
	}
}
