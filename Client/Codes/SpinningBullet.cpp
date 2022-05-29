#include "stdafx.h"
#include "SpinningBullet.h"
#include "Export_Engine.h"
#include "PlayerCamera.h"
#include "LightCamera.h"
#include "WitchBlade.h"


CSpinningBullet::CSpinningBullet(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
: CEffect(pGraphicDev, wstrName)
, m_pMeshCom(NULL)
{
	m_bIsStatic = true;
}

CSpinningBullet::~CSpinningBullet(void)
{
}

void CSpinningBullet::Update_Object(const _float & fTimeDelta)
{
	CEffect::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_STATIC, this);
}

void CSpinningBullet::Render_Object(void)
{

}

void CSpinningBullet::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
{
	switch (iPassIdx)
	{
	case 0:
		if (FAILED(SetUp_ConstantTable(pEffect))) return;
		break;

	case 1:
		if (FAILED(SetUp_ConstantTable(pEffect))) return;
		break;

	case 2:
		break;

	case 3:
		return;
		break;

	case 4:
		return;
		break;

	case 5:
		break;
	}

	m_pMeshCom->Render_Mesh(pEffect, iPassIdx);
}

void CSpinningBullet::Free(void)
{

	CEffect::Free();
}

HRESULT CSpinningBullet::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpinningBullet::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	//Mesh
	pComponent = Engine::Clone_Resource(RESOURCE_MESH, L"Mesh_Icicle2");
	if (pComponent == NULL)
		return E_FAIL;
	m_pMeshCom = (Engine::CStaticMesh*)pComponent;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh", pComponent));


	//For.Renderer Component
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	if (NULL == pComponent) return E_FAIL;
	pComponent->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));


	// For.Shader Component
	pComponent = Engine::Clone_Prototype(L"Shader_StaticMesh");
	if (pComponent == NULL)
		return E_FAIL;

	m_pShaderCom = (Engine::CShader*)pComponent;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Shader", pComponent));


	return S_OK;
}

HRESULT CSpinningBullet::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld, matView, matProj;

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetVector("g_vLightDir", &_vec4(1.0f, -1.0f, 1.0f, 0.0f));

	pEffect->SetFloat("g_fPower", 20.0f);

	D3DXMatrixInverse(&matView, NULL, &matView);
	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView._41);

	float fFar = Engine::GetCurrentCamera()->GetCamFar();
	pEffect->SetFloat("g_fFar", fFar);

	pEffect->SetBool("g_bBlurCheck", false);
	pEffect->SetBool("g_bSpecularCheck", true);
	pEffect->SetBool("g_bOutline", m_bOutlineCheck);

	Engine::Safe_Release(pEffect);

	return S_OK;
}




CSpinningBullet * CSpinningBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CSpinningBullet*		pInstance = new CSpinningBullet(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("SpinningBullet Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpinningBullet::SetAni(_bool bAni)
{
	m_bAni = bAni;
}

_bool CSpinningBullet::GetAni(void)
{
	return m_bAni;
}
