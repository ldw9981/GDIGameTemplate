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
	HDC frontMemDC;    // 앞면 DC
	HDC backMemDC;    // 뒷면 DC

	HBITMAP backBitmap = nullptr;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::Graphics* graphics = nullptr;
	// https://learn.microsoft.com/ko-kr/windows/win32/gdiplus/-gdiplus-about-gdi--about


	void InitRender(HWND hWindow, int width, int height)
	{
		hWnd = hWindow;
		nWidth = width;
		nHeight = height;

		frontMemDC = GetDC(hWnd);
		backMemDC = CreateCompatibleDC(frontMemDC);
		backBitmap = CreateCompatibleBitmap(frontMemDC, nWidth, nHeight);
		::SelectObject(backMemDC, backBitmap);

		// GDI+ 초기화
		Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
		graphics = Gdiplus::Graphics::FromHDC(backMemDC);
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
		Gdiplus::GdiplusShutdown(gdiplusToken);
		DeleteObject(backBitmap);
		DeleteDC(backMemDC);
		ReleaseDC(hWnd, frontMemDC);
	}

	void DrawLine(int x1, int y1, int x2, int y2, COLORREF color)
	{
		HPEN hPen = CreatePen(PS_SOLID, 5, color);

		HPEN hOldPen = (HPEN)SelectObject(backMemDC, hPen);

		MoveToEx(backMemDC, x1, y1, NULL);

		LineTo(backMemDC, x2, y2);

		SelectObject(backMemDC, hOldPen);

		DeleteObject(hPen);
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

	//bitmap은 GdiplusShutdown 하기전에 해제해야함
	void DrawImage(int x, int y, Gdiplus::Bitmap* bitmap, int srcX, int srcY, int srcWitdh, int srcHeight)
	{
		if (bitmap == nullptr)
			return;

		Gdiplus::Rect srcRect(srcX, srcY, srcWitdh, srcHeight); // 소스의 영역
		Gdiplus::Rect destRect(x, y, srcRect.Width, srcRect.Height); // 화면에 그릴 영역		
		// 이미지 그리기
		graphics->DrawImage(bitmap, destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, Gdiplus::UnitPixel);
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
		QueryPerformanceFrequency(&frequency);	// 고성능 타이머가 1초 동안 증가시킬수 있는 TickCount 값
		QueryPerformanceCounter(&prevCounter);  // 초기 TickCount 값
	}

	void UpdateTime()
	{
		QueryPerformanceCounter(&currentCounter); // 현재 TickCount 값
		deltaTime = static_cast<float>(currentCounter.QuadPart - prevCounter.QuadPart) /
			static_cast<float>(frequency.QuadPart);  // 카운터차이를 시간 초단위로 변환

		prevCounter = currentCounter;
	}

	float GetDeltaTime()
	{
		return deltaTime;
	}

}