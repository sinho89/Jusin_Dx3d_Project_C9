#include "stdafx.h"
#include "Map_Store.h"

#include "Export_Engine.h"
#include "Light.h"

#include "Camera.h"
//#include "Camera_MapTool.h"
//#include "MouseCol.h"

CMap_Store::CMap_Store(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
: Engine::CGameObject(pGraphicDev, wstrName)
, m_pTransCom(NULL)
, m_pRendererCom(NULL)
, m_pLightCam(NULL)
, m_pCurCam(NULL)
, m_fAlpha(1.0f)
{
	for(int i = 0 ; i < 9; ++i)
	{
		m_pMeshCom[i] = NULL;
	}
}
CMap_Store::~CMap_Store(void)
{

}

Engine::CStaticMesh** CMap_Store::GetMapMesh(_int* iSize)
{
	*iSize = 9;
	return m_pMeshCom;
}

Engine::CTransform * CMap_Store::GetTrans(void)
{
	return m_pTransCom;
}

HRESULT CMap_Store::Start_ForScene(void)
{
	m_pLightCam = Engine::GetCamera(Engine::CAMERA_LIGHT);
	m_pCurCam	= Engine::GetCurrentCamera();

	return S_OK;
}

void CMap_Store::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);
		
	//m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	//m_pRendererCom->Add_RenderGroup(Engine::RENDER_STAGE_DEPTH, this);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_STATIC, this);

	//if (m_pRendererCom->GetShadowBuffer())
	//{
	//	m_pRendererCom->Add_RenderGroup(Engine::RENEDER_SHADOW, this);

	//	//_vec3 EyePos = _vec3(-10.0f, 50.0f, 10.0f);
	//}

	_vec3	vScale;
	Compute_ViewZ(m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &vScale));
}

void CMap_Store::Render_Object(void)
{
	/*LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if(NULL == pEffect) return;

	pEffect->AddRef();

	_uint iPassIdx = 0;

	switch (Engine::GetMRTID())
	{
	case Engine::MRT_DEFERRED_0:
		iPassIdx = 2;
		if (FAILED(Set_Shadow_ConstanTable(pEffect))) return;
		break;

	case Engine::MRT_DEFERRED_1:
		iPassIdx = 0;
		if (FAILED(Set_ConstantTable(pEffect))) return;
		break;

	case Engine::MRT_DEFERRED_2:
		iPassIdx = 1;
		if (FAILED(Set_ConstantTable(pEffect))) return;
		break;

	case Engine::MRT_DEFERRED_3:
		iPassIdx = 4;
		if (FAILED(Set_ConstantTable(pEffect))) return;
		break;

	case Engine::MRT_DEFERRED_4:
		return;

	case Engine::MRT_MINIMAP:
		iPassIdx = 3;
		if (FAILED(Set_Minimap_ConstanTable(pEffect))) return;
		break;
	}
	
	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(iPassIdx);

	for (int i = 0; i < 9; ++i)
	{
		m_pMeshCom[i]->Render_Mesh(pEffect, iPassIdx);
	}

	pEffect->EndPass();
	pEffect->End();
	
	Engine::Safe_Release(pEffect);*/
}

void CMap_Store::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
{
	switch (iPassIdx)
	{
	case 0:
		if (FAILED(Set_ConstantTable(pEffect))) return;
		break;

	case 1:
		if (FAILED(Set_ConstantTable(pEffect))) return;
		break;

	case 2:
		if (FAILED(Set_Shadow_ConstanTable(pEffect))) return;
		break;

	case 3:
		if (FAILED(Set_Minimap_ConstanTable(pEffect))) return;
		break;

	case 4:
		if (FAILED(Set_ConstantTable(pEffect))) return;
		break;

	case 6:
		return;
	}

	for (int i = 0; i < 9; ++i)
	{
		m_pMeshCom[i]->Render_Mesh(pEffect, iPassIdx);
	}
}

HRESULT CMap_Store::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->Set_Position(0.0f, 0.0f, 0.0f);

//	m_pMouseCol = CMouseCol::Create();
//	m_pTransCom->Set_Scale(1.f, 0.0001f, 0.0001f);	

	return S_OK;
}

