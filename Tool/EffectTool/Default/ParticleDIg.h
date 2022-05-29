#pragma once

#include "afxwin.h"
#include "afxcmn.h"
#include "Export_Engine.h"
// CParticleDIg 대화 상자입니다.

class CEffectToolView;

class CParticleDIg : public CDialog
{
	DECLARE_DYNAMIC(CParticleDIg)

public:
	CParticleDIg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CParticleDIg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PARTICLEDIG };

private:
	CEffectToolView*			m_pView;



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL				OnInitDialog();
	void						InitParticleDlg(void);
	CComboBox m_TextureComboBox;
	afx_msg void OnCbnSelchangeTexturecombo();
	afx_msg void OnBnClickedTexturebtn();
	afx_msg void OnBnClickedChangingFade();

public:
	int								m_iDrawID;
	int								m_iParticleCount;
	D3DXVECTOR3						m_vCenterPos;
	D3DXVECTOR3						m_vSize;



public:
	void							InitParticleTexture(void);
	void							InitSettingNumber(void);

	wstring ConvertRelativePath(wstring	strFullPath);

	afx_msg void OnEnChangeParticlecount();
	afx_msg void OnEnChangeCenterx();
	afx_msg void OnEnChangeCentery();
	afx_msg void OnEnChangeEdit5();
	afx_msg void OnEnChangeParticlesizex();
	afx_msg void OnEnChangeParticlesizey();
	afx_msg void OnEnChangeParticlesizez();
	afx_msg void OnEnChangeParticleR();
	afx_msg void OnEnChangeParticleG();
	afx_msg void OnEnChangeParticleB();
	afx_msg void OnEnChangeParticleA();
	afx_msg void OnEnChangeWindx();
	afx_msg void OnEnChangeWindy();
	afx_msg void OnEnChangeWindz();
	afx_msg void OnEnChangeWindpower();
	afx_msg void OnEnChangeParticlespeed();
	CButton m_bButton;
	afx_msg void OnBnClickedCheck1();
	int m_ParticleMove;
	afx_msg void SetParticleMovement(UINT value);
	afx_msg void OnEnChangeArrangex();
	afx_msg void OnEnChangeArrangey();
	afx_msg void OnEnChangeArrangez();
	afx_msg void OnEnChangeArrangefire();
	afx_msg void OnEnChangeParticlefheight();
	int m_iParticlePathNumber;
	afx_msg void SetPathNumber(UINT value);
	afx_msg void OnEnChangeRadiusinterval();
	afx_msg void OnEnChangeAngleinterval();
	afx_msg void OnEnChangeHeightinterval();
	afx_msg void OnBnClickedParticlepathbtn();
	afx_msg void OnBnClickedParticlesavebtn();
	afx_msg void OnBnClickedLoadparticlebtn2();
};
