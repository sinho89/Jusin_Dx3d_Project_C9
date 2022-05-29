// MyForm.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AnimationTool.h"
#include "EffectTool.h"
#include "MyForm.h"


// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(CMyForm::IDD)
{

}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_TabCtrl);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMyForm::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CMyForm �����Դϴ�.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm �޽��� ó�����Դϴ�.

void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_TabCtrl.DeleteAllItems();
	m_TabCtrl.InsertItem(0, L"Animation");
	m_TabCtrl.InsertItem(1, L"Collider");
	m_TabCtrl.InsertItem(2, L"Effect");

	RECT	rcToolWindow;
	m_TabCtrl.GetClientRect(&rcToolWindow);
	m_AniTool.Create(IDD_ANITOOL);
	m_AniTool.SetWindowPos(NULL, 5, 75, rcToolWindow.right - 5,
		rcToolWindow.bottom - 29, SWP_SHOWWINDOW);

	m_TabCtrl.GetClientRect(&rcToolWindow);
	m_ColTool.Create(IDD_COLTOOL);
	m_ColTool.SetWindowPos(NULL, 5, 75, rcToolWindow.right - 5,
		rcToolWindow.bottom - 29, SWP_SHOWWINDOW);


	m_TabCtrl.GetClientRect(&rcToolWindow);
	m_EffectTool.Create(IDD_EFFECTTOOL);
	m_EffectTool.SetWindowPos(NULL, 5, 75, rcToolWindow.right - 5,
				rcToolWindow.bottom - 29, SWP_SHOWWINDOW);


	m_AniTool.ShowWindow(SW_SHOW);
	m_ColTool.ShowWindow(SW_HIDE);
	m_EffectTool.ShowWindow(SW_HIDE);

	m_eToolMode = ANI_TOOL;


	//===�ִϸ��̼� ���� ����Ʈ�� ����======//
	m_AniTool.m_pEffectTool = &m_EffectTool;

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}

void CMyForm::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int iIndex = m_TabCtrl.GetCurSel();

	switch(iIndex)
	{
	case 0:
		m_AniTool.ShowWindow(SW_SHOW);
		m_ColTool.ShowWindow(SW_HIDE);
		m_EffectTool.ShowWindow(SW_HIDE);

		m_eToolMode = ANI_TOOL;
		break;

	case 1:
		m_AniTool.ShowWindow(SW_HIDE);
		m_ColTool.ShowWindow(SW_SHOW);
		m_EffectTool.ShowWindow(SW_HIDE);

		m_eToolMode = COL_TOOL;
		break;

	case 2:
		m_AniTool.ShowWindow(SW_HIDE);
		m_ColTool.ShowWindow(SW_HIDE);
		m_EffectTool.ShowWindow(SW_SHOW);

		m_eToolMode = EFFECT_TOOL;
		break;
	}
}
