#include "stdafx.h"
#include "Model_Floor.h"
#include "Export_Engine.h"
#include "UICamera.h"

CModel_Floor::CModel_Floor(LPDIRECT3DDEVICE9 pDevice, const wstring& wstrName)
: CUI(pDevice, wstrName)
, m_pTransCom(NULL)
, m_pRendererCom(NULL)
, m_pShaderCom(NULL)
{
	ZeroMemory(&m_pMeshCom, NULL);
}

CModel_Floor::~CModel_Floor(void)
{

}

HRESULT CModel_Floor::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->Set_Position(0.f, -1.2f, 0.5f);
	m_pTransCom->Set_Scale(0.3f, 0.3f, 0.3f);
	m_pTransCom->Rotation(Engine::ROT_X, 90.f);
	m_iIndex = -1;

	return S_OK;
}

void CModel_Floor::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_STATIC, this);
}

void CModel_Floor::Render_Object(void)
{
	if(m_iIndex < 0 || m_iIndex > 4)
		return;

	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if(NULL == pEffect) return;

	pEffect->AddRef();

	if(FAILED(Set_ConstantTable(pEffect))) return;

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	m_pMeshCom[m_iIndex]->Render_Mesh(pEffect);

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

void CModel_Floor::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
{
	if (m_iIndex < 0 || m_iIndex > 4)
		return;

	switch (iPassIdx)
	{
	case 0:
		if (FAILED(Set_ConstantTable(pEffect))) return;
		break;

	case 1:
		if (FAILED(Set_ConstantTable(pEffect))) return;
		break;

	case 2:
		//if (FAILED(Set_Shadow_ConstanTable(pEffect))) return;
		return;
		break;

	case 3:
		//if (FAILED(Set_Minimap_ConstanTable(pEffect))) return;
		return;
		break;

	case 4:
		if (FAILED(Set_ConstantTable(pEffect))) return;
		break;
	}

	m_pMeshCom[m_iIndex]->Render_Mesh(pEffect, iPassIdx);
}

void CModel_Floor::SetInit(void)
{
	if(m_iIndex < 0 || m_iIndex > 4)
		return;
	m_pTransCom->Set_Angle(&_vec3(0.f, 0.f, 0.f));
}

HRESULT CModel_Floor::Add_Component(void)
{
	Engine::CComponent*	pComponent = NULL;

	//Transform
	pComponent = m_pTransCom	=	Engine::CTransform::Create();
	if(NULL == pComponent) return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	//Mesh
	pComponent = m_pMeshCom[0]		=	static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Mesh_Floor_Fighter"));
	if(NULL == pComponent) return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Model1", pComponent));

	pComponent = m_pMeshCom[1]		=	static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Mesh_Floor_Hunter"));
	if(NULL == pComponent) return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Model2", pComponent));

	pComponent = m_pMeshCom[2]		=	static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Mesh_Floor_Shaman"));
	if(NULL == pComponent) return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Model3", pComponent));

	pComponent = m_pMeshCom[3]		=	static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Mesh_Floor_WitchBlade"));
	if(NULL == pComponent) return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Model4", pComponent));

	pComponent = m_pMeshCom[4]		=	static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Mesh_Floor_Mistic"));
	if(NULL == pComponent) return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Model5", pComponent));

	//Renderer
	pComponent = m_pRendererCom	=	Engine::Get_Renderer();
	if(NULL == pComponent) return E_FAIL;
	pComponent->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));

	//Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_StaticMesh"));
	if(NULL == pComponent) return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	return S_OK;
}


HRESULT CModel_Floor::Set_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld, matView, matProj;

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	const D3DLIGHT9* pLightInfo = Engine::Find_LightInfo(0, D3DLIGHT_DIRECTIONAL);

	pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.0f));
	//pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	//pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);
	//pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfo->Specular);

	//D3DMATERIAL9	MtrlInfo;
	//ZeroMemory(&MtrlInfo, sizeof(D3DMATERIAL9));

	////지형 색 * (빛.Diffuse * 마테리얼.diffuse) + (빛.Ambient * 마테리얼.Ambient)
	//MtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//MtrlInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//MtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//MtrlInfo.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.f);
	//MtrlInfo.Power = 20.f;

	//pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&MtrlInfo.Diffuse);
	//pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&MtrlInfo.Ambient);
	//pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&MtrlInfo.Specular);
	pEffect->SetFloat("g_fPower", 20.0f);

	D3DXMatrixInverse(&matView, NULL, &matView);
	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView._41);

	float fFar = Engine::GetCurrentCamera()->GetCamFar();
	pEffect->SetFloat("g_fFar", fFar);

	pEffect->SetBool("g_bLimLight", false);

	Engine::Safe_Release(pEffect);

	return S_OK;
}

CModel_Floor* CModel_Floor::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
{
	CModel_Floor*	pPlayer = new CModel_Floor(pGraphicDev, wstrName);

	if(FAILED(pPlayer->Ready_Object()))
	{
		MSG_BOX("CPlayer Created Failed");
		::Safe_Release(pPlayer);
	}

	return pPlayer;
}

void CModel_Floor::Free(void)
{
	Engine::CGameObject::Free();
}

