
// AnimationToolView.h : CAnimationToolView 클래스의 인터페이스
//


#pragma once

#include "Defines.h"

class CAnimationApp;
class CAnimationToolDoc;

class CAnimationToolView : public CView
{
protected: // serialization에서만 만들어집니다.
	CAnimationToolView();
	DECLARE_DYNCREATE(CAnimationToolView)

// 특성입니다.
public:
	CAnimationToolDoc* GetDocument() const;

// 작업입니다.
public:
	CAnimationApp* m_pMainApp;
	LPDIRECT3DDEVICE9 m_pGraphicDev;

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CAnimationToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // AnimationToolView.cpp의 디버그 버전
inline CAnimationToolDoc* CAnimationToolView::GetDocument() const
   { return reinterpret_cast<CAnimationToolDoc*>(m_pDocument); }
#endif

