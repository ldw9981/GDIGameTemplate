#include "stdafx.h"
#include "Object.h"
#include "System.h"


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
	 m_moveDir.Set(0.0f,0.0f);		// 방향 벡터
	 m_moveDirPrev.Set(0.0f,0.0f);	// 이전 방향 벡터
	 m_inputDir.Set(0.0f,0.0f);		// 입력 벡터	
}

void Object::Update(float delta)
{
	// 입력 벡터를 Normalize 하여  방향 벡터로 변환
	if (m_inputDir != Vector2(0.0f, 0.0f))
	{
		m_inputDir.Normalize();
		m_moveDir = m_inputDir;
	}
	else
	{
		m_moveDir = Vector2(0.0f, 0.0f);
	}	

	m_posX += m_moveDir.x * m_speed * delta;
	m_posY += m_moveDir.y * m_speed * delta;	


	// 화면 밖으로 나가지 않도록 처리
	SIZE size = Render::GetScreenSize();
	m_posX = std::clamp(m_posX, 0.0f, (float)size.cx);	// 프로젝트 설정에서 C++17로 변경해야 사용가능
	m_posY = std::clamp(m_posY, 0.0f, (float)size.cy);

	// 이전 방향 벡터 저장
	m_moveDirPrev = m_moveDir;
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
	RECT rc = { (LONG)m_posX - m_colliderSize.cx / 2, (LONG)m_posY - m_colliderSize.cy / 2,
			(LONG)m_posX + m_colliderSize.cx / 2, (LONG)m_posY + m_colliderSize.cy / 2 };

	RECT rcOther = { (LONG)other.m_posX - other.m_colliderSize.cx / 2, (LONG)other.m_posY - other.m_colliderSize.cy / 2,
				(LONG)other.m_posX + other.m_colliderSize.cx / 2, (LONG)other.m_posY + other.m_colliderSize.cy / 2 };
	
	RECT rcIntersect;
	if (::IntersectRect(&rcIntersect,&rc,&rcOther))
	{	
		return true;
	}	
	return false;
}






