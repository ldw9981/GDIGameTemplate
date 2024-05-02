#pragma once

enum ObjectStatus {
	OBJECT_STATUS_IDLE,
	OBJECT_STATUS_MOVE,
	OBJECT_STATUS_ATTACK
};

struct AnimationResource;
struct Object
{
	bool m_player = true; 
	bool m_isDead = false;
	float m_posX=0.0f, m_posY=0.0f;			// 현재 위치	
	float m_speed = 500.0f;						// 이동 가능한 속력
	float m_moveDirX=0.0f, m_moveDirY=0.0f;		// 방향 벡터
	float m_moveDirXPrev = 0.0f, m_moveDirYPrev = 0.0f;		// 이전 방향 벡터

	float m_inputDirX=0.0f,m_inputDirY=0.0f;	// 입력 벡터
	SIZE  m_colliderSize ={ 50,50 };
	COLORREF m_color = RGB(255,255,255);
	ObjectStatus m_status = OBJECT_STATUS_IDLE;

	AnimationResource* m_pAnimationResource = nullptr;	
	int m_AnimationMotionIndex = -1;
	int m_AnimationFrameIndex = 0;
	float m_AnimationAccTime = 0.0f;
	bool m_AnimationFlip = false;

	void Init(bool player);
	void Update(float delta);
	void Render();
	bool Collide(const Object& other);
	void SetMotion(int index);
	void UpdateAnimation(float delta);	
	void ChangeStatus(ObjectStatus status);
	void UpdateStatus(float delta);
};

