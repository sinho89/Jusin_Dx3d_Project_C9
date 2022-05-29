#pragma once


// CNaviTab 대화 상자입니다.

class CNaviTab : public CDialog
{
	DECLARE_DYNAMIC(CNaviTab)

public:
	CNaviTab(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CNaviTab();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedModifyRadio();
	afx_msg void OnBnClickedAddcellRadio();
	afx_msg void OnBnClickedCheckcellRadio();
	afx_msg void OnBnClickedNormalRadio();
	afx_msg void OnBnClickedFallRadio();
	Engine::CGameObject*		m_pMap;
	int							m_iPointCount;
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedRemoveBtn();
	afx_msg void OnBnClickedDeleteallBtn();
	afx_msg void OnBnClickedDeleteBtn();
	afx_msg void OnBnClickedNaviSaveBtn();
	afx_msg void OnBnClickedNaviLoadBtn();
	afx_msg void OnBnClickedDebugCheck();
	BOOL m_ctlDebugCheck;
	afx_msg void OnBnClickedLinkCellBtn();
};
