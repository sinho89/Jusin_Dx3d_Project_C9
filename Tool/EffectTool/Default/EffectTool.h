
// EffectTool.h : EffectTool ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CEffectToolApp:
// �� Ŭ������ ������ ���ؼ��� EffectTool.cpp�� �����Ͻʽÿ�.
//

class CEffectToolApp : public CWinAppEx
{
public:
	CEffectToolApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();

	virtual BOOL OnIdle(LONG lCount);

	DECLARE_MESSAGE_MAP()
};

extern CEffectToolApp theApp;
