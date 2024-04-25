#include "stdafx.h"
#include "RenderSystem.h"


#pragma comment(lib, "msimg32.lib")
#pragma comment(lib, "gdiplus.lib")


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
	Gdiplus::Bitmap* bitmap = nullptr;


	void InitRender(HWND hWindow,int width,int height)
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
		

		bitmap = new Gdiplus::Bitmap(L"../Resource/terry03.bmp");
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
		delete bitmap;
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

	void DrawPolygon(POINT points[], int count, COLORREF color)
	{
		HPEN hPen = CreatePen(PS_SOLID, 1, color);

		HPEN hOldPen = (HPEN)SelectObject(backMemDC, hPen);

		HBRUSH hBrush = CreateSolidBrush(color);

		HBRUSH hOldBrush = (HBRUSH)SelectObject(backMemDC, hBrush);

		Polygon(backMemDC, points, count);

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

	void DrawTestBitmap(bool reverse)
	{		
		// 세로축 반전 변환 매트릭스 생성
		Gdiplus::Graphics graphics(backMemDC);

		Gdiplus::Rect srcRect(50,100, 300 , 300 );
		Gdiplus::Rect destRect(0, 0, srcRect.Width, srcRect.Height);

		// 이미지 그리기
		if (reverse)
		{
			static Gdiplus::Matrix matrix(-1, 0, 0, 1, srcRect.Width,0);
			graphics.SetTransform(&matrix);
		}
		graphics.DrawImage(bitmap, destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, Gdiplus::UnitPixel);		
	}

	void DrawBitmap(Gdiplus::Bitmap* bitmap,int srcX,int srcY,int srcWitdh,int srcHeight, bool mirror)
	{
		// 세로축 반전 변환 매트릭스 생성
		Gdiplus::Graphics graphics(backMemDC);

		Gdiplus::Rect srcRect(srcX, srcY, srcWitdh, srcHeight);
		Gdiplus::Rect destRect(0, 0, srcRect.Width, srcRect.Height);

		// 이미지 그리기
		if (mirror)
		{
			static Gdiplus::Matrix matrix(-1, 0, 0, 1, srcRect.Width, 0);
			graphics.SetTransform(&matrix);
		}
		graphics.DrawImage(bitmap, destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, Gdiplus::UnitPixel);
	}
	
}