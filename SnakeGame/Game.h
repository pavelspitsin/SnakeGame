#pragma once

class GameInfo;
class Snake;
class Food;

class Game
{
private:
	Snake* _snake;
	Food * _food;

	HPEN _hGridPen;
	HFONT _hLogoFont;
	HFONT _hScoreFont;

	bool _is_running;
	bool _is_game_over;

	bool _need_spawn_new_food;

	int _score;

	const GameInfo* _gameInfo;

	void Initialization();
	void Uninitialization();
	void Clear();
	void DrawGrid();
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

