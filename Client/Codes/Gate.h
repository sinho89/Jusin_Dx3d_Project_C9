#ifndef Gate_h__
#define Gate_h__

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
}
class CSound;

class CGate
	:public Engine::CGameObject
{
public:

protected:
	explicit CGate(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CGate(void);
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
	Engine::CTransform*			m_pTransCom;
	Engine::CRenderer*			m_pRendererCom;
	Engine::CShader*			m_pShaderCom;
	Engine::CDynamicMesh*		m_pMeshCom;

	Engine::CAnimator*			m_pAnimatorCom;
	Engine::CCalculator*		m_pCalculatorCom;
	Engine::CRigidbody*			m_pRigidbodyCom;
	CSound*						m_pSoundCom;
	//Engine::CCollider*			m_pColliderCom; 	//몬스터 피격
	//Engine::CCollider*			m_pSphereColCom;	//몬스터 타격

	Engine::LAYERTYPE			m_eLayerType;


protected:
	Engine::CTransform*			m_pTargetTransCom;

protected:
	Engine::CStaticMesh**		m_ppMapMesh;

protected:
	vector<Engine::ANIINFO*>	m_vecAniInfo;
	map<_uint, list<Engine::ANIMATORINFO*>>*	m_pmapNextMotion;
	_int					m_iAniIdx;
	_int					m_iGateAniState;

	Engine::CAnimator*			GetAnimatorInfo(void);

protected:
	_float					m_fAlpha;


//==피격 이펙트 관련 변수 & 함수===============================================//
protected:
	_bool					m_bDamageEffectCheck;

	vector<Engine::ANIINFO*>* GetAniInfo(void);	


protected:
	virtual void Free(void);
};



#endif Gate_h__