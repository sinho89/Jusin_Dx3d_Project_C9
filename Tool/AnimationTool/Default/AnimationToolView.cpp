
// AnimationToolView.cpp : CAnimationToolView Ŭ������ ����
//

#include "stdafx.h"
#include "AnimationTool.h"

#include "AnimationToolDoc.h"
#include "AnimationToolView.h"

#include "MainFrm.h"
#include "AnimationApp.h"
#include "Export_Engine.h"

#include "MyForm.h"
#include "DynamicObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAnimationToolView

IMPLEMENT_DYNCREATE(CAnimationToolView, CView)

BEGIN_MESSAGE_MAP(CAnimationToolView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CAnimationToolView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CAnimationToolView ����/�Ҹ�

CAnimationToolView::CAnimationToolView()
: m_pGraphicDev(NULL)
, m_pMainApp(NULL)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CAnimationToolView::~CAnimationToolView()
{
	Engine::Safe_Release(m_pGraphicDev);

	_ulong dwRefCnt = 0;
	dwRefCnt = Engine::Safe_Release(m_pMainApp);

}

BOOL CAnimationToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.


	g_hWnd = m_hWnd;
	g_hInst = AfxGetInstanceHandle();

	m_pMainApp = CAnimationApp::Create();
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

// CAnimationToolView �׸���

void CAnimationToolView::OnDraw(CDC* /*pDC*/)
{
	CAnimationToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
	
	Engine::SetUp_TimeDelta(L"Timer_Immediate");

	_float	fTimeDelta_Immediate = Engine::Get_TimeDelta(L"Timer_Immediate");

	// 1�ʿ� ����� Ʈ�縦 ����, �������� �޽�����
	if(Engine::isPermit_Call(L"Frame_60", fTimeDelta_Immediate))
	{

		Engine::SetUp_TimeDelta(L"Timer_FPS:60");

		_float	fTimeDelta_60 = Engine::Get_TimeDelta(L"Timer_FPS:60");				

		m_pMainApp->Update_AnimationApp(fTimeDelta_60);
		m_pMainApp->Render_AnimationApp();


		Engine::CGameObject* pGameObject = NULL;

		pGameObject = Engine::FindWithTag(L"DynamicObject");

		if(pGameObject != NULL)
		{
			if(((CDynamicObject*)pGameObject)->Get_Playing())
			{
				(((CMyForm*)(((CMainFrame*)AfxGetMainWnd())->m_pMyForm))->m_AniTool).Set_AnimationEdit();
			}

			Engine::Safe_Release(pGameObject);
		}

		m_pGraphicDev->EndScene();
		m_pGraphicDev->Present(NULL, NULL, GetSafeHwnd(), NULL);
	}
}


// CAnimationToolView �μ�


void CAnimationToolView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CAnimationToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CAnimationToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CAnimationToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}

void CAnimationToolView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CAnimationToolView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CAnimationToolView ����

#ifdef _DEBUG
void CAnimationToolView::AssertValid() const
{
	CView::AssertValid();
}

void CAnimationToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAnimationToolDoc* CAnimationToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAnimationToolDoc)));
	return (CAnimationToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CAnimationToolView �޽��� ó����

void CAnimationToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	CMainFrame*	pFrame = (CMainFrame*)AfxGetMainWnd();

	RECT	rcWindow;
	pFrame->GetWindowRect(&rcWindow);

	SetRect(&rcWindow, 0, 0, rcWindow.right - rcWindow.left,
		rcWindow.bottom - rcWindow.top);

	RECT	rcMainView;
	GetClientRect(&rcMainView);

	float fRowFrm = float(rcWindow.right - rcMainView.right);
	float fColFrm = float(rcWindow.bottom - rcMainView.bottom);

	pFrame->SetWindowPos(NULL, 0, 0, int(WINSIZEX + fRowFrm), int(WINSIZEY + fColFrm), SWP_NOZORDER);

}

void CAnimationToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	POINT ptMouse;
	ptMouse.x = point.x;
	ptMouse.y = point.y;

	if(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_eToolMode == CMyForm::ANI_TOOL)
	{
		(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_AniTool).MouseClick(ptMouse);
	}

	//Effect��
	if(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_eToolMode == CMyForm::EFFECT_TOOL)
	{
		(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool).MouseClick(ptMouse);
	}

	CView::OnLButtonDown(nFlags, point);
}
