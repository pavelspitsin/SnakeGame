// SnakeGame.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "SnakeGameWnd.h"
#include "GameInfo.h"
#include "Game.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;                    
WCHAR szTitle[MAX_LOADSTRING];      
WCHAR szWindowClass[MAX_LOADSTRING];


Game* game;
bool is_running = true;
GameInfo* gameInfo;


void Clear();
void GameStart();
void GameStop();
void Input_KeyDown(WPARAM wParam, LPARAM lParam);
void GameProcessMessages(WPARAM wParam, LPARAM lParam);


void				InitWindowSettings(HINSTANCE hInstance);
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
	
	InitWindowSettings(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

	MSG msg;

	const int FRAMES_PER_SECOND = 20;
	const int SKIP_TICKS = 1000 / FRAMES_PER_SECOND;
	DWORD next_game_tick = GetTickCount();

	DWORD lastTime;
	DWORD currentTime = GetTickCount();

    // Main loop:
    while (is_running)
    {
		// Messages hande
		while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (GetMessage(&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}


		if (GetTickCount() > next_game_tick)
		{
			lastTime = currentTime;
			currentTime = GetTickCount();

			game->Update(currentTime - lastTime);
			game->Draw();

			next_game_tick += SKIP_TICKS;
		}
    }

	delete game;

    return (int) msg.wParam;
}


void InitWindowSettings(HINSTANCE hInstance)
{
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_SNAKEGAME, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SNAKEGAME));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SNAKEGAME);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		{
			int nMaxXScreen = GetSystemMetrics(SM_CXSCREEN);
			int nMaxYScreen = GetSystemMetrics(SM_CYSCREEN);

			HDC hdc = GetDC(hWnd);
			HDC hMemDC = CreateCompatibleDC(hdc);
			HBITMAP hBitMap = CreateCompatibleBitmap(hdc, nMaxXScreen, nMaxYScreen);
			SelectObject(hMemDC, hBitMap);


			////////////////////////////////////////////

			gameInfo = new GameInfo();

			gameInfo->hdc = hMemDC;
			gameInfo->hWnd = hWnd;
			gameInfo->nMaxXScreen = nMaxXScreen;
			gameInfo->nMaxYScreen = nMaxYScreen;


			game = new Game(gameInfo);

			////////////////////////////////////////////

			RECT rcWindow;
			GetClientRect(hWnd, &rcWindow);

			LONG rect_width = rcWindow.right - rcWindow.left;
			LONG rect_height = rcWindow.bottom - rcWindow.top;

			int width = rect_width;
			int height = rect_height - Settings::MARGIN_TOP;

			int x = width / Settings::CELLS_WIDTH_NUMBER;		// ширина области рисования
			int y = height / Settings::CELLS_HEIGHT_NUMBER;		// высота области рисования

			gameInfo->rect_width = rect_width;
			gameInfo->rect_height = rect_height;

			gameInfo->cell_size = (x >= y) ? y : x;

			gameInfo->grid_width = gameInfo->cell_size * Settings::CELLS_WIDTH_NUMBER;
			gameInfo->grid_height = gameInfo->cell_size * Settings::CELLS_HEIGHT_NUMBER;

			gameInfo->margin_width = (width - gameInfo->grid_width) / 2;
			gameInfo->margin_height_bottom = (height - gameInfo->grid_height) / 2;
			gameInfo->margin_height_top = gameInfo->margin_height_bottom +Settings::MARGIN_TOP;

			gameInfo->cells_width_number = Settings::CELLS_WIDTH_NUMBER;
			gameInfo->cells_height_number = Settings::CELLS_HEIGHT_NUMBER;

			/////////////////////////////////////////////	

			ReleaseDC(hWnd, hdc);
		}
		break;
	case WM_GETMINMAXINFO:
		{
			LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
			lpMMI->ptMinTrackSize.x = Settings::WINDOW_WIDTH;
			lpMMI->ptMinTrackSize.y = Settings::WINDOW_HEIGHT;
			lpMMI->ptMaxTrackSize.x = Settings::WINDOW_WIDTH;
			lpMMI->ptMaxTrackSize.y = Settings::WINDOW_HEIGHT;
		}
		break;
	case WM_KEYDOWN:
		Input_KeyDown(wParam, lParam);
		break;
	case WM_GAME:
		GameProcessMessages(wParam, lParam);
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case ID_GAME_START:
				GameStart();
				break;
			case ID_GAME_STOP:
				GameStop();
				break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			BitBlt(hdc, 0, 0, gameInfo->nMaxXScreen, gameInfo->nMaxYScreen, gameInfo->hdc, 0, 0, SRCCOPY);
			EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
		is_running = false;
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}



void Clear()
{
	SelectObject(gameInfo->hdc, GetStockObject(DC_BRUSH));
	PatBlt(gameInfo->hdc, 0, 0, gameInfo->nMaxXScreen, gameInfo->nMaxYScreen, PATCOPY);
}

void GameStart()
{
	game->Start();
}

void GameStop()
{
	game->Stop();
}

void Input_KeyDown(WPARAM wParam, LPARAM lParam)
{
	game->Input_KeyDown(wParam, lParam);
}

void GameProcessMessages(WPARAM wParam, LPARAM lParam)
{
	game->ProcessMessages(wParam, lParam);
}
