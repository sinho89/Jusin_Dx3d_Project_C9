
// AnimationToolView.h : CAnimationToolView Ŭ������ �������̽�
//


#pragma once

#include "Defines.h"

class CAnimationApp;
class CAnimationToolDoc;

class CAnimationToolView : public CView
{
protected: // serialization������ ��������ϴ�.
	CAnimationToolView();
	DECLARE_DYNCREATE(CAnimationToolView)

// Ư���Դϴ�.
public:
	CAnimationToolDoc* GetDocument() const;

// �۾��Դϴ�.
public:
	CAnimationApp* m_pMainApp;
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
	virtual ~CAnimationToolView();
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
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // AnimationToolView.cpp�� ����� ����
inline CAnimationToolDoc* CAnimationToolView::GetDocument() const
   { return reinterpret_cast<CAnimationToolDoc*>(m_pDocument); }
#endif

