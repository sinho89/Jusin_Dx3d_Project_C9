// Client.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
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

// ���� ����:
HINSTANCE g_hInst;								// ���� �ν��Ͻ��Դϴ�.
TCHAR szTitle[MAX_LOADSTRING];					// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
TCHAR szWindowClass[MAX_LOADSTRING];			// �⺻ â Ŭ���� �̸��Դϴ�.
HWND g_hWnd;
_bool g_bFocus;
_bool g_bSetAquire;
_bool g_bDebugRender = false;
_int	g_iPointLightIndex = 0;
//���̴� �ɼ� ���� ����ü
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

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
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

 	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
	MSG msg;
	HACCEL hAccelTable;
	CMainApp*			pMainApp = NULL;

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
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

	// �⺻ �޽��� �����Դϴ�.
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

			// 1�ʿ� ����� Ʈ�縦 ����, �������� �޽�����
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
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
//  ����:
//
//    Windows 95���� �߰��� 'RegisterClassEx' �Լ����� ����
//    �ش� �ڵ尡 Win32 �ý��۰� ȣȯ�ǵ���
//    �Ϸ��� ��쿡�� �� �Լ��� ����մϴ�. �� �Լ��� ȣ���ؾ�
//    �ش� ���� ���α׷��� �����
//    '�ùٸ� ������' ���� �������� ������ �� �ֽ��ϴ�.
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
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   g_hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

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
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����: �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND	- ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT	- �� â�� �׸��ϴ�.
//  WM_DESTROY	- ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
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
		// �޴��� ���� ������ ���� �м��մϴ�.
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
		// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
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

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
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
