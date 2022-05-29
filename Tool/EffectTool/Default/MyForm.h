#pragma once

#include "afxcmn.h"
#include "ParticleDIg.h"
#include "SpriteDIg.h" 


// CMyForm 폼 뷰입니다.

class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

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
	CParticleDIg		m_pParticleDIg;
	CEffectSprite		m_pSpriteDIg;
	CTabCtrl			m_TapControl;
	int					m_iCurrentViewDlg;

	afx_msg void OnTcnSelchangeTabsheet(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnInitialUpdate();
};


