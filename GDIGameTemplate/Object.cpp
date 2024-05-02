#include "stdafx.h"
#include "Object.h"
#include "RenderSystem.h"
#include "AnimationResource.h"

#define ANIMATION_DELAY 0.05f

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
	 m_moveDirY = 0.0f;		// ���� ����
	 m_inputDirX = 0.0f;
	 m_inputDirY = 0.0f;	// �Է� ����
}

void Object::Update(float delta)
{
	// �Է� ���͸� Normalize �Ͽ�  ���� ���ͷ� ��ȯ
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
		m_AnimationFlip = m_moveDirX < 0 ? true : false;
	}

	//���� ���̸� ����	�������� �ʴ´�.
	if (m_status != ObjectStatus::OBJECT_STATUS_ATTACK) 
	{
		if (m_moveDirXPrev == 0.0f && m_moveDirYPrev == 0.0f)
		{
			if (m_moveDirX != 0.0f || m_moveDirY != 0.0f)
				ChangeStatus(ObjectStatus::OBJECT_STATUS_MOVE);
		}
		else if (m_moveDirXPrev != 0.0f || m_moveDirYPrev != 0.0f)
		{
			if (m_moveDirX == 0.0f && m_moveDirY == 0.0f)
				ChangeStatus(ObjectStatus::OBJECT_STATUS_IDLE);
		}
	}


	// ȭ�� ������ ������ �ʵ��� ó��
	SIZE size = Render::GetScreenSize();
	if (m_posX < 0)
		m_posX = 0;
	else if (m_posX > size.cx)
		m_posX = (float)size.cx;
	
	if (m_posY < 0)
		m_posY = 0;
	else if (m_posY > size.cy)
		m_posY = (float)size.cy;

	if (m_pAnimationResource && m_AnimationMotionIndex != -1)
		UpdateAnimation(delta);

	// ���� ���� ���� ����
	m_moveDirXPrev = m_moveDirX;
	m_moveDirYPrev = m_moveDirY;
}

void Object::Render()
{
	if(m_isDead)
		return;
	
	// �ִϸ��̼� ���ҽ��� �ְ� Ư�� ����� �����Ǿ� ������ �ش� �������� �׸���.
	if (m_pAnimationResource && m_AnimationMotionIndex != -1)
	{
		Frame& frame = m_pAnimationResource->m_motions[m_AnimationMotionIndex].Frames[m_AnimationFrameIndex];
		Gdiplus::Bitmap* bitmap = m_AnimationFlip ? m_pAnimationResource->m_bitmapFlip : m_pAnimationResource->m_bitmap;
		SIZE size = Render::GetScreenSize();
		
		int x = m_AnimationFlip ? (int)m_posX - (frame.Size.cx - frame.CenterX) : (int)m_posX - frame.CenterX;
		int y = (int)m_posY - frame.CenterY;
		int srcX = m_AnimationFlip ? m_pAnimationResource->m_bitmapFlip->GetWidth() - frame.Size.cx - frame.Source.left : frame.Source.left;
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
	m_AnimationMotionIndex = index;
	m_AnimationFrameIndex = 0;
	m_AnimationAccTime = 0.0f;
}

void Object::UpdateAnimation(float delta)
{
	int frameCount = m_pAnimationResource->m_motions[m_AnimationMotionIndex].FrameCount;
	bool isLoop = m_pAnimationResource->m_motions[m_AnimationMotionIndex].IsLoop;

	// ������ �ð����� ������ ������ ã��
	m_AnimationAccTime += delta;
	while (m_AnimationAccTime > ANIMATION_DELAY)
	{
		m_AnimationAccTime -= ANIMATION_DELAY;
	
		if(m_AnimationFrameIndex < frameCount)
			m_AnimationFrameIndex++;
	}

	// �� ������ �Ѿ���� ó��
	if (m_AnimationFrameIndex >= frameCount)
	{
		if (isLoop)
		{
			m_AnimationFrameIndex = 0;
		}
		else
		{					
			if (m_status == ObjectStatus::OBJECT_STATUS_ATTACK)
				ChangeStatus(ObjectStatus::OBJECT_STATUS_IDLE);
		}
	}
}

void Object::ChangeStatus(ObjectStatus status)
{
	if (m_status == status)
		return;

	// End ���� ���¸� �����鼭 ó���� ������ �ۼ�
	switch (m_status)
	{
	case OBJECT_STATUS_IDLE:
		break;	
	case OBJECT_STATUS_MOVE:
		break;
	case OBJECT_STATUS_ATTACK:
		break;
	default:
		break;
	}


	m_status = status;

	// Begin ���ο� ���°� ���۵ɶ� ó���� ������ �ۼ�
	switch (m_status)
	{
	case ObjectStatus::OBJECT_STATUS_IDLE:
		SetMotion(ObjectStatus::OBJECT_STATUS_IDLE);
		break;
	case ObjectStatus::OBJECT_STATUS_MOVE:
		SetMotion(ObjectStatus::OBJECT_STATUS_MOVE);
		break;
	case ObjectStatus::OBJECT_STATUS_ATTACK:
		SetMotion(ObjectStatus::OBJECT_STATUS_ATTACK);
		break;
	}
}





