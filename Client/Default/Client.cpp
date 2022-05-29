// Client.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Client.h"

#ifdef _DEBUG
//#include <crtdbg.h>
//#include "vld.h"
#endif

#include "Export_System.h"
#include "MainApp.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE g_hInst;								// 현재 인스턴스입니다.
TCHAR szTitle[MAX_LOADSTRING];					// 제목 표시줄 텍스트입니다.
TCHAR szWindowClass[MAX_LOADSTRING];			// 기본 창 클래스 이름입니다.
HWND g_hWnd;
_bool g_bFocus;
_bool g_bSetAquire;
_bool g_bDebugRender = false;
_int	g_iPointLightIndex = 0;
//쉐이더 옵션 저장 구조체
SHADEROPTION_INFO g_ShaderOption_Info;

// ID
wstring g_wstrID;
bool g_bInputID = false;

// PASSWORD
wstring g_wstrPW;
bool g_bInputPW = false;

// NickName
wstring g_wstrNick;
bool g_bInputNick = false;

_bool g_bEnterToTown = false;
_bool g_bEnterToIce = false;
_bool g_bEnterToLoadingTown = false;
_bool g_bEnterToLoadingIce = false;
_bool g_bEnterToLoadingFire = false;

_bool g_bStageResource = false;

_bool g_bHanumanDead = FALSE;
_bool g_bBardielDead = FALSE;

//Sound Value
_float g_fBGMValue = 0.5f;
_float g_fEffectSoundValue = 0.5f;

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 여기에 코드를 입력합니다.
	MSG msg;
	HACCEL hAccelTable;
	CMainApp*			pMainApp = NULL;

	// 전역 문자열을 초기화합니다.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

	pMainApp = CMainApp::Create();
	if(pMainApp == NULL)
		return FALSE;

	if(FAILED(Engine::Ready_Timer(L"Timer_Immediate")))
		return FALSE;
	if(FAILED(Engine::Ready_Timer(L"Timer_FPS:60")))
		return FALSE;
	if(FAILED(Engine::Ready_Timer(L"Timer_FPS:200")))
		return FALSE;

	if(FAILED(Engine::Ready_Frame(L"Frame_60", 60.0f)))
		return FALSE;
	if(FAILED(Engine::Ready_Frame(L"Frame_200", 200.0f)))
		return FALSE;


	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

	// 기본 메시지 루프입니다.
	while(true)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(WM_QUIT == msg.message)
				break;

			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			Engine::SetUp_TimeDelta(L"Timer_Immediate");

			_float	fTimeDelta_Immediate = Engine::Get_TimeDelta(L"Timer_Immediate");

			// 1초에 백번만 트루를 리턴, 나머지는 펄스리턴
			if(Engine::isPermit_Call(L"Frame_60", fTimeDelta_Immediate))
			{
				Engine::SetUp_TimeDelta(L"Timer_FPS:60");

				_float	fTimeDelta_60 = Engine::Get_TimeDelta(L"Timer_FPS:60");				

				pMainApp->Update_MainApp(fTimeDelta_60);
				pMainApp->Render_MainApp();
			}
			/*if(Engine::isPermit_Call(L"Frame_200", fTimeDelta_Immediate))
			{
				Engine::SetUp_TimeDelta(L"Timer_FPS:200");

				_float	fTimeDelta_200 = Engine::Get_TimeDelta(L"Timer_FPS:200");

				pMainApp->Update_MainApp(fTimeDelta_200);
				pMainApp->Render_MainApp();
			}*/
		}
	}


	_ulong dwRefCnt = 0;
	if(dwRefCnt = Safe_Release(pMainApp))
	{
		MSG_BOX("CMainApp Release Failed");
		return FALSE;
	}	

	return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
//  설명:
//
//    Windows 95에서 추가된 'RegisterClassEx' 함수보다 먼저
//    해당 코드가 Win32 시스템과 호환되도록
//    하려는 경우에만 이 함수를 사용합니다. 이 함수를 호출해야
//    해당 응용 프로그램에 연결된
//    '올바른 형식의' 작은 아이콘을 가져올 수 있습니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   g_hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   RECT			rcWindow = {0, 0, WINSIZEX, WINSIZEY};
   AdjustWindowRect(&rcWindow, WS_POPUP, FALSE);

   hWnd = CreateWindow(szWindowClass, szTitle, WS_POPUP,
	   CW_USEDEFAULT, 0, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top, NULL, NULL, hInstance, NULL);


   if (!hWnd)
   {
      return FALSE;
   }

   g_hWnd = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND	- 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT	- 주 창을 그립니다.
//  WM_DESTROY	- 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_KILLFOCUS:
		g_bFocus = false;
		g_bSetAquire = true;
		break;

	case WM_SETFOCUS:
		g_bFocus = true;
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 메뉴의 선택 영역을 구문 분석합니다.
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_BACK:
			if(g_wstrID.size() && g_bInputID)
				g_wstrID.erase(g_wstrID.size() - 1);
			if(g_wstrPW.size() && g_bInputPW)
				g_wstrPW.erase(g_wstrPW.size() - 1);
			if(g_wstrNick.size() && g_bInputNick)
				g_wstrNick.erase(g_wstrNick.size() - 1);
			break;
		default:
			if(g_bInputID && !g_bInputPW)
				g_wstrID += wParam;
			if(!g_bInputID && g_bInputPW)
				g_wstrPW += L"*";
			if(g_bInputNick)
				g_wstrNick += wParam;
			break;
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 그리기 코드를 추가합니다.
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
