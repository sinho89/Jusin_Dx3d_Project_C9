#pragma once


// CBatch ��ȭ �����Դϴ�.

class CBatch : public CDialog
{
	DECLARE_DYNAMIC(CBatch)

public:
	CBatch(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CBatch();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_BATCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
