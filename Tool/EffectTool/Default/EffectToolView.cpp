
// EffectToolView.cpp : CEffectToolView 클래스의 구현
//

#include "stdafx.h"
#include "EffectTool.h"
#include "EffectToolDoc.h"
#include "EffectToolView.h"

#include "MainFrm.h"
#include "Export_System.h"
#include "ToolApp.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEffectToolView

IMPLEMENT_DYNCREATE(CEffectToolView, CView)

BEGIN_MESSAGE_MAP(CEffectToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CEffectToolView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CEffectToolView 생성/소멸

CEffectToolView::CEffectToolView()
{
	m_pToolApp = NULL;
	m_pGraphicDev = NULL;
	// TODO: 여기에 생성 코드를 추가합니다.

}

CEffectToolView::~CEffectToolView()
{
	Engine::Safe_Release(m_pGraphicDev);

	_ulong dwRefCnt = 0;
	dwRefCnt = Engine::Safe_Release(m_pToolApp);

}

BOOL CEffectToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.


	g_hWnd = m_hWnd;
	g_hInst = AfxGetInstanceHandle();

	m_pToolApp = CToolApp::Create();

	if(m_pToolApp  == NULL)
		return FALSE;

	if(FAILED(m_pToolApp->Get_GraphicDev(&m_pGraphicDev)))
		return FALSE;


	if(FAILED(Engine::Ready_Timer(L"Timer_Immediate")))
		return FALSE;

	if(FAILED(Engine::Ready_Timer(L"Timer_FPS:60")))
		return FALSE;

	if(FAILED(Engine::Ready_Frame(L"Frame_60", 60.0f)))
		return FALSE;


	return CView::PreCreateWindow(cs);
}

// CEffectToolView 그리기

void CEffectToolView::OnDraw(CDC* /*pDC*/)
{
	CEffectToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	Engine::SetUp_TimeDelta(L"Timer_Immediate");

	_float	fTimeDelta_Immediate = Engine::Get_TimeDelta(L"Timer_Immediate");

	// 1초에 백번만 트루를 리턴, 나머지는 펄스리턴
	if(Engine::isPermit_Call(L"Frame_60", fTimeDelta_Immediate))
	{
		Engine::SetUp_TimeDelta(L"Timer_FPS:60");

		_float	fTimeDelta_60 = Engine::Get_TimeDelta(L"Timer_FPS:60");

		m_pToolApp->Update_ToolApp(fTimeDelta_60);
		m_pToolApp->Render_ToolApp();

		m_pGraphicDev->EndScene();
		m_pGraphicDev->Present(NULL, NULL, GetSafeHwnd(), NULL);
	}


	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CEffectToolView 인쇄


void CEffectToolView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CEffectToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CEffectToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CEffectToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CEffectToolView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CEffectToolView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CEffectToolView 진단

#ifdef _DEBUG
void CEffectToolView::AssertValid() const
{
	CView::AssertValid();
}

void CEffectToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEffectToolDoc* CEffectToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEffectToolDoc)));
	return (CEffectToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CEffectToolView 메시지 처리기

void CEffectToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();


	CMainFrame*	pFrame = (CMainFrame*)AfxGetMainWnd();

	RECT	rcWindow;
	pFrame->GetWindowRect(&rcWindow);	// 전체 프레임의 사이즈를 얻어오는 함수

	SetRect(&rcWindow, 0, 0, 
		rcWindow.right - rcWindow.left,
		rcWindow.bottom - rcWindow.top);

	RECT	rcMainView;

	// 뷰 창의 크기를 얻어오는 함수
	GetClientRect(&rcMainView);

	float fRowFrm = float(rcWindow.right - rcMainView.right);// 가로 프레임의 길이
	float fColFrm = float(rcWindow.bottom - rcMainView.bottom);// 세로 프레임의 길이

	pFrame->SetWindowPos(NULL,0,0,
		int(WINSIZEX + fRowFrm), 
		int(WINSIZEY + fColFrm), 
		SWP_NOZORDER);

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}
