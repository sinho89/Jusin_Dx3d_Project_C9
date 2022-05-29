#ifndef	MONSTER_h__
#define	MONSTER_h__

#include "Defines.h"
#include "GameObject.h"

//맵툴
namespace Engine
{
	class CTransform;
	class CRenderer;
	class CStaticMesh;
	class CDynamicMesh;
	class CShader;
	class CLine;
	class CNavigationMesh;

	class CAnimator;
	class CCalculator;
	class CRigidbody;

	class CAI;
}

class CMouseCol;
class CPickingRect;
//class CSphere_Tool;

class CMonster
	:public Engine::CGameObject
{
public:
	enum BABEGAZI_WARRIOR{ STAND_IDLE, WALK, WALK_B, WALK_L, WALK_R, STAND_DOWN, STAND_UP, STUN, SPIN_A, SPIN_B
	, MOVE_RUN, DOWN, DISCOVERY, DASH_BACK, DEFENCE_SWORD, DEFENCE, ATTACK, BATTLE_IDLE, CAPTURE, DAM_B, DAM_F
	, DAM_H, DAM_HARD, DAM_L, DAM_R, DAM_SWO, DAM_UPPER_A, DAM_UPPER_B, DAM_UPPER_C, JUMP_A, JUMP_B, JUMP_C, ANI_END};

	enum ROAM_DIR{ DIR_EAST, DIR_WEST, DIR_SOUTH, DIR_NORTH, DIR_END};

	enum AI_STATE { AI_ROAMING, AI_DISCOVERY, AI_TRACE, AI_END };

private:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CMonster(void);
public:
	virtual HRESULT				Start_ForScene(void);
	virtual void				Update_Object(const _float& fTimeDelta);
	virtual void				Render_Object(void);

public:
	void						Stop(void);
	void						Complete(void);
	Engine::CTransform*			GetTransCom(void);
	OBJECT_MODE					GetMode(void);


public:
	void			SetMode(OBJECT_MODE eMode);
	void			SetObjectInfo(_vec3* vPos, _vec3* vScale, _vec3* vAngle);
	_bool			GetChecked(void);
	void			CheckTrue(void);
	void			CheckFalse(void);
	OBJ_INFO*		GetObjectInfo(void);

private:
	Engine::CResource_Manager* m_pResourceMgr;
private:
	Engine::CTransform*			m_pTransCom;
	Engine::CRenderer*			m_pRendererCom;
	Engine::CShader*			m_pShaderCom;
	//Engine::CStaticMesh*		m_pMeshCom;
	Engine::CDynamicMesh*		m_pMeshCom;
	Engine::CNavigationMesh*	m_pNaviMeshCom;

	Engine::CAnimator*			m_pAnimatorCom;
	Engine::CCalculator*		m_pCalculatorCom;
	Engine::CRigidbody*			m_pRigidbodyCom;

	Engine::CAI*				m_pAICom;
	Engine::CTransform*			m_pTargetTransCom;

	Engine::LAYERTYPE			m_eLayerType;
	Engine::VTXTEX*				m_pPickingVtx;
	Engine::CLine*				m_pLine;
	Engine::CLine*				m_pNaviTraceLine;
	LPD3DXLINE					m_pD3DXLine;

	//CSphere_Tool*			m_pSphere;
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
	_int					m_iMonsterAniState;
private:
	_vec3					m_vStartPos;
	_vec3					m_vRoamPos[DIR_END];
	_vec3					m_vTargetPos;
	_vec3					m_vPivotPos;
	_vec3					m_vRayDir;
	_vec3					m_vPlayerPos;
	_float					m_fAggroRad;	//몬스터 어그로 반경
	_float					m_fPlayerDis;	//플레이어와의 거리
	_float					m_fRayDis;		//레이의 길이

private:
	_float					m_fRoamingTime;
	ROAM_DIR				m_eRoamDir;
	ROAM_DIR				m_eCompareRoamDir;

private:
	_bool					m_bCheck;
	_bool					m_bCreateRect;
	_bool					m_bCubeRayCol;

private:
	_bool					m_bJump;		//점프
	_bool					m_bJumpMove;	//점프중
	_bool					m_bAstarMove;	//A스타이동을 알려줌
	_bool					m_bRayMove;		//레이이동을 알려줌
	_bool					m_bAggro;		//어그로먹엇다..
	_bool					m_bRoam;		//어그로없을시 로밍이동을위함
	_bool					m_bMove;		//이동가능판단..


private:
	HRESULT		Ready_Object(void);
	HRESULT		Add_Component(void);
	HRESULT		Set_ConstantTable(LPD3DXEFFECT pEffect);

private:
	HRESULT 	LoadAniInfo(const _tchar* pFileName);
	void		SettingWork(void);
	void		DebugWork(const _float& fTimeDelta);
	void		RoamPosSet(void);
	void		NormalMove(const _float& fTimeDelta);
	void		Render_Ray(void);
	void		PlayerRayCheck(void);
	void		CubeRayCheck(void);
	void		LookAtDir(_vec3* pDir, const _float& fTimeDelta);
public:
	static CMonster* Create(LPDIRECT3DDEVICE9 pGraphicDev
		,const wstring& wstrName);
private:
	virtual void Free(void);
};


#endif // NPC_h__
