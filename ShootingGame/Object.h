#pragma once
#include "Vector2.h"

struct Object
{
	bool m_player = true; 
	bool m_isDead = false;
	float m_posX=0.0f, m_posY=0.0f;				// 현재 위치	
	float m_speed = 500.0f;						// 이동 가능한 속력

	Vector2 m_moveDir = { 0.0f,0.0f };			// 현재의 방향 벡터
	Vector2 m_moveDirPrev = { 0.0f,0.0f };		// 이전의 방향 벡터
	Vector2 m_inputDir;							// 입력 벡터

	SIZE  m_colliderSize ={ 50,50 };
	COLORREF m_color = RGB(255,255,255);
	
	void Init(bool player);
	void Update(float delta);
	void Render();
	bool Collide(const Object& other);
};

