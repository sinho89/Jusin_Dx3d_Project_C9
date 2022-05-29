// MyForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EffectTool.h"
#include "MyForm.h"

// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(CMyForm::IDD)
{
	m_iCurrentViewDlg = 0;
}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABSHEET, m_TapControl);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABSHEET, &CMyForm::OnTcnSelchangeTabsheet)
END_MESSAGE_MAP()


// CMyForm 진단입니다.

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


void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_TapControl.DeleteAllItems();


	m_TapControl.InsertItem(0, L"Particle");
	m_TapControl.InsertItem(1, L"Sprite");
	m_TapControl.InsertItem(2, L"Normal2");
	m_TapControl.InsertItem(3, L"Normal3");


	RECT	rcToolWindow;
	m_TapControl.GetClientRect(&rcToolWindow);
	m_pParticleDIg.Create(IDD_PARTICLEDIG);
	m_pParticleDIg.SetWindowPos(NULL, 15, 86, rcToolWindow.right - 5,
								rcToolWindow.bottom - 29, SWP_SHOWWINDOW);


	m_pSpriteDIg.Create(IDD_EFFECTSPRITE);
	m_pSpriteDIg.SetWindowPos(NULL, 15, 86, rcToolWindow.right - 5,
								rcToolWindow.bottom - 29, SWP_SHOWWINDOW);
	


	m_pParticleDIg.ShowWindow(SW_SHOW);
	m_pSpriteDIg.ShowWindow(SW_HIDE);
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}



// CMyForm 메시지 처리기입니다.

void CMyForm::OnTcnSelchangeTabsheet(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int iIndex = m_TapControl.GetCurSel();

	m_iCurrentViewDlg = iIndex;

	switch(m_iCurrentViewDlg)
	{
	case 0:
		m_pParticleDIg.ShowWindow(SW_SHOW);
		m_pSpriteDIg.ShowWindow(SW_HIDE);
		break;

	case 1:
		m_pParticleDIg.ShowWindow(SW_HIDE);
		m_pSpriteDIg.ShowWindow(SW_SHOW);
		break;
	
	case 2:
		m_pParticleDIg.ShowWindow(SW_HIDE);
		m_pSpriteDIg.ShowWindow(SW_HIDE);
		break;

	case 3:
		m_pParticleDIg.ShowWindow(SW_HIDE);
		m_pSpriteDIg.ShowWindow(SW_HIDE);
		break;
	}

}
