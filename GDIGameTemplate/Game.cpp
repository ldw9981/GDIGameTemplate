#include "stdafx.h"
#include "Game.h"
#include "InputSystem.h"
#include "RenderSystem.h"
#include "TimeSystem.h"
#include "Object.h"
#include "AnimationResource.h"

#define MAX_ENEMY 10


namespace Game
{	
	Object g_player;
	Object g_enemy[MAX_ENEMY];
	AnimationResource* g_PlayerAnim;


	void Init(HWND hWnd, int width, int height)
	{
		Render::InitRender(hWnd, width, height);
		Input::InitInput(hWnd, width, height);
		Time::InitTime();

		g_PlayerAnim = new AnimationResource;

		g_PlayerAnim->LoadAnimImage(L"../Resource/Ken.png");
		g_PlayerAnim->LoadAnimMotion(L"../Resource/KenIdle.txt");				 //	OBJECT_STATUS_IDLE,
		g_PlayerAnim->LoadAnimMotion(L"../Resource/KenMove.txt");				 //	OBJECT_STATUS_MOVE,
		g_PlayerAnim->LoadAnimMotion(L"../Resource/KenAttack.txt", false);		 //	OBJECT_STATUS_ATTACK

		g_player.Init(true);
		g_player.m_pAnimationResource = g_PlayerAnim;
		g_player.SetMotion(0);


		for (int i = 0; i < MAX_ENEMY; i++)
		{
			g_enemy[i].Init(false);
		}
	}

	void Uninit()
	{
		delete g_PlayerAnim;

		Input::ReleaseInput();
		Render::Release();
		
	}
	void Update()
	{
		Time::UpdateTime();
		float deltaTime = Time::GetDeltaTime();
		Input::Update();


		g_player.m_input.Set(0.0f, 0.0f);
		if (Input::IsCurrDn(VK_LEFT))
		{
			g_player.m_input.x = -1;
		}
		else if (Input::IsCurrDn(VK_RIGHT))
		{
			g_player.m_input.x = 1;
		}
		if (Input::IsCurrDn(VK_UP))
		{
			g_player.m_input.y = -1;
		}
		else if (Input::IsCurrDn(VK_DOWN))
		{
			g_player.m_input.y = 1;
		}
		if (Input::IsTurnDn('O'))
		{
			std::cout << g_player.m_posX << ' ' << g_player.m_posY << std::endl;
		}
		if (Input::IsCurrDn(VK_LBUTTON))
		{
			POINT pt = Input::GetMouseClient();
			std::cout << pt.x << ' ' << pt.y << std::endl;
		}
		if (Input::IsTurnDn('T'))
		{
			std::cout << deltaTime << std::endl;
		}
		if (Input::IsTurnDn(VK_SPACE))
		{
			g_player.ChangeStatus(ObjectStatus::OBJECT_STATUS_ATTACK);
		}

		if (Input::IsTurnDn('1'))
		{
			Render::SetColorScale(1.0f, 1.0f, 1.0f, 0.1f);
		}



		// 위치 갱신
		g_player.Update(deltaTime);
		for (int i = 0; i < MAX_ENEMY; i++)
		{
			g_enemy[i].Update(deltaTime);
		}

		//충돌확인
		for (int i = 0; i < MAX_ENEMY; i++)
		{
			if (g_enemy[i].m_isDead == false)
			{
				if (g_player.Collide(g_enemy[i]))
				{
					g_enemy[i].m_isDead = true;
				}
			}
		}
	}
	void Render()
	{
		Render::BeginDraw();

		g_player.Render();
		for (int i = 0; i < MAX_ENEMY; i++)
		{
			g_enemy[i].Render();
		}
		Render::EndDraw();
	}
}

