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
	Gdiplus::Graphics* graphics=nullptr;


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

	void DrawGDIBitmap(int x, int y,Gdiplus::Bitmap* bitmap,int srcX,int srcY,int srcWitdh,int srcHeight, bool mirror)
	{
		if (bitmap == nullptr)
			return;

		Gdiplus::Rect srcRect(srcX, srcY, srcWitdh, srcHeight); // 소스의 영역
		Gdiplus::Rect destRect(x, y,  srcRect.Width, srcRect.Height); // 화면에 그릴 영역
		if (mirror)	//	Y 축 대칭 인가?
		{	
			Gdiplus::Matrix matrixMirror(-1, 0, 0, 1,(float) srcRect.Width,0);
			graphics->SetTransform(&matrixMirror);
		}
		else
		{			
			Gdiplus::Matrix matrixDefault(1, 0, 0, 1, 0, 0);
			graphics->SetTransform(&matrixDefault);
		}
		// 이미지 그리기
		graphics->DrawImage(bitmap, destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, Gdiplus::UnitPixel);
	}
	
}