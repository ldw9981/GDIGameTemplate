#include "stdafx.h"
#include "TimeSystem.h"
#include <cmath>

namespace Time
{
	LARGE_INTEGER frequency;
	LARGE_INTEGER prevCounter;
	LARGE_INTEGER currentCounter;
	float deltaTime;

	void InitTime()
	{
		QueryPerformanceFrequency(&frequency);	// ���� Ÿ�̸��� 1�ʴ� ������ Ƚ���� ������
		QueryPerformanceCounter(&prevCounter);  // �ʱ� ī���� �� ������
	}

	void UpdateTime()
	{
		QueryPerformanceCounter(&currentCounter); // ���� ī���� �� ������
		deltaTime = static_cast<float>(currentCounter.QuadPart - prevCounter.QuadPart) /
			static_cast<float>(frequency.QuadPart);  // ī�������̸� �ʴ����� ��ȯ

		prevCounter = currentCounter;
	}	

	float GetDeltaTime() 
	{ 
		return deltaTime; 
	}

}