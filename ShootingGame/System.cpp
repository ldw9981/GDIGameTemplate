#include "stdafx.h"
#include "System.h"

#pragma comment(lib, "msimg32.lib")
#pragma comment(lib, "gdiplus.lib")

namespace Input
{
	// 키보드 입력 관련 변수
#define KEYBOARD_MAX 256		// 전체 가상 키의 수  https://learn.microsoft.com/ko-kr/windows/win32/inputdev/virtual-key-codes
	BYTE g_byKeyPrev[KEYBOARD_MAX];		// 이전 키의 정보
	BYTE g_byKeyCurr[KEYBOARD_MAX];		// 현재 키의 정보
	BYTE g_byKeyTurnDn[KEYBOARD_MAX];	// Down된 키의 정보
	BYTE g_byKeyTurnUp[KEYBOARD_MAX];   // Up 된 키의 정보

	HWND hWnd;
	int nWidth;
	int nHeight;

	POINT mouseClient;

	void Update()
	{
		GetCursorPos(&mouseClient);
		ScreenToClient(hWnd, &mouseClient);

		// 키보드 상태 갱신
		bool ret = GetKeyboardState((PBYTE)&g_byKeyCurr);	// 0x80 : 눌림, 0x00 : 눌리지 않음

		for (int i = 0; i < KEYBOARD_MAX; i++)
		{
			g_byKeyTurnUp[i] = (g_byKeyPrev[i] ^ g_byKeyCurr[i]) & g_byKeyPrev[i];
			g_byKeyTurnDn[i] = (g_byKeyPrev[i] ^ g_byKeyCurr[i]) & g_byKeyCurr[i];
		}
		memcpy(&g_byKeyPrev, &g_byKeyCurr, KEYBOARD_MAX);
	}

	void InitInput(HWND hWindow, int width, int height)
	{
		hWnd = hWindow;
		nWidth = width;
		nHeight = height;

		mouseClient.x = nWidth / 2;
		mouseClient.x = nHeight / 2;
		SetCursorPos(mouseClient.x, mouseClient.y);
	}

	void ReleaseInput()
	{

	}

	BOOL IsTurnDn(BYTE vk)
	{
		if (g_byKeyTurnDn[vk] & 0x80)
			return TRUE;

		return FALSE;
	}

	BOOL IsTurnUp(BYTE vk)
	{
		if (g_byKeyTurnUp[vk] & 0x80)
			return TRUE;

		return FALSE;

	}

	BOOL IsCurrDn(BYTE vk)
	{
		if (g_byKeyCurr[vk] & 0x80)
			return TRUE;

		return FALSE;
	}

	BOOL IsCurrUp(BYTE vk)
	{
		if (g_byKeyCurr[vk] & 0x80)
			return FALSE;

		return TRUE;

	}

	POINT GetMouseClient()
	{
		return mouseClient;
	}

}

namespace Render
{
	HWND hWnd;
	int nWidth;
	int nHeight;
	HDC frontMemDC;    
	HDC backMemDC;    
	HBITMAP backBitmap = nullptr;	
	void InitRender(HWND hWindow, int width, int height)
	{
		hWnd = hWindow;
		nWidth = width;
		nHeight = height;
		frontMemDC = GetDC(hWnd);
		backMemDC = CreateCompatibleDC(frontMemDC);
		backBitmap = CreateCompatibleBitmap(frontMemDC, nWidth, nHeight);
		::SelectObject(backMemDC, backBitmap);
	}
	void BeginDraw()
	{
		::PatBlt(backMemDC, 0, 0, nWidth, nHeight, BLACKNESS);
	}
	void EndDraw()
	{
		::BitBlt(frontMemDC, 0, 0, nWidth, nHeight, backMemDC, 0, 0, SRCCOPY);
	}
	void ReleaseRender()
	{		
		DeleteObject(backBitmap);
		DeleteDC(backMemDC);
		ReleaseDC(hWnd, frontMemDC);
	}
	void DrawRect(int x, int y, int width, int height, COLORREF color)
	{
		HPEN hPen = CreatePen(PS_SOLID, 1, color);
		HPEN hOldPen = (HPEN)SelectObject(backMemDC, hPen);
		HBRUSH hBrush = CreateSolidBrush(color);
		HBRUSH hOldBrush = (HBRUSH)SelectObject(backMemDC, hBrush);
		Rectangle(backMemDC, x, y, x + width, y + height);
		SelectObject(backMemDC, hOldPen);
		SelectObject(backMemDC, hOldBrush);
		DeleteObject(hPen);
		DeleteObject(hBrush);
	}
	void DrawCircle(int x, int y, int radius, COLORREF color)
	{
		HPEN hPen = CreatePen(PS_SOLID, 1, color);
		HPEN hOldPen = (HPEN)SelectObject(backMemDC, hPen);
		HBRUSH hBrush = CreateSolidBrush(color);
		HBRUSH hOldBrush = (HBRUSH)SelectObject(backMemDC, hBrush);
		Ellipse(backMemDC, x - radius, y - radius, x + radius, y + radius);
		SelectObject(backMemDC, hOldPen);
		SelectObject(backMemDC, hOldBrush);
		DeleteObject(hPen);
		DeleteObject(hBrush);
	}
	void DrawText(int x, int y, const char* text, COLORREF color)
	{
		SetTextColor(backMemDC, color);
		TextOutA(backMemDC, x, y, text, (int)strlen(text));
	}
	SIZE GetScreenSize()
	{
		SIZE size;
		size.cx = nWidth;
		size.cy = nHeight;
		return size;
	}
}


namespace Time
{
	LARGE_INTEGER frequency;
	LARGE_INTEGER prevCounter;
	LARGE_INTEGER currentCounter;
	float deltaTime;
	void InitTime()
	{
		QueryPerformanceFrequency(&frequency);	
		QueryPerformanceCounter(&prevCounter); 
	}
	void UpdateTime()
	{
		QueryPerformanceCounter(&currentCounter);
		deltaTime = static_cast<float>(currentCounter.QuadPart - prevCounter.QuadPart) / static_cast<float>(frequency.QuadPart); 
		prevCounter = currentCounter;
	}
	float GetDeltaTime()
	{
		return deltaTime;
	}
}