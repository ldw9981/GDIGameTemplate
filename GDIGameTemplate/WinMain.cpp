#include "stdafx.h"
#include "InputSystem.h"
#include "RenderSystem.h"
#include "TimeSystem.h"

#include <stdio.h>	
#include <ConsoleApi.h>

// ���־� ��Ʃ����� ���� ���ø��� �ٸ� �߰����� ������ ���� �����ϱ� ����� ���� �����ϰ� �ۼ���.

// ������ ���ν��� �Լ� ����
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void UpdateGame();
void RenderGame();

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
	SIZE clientSize = { 800, 600 };
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

	bool bUseConsole=true;
	if (bUseConsole)
	{
		AllocConsole();
		FILE* _tempFile;
		freopen_s(&_tempFile, "CONOUT$", "w", stdout);
	}	

	Render::InitRender(hwnd, clientSize.cx, clientSize.cy);
	Input::InitInput(hwnd, clientSize.cx, clientSize.cy);
	Time::InitTime();

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

	if (Input::IsTurnDn('A'))
	{
		std::cout << "A key is pressed" << std::endl;
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
}
void RenderGame()
{
	Render::BeginDraw();
	Render::DrawRect(100, 100, 200, 200, RGB(255, 0, 0));
	Render::EndDraw();
}