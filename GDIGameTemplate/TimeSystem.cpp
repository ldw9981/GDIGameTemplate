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
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&prevCounter);
	}

	void UpdateTime()
	{
		QueryPerformanceCounter(&currentCounter);

		deltaTime = static_cast<float>(currentCounter.QuadPart - prevCounter.QuadPart) /
			static_cast<float>(frequency.QuadPart);

		prevCounter = currentCounter;
	}	

	float GetDeltaTime() 
	{ 
		return deltaTime; 
	}

}