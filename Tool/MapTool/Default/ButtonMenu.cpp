// ButtonMenu.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MapTool.h"
#include "ButtonMenu.h"
#include "Map_Ice.h"
#include "Map_Store.h"


// CButtonMenu

IMPLEMENT_DYNCREATE(CButtonMenu, CFormView)

CButtonMenu::CButtonMenu()
	: CFormView(CButtonMenu::IDD)
{
	m_pMapToolTab = NULL;
	m_pNaviTab = NULL;
	m_pMap = NULL;

}

CButtonMenu::~CButtonMenu()
{
	Safe_Delete(m_pMapToolTab);
	Safe_Delete(m_pNaviTab);
}

void CButtonMenu::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_ctrlTab);
}

BEGIN_MESSAGE_MAP(CButtonMenu, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CButtonMenu::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CButtonMenu �����Դϴ�.

#ifdef _DEBUG
void CButtonMenu::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CButtonMenu::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CButtonMenu �޽��� ó�����Դϴ�.

void CButtonMenu::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_ctrlTab.DeleteAllItems();
	m_ctrlTab.InsertItem(0, L"MapTool Tab");
	m_ctrlTab.InsertItem(1, L"Navi Tab");

	CRect rect;

	m_pMapToolTab = new CMapToolTab;
	m_pMapToolTab->Create(IDD_DIALOG1, &m_ctrlTab);
	m_pMapToolTab->GetWindowRect(&rect);
	m_pMapToolTab->MoveWindow(5, 25, rect.Width(), rect.Height());
	m_pMapToolTab->ShowWindow(SW_SHOW);

	m_pNaviTab = new CNaviTab;
	m_pNaviTab->Create(IDD_DIALOG2, &m_ctrlTab);
	m_pNaviTab->GetWindowRect(&rect);
	m_pNaviTab->MoveWindow(5, 25, rect.Width(), rect.Height());
	m_pNaviTab->ShowWindow(SW_HIDE);

	UpdateData(FALSE);
	

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}

void CButtonMenu::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	int select = m_ctrlTab.GetCurSel();

	switch(select)
	{
	case 0:
		m_pMapToolTab->ShowWindow(SW_SHOW);
		m_pNaviTab->ShowWindow(SW_HIDE);
		if(NULL != Engine::FindList(L"Map_Ice"))
		{
			list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Map_Ice")->begin();
			((CMap_Ice*)(*iter))->m_eNaviMode = CMap_Ice::MODE_NONE;
		}

		if(NULL != Engine::FindList(L"Map_Store"))
		{
			list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Map_Store")->begin();
			((CMap_Store*)(*iter))->m_eNaviMode = CMap_Store::MODE_NONE;
		}


		break;
	case 1:
		m_pMapToolTab->ShowWindow(SW_HIDE);
		m_pNaviTab->ShowWindow(SW_SHOW);
		break;
	}

	*pResult = 0;
}
