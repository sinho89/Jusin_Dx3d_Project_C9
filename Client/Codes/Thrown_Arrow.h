#ifndef Thrown_Arrow_h__
#define Thrown_Arrow_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CRenderer;
	class CStaticMesh;
	class CShader;
}

class CThrown_Arrow
	:public Engine::CGameObject
{

private:
	explicit CThrown_Arrow(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CThrown_Arrow(void);
public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);
	virtual	void Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);

private:
	void	Shot(const _float& fTimeDelta);
	void	LookAtDir(const _float& fTimeDelta);

private:
	Engine::CTransform*		m_pTransCom;
	Engine::CRenderer*		m_pRendererCom;
	Engine::CShader*		m_pShaderCom;
	Engine::CStaticMesh*	m_pMeshCom;
	Engine::CCalculator*	m_pCalculatorCom;
	Engine::CRigidbody*		m_pRigidbodyCom;
	Engine::CCollider*		m_pSphereColCom;	//몬스터 타격.

protected:
	Engine::CStaticMesh**		m_ppMapMesh;

private:		//베지어곡선을 위한 변수..
	_vec3					m_vLastPos;
	_vec3					m_vNextPos;
	_vec3					m_vCurvePos[3];
	_vec3					m_vDir;
	_vec3					m_vMovingA;
	_vec3					m_vMovingB;
	_vec3					m_vADir;
	_vec3					m_vBDir;


private:
	_vec3					m_vPos;
	_vec3					m_vLook;
	_float					m_fAlpha;
	_float					m_fPower;
	_bool					m_bJump;
	_bool					m_bStop;
	_bool					m_bAttackCol;

private:
	_float					m_fDestroyCount;



private:
	HRESULT Ready_Object(_vec3* pPos, _vec3* pTargetPos, _vec3* pDir, _float fPower);
	HRESULT Add_Component(void);
	HRESULT	Set_ConstantTable(LPD3DXEFFECT pEffect);

public:
	static CThrown_Arrow* Create(LPDIRECT3DDEVICE9 pGraphicDev
		,const wstring& wstrName, _vec3* pPos, _vec3* pTargetPos, _vec3* pDir, _float fPower);
private:
	virtual void Free(void);
};


#endif // Environment_h__
