#pragma once

#include "Engine_Defines.h"

// CColTool 대화 상자입니다.

class CColTool : public CDialog
{
	DECLARE_DYNAMIC(CColTool)
public:
	Engine::COL_TYPE m_eColType;

public:
	CColTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CColTool();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_COLTOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedColcreatebtn();
	afx_msg void OnBnClickedColdelbtn();
	afx_msg void OnBnClickedColadjustbtn();
	int m_iColTypeRadio;
	afx_msg void OnBnClickedColsavebtn();
	afx_msg void OnBnClickedColloadbtn();
	afx_msg void SetColliderTypeRadioStatus(UINT value);
	virtual BOOL OnInitDialog();
};
