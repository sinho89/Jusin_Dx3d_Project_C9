#include "stdafx.h"
#include "ActionStartObj.h"

CActionStartObj::CActionStartObj(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
: Engine::CGameObject(pGraphicDev, wstrName)
, m_pTransCom(NULL)
, m_pColliderCom(NULL)
, m_pRendererCom(NULL)
, m_fStartDelay(0.f)
, m_fAlpha(1.0f)
, m_bOnceAdd(true)
{

}

CActionStartObj::~CActionStartObj(void)
{

}

void CActionStartObj::Update_Object(const _float & fTimeDelta)
{
	if (m_fStartDelay > 5.f && m_bOnceAdd)
	{
		Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"ActionStart", m_pColliderCom);
		m_bOnceAdd = false;
	}

	Engine::CGameObject::Update_Object(fTimeDelta);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_STATIC, this);

	m_fStartDelay += fTimeDelta;
}

void CActionStartObj::Render_Object(void)
{
	if (m_pColliderCom)
		m_pColliderCom->Render_Collider(g_bDebugRender);
}

void CActionStartObj::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
{

}

HRESULT CActionStartObj::Ready_Object(const _vec3 & vPos)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	_vec3 vPosition = vPos;

	m_pTransCom->Set_Position(&vPosition);

	return S_OK;
}

HRESULT CActionStartObj::Add_Component(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;
	m_pTransCom = (Engine::CTransform*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"0", pComponent));

	//Renderer
	pComponent = Engine::Get_Renderer();
	if (pComponent == NULL)
		return E_FAIL;
	m_pRendererCom = (Engine::CRenderer*)pComponent;
	m_pRendererCom->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Renderer", pComponent));

	//Collider
	pComponent = Engine::Create_SphereCollider(m_pGraphicDev, m_pTransCom, L"ActionStart", _vec3(0.f, 3.f, 0.f), 5.f);
	if (pComponent == NULL)
		return E_FAIL;
	m_pColliderCom = (Engine::CSphereCollider*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"SphereCol", pComponent));
	m_pColliderCom->SetScale(10.f);
	m_pColliderCom->Set_Render(true);
	return S_OK;
}

CActionStartObj * CActionStartObj::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName, const _vec3 & vPos)
{
	CActionStartObj* pInstance = new CActionStartObj(pGraphicDev, wstrName);
	if (FAILED(pInstance->Ready_Object(vPos)))
	{
		MSG_BOX("CActionStartObj Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

void CActionStartObj::Free(void)
{
	Engine::CGameObject::Free();
}
