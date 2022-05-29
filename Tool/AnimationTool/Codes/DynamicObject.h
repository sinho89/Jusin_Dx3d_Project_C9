#ifndef DynamicObject_h__
#define DynamicObject_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CRenderer;
	class CDynamicMesh;
	class CShader;
	class CCollider;
}

class CDynamicObject
	: public Engine::CGameObject
{
private:
	explicit CDynamicObject(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CDynamicObject(void);
public: //Getter
	_double Get_TrackPos(void);
	_double Get_Period(void);
	_double Get_Period(const _uint& iIndex);
	_double Get_OldTrackPos(void);
	_bool	Get_Playing(void);
	_bool	Is_AnimationEnd(_double dMagicNumber = 0.0);
public: //Setter
	void Set_AnimationSet(const _uint& iIdx, const Engine::MOTIONINFO* pMotionInfo, _double dMagicNumber = 0.25);
	void Set_AniTrackPos(const _double& dPosition);
	void Set_Playing(_bool bIsPlaying);
	void Set_TrackSpeed(const _float& fSpeed);
	void Set_AniInfo(const Engine::MOTIONINFO* pMotionInfo);
	void Set_Collider(const _uint& iType);
	void Delete_Collider(void);
	void Set_ColliderPos(const _vec3& vPos);
	void Set_ColliderSize(const _vec3& vSize);
	void Set_ColliderSize(const _float& fRad);
public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);
private:
	Engine::CTransform* m_pTransCom;
	Engine::CRenderer* m_pRendererCom;
	Engine::CDynamicMesh* m_pMeshCom;
	Engine::CShader* m_pShaderCom;
	Engine::CCollider* m_pColliderCom;
private:
	_float m_fTimeDelta;
	_double m_dAniOldPosition;
	_int m_iAniIdx;
	_bool m_bIsPlaying;
private:
	HRESULT Ready_Object(const wstring& wstrName);
	HRESULT Add_Component(const wstring& wstrName);
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	static CDynamicObject* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
private:
	virtual void Free(void);	
};

#endif // DynamicObject_h__
