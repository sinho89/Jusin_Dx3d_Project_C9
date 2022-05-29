#include "stdafx.h"

#include "Gate.h"

#include "Export_Engine.h"
#include "VIBuffer.h"

#include "Map_Ice.h"
#include "Map_Fire.h"


#include "Environment.h"
#include "Player.h"
#include "Cube.h"
#include "NavigationMesh.h"
#include "NumberingMgr.h"
#include "Camera.h"
#include "DamParent.h"
#include "Light.h"

//Å¬¶ó Gate

CGate::CGate(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
	: Engine::CGameObject(pGraphicDev, wstrName)
	, m_pTransCom(NULL)
	, m_pMeshCom(NULL)
	, m_pRendererCom(NULL)
	, m_pShaderCom(NULL)
	, m_pAnimatorCom(NULL)
	, m_pCalculatorCom(NULL)
	, m_pRigidbodyCom(NULL)
	, m_ppMapMesh(NULL)
	, m_pTargetTransCom(NULL)
	, m_iAniIdx(0)
	, m_iGateAniState(0)
	, m_fAlpha(1.0f)
{
}
CGate::~CGate(void)
{

}

HRESULT CGate::Start_ForScene(void)
{
	Engine::CGameObject* pGameObject = Engine::FindWithTag(L"Map");
	if (pGameObject == NULL)
		return E_FAIL;

	if (dynamic_cast<CMap_Ice*>(pGameObject) != NULL)
	{
		_int iSize = 0;
		Engine::CStaticMesh** ppMesh = ((CMap_Ice*)pGameObject)->GetMapMesh(&iSize);
		m_ppMapMesh = ((CMap_Ice*)pGameObject)->GetMapMesh(&iSize);

		for (_int i = 0; i < iSize; i++)
		{
			m_pCalculatorCom->Set_TargetMesh(ppMesh[i]->Get_Mesh());
		}
	}

	if (dynamic_cast<CMap_Fire*>(pGameObject) != NULL)
	{
		_int iSize = 0;
		Engine::CStaticMesh** ppMesh = ((CMap_Fire*)pGameObject)->GetMapMesh(&iSize);
		m_ppMapMesh = ((CMap_Fire*)pGameObject)->GetMapMesh(&iSize);

		for (_int i = 0; i < iSize; i++)
		{
			m_pCalculatorCom->Set_TargetMesh(ppMesh[i]->Get_Mesh());
		}
	}

	Engine::Safe_Release(pGameObject);

	pGameObject = Engine::FindWithTag(L"Player");
	if (pGameObject == NULL)
		return E_FAIL;

	m_pTargetTransCom = (Engine::CTransform*)pGameObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);
	if (m_pTargetTransCom == NULL)
		return E_FAIL;


	Engine::Safe_Release(m_pTargetTransCom);
	Engine::Safe_Release(pGameObject);

	return S_OK;
}

void CGate::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);
	
	_vec3 vTargetPos;
	vTargetPos = m_pTargetTransCom->Get_Now_Position();
	_vec3 vMyPos;
	vMyPos = m_pTransCom->Get_Now_Position();

	_vec3 vDis;
	vDis = vTargetPos - vMyPos;

	m_pMeshCom->Play_AnimationSet(fTimeDelta);

	/////////////////////////////////////////////

	///////////////////////////////////
	if (m_pRendererCom->GetShadowBuffer() ||
		m_pRendererCom->GetSoftShadow() ||
		m_pRendererCom->GetCACDShadow())
	{
		m_pRendererCom->Add_RenderGroup(Engine::RENEDER_DYANAMIC_SHADOW, this);
	}

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_DYNAMIC, this);

}


Engine::CTransform* CGate::GetTransCom(void)
{
	return m_pTransCom;
}


void CGate::SetObjectInfo(_vec3* vPos, _vec3* vScale, _vec3* vAngle)
{
	m_pTransCom->Set_Position(vPos);
	m_pTransCom->Set_Scale(vScale->x, vScale->y, vScale->z);
	m_pTransCom->Rotation(Engine::ROT_Y, vAngle->y);
}

void CGate::Free(void)
{
	Engine::CGameObject::Free();
}

vector<Engine::ANIINFO*>* CGate::GetAniInfo(void)
{
	return &m_vecAniInfo;
}

Engine::CAnimator * CGate::GetAnimatorInfo(void)
{
	return m_pAnimatorCom;
}








