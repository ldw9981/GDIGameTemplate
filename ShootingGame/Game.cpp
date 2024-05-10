#include "stdafx.h"
#include "Game.h"

#include "System.h"
#include "Object.h"
#include "List.h"

namespace Game
{
	const int MAX_ENEMY = 10;
	Object g_player;
	Object g_enemy[MAX_ENEMY];
	List<Object> g_enemyContainer;


	void Initialize(HWND hwnd,int witdh,int height)
	{
		Render::Initialize(hwnd, witdh, height);
		Input::Initialize(hwnd, witdh, height);
		Time::Initialize();

		g_player.Init(true);

		for (int i = 0; i < MAX_ENEMY; i++)
		{
			g_enemy[i].Init(false);
		}
	}

	void Update()
	{
		Time::UpdateTime();
		float deltaTime = Time::GetDeltaTime();
		Input::Update();


		g_player.m_inputDir.Set(0.0f, 0.0f);
		if (Input::IsCurrDn(VK_LEFT))
		{
			g_player.m_inputDir.x = -1;
		}
		else if (Input::IsCurrDn(VK_RIGHT))
		{
			g_player.m_inputDir.x = 1;
		}
		if (Input::IsCurrDn(VK_UP))
		{
			g_player.m_inputDir.y = -1;
		}
		else if (Input::IsCurrDn(VK_DOWN))
		{
			g_player.m_inputDir.y = 1;
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


	void Uninitialize()
	{
		Input::Uninitialize();
		Render::Uninitialize();
	}

}