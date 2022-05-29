#ifndef	PLAYER_h__
#define	PLAYER_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CRenderer;
	class CStaticMesh;
	class CDynamicMesh;
	class CResource_Manager;
	class CShader;
	class CNavigationMesh;

	class CAnimator;
	class CCalculator;
	class CRigidbody;
}

class CMouseCol;
class CPickingRect;

class CPlayer
	:public Engine::CGameObject
{
public:
	enum WITCHBLADE { WITCH_SELIDLE, WITCH_LOVE, WITCH_IDLE, WITCH_WALKF, WITCH_WALKB, WITCH_RUNF, WITCH_RUNL,
		WITCH_RUNR, WITCH_RUNF_IN, WITCH_RUNB_IN, WITCH_RUNL_IN, WITCH_RUNR_IN, WITCH_DASHF, WITCH_DASHB, WITCH_DASHL, WITCH_DASHR,
		WITCH_JUMPFA, WITCH_JUMPFB, WITCH_JUMPFC, WITCH_JUMPFD, WITCH_JUMPFSB, WITCH_JUMPFSC, WITCH_JUMPFSD, 
		WITCH_JUMPBA, WITCH_JUMPBB, WITCH_JUMPBC, WITCH_JUMPBD, WITCH_JUMPLA, WITCH_JUMPLB, WITCH_JUMPLC, WITCH_JUMPLD, WITCH_JUMPRA, WITCH_JUMPRB, WITCH_JUMPRC,
		WITCH_JUMPRD, WITCH_INSTALL, WITCH_DISARM, WITCH_ATTACKIDLE, WITCH_ATTACK1, WITCH_ATTACK2, WITCH_ATTACK3, WITCH_ATTACK4, 
		WITCH_ATTACK5, WITCH_ATTACK6, WITCH_ATTACK7, WITCH_ATTACK8, WITCH_ATTACK9, WITCH_ATTACK10, WITCH_JUMPATTACK,
		WITCH_JUMPATTACKL, WITCH_JUMPATTACKR, WITCH_GROUNDKICK, WITCH_SKILLRISINGF, WITCH_SKILLEVASIONR, WITCH_SKILLEVASIONL,
		WITCH_SKILLJUMPKICK, WITCH_SKILLJUMPKICK2, WITCH_SKILLCATCH, WITCH_SKILLCAPTURE, WITCH_SKILLROTATIONKICK, WITCH_SKILLKICKCHOP,
		WITCH_SKILL2ANTHREAT, WITCH_SKILLBASH, WITCH_SKILLBLADERUNNER, WITCH_SKILLCOMBOATTACK, WITCH_SKILLCOMBOBLEEDING, WITCH_SKILLBUFF,
		WITCH_SKILLCOUNTER, WITCH_SKILLDARKGRAVE, WITCH_SKILLDESH, WITCH_SKILLEVASIONL_2, WITCH_SKILLEVASIONR_2, WITCH_SKILLFLING,
		WITCH_SKILLGROUNDKICKSWO, WITCH_SKILLRABBIT, WITCH_SKILLRCATCH, WITCH_SKILLMOVEBSPINNINGHEART, WITCH_SKILLMOVELSPINNINGHEART,
		WITCH_SKILLMOVERSPINNINGHEART, WITCH_SKILLSPINNINGHEARTA, WITCH_SKILLSPINNINGHEARTB, WITCH_SKILLSPININGHEARTC,
		WITCH_SKILLMAGICSHIELD, WITCH_SKILLMAGICSHIELDLOOP, WITCH_STUN, WITCH_DAMAGEF, WITCH_DAMAGEB, 
		WITCH_DAMAGEL, WITCH_DAMAGER, WITCH_DAMAGESPINA, WITCH_DAMAGESPINB, WITCH_DHFA, WITCH_DHFB, WITCH_DHFC,
		WITCH_DOWNROLLL, WITCH_DOWNROLLR, WITCH_DOWN, WITCH_STANDUP, WITCH_QUICKSTANDUPF, WITCH_QUICKSTANDUPB, WITCH_DIE,
		WITCH_PICKUP, WITCH_LEVELUP, WITCH_END};

public:
	enum PLAYERKEY { KEY_W, KEY_S, KEY_A, KEY_D, KEY_E, KEY_SPACE, KEY_END };
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CPlayer(void);
public:
	virtual HRESULT				Start_ForScene(void);
	virtual void				Update_Object(const _float& fTimeDelta);
	virtual void				Render_Object(void);
	void						Compute_CameraLook(const _vec3* pTargetPos);

public:
	void						Stop(void);
	void						Complete(void);
	Engine::CTransform*			GetTransCom(void);
	OBJECT_MODE					GetMode(void);


public:
	void						SetMode(OBJECT_MODE eMode);
	void						SetObjectInfo(_vec3* vPos, _vec3* vScale, _vec3* vAngle);
	_bool						GetChecked(void);
	void						CheckTrue(void);
	void						CheckFalse(void);
	OBJ_INFO*					GetObjectInfo(void);
	Engine::CNavigationMesh*	GetNaviCom(void);

private:
	Engine::CResource_Manager* m_pResourceMgr;
private:
	Engine::CTransform*			m_pTransCom;
	Engine::CRenderer*			m_pRendererCom;
	Engine::CShader*			m_pShaderCom;
	//Engine::CStaticMesh*	m_pMeshCom;
	Engine::CDynamicMesh*		m_pMeshCom;
	Engine::CNavigationMesh*	m_pNaviMeshCom;
	Engine::LAYERTYPE			m_eLayerType;

	Engine::CAnimator*			m_pAnimatorCom;
	Engine::CCalculator*		m_pCalculatorCom;
	Engine::CRigidbody*			m_pRigidbodyCom;

	Engine::VTXTEX*			m_pPickingVtx;
	CPickingRect*			m_pPickingRect;
	CMouseCol*				m_pMouseCol;
	OBJECT_MODE				m_eMode;
	OBJ_INFO				m_tInfo;

private:
	vector<Engine::ANIINFO*>	m_vecAniInfo;
	map<_uint, list<Engine::ANIMATORINFO*>>*	m_pmapNextMotion;

private:
	_bool					m_bStop;
	_bool					m_bComplete;
	_long					m_lMouseMove;
	_float					m_fAlpha;
	_int					m_iAniIdx;
	_int					m_iPlayerAniState;

private:
	_bool m_bPush[KEY_END];

private:
	_bool					m_bCheck;
	_bool					m_bCreateRect;


private:
	HRESULT		Ready_Object(void);
	HRESULT		Add_Component(void);
	HRESULT		Set_ConstantTable(LPD3DXEFFECT pEffect);

private:
	HRESULT 	LoadAniInfo(const _tchar* pFileName);
	void		SettingWork(void);
	void		DebugWork(const _float& fTimeDelta);
	void		MoveCheck(const _float& fTimeDelta);
public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev
		,const wstring& wstrName);
private:
	virtual void Free(void);
};


#endif // NPC_h__
