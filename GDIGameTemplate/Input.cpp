#include "Input.h"




#define KEYBOARD_MAX 256		// 전체 가상 키의 수


BYTE g_byKeyPrev[KEYBOARD_MAX];		// 이전 키의 정보
BYTE g_byKeyCurr[KEYBOARD_MAX];		// 현재 키의 정보

BYTE g_byKeyTurnDn[KEYBOARD_MAX];	// Down된 키의 정보
BYTE g_byKeyTurnUp[KEYBOARD_MAX];   // Up 된 키의 정보

BOOL   g_bInvalidMousePoint;
POINTS g_ptMouse;
BOOL   g_bLButtonUp;
BOOL   g_bLButtonDown;
BOOL   g_bRButtonUp;
BOOL   g_bRButtonDown;

void InputUpdate()
{
	int i = 0;
	GetKeyboardState((PBYTE)&g_byKeyCurr);

	for (i = 0; i < KEYBOARD_MAX; i++)	// 키입력 전체가 아닌 일부만 사용한다면 루프를 줄일수있다.
	{
		g_byKeyTurnUp[i] = (g_byKeyPrev[i] ^ g_byKeyCurr[i]) & g_byKeyPrev[i];
		g_byKeyTurnDn[i] = (g_byKeyPrev[i] ^ g_byKeyCurr[i]) & g_byKeyCurr[i];
	}
	memcpy(&g_byKeyPrev, &g_byKeyCurr, KEYBOARD_MAX);
}


BOOL InputIsTurnDn(BYTE vk)
{
	if (g_byKeyTurnDn[vk] & 0x80)
		return TRUE;

	return FALSE;
}

BOOL InputIsTurnUp(BYTE vk)
{
	if (g_byKeyTurnUp[vk] & 0x80)
		return TRUE;

	return FALSE;

}

BOOL InputIsCurrDn(BYTE vk)
{
	if (g_byKeyCurr[vk] & 0x80)
		return TRUE;

	return FALSE;
}

BOOL InputIsCurrUp(BYTE vk)
{
	if (g_byKeyCurr[vk] & 0x80)
		return FALSE;

	return TRUE;

}