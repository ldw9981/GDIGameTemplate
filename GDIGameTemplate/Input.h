#pragma once
/************************************************************************/
/*
	Ű���忡 ���� ���
	InputUpdate()�� ���� �������� ȣ���ϸ� Ű�� ���¸�
	BYTE g_byKeyPrev[KEYBOARD_MAX];		// ���� Ű�� ����
	BYTE g_byKeyCurr[KEYBOARD_MAX];		// ���� Ű�� ����

	BYTE g_byKeyTurnDn[KEYBOARD_MAX];	// Down�� Ű�� ����
	BYTE g_byKeyTurnUp[KEYBOARD_MAX];   // Up �� Ű�� ����
	�� �־� �����Ѵ�.

	IsCurr,IsTurn���� �Լ��� ����Ͽ� �ʿ��Ҷ� �ش� Ű���¸� Ȯ���Ѵ�.
*/
/************************************************************************/
#include <Windows.h>

extern BOOL   g_bInvalidMousePoint;
extern POINTS g_ptMouse;
extern BOOL   g_bLButtonUp;
extern BOOL   g_bLButtonDown;
extern BOOL   g_bRButtonUp;
extern BOOL   g_bRButtonDown;

void InputUpdate(); // WindowsMessage������ ����Ҷ��� �۵��մϴ�.
	 
BOOL InputIsCurrDn(BYTE vk);
BOOL InputIsCurrDn(BYTE vk);
BOOL InputIsTurnDn(BYTE vk);
BOOL InputIsTurnUp(BYTE vk);
BOOL InputIsCurrDn(BYTE vk);
BOOL InputIsCurrUp(BYTE vk);
