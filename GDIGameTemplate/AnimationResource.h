#pragma once

// ������ �ϳ��� ������ ����
struct Frame
{
	RECT Source = {0,};			// �κ������ �̹��� ����
	int CenterX=0,CenterY=0;	// �߽��� �Ǵ� �̵� ��ġ 
};

struct Motion 
{
	Frame Frames[10];		// ������ �����̳�
	int FrameCount=0;			// ��ü ������ ��
	bool IsLoop = false;	// �ݺ� ����
};

struct AnimationResource
{	
	std::wstring m_fileName;	

	Gdiplus::Bitmap* m_bitmap = nullptr;
	Motion m_motions[10];	// �ִ� 10���� ������ ���� �� ����
	int m_motionCount = 0;	// ���� ������ ����

	~AnimationResource();

	void LoadAnimImage(const WCHAR* fileName);
	void LoadAnimMotion(const WCHAR* fileName,bool IsLoop=true);
};

