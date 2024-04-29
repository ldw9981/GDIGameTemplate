#include "stdafx.h"
#include "Object.h"
#include "RenderSystem.h"

void Object::Init(bool player)
{
	m_player = player;
	if (m_player)
	{
		m_posX = 300.0f;
		m_posY = 300.0f;
		m_color = RGB(0, 0, 255);
	}
	else
	{
		m_posX = (float)(rand()%640);
		m_posY = (float)(rand()%480);
		m_color = RGB(255, 0, 0);
	}
			

	 m_isDead = false;
	 m_moveDirX = 0.0f;
	 m_moveDirY = 0.0f;		// 방향 벡터
	 m_inputDirX = 0.0f;
	 m_inputDirY = 0.0f;	// 입력 벡터
}

void Object::Update(float delta)
{
	// 입력 벡터를 방향 벡터로 변환
	if(m_inputDirX != 0.0f)
		m_moveDirX = m_inputDirX / sqrt(m_inputDirX* m_inputDirX + m_inputDirY * m_inputDirY);
	else
		m_moveDirX = 0.0f;

	if(m_inputDirY != 0.0f)
		m_moveDirY = m_inputDirY / sqrt(m_inputDirX * m_inputDirX + m_inputDirY * m_inputDirY);
	else
		m_moveDirY = 0.0f;

	m_posX += m_moveDirX * m_speed * delta;
	m_posY += m_moveDirY * m_speed * delta;	


	// 화면 밖으로 나가지 않도록 처리
	SIZE size = Render::GetScreenSize();
	if (m_posX < 0)
		m_posX = 0;
	else if (m_posX > size.cx)
		m_posX = (float)size.cx;
	
	if (m_posY < 0)
		m_posY = 0;
	else if (m_posY > size.cy)
		m_posY = (float)size.cy;
}

void Object::Render()
{
	if(m_isDead)
		return;

	Render::DrawRect((int)m_posX - m_colliderSize.cx / 2, (int)m_posY - m_colliderSize.cy / 2,
		(int)m_colliderSize.cx, (int)m_colliderSize.cy, m_color);
}

bool Object::Collide(const Object& other)
{
	if (m_posX < other.m_posX + other.m_colliderSize.cx &&
		m_posX + m_colliderSize.cx > other.m_posX &&
		m_posY < other.m_posY + other.m_colliderSize.cy &&
		m_posY + m_colliderSize.cy > other.m_posY)
	{	
		return true;
	}	
	return false;
}

