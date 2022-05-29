#pragma once
#include "afxwin.h"
#include "Defines.h"
#include "Export_Engine.h"
#include "afxcmn.h"

// EffectTool 대화 상자입니다.

class CEffectTool : public CDialog
{
	DECLARE_DYNAMIC(CEffectTool)

public:
	vector<const char*>*			m_pvecBone;
	_int							m_iDrawID;

	CString							EffectName;
	_bool							m_bRenderCheck;


public:
	void					InitSetting(void);
	void					InitEffectStruct(void);


public:
	CEffectTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEffectTool();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_EFFECTTOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListBox m_BoneList;
	afx_msg void OnLbnSelchangeEffectbonelist();
	afx_msg void OnBnClickedEffectcreatebtn();
	afx_msg void OnBnClickedEffectsetbtn();

public:
	void	MouseClick(POINT ptMouse);
	afx_msg void OnBnClickedDeletebtn();
	CListBox m_EffectList;
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedRectrender();
	CButton m_bBillBoardBtn;
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedAnimationstart();
};
