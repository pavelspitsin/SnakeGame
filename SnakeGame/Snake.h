#pragma once

class BodyPart;
class Food;
class GameInfo;

class Snake
{
private:
	Position _headPosition;				// Позиция головы змейки
	BodyPart* _nextPart;				// Ссылка на слудующую часть тела
	Rotation _nextRotation;				// Поворот змейки на следующем шаге
	Rotation _currentRotation;			// Текущий поворот змейки
	COLORREF _color;					// Цвет змейки
	float _stepTime;					// Количество времени, потраченное на один шаг (в секунду)
	float _nextStepTime;				// Временная переменная, для накопления времени до следующего шага

	bool SelfAte();
	Position UpdateHeadPosition();
	void UpdatePartsPositions(const Position prevHeadPosition, bool addPart);

public:
	Snake(Position snakePos, float stepTime, Rotation rotation);
	~Snake();

	Rotation GetRotation() const;
	void SetRotation(Rotation rotation);
	void UpdateRotation();
	void Update(const GameInfo* gameInfo, DWORD deltaTime, const Food* food);
	void Draw(const GameInfo* gameInfo);
};

