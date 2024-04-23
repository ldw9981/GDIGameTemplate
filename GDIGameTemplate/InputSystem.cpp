#include "stdafx.h"
#include "InputSystem.h"

namespace input
{
	HWND hWnd;
	int nWidth;
	int nHeight;
	bool isKeyDown[256];
	bool isKeyUp[256];
	bool isKey[256];

	MouseState curMouse;
	MouseState prevMouse;


	void ResetInput()
	{
		for (int i = 0; i < 256; i++)
		{
			isKeyDown[i] = false;
			isKeyUp[i] = false;
		}
	}

	void KeyDown(unsigned int key)
	{
		isKeyDown[key] = true;
		isKey[key] = true;
	}

	void KeyUp(unsigned int key)
	{
		isKeyUp[key] = true;
		isKey[key] = false;
	}

	bool IsKeyDown(unsigned int key)
	{
		return isKeyDown[key];
	}

	bool IsKeyUp(unsigned int key)
	{
		return isKeyUp[key];
	}

	bool IsKey(unsigned int key)
	{
		return isKey[key];
	}

	void InitMouse()
	{
		curMouse.x = nWidth / 2;
		curMouse.y = nHeight / 2;
		curMouse.wheel = 0;

		curMouse.left = false;
		curMouse.right = false;
		curMouse.middle = false;

		prevMouse = curMouse;

		SetCursorPos(curMouse.x, curMouse.y);
	}

	void UpdateMouse()
	{
		prevMouse = curMouse;

		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(hWnd, &pt);

		curMouse.x = pt.x;
		curMouse.y = pt.y;
		curMouse.wheel = 0;

		curMouse.left = (GetKeyState(VK_LBUTTON) & 0x8000) != 0;
		curMouse.right = (GetKeyState(VK_RBUTTON) & 0x8000) != 0;
		curMouse.middle = (GetKeyState(VK_MBUTTON) & 0x8000) != 0;
	}

	const MouseState& GetMouseState()
	{
		return curMouse;
	}

	const MouseState& GetPrevMouseState()
	{
		return prevMouse;
	}

	void InitInput(HWND hWindow,int width,int height)
	{
		hWnd = hWindow;
		nWidth = width;
		nHeight = height;

		for (int i = 0; i < 256; i++)
		{
			isKeyDown[i] = false;
			isKeyUp[i] = false;
			isKey[i] = false;
		}

		InitMouse();
	}

	void ReleaseInput()
	{

	}

	bool IsSame(const MouseState& a, const MouseState& b)
	{
		return a.x == b.x && a.y == b.y && a.wheel == b.wheel && a.left == b.left && a.right == b.right && a.middle == b.middle;
	}
}