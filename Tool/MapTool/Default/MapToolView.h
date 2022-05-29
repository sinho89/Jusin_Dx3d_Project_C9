
// MapToolView.h : CMapToolView Ŭ������ �������̽�
//


#pragma once

#include "Defines.h"

namespace Engine
{
	class CGameObject;
}

class CMainApp;
class CMapToolDoc;

class CMapToolView : public CView
{
protected: // serialization������ ��������ϴ�.
	CMapToolView();
	DECLARE_DYNCREATE(CMapToolView)

// Ư���Դϴ�.
public:
	CMapToolDoc* GetDocument() const;

// �۾��Դϴ�.
public:
	CMainApp*		  m_pMainApp;
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
	virtual ~CMapToolView();
//#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
//#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

public:
	virtual void OnInitialUpdate();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // MapToolView.cpp�� ����� ����
inline CMapToolDoc* CMapToolView::GetDocument() const
   { return reinterpret_cast<CMapToolDoc*>(m_pDocument); }
#endif
