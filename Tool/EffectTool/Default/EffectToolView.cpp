
// EffectToolView.cpp : CEffectToolView Ŭ������ ����
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
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CEffectToolView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CEffectToolView ����/�Ҹ�

CEffectToolView::CEffectToolView()
{
	m_pToolApp = NULL;
	m_pGraphicDev = NULL;
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CEffectToolView::~CEffectToolView()
{
	Engine::Safe_Release(m_pGraphicDev);

	_ulong dwRefCnt = 0;
	dwRefCnt = Engine::Safe_Release(m_pToolApp);

}

BOOL CEffectToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.


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

// CEffectToolView �׸���

void CEffectToolView::OnDraw(CDC* /*pDC*/)
{
	CEffectToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	Engine::SetUp_TimeDelta(L"Timer_Immediate");

	_float	fTimeDelta_Immediate = Engine::Get_TimeDelta(L"Timer_Immediate");

	// 1�ʿ� ����� Ʈ�縦 ����, �������� �޽�����
	if(Engine::isPermit_Call(L"Frame_60", fTimeDelta_Immediate))
	{
		Engine::SetUp_TimeDelta(L"Timer_FPS:60");

		_float	fTimeDelta_60 = Engine::Get_TimeDelta(L"Timer_FPS:60");

		m_pToolApp->Update_ToolApp(fTimeDelta_60);
		m_pToolApp->Render_ToolApp();

		m_pGraphicDev->EndScene();
		m_pGraphicDev->Present(NULL, NULL, GetSafeHwnd(), NULL);
	}


	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CEffectToolView �μ�


void CEffectToolView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CEffectToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CEffectToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CEffectToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
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


// CEffectToolView ����

#ifdef _DEBUG
void CEffectToolView::AssertValid() const
{
	CView::AssertValid();
}

void CEffectToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEffectToolDoc* CEffectToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEffectToolDoc)));
	return (CEffectToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CEffectToolView �޽��� ó����

void CEffectToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();


	CMainFrame*	pFrame = (CMainFrame*)AfxGetMainWnd();

	RECT	rcWindow;
	pFrame->GetWindowRect(&rcWindow);	// ��ü �������� ����� ������ �Լ�

	SetRect(&rcWindow, 0, 0, 
		rcWindow.right - rcWindow.left,
		rcWindow.bottom - rcWindow.top);

	RECT	rcMainView;

	// �� â�� ũ�⸦ ������ �Լ�
	GetClientRect(&rcMainView);

	float fRowFrm = float(rcWindow.right - rcMainView.right);// ���� �������� ����
	float fColFrm = float(rcWindow.bottom - rcMainView.bottom);// ���� �������� ����

	pFrame->SetWindowPos(NULL,0,0,
		int(WINSIZEX + fRowFrm), 
		int(WINSIZEY + fColFrm), 
		SWP_NOZORDER);

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}
