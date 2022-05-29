#pragma once
#include "afxcmn.h"

#include "MapToolTab.h"
#include "NaviTab.h"


// CButtonMenu �� ���Դϴ�.

class CButtonMenu : public CFormView
{
	DECLARE_DYNCREATE(CButtonMenu)

protected:
	CButtonMenu();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CButtonMenu();

public:
	enum { IDD = IDD_BUTTONMENU };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_ctrlTab;

	CMapToolTab*		m_pMapToolTab;
	CNaviTab*			m_pNaviTab;
	Engine::CGameObject*	m_pMap;

	virtual void OnInitialUpdate();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};


