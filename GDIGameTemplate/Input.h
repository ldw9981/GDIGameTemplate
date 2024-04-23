#pragma once
/************************************************************************/
/*
	키보드에 관한 모듈
	InputUpdate()를 게임 루프에서 호출하면 키의 상태를
	BYTE g_byKeyPrev[KEYBOARD_MAX];		// 이전 키의 정보
	BYTE g_byKeyCurr[KEYBOARD_MAX];		// 현재 키의 정보

	BYTE g_byKeyTurnDn[KEYBOARD_MAX];	// Down된 키의 정보
	BYTE g_byKeyTurnUp[KEYBOARD_MAX];   // Up 된 키의 정보
	에 넣어 보관한다.

	IsCurr,IsTurn관련 함수를 사용하여 필요할때 해당 키상태를 확인한다.
*/
/************************************************************************/
#include <Windows.h>

extern BOOL   g_bInvalidMousePoint;
extern POINTS g_ptMouse;
extern BOOL   g_bLButtonUp;
extern BOOL   g_bLButtonDown;
extern BOOL   g_bRButtonUp;
extern BOOL   g_bRButtonDown;

void InputUpdate(); // WindowsMessage루프를 사용할때만 작동합니다.
	 
BOOL InputIsCurrDn(BYTE vk);
BOOL InputIsCurrDn(BYTE vk);
BOOL InputIsTurnDn(BYTE vk);
BOOL InputIsTurnUp(BYTE vk);
BOOL InputIsCurrDn(BYTE vk);
BOOL InputIsCurrUp(BYTE vk);
