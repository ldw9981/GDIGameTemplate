#include "stdafx.h"
#include "System.h"
#include "Object.h"

#define MAX_ENEMY 10

Object g_player;
Object g_enemy[MAX_ENEMY];
SIZE g_ClientSize = { 1280, 960 };

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void GameUpdate();
void GameRender();
void GameInitialize(HWND hwnd);
void GameUninitialize();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;	
	wc.hInstance = hInstance;   
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszClassName = L"BasicWindowClass";

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"윈도우 클래스 등록 실패", L"에러", MB_OK | MB_ICONERROR);
		return 1;
	}


	RECT clientRect = { 0, 0, g_ClientSize.cx, g_ClientSize.cy };
	AdjustWindowRect(&clientRect, WS_OVERLAPPEDWINDOW, FALSE);

	HWND hwnd = CreateWindowEx(0, L"BasicWindowClass", L"간단한 윈도우", WS_OVERLAPPEDWINDOW,0,0,	
		clientRect.right-clientRect.left,  clientRect.bottom - clientRect.top , NULL, NULL, hInstance, NULL );

	if (!hwnd)
	{
		MessageBox(NULL, L"윈도우 생성 실패", L"에러", MB_OK | MB_ICONERROR);
		return 1;
	}
		
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	GameInitialize(hwnd);

	MSG msg;
	while (true)
	{		
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{					
			GameUpdate();
			GameRender();
		}
	}

	GameUninitialize();

	
	return static_cast<int>(msg.wParam);
}

// 윈도우 프로시저 함수 정의
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

void GameInitialize(HWND hwnd)
{
	Render::InitRender(hwnd, g_ClientSize.cx, g_ClientSize.cy);
	Input::InitInput(hwnd, g_ClientSize.cx, g_ClientSize.cy);
	Time::InitTime();

	g_player.Init(true);

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		g_enemy[i].Init(false);
	}
}

void GameUpdate()
{
	Time::UpdateTime();
	float deltaTime = Time::GetDeltaTime();
	Input::Update();


	g_player.m_inputDir.Set(0.0f,0.0f);
	if (Input::IsCurrDn(VK_LEFT))
	{
		g_player.m_inputDir.x = -1;
	}
	else if (Input::IsCurrDn(VK_RIGHT))
	{
		g_player.m_inputDir.x = 1;
	}
	if (Input::IsCurrDn(VK_UP))
	{
		g_player.m_inputDir.y = -1;
	}
	else if (Input::IsCurrDn(VK_DOWN))
	{
		g_player.m_inputDir.y = 1;
	}
	

	// 위치 갱신
	g_player.Update(deltaTime);
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		g_enemy[i].Update(deltaTime);
	}

	//충돌확인
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_enemy[i].m_isDead == false)
		{
			if (g_player.Collide(g_enemy[i]))
			{
				g_enemy[i].m_isDead = true;
			}			
		}
	}		
}
void GameRender()
{
	Render::BeginDraw();
	
	g_player.Render();
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		g_enemy[i].Render();
	}
	Render::EndDraw();
}


void GameUninitialize()
{
	Input::ReleaseInput();
	Render::ReleaseRender();
}