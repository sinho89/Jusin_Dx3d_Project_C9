#include "stdafx.h"
#include "ReflectMesh.h"
#include "Export_Engine.h"
#include "PlayerCamera.h"

CReflectMesh::CReflectMesh(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
:CEffect(pGraphicDev, wstrName)
,m_pMeshCom(NULL)
,m_fReflectionIndex(0.05f)
, m_ePattern(PATTERN_END)
, m_fScaleSize(0.0f)
{
	m_bAni = false;
	m_fAlpha = 1.0f;
}

CReflectMesh::~CReflectMesh(void)
{
}

void CReflectMesh::Update_Object(const _float & fTimeDelta)
{
	if (!m_bAni)
		return;

	switch(m_ePattern)
	{
	case PATTERN_ONE:
		if (m_fScaleSize >= 2.0f)
		{
			m_fScaleSize = 0.f;
			m_bAni = false;
		}
		else
		{
			m_pTransCom->Add_NotYScale(0.075f);
			m_fScaleSize += 0.075f;

			_vec3 Pos = m_pTransCom->Get_Now_Position();
			Pos.y -= 0.035f;

			m_pTransCom->Set_Position(&Pos);
		}
		break;


	case PATTERN_TWO:
		if (m_fScaleSize >= 0.3f)
		{
			m_fScaleSize = 0.f;
			m_bAni = false;
		}
		else
		{
			m_pTransCom->Add_NotYScale(0.04f);
			m_fScaleSize += 0.04f;
			_vec3 Pos = m_pTransCom->Get_Now_Position();
			Pos.y -= 0.05f;

			m_pTransCom->Set_Position(&Pos);
		}
		break;

	case PATTERN_THREE:
		if (m_fScaleSize >= 1.5f)
		{
			m_fScaleSize = 0.f;
			m_bAni = false;
		}
		else
		{
			m_pTransCom->Add_NotYScale(0.12f);
			m_fScaleSize += 0.12f;
			_vec3 Pos = m_pTransCom->Get_Now_Position();
			Pos.y -= 0.06f;

			m_pTransCom->Set_Position(&Pos);
		}
		break;
	}

	CEffect::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_POSTEFFECT, this);
}

void CReflectMesh::Render_Object(void)
{
	if (!m_bAni)
		return;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	if (pEffect == NULL)
		return;

	pEffect->AddRef();


	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	m_pMeshCom->Render_Mesh(pEffect);

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

void CReflectMesh::Free(void)
{
	CEffect::Free();
}

HRESULT CReflectMesh::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

HRESULT CReflectMesh::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));


	//Mesh
	pComponent = Engine::Clone_Resource(RESOURCE_MESH, L"Mesh_Reflect");
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
	pComponent = Engine::Clone_Prototype(L"Shader_Reflect");
	if (pComponent == NULL)
		return E_FAIL;
	m_pShaderCom = (Engine::CShader*)pComponent;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Shader", pComponent));


	// For.Texture Component 
	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_EffectNormal");
	pComponent = (Engine::CComponent*)m_pTextureCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));

	return S_OK;
}

HRESULT CReflectMesh::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix	 matWorld, matView, matProj;

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);


	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);

	pEffect->SetMatrix("g_matViewInv", &matView);
	pEffect->SetMatrix("g_matProjInv", &matProj);

	pEffect->SetFloat("g_fReflect", m_fReflectionIndex);

	Engine::SetUp_OnShader(L"Target_DepthMap", pEffect, "g_DepthTexture");

	Engine::SetUp_OnShader(L"Target_Final", pEffect, "g_BlendTexture");

	m_pTextureCom->Set_Texture(pEffect, "g_NormalTexture", 0);

	pEffect->SetFloat("g_fColor", m_fAlpha);

	float m_fFar = static_cast<CPlayerCamera*>(Engine::GetCurrentCamera())->GetCamFar();

	pEffect->SetFloat("g_fFar", m_fFar);

	Engine::Safe_Release(pEffect);


	return S_OK;
}

CReflectMesh* CReflectMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CReflectMesh*		pInstance = new CReflectMesh(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("ReflectMesh Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CReflectMesh::SetPattern(PATTERN_REFLECT ePattern)
{
	m_ePattern = ePattern;
}

void CReflectMesh::SetAni(_bool bAni)
{
	m_bAni = bAni;
}

_bool CReflectMesh::GetAni(void)
{
	return m_bAni;
}

void CReflectMesh::SetReflectIndex(_float fReflect)
{
	m_fReflectionIndex = fReflect;
}

_float CReflectMesh::GetReflectIndex(void)
{
	return m_fReflectionIndex;
}

void CReflectMesh::MinusIndex(_float fResult)
{
	m_fReflectionIndex += fResult;
}
