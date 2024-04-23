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
		QueryPerformanceFrequency(&frequency);	// 고성능 타이머의 1초당 증가한 횟수를 가져옴
		QueryPerformanceCounter(&prevCounter);  // 초기 카운터 값 가져옴
	}

	void UpdateTime()
	{
		QueryPerformanceCounter(&currentCounter); // 현재 카운터 값 가져옴
		deltaTime = static_cast<float>(currentCounter.QuadPart - prevCounter.QuadPart) /
			static_cast<float>(frequency.QuadPart);  // 카운터차이를 초단위로 변환

		prevCounter = currentCounter;
	}	

	float GetDeltaTime() 
	{ 
		return deltaTime; 
	}

}