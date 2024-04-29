#include "stdafx.h"
#include "AnimationResource.h"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
AnimationResource::~AnimationResource()
{
	if (m_bitmap)
		delete m_bitmap;
}
void AnimationResource::LoadAnimImage(const WCHAR* fileName)
{
    m_fileName = fileName;
    m_bitmap = Gdiplus::Bitmap::FromFile(fileName);
}

void AnimationResource::LoadAnimMotion(const WCHAR* fileName,bool IsLoop)
{     
    wifstream file(fileName);
    if (!file.is_open()) {
        std::cout << "������ �� �� �����ϴ�." << endl;
    }	
    wstring line;			// ������ ���ڿ�	
	Motion motion;
	{
		getline(file, line);		// ù��° �� �б�
		wstringstream wss(line);		
		wss >> motion.FrameCount;
	}		

    for (int j = 0; j < motion.FrameCount; j++) 
	{
		getline(file, line);		// ���� �б�
		wstringstream wss(line);    // ������ �о wstringstream�� ����
		wstring token;
		{
			getline(wss, token, L',');	// wss�� ������ ,�� �������� ���ڿ��� �и�
			motion.Frames[j].Source.left = _wtoi(token.c_str());
			getline(wss, token, L',');
			motion.Frames[j].Source.top = _wtoi(token.c_str());
			getline(wss, token, L',');
			motion.Frames[j].Source.right = _wtoi(token.c_str());
			getline(wss, token, L',');
			motion.Frames[j].Source.bottom = _wtoi(token.c_str());
			getline(wss, token, L',');
			motion.Frames[j].CenterX = _wtoi(token.c_str());
			getline(wss, token, L',');
			motion.Frames[j].CenterY = _wtoi(token.c_str());
		}
	}
	motion.IsLoop = IsLoop;

	m_motions[m_motionCount] = motion;
	m_motionCount++;
}
