
// MapToolView.cpp : CMapToolView 클래스의 구현
//

#include "stdafx.h"
#include "MapTool.h"

#include "MapToolDoc.h"
#include "MapToolView.h"


#include "MainFrm.h"
#include "MainApp.h"
#include "Export_Engine.h"

#include "ButtonMenu.h"
#include "MapToolTab.h"

#include "Scene_MapTool.h"

#include "Environment.h"
#include "NPC.h"
#include "Monster.h"
#include "Player.h"
#include "Cube.h"
#include "Trap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMapToolView

IMPLEMENT_DYNCREATE(CMapToolView, CView)

BEGIN_MESSAGE_MAP(CMapToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMapToolView::OnFilePrintPreview)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CMapToolView 생성/소멸

CMapToolView::CMapToolView()
: m_pGraphicDev(NULL)
, m_pMainApp(NULL)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMapToolView::~CMapToolView()
{
	Engine::Safe_Release(m_pGraphicDev);

	_ulong dwRefCnt = 0;
	dwRefCnt = Engine::Safe_Release(m_pMainApp);
}

BOOL CMapToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	g_hWnd = m_hWnd;
	g_hInst = AfxGetInstanceHandle();

	m_pMainApp = CMainApp::Create();
	if(m_pMainApp == NULL)
		return FALSE;

	if(FAILED(m_pMainApp->Get_GraphicDev(&m_pGraphicDev)))
		return FALSE;

	if(FAILED(Engine::Ready_Timer(L"Timer_Immediate")))
		return FALSE;
	if(FAILED(Engine::Ready_Timer(L"Timer_FPS:60")))
		return FALSE;
	if(FAILED(Engine::Ready_Frame(L"Frame_60", 60.0f)))
		return FALSE;

	return CView::PreCreateWindow(cs);
}

// CMapToolView 그리기

void CMapToolView::OnDraw(CDC* /*pDC*/)
{
	CMapToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.

	Engine::SetUp_TimeDelta(L"Timer_Immediate");

	_float	fTimeDelta_Immediate = Engine::Get_TimeDelta(L"Timer_Immediate");

	// 1초에 백번만 트루를 리턴, 나머지는 펄스리턴
	if(Engine::isPermit_Call(L"Frame_60", fTimeDelta_Immediate))
	{

		Engine::SetUp_TimeDelta(L"Timer_FPS:60");

		_float	fTimeDelta_60 = Engine::Get_TimeDelta(L"Timer_FPS:60");				

		m_pMainApp->Update_MainApp(fTimeDelta_60);
		m_pMainApp->Render_MainApp();


		m_pGraphicDev->EndScene();
		m_pGraphicDev->Present(NULL, NULL, GetSafeHwnd(), NULL);
	}


}


// CMapToolView 인쇄


void CMapToolView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CMapToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMapToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMapToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CMapToolView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMapToolView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}

void CMapToolView::AssertValid() const
{
	CView::AssertValid();
}

void CMapToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

void CMapToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	//1.매핑 모드 지정 인자(이 뷰에 대한 스크롤 맵핑 모드)
	//스크롤과 실제 화면과의 비율에 대한 설정..
	//MM_TEXT - 1:1 픽셀 단위 매핑
	//MM_HIMETRIC - 0.01mm 단위로 하고
	//MM_ISOTROPIC, MM_ANISOTROPIC 사용하지 말 것...

	//2.스크롤 뷰의 토탈 사이즈가 들어간다...
	//CSize라는 것에 가로, 세로 사이즈가 들어가면 됨...
	//3, 4 인자는
	//가로 세로 스크롤 시 마우스 움직임하고 스크롤바 움직임 사이의 비율을 따로 설정할 수가 있다...

	//창 사이즈를 조절해보자...
	CMainFrame*	pFrame = (CMainFrame*)AfxGetMainWnd();

	RECT	rcWindow;
	pFrame->GetWindowRect(&rcWindow);

	SetRect(&rcWindow, 0, 0, 
		rcWindow.right - rcWindow.left,
		rcWindow.bottom - rcWindow.top);

	RECT	rcMainView;
	GetClientRect(&rcMainView);

	float fRowFrm = float(rcWindow.right - rcMainView.right);
	float fColFrm = float(rcWindow.bottom - rcMainView.bottom);

	pFrame->SetWindowPos(NULL, 0, 0, 
		int(WINSIZEX + fRowFrm),						//WINCX
		int(WINSIZEY + fColFrm),						//WINCY
		SWP_NOZORDER);

	//g_hWnd = m_hWnd;

	//((CMapToolApp*)AfxGetApp())->CreateMain();
}