HRESULT CMap_Store::Add_Component(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = Engine::CTransform::Create();
	if(pComponent == NULL)
		return E_FAIL;
	m_pTransCom = (Engine::CTransform*)pComponent;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Transform", m_pTransCom));

	//Mesh		≈Î∏ ..

	//pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Store");
	//m_pMeshCom2 = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	//m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Store", pComponent));


	//Mesh		∫–«“∏ ..
	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_TOWN, L"Mesh_Map_Store_BigHall");
	m_pMeshCom[0] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Store_BigHall", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_TOWN, L"Mesh_Map_Store_FrontHall");
	m_pMeshCom[1] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Store_FrontHall", pComponent));


	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_TOWN, L"Mesh_Map_Store_FrontHall_Front");
	m_pMeshCom[2] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Store_FrontHall_Front", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_TOWN, L"Mesh_Map_Store_FrontHall_Right");
	m_pMeshCom[3] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Store_FrontHall_Right", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_TOWN, L"Mesh_Map_Store_LeftHall");
	m_pMeshCom[4] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Store_LeftHall", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_TOWN, L"Mesh_Map_Store_LeftHall_Front");
	m_pMeshCom[5] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Store_LeftHall_Front", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_TOWN, L"Mesh_Map_Store_LeftHall_Right");
	m_pMeshCom[6] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Store_LeftHall_Right", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_TOWN, L"Mesh_Map_Store_Stairs");
	m_pMeshCom[7] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Store_Stairs", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_TOWN, L"Mesh_Map_Store_Stairs_front");
	m_pMeshCom[8] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Store_Stairs_front", pComponent));

	//pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Store_roof");
	//m_pMeshCom[9] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	//m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Store_roof", pComponent));

	// For.Renderer Component
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	if(NULL == pComponent)
		return E_FAIL;
	pComponent->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));

	//Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_StaticMesh"));
	if(NULL == pComponent) return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	return S_OK;
}

HRESULT CMap_Store::Set_ConstantTable( LPD3DXEFFECT pEffect )
{
	pEffect->AddRef();

	_matrix		matWorld, matView, matProj;

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	//Engine::SetUp_OnShader(L"Target_Shade", pEffect, "g_Light_Texture");

	pEffect->SetVector("g_vLightDir", &_vec4(1.0f, -1.0f, 1.0f, 0.0f));

	//const D3DLIGHT9* pLightInfo = Engine::Find_LightInfo(0, D3DLIGHT_DIRECTIONAL);

	//pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.0f));
	//pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	//pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);
	//pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfo->Specular);

	D3DMATERIAL9	MtrlInfo;
	ZeroMemory(&MtrlInfo, sizeof(D3DMATERIAL9));

	//¡ˆ«¸ ªˆ * (∫˚.Diffuse * ∏∂≈◊∏ÆæÛ.diffuse) + (∫˚.Ambient * ∏∂≈◊∏ÆæÛ.Ambient)
	MtrlInfo.Diffuse = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);
	MtrlInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	//MtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//MtrlInfo.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.f);
	//MtrlInfo.Power = 20.f;

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&MtrlInfo.Diffuse);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&MtrlInfo.Ambient);
	//pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&MtrlInfo.Specular);
	pEffect->SetFloat("g_fPower", 20.0f);

	D3DXMatrixInverse(&matView, NULL, &matView);
	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView._41);
	pEffect->SetFloat("g_fAlpha", m_fAlpha);

	float fFar = Engine::GetCurrentCamera()->GetCamFar();
	pEffect->SetFloat("g_fFar", fFar);

	pEffect->SetBool("g_bLimLight", false);
	pEffect->SetBool("g_bSpecularCheck", false);
	pEffect->SetBool("g_bBlurCheck", false);
	pEffect->SetBool("g_bOutline", m_bOutlineCheck);

	Engine::Safe_Release(pEffect);

	return S_OK;
}

HRESULT CMap_Store::Set_Shadow_ConstanTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld;

	m_pTransCom->Get_WorldMatrix(&matWorld);

	pEffect->SetMatrix("g_matWorld", &matWorld);

	_matrix		matLightView, matLightProj;

	matLightView = m_pLightCam->GetMatView();
	matLightProj = m_pLightCam->GetMatProj();

	pEffect->SetMatrix("g_matLightView", &matLightView);
	pEffect->SetMatrix("g_matLightProj", &matLightProj);

	Engine::Safe_Release(pEffect);

	return S_OK;
}

HRESULT CMap_Store::Set_Minimap_ConstanTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld;

	m_pTransCom->Get_WorldMatrix(&matWorld);

	pEffect->SetMatrix("g_matWorld", &matWorld);

	_matrix		matLightView, matLightProj;

	matLightView = Engine::GetCamera(Engine::CAMERA_MINIMAP)->GetMatView();
	matLightProj = Engine::GetCamera(Engine::CAMERA_MINIMAP)->GetMatProj();

	pEffect->SetMatrix("g_matView", &matLightView);
	pEffect->SetMatrix("g_matProj", &matLightProj);

	Engine::Safe_Release(pEffect);

	return S_OK;
}


CMap_Store* CMap_Store::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
{
	CMap_Store*		pInstance = new CMap_Store(pGraphicDev, wstrName);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CMap_Store Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CMap_Store::Free(void)
{
	Engine::CGameObject::Free();
}



