#include "Effect.h"

USING(Engine)

Engine::CEffect::CEffect(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
: Engine::CGameObject(pGraphicDev, wstrName)
, m_pTransCom(NULL)
, m_pRendererCom(NULL)
, m_pShaderCom(NULL)
, m_pTextureCom(NULL)
, m_pPlayerAction(NULL)
, m_bEffectBlurCheck(false)
{
	m_PrtD  = NULL;
	m_bAni  = FALSE;
	m_pVtx  = NULL;
	m_iVtx  = 0;
	m_bAni  = false;

	ZeroMemory(&m_pParticleInfo, sizeof(Engine::PARTICLEINFO));
}

Engine::CEffect::~CEffect(void)
{
}

HRESULT Engine::CEffect::Ready_Object(void)
{

	return S_OK;
}




void Engine::CEffect::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);
}



void Engine::CEffect::Render_Object(void)
{

}

void Engine::CEffect::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
{

}

CEffect* Engine::CEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev , const wstring& wstrName)
{
	CEffect*		pInstance = new CEffect(pGraphicDev, wstrName);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("Effect Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void Engine::CEffect::Free(void)
{
	Engine::CGameObject::Free();
}

void Engine::CEffect::Destroy(void)
{
	Engine::Safe_Delete_Array(m_PrtD);
	Engine::Safe_Delete_Array(m_pVtx);
}


void Engine::CEffect::SetAvgTime(_float fTime)
{
	m_pParticleInfo.m_fTimeAvg = fTime;
}

_int Engine::CEffect::SortFnc(const TPART* p1, const TPART* p2)
{
	_float  v1,  v2;

	v1 = p1->PrsZ;
	v2 = p2->PrsZ;

	if(v1 < v2)
		return 1;

	else if(v1  == v2)
		return 0;

	else
		return -1;
}



PARTICLEINFO* Engine::CEffect::GetParticleInfo(void)
{
	return &m_pParticleInfo;
}


void Engine::CEffect::SetTransCom(_vec3* m_Pos)
{
	m_pTransCom->Set_Position(m_Pos);
}

CTransform* Engine::CEffect::GetTransCom(void)
{
	return m_pTransCom;
}


void Engine::CEffect::RandomSpeed(void)
{
	srand(unsigned(time(NULL)));

	m_pParticleInfo.m_tSpriteInfo.m_fSpriteSpeed -= (rand()%30);
}

void Engine::CEffect::SetPos(const _vec3& vPos)
{
	m_pTransCom->Set_Position(&vPos);
}

void Engine::CEffect::SetScale(const _vec3& vScale)
{
	m_pTransCom->Set_Scale(vScale.x, vScale.y, vScale.z);
}

void Engine::CEffect::SetRot(const _vec3& vRot)
{
	m_pTransCom->Rotation(Engine::ROT_X, vRot.x);
	m_pTransCom->Rotation(Engine::ROT_Y, vRot.y);
	m_pTransCom->Rotation(Engine::ROT_Z, vRot.z);
}

void CEffect::SetAngle(const _vec3* vRot)
{
	m_pTransCom->Set_Angle(vRot);
}

void Engine::CEffect::SetTranscomInfo(void)
{
	m_pTransCom->Set_Position(&m_pParticleInfo.m_vParentTranform);

	m_pTransCom->Set_Scale(m_pParticleInfo.m_vParentScale.x,
		m_pParticleInfo.m_vParentScale.y,
		m_pParticleInfo.m_vParentScale.z);

	m_pTransCom->Set_Angle(&m_pParticleInfo.m_vParentRotate);
}

void CEffect::SetBlurRenderCheck(_bool bCheck)
{
	m_bEffectBlurCheck = bCheck;
}

void CEffect::SetBlurVariable(_int iBlurCnt, _float fBlurPower, _float fBlurValue, BLURTYPE eBlurType)
{
	m_iBlurCnt = iBlurCnt;
	m_fBlurPower = fBlurPower;
	m_fBlurValue = fBlurValue;
	m_eBlurType = eBlurType;
}
