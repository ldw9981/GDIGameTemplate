#pragma once

namespace Input
{
	void InitInput(HWND hWindow,int width,int height);
	void ReleaseInput();
	void Update();

	BOOL IsTurnDn(BYTE vk);
	BOOL IsTurnUp(BYTE vk);
	BOOL IsCurrDn(BYTE vk);
	BOOL IsCurrUp(BYTE vk);
	POINT GetMouseClient();
}