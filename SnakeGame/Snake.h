#pragma once

class BodyPart;
class Food;
class GameInfo;

class Snake
{
private:
	Position _headPosition;				// ������� ������ ������
	BodyPart* _nextPart;				// ������ �� ��������� ����� ����
	Rotation _nextRotation;				// ������� ������ �� ��������� ����
	Rotation _currentRotation;			// ������� ������� ������
	COLORREF _color;					// ���� ������
	float _stepTime;					// ���������� �������, ����������� �� ���� ��� (� �������)
	float _nextStepTime;				// ��������� ����������, ��� ���������� ������� �� ���������� ����

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

