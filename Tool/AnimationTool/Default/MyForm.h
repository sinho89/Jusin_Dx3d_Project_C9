#pragma once
#include "afxcmn.h"


#include "AniTool.h"
#include "ColTool.h"
#include "EffectTool.h"


// CMyForm �� ���Դϴ�.

class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)
public:
	enum TOOLMODE { ANI_TOOL, COL_TOOL, EFFECT_TOOL };
public:
	CAniTool	m_AniTool;
	CColTool	m_ColTool;
	CEffectTool m_EffectTool;

	TOOLMODE	m_eToolMode;
protected:
	CMyForm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CMyForm();

public:
	enum { IDD = IDD_MYFORM };
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
	CTabCtrl m_TabCtrl;
	virtual void OnInitialUpdate();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};


