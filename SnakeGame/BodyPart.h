#pragma once
#include "Snake.h"

class BodyPart
{
	friend class Snake;

private:
	Position position;		// ������� ����� ����
	BodyPart* next;			// ��������� ����� ����

public:
	BodyPart();
	~BodyPart();
};

