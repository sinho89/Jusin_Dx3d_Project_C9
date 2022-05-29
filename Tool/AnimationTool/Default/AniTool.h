#pragma once
#include "afxwin.h"

#include "Engine_Defines.h"
#include "Defines.h"
#include "afxcmn.h"
#include "EffectTool.h"

// CAniTool 대화 상자입니다.

class CAniTool : public CDialog
{
	DECLARE_DYNAMIC(CAniTool)
public:
	enum DYNAMICMESHTYPE { MESH_PLAYER, MESH_MONSTER, MESH_END };
public:
	DYNAMICMESHTYPE m_eDynamicMeshType;
	Engine::MESHTYPE m_eMeshType;
public:
	map<wstring, WEAPONINFO*> m_mapWeapon;								//스태틱 메쉬 정보
	typedef map<wstring, WEAPONINFO*> MAPWEAPONINFO;

	map<_uint, Engine::MOTIONINFO*> m_mapMotionInfo;
	typedef map<_uint, Engine::MOTIONINFO*> MAPMOTIONINFO;						//기본 정보

	map<_uint, list<Engine::ANIMATORINFO*>>* m_pmapAnimatorInfo;					//다음 넘어갈 동작
	typedef map<_uint, list<Engine::ANIMATORINFO*>> MAPANIMATORINFO;
	typedef list<Engine::ANIMATORINFO*>	ANIMATORLIST;

	map<_uint, list<Engine::MOTIONEVENTINFO*>> m_mapMotionEventInfo;			//이벤트
	typedef map<_uint, list<Engine::MOTIONEVENTINFO*>> MAPMOTIONEVENTINFO;
	typedef list<Engine::MOTIONEVENTINFO*> MOTIONEVENTLIST;

	map<_uint, list<_bool*>> m_mapUse;
	typedef map<_uint, list<_bool*>> MAPUSE;
	typedef list<_bool*> LISTUSE;

	vector<const char*> m_vecBone;
	typedef vector<const char*> VECBONE;
	//========Effect_Tool용 변수====================//
public:
	CEffectTool*		m_pEffectTool;
	//==============================================//
public:
	_uint m_iAniCount;
	_uint m_iNextMotionContainerSize;
	_bool m_bIsChange;

public:
	void BaseAniSetting(void);
	void BaseStaticSetting(void);
	void Release_Dynamic(void);
	void Release_Static(void);
	void SetAnimationData(const _int& iIndex);
	void Set_AnimationEdit(void);
	void MouseClick(POINT ptMouse);
public:
	CAniTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAniTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANITOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_XFileListBox;
	CComboBox m_ChoiceComboBox;
	int m_iMeshType;
	int m_iDynamicMeshType;
	virtual BOOL OnInitDialog();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedXfiledelbtn();
	afx_msg void OnBnClickedMeshcreatebtn();
	afx_msg void SetMeshTypeRadioStatus(UINT value);
	afx_msg void SetDynamicMeshTypeRadioStatus(UINT value);
	afx_msg void OnBnClickedMeshdelbtn();
	CListBox m_AniListBox;
	CSliderCtrl m_TimeSlider;
	CButton m_LoopCheck;
	CButton m_EventCheck;
	CComboBox m_EventComboBox;
	CComboBox m_StateComboBox;
	CComboBox m_NextMotionComboBox;
	CListBox m_NextMotionList;
	CListBox m_BoneList;
	afx_msg void OnLbnSelchangeAnilistbox();
	afx_msg void OnBnClickedPlaybtn();
	afx_msg void OnBnClickedStopbtn();
	afx_msg void OnBnClickedStarttimesetbtn();
	afx_msg void OnBnClickedEndtimesetbtn();
	afx_msg void OnBnClickedMotionsetbtn();
	afx_msg void OnCbnSelchangeEventcombobox();
	afx_msg void OnBnClickedEventsetbtn();
	afx_msg void OnCbnSelchangeStatecombobox();
	afx_msg void OnBnClickedAddmotionbtn();
	afx_msg void OnLbnSelchangeNextmotionlist();
	afx_msg void OnBnClickedNextmotiondelbtn();
	afx_msg void OnBnClickedNextmotionset();
	afx_msg void OnBnClickedAnisavebtn();
	afx_msg void OnBnClickedAniloadbtn();
	afx_msg void OnLbnSelchangeBonelist();
	afx_msg void OnBnClickedStaticmeshinfosetbtn();
	afx_msg void OnBnClickedSavestaticbtn();
	afx_msg void OnBnClickedLoadstaticbtn();
	CButton m_AnimationCheck;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedCanceltimebtn();
	afx_msg void OnBnClickedAnimationusesetbtn();
};
