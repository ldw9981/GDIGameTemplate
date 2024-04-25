#pragma once

namespace Render
{
	void InitRender(HWND hWindow,int witdh,int height);

	void BeginDraw();

	void EndDraw();

	void ReleaseRender();

	void DrawLine(int x1, int y1, int x2, int y2, COLORREF color);

	void DrawRect(int x, int y, int width, int height, COLORREF color);

	void DrawCircle(int x, int y, int radius, COLORREF color);

	void DrawPolygon(POINT points[], int count, COLORREF color);

	void DrawText(int x, int y, const char* text, COLORREF color);

	void DrawFont(int x, int y, const char* text, COLORREF color, int fontSize, const wchar_t* fontName, int fontStyle);
	SIZE GetScreenSize();

	void DrawTestBitmap(bool reverse);
	void DrawBitmap(Gdiplus::Bitmap* bitmap, int srcX, int srcY, int srcWitdh, int srcHeight, bool reverse);
}