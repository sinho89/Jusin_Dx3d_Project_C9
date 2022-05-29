#include "stdafx.h"
#include "Rock.h"
#include "Export_Engine.h"
#include "PlayerCamera.h"
#include "WitchBlade.h"
#include "Sprite.h"


CRock::CRock(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
:CEffect(pGraphicDev, wstrName)
, m_pMeshCom(NULL)
, m_pTargetTransform(NULL)
{
}

CRock::~CRock(void)
{
}

void CRock::Update_Object(const _float & fTimeDelta)
{
	CEffect::Update_Object(fTimeDelta);

	m_pTransCom->Rotation(Engine::ROT_X, 20.f);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_STATIC, this);
}

void CRock::Render_Object(void)
{

}

void CRock::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
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

void CRock::Free(void)
{
	CEffect::Free();
}

HRESULT CRock::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;


	//===============================================
	m_pTransCom->Set_Scale(10.f, 10.f, 10.f);
	//==================================================

		
	return S_OK;
}

HRESULT CRock::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));


	//Mesh
	pComponent = Engine::Clone_Resource(RESOURCE_MESH, L"Mesh_FireBall");
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

HRESULT CRock::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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
	pEffect->SetFloat("g_fColorCheck", 0.15f);
	pEffect->SetFloat("g_fOutlineThickness", 0.00045f);

	Engine::Safe_Release(pEffect);
	return S_OK;
}


CRock * CRock::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CRock* pInstance = new CRock(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("Rock Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}