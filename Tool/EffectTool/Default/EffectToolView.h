
// EffectToolView.h : CEffectToolView Ŭ������ �������̽�
//


#pragma once

class CToolApp;
class CEffectToolDoc;

#include "Defines.h"

class CEffectToolView : public CView
{
protected: // serialization������ ��������ϴ�.
	CEffectToolView();
	DECLARE_DYNCREATE(CEffectToolView)

// Ư���Դϴ�.
public:
	CEffectToolDoc* GetDocument() const;

// �۾��Դϴ�.
public:
	CToolApp*		m_pToolApp;
	LPDIRECT3DDEVICE9 m_pGraphicDev;


// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CEffectToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // EffectToolView.cpp�� ����� ����
inline CEffectToolDoc* CEffectToolView::GetDocument() const
   { return reinterpret_cast<CEffectToolDoc*>(m_pDocument); }
#endif

