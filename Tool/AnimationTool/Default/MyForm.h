#pragma once
#include "afxcmn.h"


#include "AniTool.h"
#include "ColTool.h"
#include "EffectTool.h"


// CMyForm 폼 뷰입니다.

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
	CMyForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_TabCtrl;
	virtual void OnInitialUpdate();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};


