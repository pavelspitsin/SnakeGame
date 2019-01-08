#pragma once

class GameInfo;
class Snake;
class Food;

class Game
{
private:
	Snake* _snake;
	Food * _food;
	Rotation _snakeRotation;

	bool is_running;
	bool is_game_over;

	bool need_spawn_new_food;

	int score;

	const GameInfo* _gameInfo;

	void Initialization();
	void Destroy();
	void Clear();
	void DrawCells();
	void DrawLogo();
	void DrawGameOverLogo();
	void DrawScore();

public:
	Game(const GameInfo* gameInfo);
	~Game();

	void Update(DWORD deltaTime);
	void Draw();

	void ProcessMessages(WPARAM wParam, LPARAM lParam);
	void Input_KeyDown(WPARAM wParam, LPARAM lParam);

	void Start();
	void Stop();
};

