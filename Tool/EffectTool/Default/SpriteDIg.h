#pragma once

#include "afxwin.h"
#include "afxcmn.h"
#include "Export_Engine.h"

// CEffectSprite 대화 상자입니다.

class CEffectToolView;

class CEffectSprite : public CDialog
{
	DECLARE_DYNAMIC(CEffectSprite)

public:
	CEffectSprite(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEffectSprite();

// 대화 상자 데이터입니다.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
