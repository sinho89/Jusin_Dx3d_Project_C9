#pragma once
#ifndef AI_State_h__
#define AI_State_h__

#include "Engine_Defines.h"
#include "Base.h"

class CSound;

BEGIN(Engine)

class CGameObject;
class CTransform;
class CAnimator;
class CNavigationMesh;
class CStaticMesh;
class CCalculator;
class CRigidbody;
class CCollider;
class CAI;

class ENGINE_DLL CAI_State
	: public CBase
{
protected:
	explicit CAI_State(void);
	virtual ~CAI_State(void);

public:
	void Set_MapMesh(LPD3DXMESH* pMesh);
	void Set_StartPos(_vec3* pPos);
	void Set_AniInfo(vector<Engine::ANIINFO*>* pvecAniInfo);
	void Set_SphereCol(Engine::CCollider* pSphereCollider);
public:
	void		PlayerRayCheck(void);
	bool		LookAtDir(_vec3* pDir, const _float& fTimeDelta);
	bool		LookAtRoamDir(_vec3* pDir, const _float& fTimeDelta);
	void        Set_LookAtDir(_bool bCheck);
public:
	virtual void Start_State(void)PURE;
	virtual void Update_State(const _float& fTimeDelta)PURE;
	virtual void End_State(void)PURE;

protected:
	CTransform*			m_pTransform;
	CAnimator*			m_pAnimator;
	CNavigationMesh*	m_pNavigationMesh;
	CCalculator*		m_pCalculator;
	CRigidbody*			m_pRigidbody;
	CCollider*			m_pCollider;
	CCollider*			m_pSphereCollider;
	CAI*				m_pAI;
	CSound*				m_pSound;
	

protected:
	CTransform*			m_pTargetTransform;


protected:
	_vec3				m_vStartPos;
	_vec3				m_vPos;
protected:
	Engine::COLINFO*	m_pColInfo;
	_bool*				m_pDamaged;
	_bool*				m_pStopState;
	_bool				m_bCol[Engine::COL_INFO_END];
	_bool				m_bSpinB;    //Æ¨°åÀ»¶§ Ã³¸®¸¦ À§ÇØ..
	_bool				m_bWallCol;
	_ulong				m_dwIdx;
	_bool				m_bDamaged;

protected:
	_vec3		m_vPlayerPos;
	_vec3		m_vPivotPos;
	_vec3		m_vTargetPos;
	_vec3		m_vRayDir;
	_vec3		m_vLook;
	_uint		m_iMeshSize;
	_float		m_fRayDis;
	_float		m_fPlayerDis;
	_float      m_fStopTime;
	_float		m_fMonsterSpeed;
	_float		m_fRayTime;		//·¹ÀÌ ½î´Â°Í µô·¹ÀÌ¿ë..

protected:
	_bool		m_bCubeRayCol;
	_bool		m_bRayMove;
	_bool		m_bAStarMove;
	_bool		m_bStop;
	_bool		m_bRayTime;

protected:
	_bool		m_bJump;
	_bool		m_bJumpMove;

protected:
	_bool		m_bLookAtDir;

protected:
	map<_int, LPD3DXMESH*> m_mapMesh;
	typedef map<_int, LPD3DXMESH*> MAPMESH;

	vector<Engine::ANIINFO*>*	m_pvecAniInfo;

	list<Engine::CGameObject*>*	m_pCubeList;
	
	
private:
	HRESULT Ready_AI(void);

private:
	virtual void Free(void)PURE;
};

END



#endif // AI_State_h__
