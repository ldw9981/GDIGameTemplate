#pragma once

namespace Input
{
	void InitInput(HWND hWindow, int width, int height);
	void ReleaseInput();
	void Update();

	BOOL IsTurnDn(BYTE vk);
	BOOL IsTurnUp(BYTE vk);
	BOOL IsCurrDn(BYTE vk);
	BOOL IsCurrUp(BYTE vk);
	POINT GetMouseClient();
}
namespace Render
{
	void InitRender(HWND hWindow, int witdh, int height);
	void BeginDraw();
	void EndDraw();
	void ReleaseRender();
	void DrawLine(int x1, int y1, int x2, int y2, COLORREF color);
	void DrawRect(int x, int y, int width, int height, COLORREF color);
	void DrawCircle(int x, int y, int radius, COLORREF color);
	void DrawText(int x, int y, const char* text, COLORREF color);
	SIZE GetScreenSize();
	void DrawImage(int x, int y, Gdiplus::Bitmap* bitmap, int srcX, int srcY, int srcWitdh, int srcHeight);
}



namespace Time
{
	void InitTime();
	void UpdateTime();
	float GetDeltaTime();
}