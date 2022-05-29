#pragma once
#include "afxcmn.h"


// CMapToolTab 대화 상자입니다.

class CMapToolTab : public CDialog
{
	DECLARE_DYNAMIC(CMapToolTab)

public:
	CMapToolTab(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMapToolTab();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedMapIceBtn();
	afx_msg void OnBnClickedMapShopBtn();
	CTreeCtrl m_ctrlTreeObject;
	virtual BOOL OnInitDialog();
	afx_msg void OnTvnSelchangedTreeObject(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCreateBtn();
	afx_msg void OnBnClickedDelBtn();
	CSliderCtrl m_ctrObjXSlider;
	afx_msg void OnNMReleasedcaptureSliderObjX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CSliderCtrl m_ctrObjYSlider;
	CSliderCtrl m_ctrObjZSlider;
	CSliderCtrl m_ctrCamSpeedSlider;
	afx_msg void OnBnClickedButtonEyetomid();
	afx_msg void OnBnClickedDelallBtn();
	afx_msg void OnBnClickedSaveobjBtn();
	afx_msg void OnBnClickedLoadobjBtn();
	afx_msg void OnTvnItemexpandedTreeObject(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedMapFireBtn();
};
