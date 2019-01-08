#pragma once

struct Position
{
	int x, y;

	bool operator == (Position other)
	{
		return (this->x == other.x) && (this->y == other.y);
	}

	bool operator != (Position other)
	{
		return (this->x != other.x) || (this->y != other.y);
	}
};


enum Rotation
{
	LEFT, UP, RIGHT, DOWN
};