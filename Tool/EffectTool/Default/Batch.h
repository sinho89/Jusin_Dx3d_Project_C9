#pragma once


// CBatch 대화 상자입니다.

class CBatch : public CDialog
{
	DECLARE_DYNAMIC(CBatch)

public:
	CBatch(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CBatch();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_BATCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
