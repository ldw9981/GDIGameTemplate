#include "stdafx.h"
#include "Object.h"
#include "RenderSystem.h"
#include "AnimationResource.h"

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

	if (m_moveDirX != 0.0f)
	{
		if (m_moveDirX > 0)
			m_Flip = false;
		else
			m_Flip = true;
	}


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


	if (m_pAnimationResource && m_MotionIndex != -1)
	{
		m_AnimationAccTime += delta;
		if (m_AnimationAccTime >= 0.1f)
		{
			m_AnimationAccTime -= 0.1f;
			m_FrameIndex++;
			if (m_FrameIndex >= m_pAnimationResource->m_motions[m_MotionIndex].FrameCount)
			{
				m_FrameIndex = 0;
			}			
		}
	}
}

void Object::Render()
{
	if(m_isDead)
		return;
	
	// 애니메이션 리소스가 있고 특정 모션이 설정되어 있으면 해당 프레임을 그린다.
	if (m_pAnimationResource && m_MotionIndex != -1)
	{
		Frame& frame = m_pAnimationResource->m_motions[m_MotionIndex].Frames[m_FrameIndex];
		Gdiplus::Bitmap* bitmap = m_Flip ? m_pAnimationResource->m_bitmapFlip : m_pAnimationResource->m_bitmap;
		SIZE size = Render::GetScreenSize();
		
		int x = m_Flip ? (int)m_posX - (frame.Size.cx - frame.CenterX) : (int)m_posX - frame.CenterX;
		int y = (int)m_posY - frame.CenterY;
		int srcX = m_Flip ? m_pAnimationResource->m_bitmapFlip->GetWidth() - frame.Size.cx - frame.Source.left : frame.Source.left;
		int srcY = frame.Source.top;
	
		Render::DrawGDIBitmap(x, y, bitmap, srcX, srcY, frame.Size.cx, frame.Size.cy);		
	}
	

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

void Object::SetMotion(int index)
{
	if (m_pAnimationResource == nullptr)
		return;

	assert(m_pAnimationResource->m_motionCount > index);
	m_MotionIndex = index;
	m_FrameIndex = 0;
	m_AnimationAccTime = 0.0f;
}

