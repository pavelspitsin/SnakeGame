#pragma once

class GameInfo;

class Food
{
private:
	Position _position;			// ������� ���
	COLORREF _color;			// ���� ���

public:
	Food();
	~Food();
	void Update(const GameInfo* gameInfo, bool need_spawn_new_food);
	void Draw(const GameInfo* gameInfo);
	Position GetPosition() const;
};

