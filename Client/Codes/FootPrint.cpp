#include "stdafx.h"
#include "FootPrint.h"
#include "Texture.h"
#include "Cube_Texture.h"
#include "Camera.h"

CFootPrint::CFootPrint(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName, const _int iDecalNum)
	: Engine::CGameObject(pGraphicDev, wstrName)
	, m_pTransCom(NULL)
	, m_pBufferCom(NULL)
	, m_pRendererCom(NULL)
	, m_iDecalNum(iDecalNum)
	, m_fDecalDeadTime(0.0f)
{
}

CFootPrint::~CFootPrint(void)
{
}

void CFootPrint::Update_Object(const _float & fTimeDelta)
{
	m_fDecalDeadTime += fTimeDelta;
	if (m_fDecalDeadTime >= 10.0f)
	{
		m_eState = Engine::STATE_DESTROY;
		return;
	}

	Engine::CGameObject::Update_Object(fTimeDelta);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_DECAL, this);
}

void CFootPrint::Render_Object(void)
{
}

void CFootPrint::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
{
	if (m_fDecalDeadTime >= 15.0f)
		return;

	pEffect->AddRef();

	_matrix			matWorld, matView, matProj;

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetVector("g_vLightDir", &_vec4(1.0f, -1.0f, 1.0f, 0.0f));

	_vec3 vCubePos[6];

	vCubePos[0] = _vec3(-1.0f, 0.0f, 0.0f);
	vCubePos[1] = _vec3(1.0f, 0.0f, 0.0f);
	vCubePos[2] = _vec3(0.0f, -1.0f, 0.0f);
	vCubePos[3] = _vec3(0.0f, 1.0f, 0.0f);
	vCubePos[4] = _vec3(0.0f, 0.0f, -1.0f);
	vCubePos[5] = _vec3(0.0f, 0.0f, 1.0f);

	//for (int i = 0; i < 6; ++i)
	//{
	//	D3DXVec3TransformCoord(&vCubePos[i], &vCubePos[i], &matWorld);
	//}

	D3DXMatrixInverse(&matProj, NULL, &matProj);
	pEffect->SetMatrix("g_matProjInv", &matProj);

	D3DXMatrixInverse(&matView, NULL, &matView);
	pEffect->SetMatrix("g_matViewInv", &matView);

	D3DXMatrixInverse(&matWorld, NULL, &matWorld);
	pEffect->SetMatrix("g_matWorldInv", &matWorld);

	//D3DXVec3TransformCoord(&m_pDecalPosition, &m_pDecalPosition, &matWorld);
	//pEffect->SetVector("g_vCubePosition", &_vec4(m_pDecalPosition, 1.0f));

	m_pTextureCom->Set_Texture(pEffect, "g_SSDTexture", m_iDecalNum);
	
	/*if(0 == m_iDecalNum)
		m_pTextureCom->Set_Texture(pEffect, "g_NormalTexture", 1);
	else if(2 == m_iDecalNum)
		m_pTextureCom->Set_Texture(pEffect, "g_NormalTexture", 3);*/
	
	Engine::SetUp_OnShader(L"Target_DepthMap", pEffect, "g_DepthTexture");
	Engine::SetUp_OnShader(L"Target_StageMap_DepthMap", pEffect, "g_MapDepthTexture");
	//Engine::SetUp_OnShader(L"Target_Albedo", pEffect, "g_BaseTexture");

	/*_vec4 vCamTopRight(1.0f, 1.0f, 1.0f, 1.0f);
	D3DXVec4Transform(&vCamTopRight, &vCamTopRight, &matProj);
	vCamTopRight = vCamTopRight / vCamTopRight.w;
	pEffect->SetVector("g_vCamTopRight", &vCamTopRight);*/

	//pEffect->SetFloat("g_fCubeScale", 5.0f);

	//for (int i = 0; i < 6; ++i)
	//{
	//	D3DXVec3TransformCoord(&vCubePos[i], &vCubePos[i], &matWorld);
	//}

	pEffect->SetVector("g_vCubeScale_0", &_vec4(vCubePos[0], 1.0f));
	pEffect->SetVector("g_vCubeScale_1", &_vec4(vCubePos[1], 1.0f));
	pEffect->SetVector("g_vCubeScale_2", &_vec4(vCubePos[2], 1.0f));
	pEffect->SetVector("g_vCubeScale_3", &_vec4(vCubePos[3], 1.0f));
	pEffect->SetVector("g_vCubeScale_4", &_vec4(vCubePos[4], 1.0f));
	pEffect->SetVector("g_vCubeScale_5", &_vec4(vCubePos[5], 1.0f));

	float fFar = Engine::GetCurrentCamera()->GetCamFar();
	pEffect->SetFloat("g_fFar", fFar);

	::Safe_Release(pEffect);
}

Engine::CTransform * CFootPrint::GetTransCom(void)
{
	return m_pTransCom;
}

HRESULT CFootPrint::Ready_Object(const _vec3 vPickPos, const _vec3 vPickAngle)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->Set_Position(vPickPos.x, vPickPos.y, vPickPos.z);
	m_pTransCom->Set_Angle(&vPickAngle);//_vec3(90.0f, 0.0f, 0.0f));
	m_pTransCom->Set_Scale(0.25f, 0.5f, 0.25f);

	m_pDecalPosition = vPickPos;

	return S_OK;
}

HRESULT CFootPrint::Add_Component(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;
	m_pTransCom = (Engine::CTransform*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Transform", m_pTransCom));

	// For.Buffer Component
	m_pBufferCom = (Engine::CCube_Texture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_RcTex");
	pComponent = (Engine::CComponent*)m_pBufferCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));

	// For.Renderer Component
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	if (NULL == pComponent)
		return E_FAIL;
	pComponent->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));

	//Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Effect"));
	if (NULL == pComponent) return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	// For.Texture Component 
	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_Decal");
	pComponent = (Engine::CComponent*)m_pTextureCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));

	return S_OK;
}

HRESULT CFootPrint::Set_ConstantTable(LPD3DXEFFECT pEffect)
{
	return S_OK;
}

CFootPrint * CFootPrint::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName, const _vec3 vPickPos, const _vec3 vPickAngle, const _int iDecalNum)
{
	CFootPrint*		pInstance = new CFootPrint(pGraphicDev, wstrName, iDecalNum);

	if (FAILED(pInstance->Ready_Object(vPickPos, vPickAngle)))
	{
		MSG_BOX("CFootPrint Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CFootPrint::Free(void)
{
	Engine::CGameObject::Free();
}
