#pragma once
#include "Vector2.h"

struct Object
{
	bool m_player = true; 
	bool m_isDead = false;
	float m_posX=0.0f, m_posY=0.0f;				// ���� ��ġ	
	float m_speed = 500.0f;						// �̵� ������ �ӷ�

	Vector2 m_moveDir = { 0.0f,0.0f };			// ������ ���� ����
	Vector2 m_moveDirPrev = { 0.0f,0.0f };		// ������ ���� ����
	Vector2 m_inputDir;							// �Է� ����

	SIZE  m_colliderSize ={ 50,50 };
	COLORREF m_color = RGB(255,255,255);
	
	void Init(bool player);
	void Update(float delta);
	void Render();
	bool Collide(const Object& other);
};

