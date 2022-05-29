#pragma once

#include "afxcmn.h"
#include "ParticleDIg.h"
#include "SpriteDIg.h" 


// CMyForm �� ���Դϴ�.

class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

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
	CParticleDIg		m_pParticleDIg;
	CEffectSprite		m_pSpriteDIg;
	CTabCtrl			m_TapControl;
	int					m_iCurrentViewDlg;

	afx_msg void OnTcnSelchangeTabsheet(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnInitialUpdate();
};


