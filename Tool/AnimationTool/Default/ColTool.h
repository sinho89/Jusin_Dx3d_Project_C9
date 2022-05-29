#pragma once

#include "Engine_Defines.h"

// CColTool ��ȭ �����Դϴ�.

class CColTool : public CDialog
{
	DECLARE_DYNAMIC(CColTool)
public:
	Engine::COL_TYPE m_eColType;

public:
	CColTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CColTool();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_COLTOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
