#pragma once

#include "afxwin.h"
#include "afxcmn.h"
#include "Export_Engine.h"

// CEffectSprite ��ȭ �����Դϴ�.

class CEffectToolView;

class CEffectSprite : public CDialog
{
	DECLARE_DYNAMIC(CEffectSprite)

public:
	CEffectSprite(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CEffectSprite();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_EFFECTSPRITE };

private:
	CEffectToolView*			m_pView;

	int							m_iDrawID;


public:
	virtual						BOOL OnInitDialog();
	void						InitSpriteDIg(void);
	void						InitSpriteTexture(void);

	void						InitSettingNumber(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	CComboBox m_TextureComboBox;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedSpritebillboard();
	CButton m_bButton;
	afx_msg void OnEnChangeSpritecount();
	afx_msg void OnEnChangeSpritespeed();
	afx_msg void OnEnChangeMaxframe();
	afx_msg void OnEnChangeRowframe();
	afx_msg void OnEnChangeColumnframe();
	afx_msg void OnEnChangeFramecenterx();
	afx_msg void OnEnChangeFramecentery();
	afx_msg void OnEnChangeFramecenterz();
	afx_msg void OnEnChangeSpritesizex();
	afx_msg void OnEnChangeSpritesizey();
	afx_msg void OnEnChangeSpritesizez();
	afx_msg void OnEnChangeSpritecolorr();
	afx_msg void OnEnChangeSpritecolorg();
	afx_msg void OnEnChangeSpritecolorb();
	afx_msg void OnEnChangeSpritecolora();
	afx_msg void OnBnClickedSpritepath();
	wstring ConvertRelativePath(wstring	strFullPath);
	afx_msg void OnBnClickedSpritesave();
	afx_msg void OnBnClickedSpriteload();
	CButton m_bLoop;
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedButton1();
	int m_iPathNumber;
	afx_msg void SetSpritePathNumber(UINT value);
};
