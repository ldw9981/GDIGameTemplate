#pragma once

// 동작의 하나의 프레임 정보
struct Frame
{
	RECT Source = {0,};			// 부분출력할 이미지 영역
	SIZE Size = {0,};				// 출력 크기
	int CenterX=0,CenterY=0;	// 중심이 되는 이동 위치 
};

struct Motion 
{
	Frame Frames[10];		// 프레임 컨테이너
	int FrameCount=0;			// 전체 프레임 수
	bool IsLoop = false;	// 반복 여부
};

struct AnimationResource
{	
	std::wstring m_fileName;	

	Gdiplus::Bitmap* m_bitmap = nullptr;
	Gdiplus::Bitmap* m_bitmapFlip = nullptr;
	Motion m_motions[10];	// 최대 10개의 동작을 가질 수 있음
	int m_motionCount = 0;	// 현재 동작의 개수

	~AnimationResource();	// GDI+가 파괴되기전에 비트맵 메모리 해제해야한다.

	void LoadAnimImage(const WCHAR* fileName);
	void LoadAnimMotion(const WCHAR* fileName,bool IsLoop=true);
};

