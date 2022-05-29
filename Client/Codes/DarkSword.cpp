#include "stdafx.h"
#include "DarkSword.h"
#include "Export_Engine.h"
#include "PlayerCamera.h"
#include "LightCamera.h"
#include "WitchBlade.h"


CDarkSword::CDarkSword(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
: CEffect(pGraphicDev, wstrName)
, m_pMeshCom(NULL)
, m_pTargetTransform(NULL)
{
	m_bIsStatic = true;
	m_bAni = false;

}

CDarkSword::~CDarkSword(void)
{
}

void CDarkSword::Update_Object(const _float & fTimeDelta)
{
	if (!m_bAni)
		return;

	CEffect::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_STATIC, this);
}

void CDarkSword::Render_Object(void)
{
}

void CDarkSword::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
{
	if (!m_bAni)
		return;

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
		break;

	case 4:
		break;

	case 5:
		break;
	}

	m_pMeshCom->Render_Mesh(pEffect, iPassIdx);
}

HRESULT CDarkSword::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->Set_Scale(0.175f, 0.05f, 0.05f);
	
	m_pTransCom->Rotation(Engine::ROT_Z, -90.f);

	return S_OK;
}

CDarkSword * CDarkSword::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CDarkSword*		pInstance = new CDarkSword(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("DarkSword Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CDarkSword::Free(void)
{
	CEffect::Free();
}

HRESULT CDarkSword::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));



	//Mesh
	pComponent = Engine::Clone_Resource(RESOURCE_MESH, L"Mesh_DarkSword");
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


	// For.Texture Component 
	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_Sword");
	pComponent = (Engine::CComponent*)m_pTextureCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));


	return S_OK;
}

HRESULT CDarkSword::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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
	
	pEffect->SetBool("g_bOutline", false);
	pEffect->SetFloat("g_fColorCheck", 0.15f);
	pEffect->SetFloat("g_fOutlineThickness", 0.00045f);
	

	Engine::Safe_Release(pEffect);

	return S_OK;
}

HRESULT CDarkSword::Start_ForScene(void)
{

	if (m_bOnceStart)
		return S_OK;

	m_bOnceStart = true;

	Engine::CGameObject* pGameObject = Engine::FindWithTag(L"Player");

	if (pGameObject == NULL)
		return S_OK;

	m_pPlayerAction = ((CWitchBlade*)pGameObject)->Get_IsSkill(PLAYER_FIGHT);


	Engine::CTransform* pTransform = (Engine::CTransform*)pGameObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

	m_pTargetTransform = pTransform;

	Engine::Safe_Release(pTransform);
	Engine::Safe_Release(pGameObject);
	return S_OK;
}


void CDarkSword::SetAni(_bool bAni)
{
	m_bAni = bAni;

}

_bool CDarkSword::GetAni(void)
{
	return m_bAni;
}
