#pragma once

namespace Input
{
	void Initialize(HWND hWindow, int width, int height);
	void Uninitialize();
	void Update();
	BOOL IsTurnDn(BYTE vk);
	BOOL IsTurnUp(BYTE vk);
	BOOL IsCurrDn(BYTE vk);
	BOOL IsCurrUp(BYTE vk);
	POINT GetMouseClient();
}
namespace Render
{
	void Initialize(HWND hWindow, int witdh, int height);
	void BeginDraw();
	void EndDraw();
	void Uninitialize();	
	void DrawRect(int x, int y, int width, int height, COLORREF color);
	void DrawCircle(int x, int y, int radius, COLORREF color);
	void DrawText(int x, int y, const char* text, COLORREF color);
	SIZE GetScreenSize();	
}



namespace Time
{
	void Initialize();
	void UpdateTime();
	float GetDeltaTime();
}