#pragma once
#include "afxcmn.h"


// CMapToolTab ��ȭ �����Դϴ�.

class CMapToolTab : public CDialog
{
	DECLARE_DYNAMIC(CMapToolTab)

public:
	CMapToolTab(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMapToolTab();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
