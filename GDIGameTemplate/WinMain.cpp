#include "stdafx.h"
#include "InputSystem.h"
#include "RenderSystem.h"
#include "TimeSystem.h"
#include "Object.h"
#include "AnimationResource.h"


#define MAX_ENEMY 10

Object g_player;
Object g_enemy[MAX_ENEMY];

AnimationResource* g_PlayerAnim;


bool g_Mirror = false;
// 비주얼 스튜디오가 만든 템플릿은 다른 추가적인 내용이 많아 이해하기 어려워 가장 간단하게 작성함.

// 윈도우 프로시저 함수 선언
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void UpdateGame();
void RenderGame();
void LoadResource();
void ReleaseResource();

// 프로젝트 속성 -> 링커 -> 시스템 -> 하위 시스템 -> Windows로 변경
// 진입점 함수 정의
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// 윈도우 클래스 구조체 초기화
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;	// 윈도우 프로시저 함수
	wc.hInstance = hInstance;   // 인스턴스 핸들
	// NULL을 사용하면 현재 실행 중인 프로그램의 인스턴스 핸들을 사용하게 됩니다.두 번째 인자는 로드할 커서의 ID입니다.
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszClassName = L"BasicWindowClass";

	// 윈도우 클래스 등록
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"윈도우 클래스 등록 실패", L"에러", MB_OK | MB_ICONERROR);
		return 1;
	}

	// 원하는 크기가 조정되어 리턴
	SIZE clientSize = { 1920, 1280 };
	RECT clientRect = { 0, 0, clientSize.cx, clientSize.cy };
	AdjustWindowRect(&clientRect, WS_OVERLAPPEDWINDOW, FALSE);

	// 윈도우 생성
	HWND hwnd = CreateWindowEx(
		0,
		L"BasicWindowClass",
		L"간단한 윈도우",
		WS_OVERLAPPEDWINDOW,	// OR연산으로 조합된 윈도우창 스타일
		0,0,	// 시작위치
		clientRect.right-clientRect.left,  clientRect.bottom - clientRect.top , // 너비, 높이
		NULL, NULL, hInstance, NULL
	);

	if (!hwnd)
	{
		MessageBox(NULL, L"윈도우 생성 실패", L"에러", MB_OK | MB_ICONERROR);
		return 1;
	}

	// 윈도우 표시
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	bool bUseConsole=false;
	if (bUseConsole)
	{
		AllocConsole();
		FILE* _tempFile;
		freopen_s(&_tempFile, "CONOUT$", "w", stdout);
	}	

	Render::InitRender(hwnd, clientSize.cx, clientSize.cy);
	Input::InitInput(hwnd, clientSize.cx, clientSize.cy);
	Time::InitTime();
	LoadResource();

	g_player.Init(true);
	g_player.m_pAnimationResource = g_PlayerAnim;
	g_player.SetMotion(0);
	

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		g_enemy[i].Init(false);
	}

	// 대기가 없는 메세지 루프
	MSG msg;
	while (true)
	{
		// 메시지가 있으면 처리, 없으면 바로 게임 루프로 이동
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// WM_QUIT 메시지 확인
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// 게임 루프					
			UpdateGame();
			RenderGame();
		}
	}

	ReleaseResource();
	Input::ReleaseInput();
	Render::Release();
	if (bUseConsole)
	{
		FreeConsole();
	}

	return static_cast<int>(msg.wParam);
}

// 윈도우 프로시저 함수 정의
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

void UpdateGame()
{
	Time::UpdateTime();
	float deltaTime = Time::GetDeltaTime();
	Input::Update();


	g_player.m_inputDir.Set(0.0f,0.0f);
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
void RenderGame()
{
	Render::BeginDraw();
	
	g_player.Render();
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		g_enemy[i].Render();
	}
	Render::EndDraw();
}

void LoadResource()
{
	g_PlayerAnim = new AnimationResource;

	g_PlayerAnim->LoadAnimImage(L"../Resource/Ken.png");
	g_PlayerAnim->LoadAnimMotion(L"../Resource/KenIdle.txt");				 //	OBJECT_STATUS_IDLE,
	g_PlayerAnim->LoadAnimMotion(L"../Resource/KenMove.txt");				 //	OBJECT_STATUS_MOVE,
	g_PlayerAnim->LoadAnimMotion(L"../Resource/KenAttack.txt", false);		 //	OBJECT_STATUS_ATTACK

}

void ReleaseResource()
{
	delete g_PlayerAnim;
}