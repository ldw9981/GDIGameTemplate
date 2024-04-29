#pragma once


struct Object
{
	bool m_player = true; 
	bool m_isDead = false;
	float m_posX=0.0f, m_posY=0.0f;
	float m_speed = 500.0f;
	float m_moveDirX=0.0f, m_moveDirY=0.0f;		// πÊ«‚ ∫§≈Õ
	float m_inputDirX=0.0f,m_inputDirY=0.0f;	// ¿‘∑¬ ∫§≈Õ

	SIZE  m_colliderSize ={ 50,50 };

	COLORREF m_color = RGB(255,255,255);
	
	void Init(bool player);
	void Update(float delta);
	void Render();
	bool Collide(const Object& other);
};