// CMapToolView 진단

#ifdef _DEBUG

int CMapToolView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}

CMapToolDoc* CMapToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapToolDoc)));
	return (CMapToolDoc*)m_pDocument;
}


#endif //_DEBUG


// CMapToolView 메시지 처리기

void CMapToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnMouseMove(nFlags, point);

	g_vMouse.x = float(point.x);
	g_vMouse.y = float(point.y);
	g_vMouse.z = 0.f;

	CMapToolTab* pDlg = ((CMainFrame*)AfxGetMainWnd())->m_pButtonMenu->m_pMapToolTab;

	pDlg->SetDlgItemInt(IDC_EDIT_MOUSEX, int(g_vViewMouse.x));
	pDlg->SetDlgItemInt(IDC_EDIT_MOUSEY, int(g_vViewMouse.y));
	pDlg->SetDlgItemInt(IDC_EDIT_MOUSEZ, int(g_vViewMouse.z));


	if( Engine::FindList(L"Environment") != NULL )
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Environment")->begin();
		list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"Environment")->end();
		for(iter; iter != iter_end;iter++)
		{
			if(((CEnvironment*)(*iter))->GetMode() == MODE_MODIFY)
			{
				_vec3 vPos;
				((CEnvironment*)(*iter))->GetTransCom()->Get_Infomation(Engine::INFO_POSITION, &vPos);
				if(!((CEnvironment*)(*iter))->GetChecked())
				{
					pDlg->m_ctrObjXSlider.SetRange(int((vPos.x - 5.f)*100.f), int((vPos.x + 5.f)*100.f));
					pDlg->m_ctrObjXSlider.SetRangeMin(int((vPos.x - 5.f)*100.f));
					pDlg->m_ctrObjXSlider.SetRangeMax(int((vPos.x + 5.f)*100.f));
					pDlg->m_ctrObjXSlider.SetPos(int(vPos.x * 100.f));

					pDlg->m_ctrObjYSlider.SetRange(int((vPos.y - 5.f)*100.f), int((vPos.y + 5.f)*100.f));
					pDlg->m_ctrObjYSlider.SetRangeMin(int((vPos.y - 5.f)*100.f));
					pDlg->m_ctrObjYSlider.SetRangeMax(int((vPos.y + 5.f)*100.f));
					pDlg->m_ctrObjYSlider.SetPos(int(vPos.y * 100.f));

					pDlg->m_ctrObjZSlider.SetRange(int((vPos.z - 5.f)*100.f), int((vPos.z + 5.f)*100.f));
					pDlg->m_ctrObjZSlider.SetRangeMin(int((vPos.z - 5.f)*100.f));
					pDlg->m_ctrObjZSlider.SetRangeMax(int((vPos.z + 5.f)*100.f));
					pDlg->m_ctrObjZSlider.SetPos(int(vPos.z * 100.f));

					((CEnvironment*)(*iter))->CheckTrue();
				}
				
			}
		}
	}

	if( Engine::FindList(L"NPC") != NULL )
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"NPC")->begin();
		list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"NPC")->end();
		for(iter; iter != iter_end;iter++)
		{
			if(((CNPC*)(*iter))->GetMode() == MODE_MODIFY)
			{
				_vec3 vPos;
				((CNPC*)(*iter))->GetTransCom()->Get_Infomation(Engine::INFO_POSITION, &vPos);
				if(!((CNPC*)(*iter))->GetChecked())
				{
					pDlg->m_ctrObjXSlider.SetRange(int((vPos.x - 5.f)*100.f), int((vPos.x + 5.f)*100.f));
					pDlg->m_ctrObjXSlider.SetRangeMin(int((vPos.x - 5.f)*100.f));
					pDlg->m_ctrObjXSlider.SetRangeMax(int((vPos.x + 5.f)*100.f));
					pDlg->m_ctrObjXSlider.SetPos(int(vPos.x * 100.f));

					pDlg->m_ctrObjYSlider.SetRange(int((vPos.y - 5.f)*100.f), int((vPos.y + 5.f)*100.f));
					pDlg->m_ctrObjYSlider.SetRangeMin(int((vPos.y - 5.f)*100.f));
					pDlg->m_ctrObjYSlider.SetRangeMax(int((vPos.y + 5.f)*100.f));
					pDlg->m_ctrObjYSlider.SetPos(int(vPos.y * 100.f));

					pDlg->m_ctrObjZSlider.SetRange(int((vPos.z - 5.f)*100.f), int((vPos.z + 5.f)*100.f));
					pDlg->m_ctrObjZSlider.SetRangeMin(int((vPos.z - 5.f)*100.f));
					pDlg->m_ctrObjZSlider.SetRangeMax(int((vPos.z + 5.f)*100.f));
					pDlg->m_ctrObjZSlider.SetPos(int(vPos.z * 100.f));

					((CNPC*)(*iter))->CheckTrue();
				}

			}
		}
	}

	if( Engine::FindList(L"Monster") != NULL )
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Monster")->begin();
		list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"Monster")->end();
		for(iter; iter != iter_end;iter++)
		{
			if(((CMonster*)(*iter))->GetMode() == MODE_MODIFY)
			{
				_vec3 vPos;
				((CMonster*)(*iter))->GetTransCom()->Get_Infomation(Engine::INFO_POSITION, &vPos);
				if(!((CMonster*)(*iter))->GetChecked())
				{
					pDlg->m_ctrObjXSlider.SetRange(int((vPos.x - 5.f)*100.f), int((vPos.x + 5.f)*100.f));
					pDlg->m_ctrObjXSlider.SetRangeMin(int((vPos.x - 5.f)*100.f));
					pDlg->m_ctrObjXSlider.SetRangeMax(int((vPos.x + 5.f)*100.f));
					pDlg->m_ctrObjXSlider.SetPos(int(vPos.x * 100.f));

					pDlg->m_ctrObjYSlider.SetRange(int((vPos.y - 5.f)*100.f), int((vPos.y + 5.f)*100.f));
					pDlg->m_ctrObjYSlider.SetRangeMin(int((vPos.y - 5.f)*100.f));
					pDlg->m_ctrObjYSlider.SetRangeMax(int((vPos.y + 5.f)*100.f));
					pDlg->m_ctrObjYSlider.SetPos(int(vPos.y * 100.f));

					pDlg->m_ctrObjZSlider.SetRange(int((vPos.z - 5.f)*100.f), int((vPos.z + 5.f)*100.f));
					pDlg->m_ctrObjZSlider.SetRangeMin(int((vPos.z - 5.f)*100.f));
					pDlg->m_ctrObjZSlider.SetRangeMax(int((vPos.z + 5.f)*100.f));
					pDlg->m_ctrObjZSlider.SetPos(int(vPos.z * 100.f));

					((CMonster*)(*iter))->CheckTrue();
				}

			}
		}
	}

	if( Engine::FindList(L"Player") != NULL )
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Player")->begin();
		list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"Player")->end();
		for(iter; iter != iter_end;iter++)
		{
			if(((CPlayer*)(*iter))->GetMode() == MODE_MODIFY)
			{
				_vec3 vPos;
				((CPlayer*)(*iter))->GetTransCom()->Get_Infomation(Engine::INFO_POSITION, &vPos);
				if(!((CPlayer*)(*iter))->GetChecked())
				{
					pDlg->m_ctrObjXSlider.SetRange(int((vPos.x - 5.f)*100.f), int((vPos.x + 5.f)*100.f));
					pDlg->m_ctrObjXSlider.SetRangeMin(int((vPos.x - 5.f)*100.f));
					pDlg->m_ctrObjXSlider.SetRangeMax(int((vPos.x + 5.f)*100.f));
					pDlg->m_ctrObjXSlider.SetPos(int(vPos.x * 100.f));

					pDlg->m_ctrObjYSlider.SetRange(int((vPos.y - 5.f)*100.f), int((vPos.y + 5.f)*100.f));
					pDlg->m_ctrObjYSlider.SetRangeMin(int((vPos.y - 5.f)*100.f));
					pDlg->m_ctrObjYSlider.SetRangeMax(int((vPos.y + 5.f)*100.f));
					pDlg->m_ctrObjYSlider.SetPos(int(vPos.y * 100.f));

					pDlg->m_ctrObjZSlider.SetRange(int((vPos.z - 5.f)*100.f), int((vPos.z + 5.f)*100.f));
					pDlg->m_ctrObjZSlider.SetRangeMin(int((vPos.z - 5.f)*100.f));
					pDlg->m_ctrObjZSlider.SetRangeMax(int((vPos.z + 5.f)*100.f));
					pDlg->m_ctrObjZSlider.SetPos(int(vPos.z * 100.f));

					((CPlayer*)(*iter))->CheckTrue();
				}

			}
		}
	}

	if( Engine::FindList(L"Cube") != NULL )
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Cube")->begin();
		list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"Cube")->end();
		for(iter; iter != iter_end;iter++)
		{
			if(((CCube*)(*iter))->GetMode() == MODE_MODIFY)
			{
				_vec3 vPos;
				((CCube*)(*iter))->GetTransCom()->Get_Infomation(Engine::INFO_POSITION, &vPos);
				if(!((CCube*)(*iter))->GetChecked())
				{
					pDlg->m_ctrObjXSlider.SetRange(int((vPos.x - 5.f)*100.f), int((vPos.x + 5.f)*100.f));
					pDlg->m_ctrObjXSlider.SetRangeMin(int((vPos.x - 5.f)*100.f));
					pDlg->m_ctrObjXSlider.SetRangeMax(int((vPos.x + 5.f)*100.f));
					pDlg->m_ctrObjXSlider.SetPos(int(vPos.x * 100.f));

					pDlg->m_ctrObjYSlider.SetRange(int((vPos.y - 5.f)*100.f), int((vPos.y + 5.f)*100.f));
					pDlg->m_ctrObjYSlider.SetRangeMin(int((vPos.y - 5.f)*100.f));
					pDlg->m_ctrObjYSlider.SetRangeMax(int((vPos.y + 5.f)*100.f));
					pDlg->m_ctrObjYSlider.SetPos(int(vPos.y * 100.f));

					pDlg->m_ctrObjZSlider.SetRange(int((vPos.z - 5.f)*100.f), int((vPos.z + 5.f)*100.f));
					pDlg->m_ctrObjZSlider.SetRangeMin(int((vPos.z - 5.f)*100.f));
					pDlg->m_ctrObjZSlider.SetRangeMax(int((vPos.z + 5.f)*100.f));
					pDlg->m_ctrObjZSlider.SetPos(int(vPos.z * 100.f));

					((CCube*)(*iter))->CheckTrue();
				}

			}
		}
	}

	if (Engine::FindList(L"Trap") != NULL)
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Trap")->begin();
		list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"Trap")->end();
		for (iter; iter != iter_end; iter++)
		{
			if (((CTrap*)(*iter))->GetMode() == MODE_MODIFY)
			{
				_vec3 vPos;
				((CTrap*)(*iter))->GetTransCom()->Get_Infomation(Engine::INFO_POSITION, &vPos);
				if (!((CTrap*)(*iter))->GetChecked())
				{
					pDlg->m_ctrObjXSlider.SetRange(int((vPos.x - 5.f)*100.f), int((vPos.x + 5.f)*100.f));
					pDlg->m_ctrObjXSlider.SetRangeMin(int((vPos.x - 5.f)*100.f));
					pDlg->m_ctrObjXSlider.SetRangeMax(int((vPos.x + 5.f)*100.f));
					pDlg->m_ctrObjXSlider.SetPos(int(vPos.x * 100.f));

					pDlg->m_ctrObjYSlider.SetRange(int((vPos.y - 5.f)*100.f), int((vPos.y + 5.f)*100.f));
					pDlg->m_ctrObjYSlider.SetRangeMin(int((vPos.y - 5.f)*100.f));
					pDlg->m_ctrObjYSlider.SetRangeMax(int((vPos.y + 5.f)*100.f));
					pDlg->m_ctrObjYSlider.SetPos(int(vPos.y * 100.f));

					pDlg->m_ctrObjZSlider.SetRange(int((vPos.z - 5.f)*100.f), int((vPos.z + 5.f)*100.f));
					pDlg->m_ctrObjZSlider.SetRangeMin(int((vPos.z - 5.f)*100.f));
					pDlg->m_ctrObjZSlider.SetRangeMax(int((vPos.z + 5.f)*100.f));
					pDlg->m_ctrObjZSlider.SetPos(int(vPos.z * 100.f));

					((CTrap*)(*iter))->CheckTrue();
				}

			}
		}
	}

	//네비탭 마우스 클릭카운트 전달..
	CNaviTab* pDlg2 = ((CMainFrame*)AfxGetMainWnd())->m_pButtonMenu->m_pNaviTab;

	pDlg2->m_iPointCount = CScene_MapTool::m_iPointCount;
	pDlg2->SetDlgItemInt(IDC_COUNT_EDIT, pDlg2->m_iPointCount);


}

void CMapToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.


	CView::OnLButtonDown(nFlags, point);
}
