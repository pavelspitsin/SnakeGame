#pragma once
#include "Snake.h"

class BodyPart
{
	friend class Snake;

private:
	Position position;		// Позиция части тела
	BodyPart* next;			// Следующая часть тела

public:
	BodyPart();
	~BodyPart();
};

