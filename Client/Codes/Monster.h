#ifndef Monster_h__
#define Monster_h__

#include "Defines.h"
#include "GameObject.h"

//클라
namespace Engine
{
	class CTransform;
	class CRenderer;
	class CStaticMesh;
	class CDynamicMesh;
	class CShader;
	class CNavigationMesh;

	class CAnimator;
	class CCalculator;
	class CRigidbody;
	class CCollider;
	class CLight;

	class CAI;
}

class CSound;
class CUI;

class CMonster
	:public Engine::CGameObject
{
public:
	enum ROAM_DIR { DIR_EAST, DIR_WEST, DIR_SOUTH, DIR_NORTH, DIR_END };

protected:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CMonster(void);
protected:
	virtual HRESULT				Start_ForScene(void);
	virtual void				Update_Object(const _float& fTimeDelta);

protected:
	void						Collision_CameraWork(void);

public:
	Engine::CTransform*			GetTransCom(void);

public:
	void			SetObjectInfo(_vec3* vPos, _vec3* vScale, _vec3* vAngle);

protected:
	list<_vec3>					m_BestList;

protected:
	Engine::CTransform*			m_pTransCom;
	Engine::CRenderer*			m_pRendererCom;
	Engine::CShader*			m_pShaderCom;
	Engine::CDynamicMesh*		m_pMeshCom;
	Engine::CNavigationMesh*	m_pNaviCom;

	Engine::CAnimator*			m_pAnimatorCom;
	Engine::CCalculator*		m_pCalculatorCom;
	Engine::CRigidbody*			m_pRigidbodyCom;

	Engine::CCollider*			m_pColliderCom; 	//몬스터 피격
	Engine::CCollider*			m_pSphereColCom;	//몬스터 타격.

	Engine::CAI*				m_pAICom;

	Engine::LAYERTYPE			m_eLayerType;

	CSound*						m_pSoundCom;


protected:
	Engine::CTransform*			m_pTargetTransCom;

protected:
	Engine::CStaticMesh**		m_ppMapMesh;

protected:
	vector<Engine::ANIINFO*>	m_vecAniInfo;
	map<_uint, list<Engine::ANIMATORINFO*>>*	m_pmapNextMotion;
	_int					m_iAniIdx;
	_int					m_iMonsterAniState;

protected:
	Engine::COLINFO				m_tColInfo;
	_bool						m_bDamaged;	//데미지관련
	MON_INFO					m_tMonInfo;
	CUI*						m_pDamParent;
	_bool						m_bDeath;
	_float						m_fClearTime;
	_bool						m_bDown;
	_bool						m_bDefence;
public:
	MON_INFO*	GetInfo() { return &m_tMonInfo; }
	Engine::CAnimator*			GetAnimatorInfo(void);
	const _bool					GetMonsterDeath(void) const { return m_bDeath; }

protected:
	_float					m_fAlpha;
	MONSTER_ID				m_eMonsterID;

protected:
	_bool					m_bCubeRayCol;

	_bool					m_bCriRadialBlur;
	_float					m_bCriRadialTime;


//==피격 이펙트 관련 변수 & 함수===============================================//
protected:
	_bool					m_bDamageEffectCheck;
	Engine::CLight*			m_pPointLight;
	_float					m_fLightRange;

protected:
	HRESULT		LoadCollider(const _tchar* pFileName);
protected:
	void		Render_Ray(void);
	void		PlayerRayCheck(void);
	void		CubeRayCheck(void);
	vector<Engine::ANIINFO*>* GetAniInfo(void);	


	void Collision_Light(void);

protected:
	virtual void Free(void);
};



#endif Monster_h__