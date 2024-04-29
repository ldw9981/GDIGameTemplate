#include "stdafx.h"
#include "InputSystem.h"
#include "RenderSystem.h"
#include "TimeSystem.h"
#include "Object.h"
#include "AnimationResource.h"

#include <stdio.h>	
#include <ConsoleApi.h>


#define MAX_ENEMY 10

Object g_player;
Object g_enemy[MAX_ENEMY];

AnimationResource g_PlayerAnim;

bool g_Mirror = false;
// ���־� ��Ʃ����� ���� ���ø��� �ٸ� �߰����� ������ ���� �����ϱ� ����� ���� �����ϰ� �ۼ���.

// ������ ���ν��� �Լ� ����
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void UpdateGame();
void RenderGame();
void LoadResource();
void ReleaseResource();

// ������Ʈ �Ӽ� -> ��Ŀ -> �ý��� -> ���� �ý��� -> Windows�� ����
// ������ �Լ� ����
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ������ Ŭ���� ����ü �ʱ�ȭ
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;	// ������ ���ν��� �Լ�
	wc.hInstance = hInstance;   // �ν��Ͻ� �ڵ�
	// NULL�� ����ϸ� ���� ���� ���� ���α׷��� �ν��Ͻ� �ڵ��� ����ϰ� �˴ϴ�.�� ��° ���ڴ� �ε��� Ŀ���� ID�Դϴ�.
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszClassName = L"BasicWindowClass";

	// ������ Ŭ���� ���
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"������ Ŭ���� ��� ����", L"����", MB_OK | MB_ICONERROR);
		return 1;
	}

	// ���ϴ� ũ�Ⱑ �����Ǿ� ����
	SIZE clientSize = { 1920, 1280 };
	RECT clientRect = { 0, 0, clientSize.cx, clientSize.cy };
	AdjustWindowRect(&clientRect, WS_OVERLAPPEDWINDOW, FALSE);

	// ������ ����
	HWND hwnd = CreateWindowEx(
		0,
		L"BasicWindowClass",
		L"������ ������",
		WS_OVERLAPPEDWINDOW,	// OR�������� ���յ� ������â ��Ÿ��
		0,0,	// ������ġ
		clientRect.right-clientRect.left,  clientRect.bottom - clientRect.top , // �ʺ�, ����
		NULL, NULL, hInstance, NULL
	);

	if (!hwnd)
	{
		MessageBox(NULL, L"������ ���� ����", L"����", MB_OK | MB_ICONERROR);
		return 1;
	}

	// ������ ǥ��
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	bool bUseConsole=false;
	if (bUseConsole)
	{
		AllocConsole();
		FILE* _tempFile;
		freopen_s(&_tempFile, "CONOUT$", "w", stdout);
	}	

	Render::InitRender(hwnd, clientSize.cx, clientSize.cy);
	Input::InitInput(hwnd, clientSize.cx, clientSize.cy);
	Time::InitTime();
	LoadResource();

	g_player.Init(true);
	g_player.SetMotion(0);

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		g_enemy[i].Init(false);
	}

	// ��Ⱑ ���� �޼��� ����
	MSG msg;
	while (true)
	{
		// �޽����� ������ ó��, ������ �ٷ� ���� ������ �̵�
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// WM_QUIT �޽��� Ȯ��
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// ���� ����					
			UpdateGame();
			RenderGame();
		}
	}

	ReleaseResource();
	Input::ReleaseInput();
	Render::ReleaseRender();
	if (bUseConsole)
	{
		FreeConsole();
	}

	return static_cast<int>(msg.wParam);
}

// ������ ���ν��� �Լ� ����
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

void UpdateGame()
{
	Time::UpdateTime();
	float deltaTime = Time::GetDeltaTime();
	Input::Update();


	g_player.m_inputDirX = 0;
	g_player.m_inputDirY = 0;
	if (Input::IsCurrDn(VK_LEFT))
	{
		g_player.m_inputDirX = -1;
	}
	else if (Input::IsCurrDn(VK_RIGHT))
	{
		g_player.m_inputDirX = 1;
	}
	if (Input::IsCurrDn(VK_UP))
	{
		g_player.m_inputDirY = -1;
	}
	else if (Input::IsCurrDn(VK_DOWN))
	{
		g_player.m_inputDirY = 1;
	}
	if (Input::IsTurnDn('O'))
	{
		std::cout << g_player.m_posX << ' ' << g_player.m_posY << std::endl;
	}
	if (Input::IsCurrDn(VK_LBUTTON))
	{
		POINT pt = Input::GetMouseClient();
		std::cout << pt.x << ' ' << pt.y << std::endl;
	}
	if (Input::IsTurnDn('T'))
	{
		std::cout << deltaTime << std::endl;
	}
	if (Input::IsTurnDn('R'))
	{
		g_Mirror = !g_Mirror;
	}
	

	// ��ġ ����
	g_player.Update(deltaTime);
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		g_enemy[i].Update(deltaTime);
	}

	//�浹Ȯ��
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
void RenderGame()
{
	Render::BeginDraw();
	
	g_player.Render();
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		g_enemy[i].Render();
	}
	Render::EndDraw();
}

void LoadResource()
{
	
	g_PlayerAnim.LoadAnimImage(L"../Resource/Ken.png");
	g_PlayerAnim.LoadAnimMotion(L"../Resource/KenIdle.txt");
	g_PlayerAnim.LoadAnimMotion(L"../Resource/KenMove.txt");
	g_player.m_pAnim = &g_PlayerAnim;
}

void ReleaseResource()
{
	
}