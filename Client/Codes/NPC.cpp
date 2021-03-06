#include "stdafx.h"
#include "NPC.h"
#include "LightCamera.h"
#include "Export_Engine.h"

#include "Sound.h"


//Ŭ?? NPC...

CNPC::CNPC(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
: Engine::CGameObject(pGraphicDev, wstrName)
, m_pTransCom(NULL)
, m_pMeshCom(NULL)
, m_pRendererCom(NULL)
//, m_pMouseCol(NULL)
, m_pSoundCom(NULL)
, m_pUIEff(NULL)
, m_lMouseMove(0)
, m_fFrame(0.f)
, m_fAlpha(1.0f)
{
	//ZeroMemory(&m_tInfo, sizeof(OBJ_INFO));
}
CNPC::~CNPC(void)
{

}

void CNPC::PlayNPCSound(void)
{
	_uint iRand = rand() % 3;

	switch (iRand)
	{
	case 0:
		m_pSoundCom->MyPlaySound(L"Sound1");
		break;

	case 1:
		m_pSoundCom->MyPlaySound(L"Sound2");
		break;

	case 2:
		m_pSoundCom->MyPlaySound(L"Sound3");
		break;
	}
}

void CNPC::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	m_pMeshCom->Play_AnimationSet(fTimeDelta);

	if (m_pRendererCom->GetShadowBuffer()	||
		m_pRendererCom->GetSoftShadow()		||
		m_pRendererCom->GetCACDShadow())
	{
		m_pRendererCom->Add_RenderGroup(Engine::RENEDER_DYANAMIC_SHADOW, this);
	}

	//m_pRendererCom->Add_RenderGroup(Engine::RENEDER_SHADOW, this);
	//m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_DYNAMIC, this);

	_vec3	vScale;
	Compute_ViewZ(m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &vScale));

}

void CNPC::Render_Object(void)
{
	/*LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if(NULL == pEffect) return;

	pEffect->AddRef();
	_uint iPass = 0;

	switch (Engine::GetMRTID())
	{
	case Engine::MRT_DEFERRED_0:
		iPass = 3;
		if (FAILED(Set_Shadow_ConstanTable(pEffect)))return;
		break;

	case Engine::MRT_DEFERRED_1:
		iPass = 0;
		if (FAILED(Set_ConstantTable(pEffect)))return;
		break;

	case Engine::MRT_DEFERRED_2:
		iPass = 1;
		if (FAILED(Set_ConstantTable(pEffect)))return;
		break;

	case Engine::MRT_DEFERRED_4:
		return;

	case Engine::MRT_MOTION_BLUR:
		iPass = 2;
		if (FAILED(Set_ConstantTable(pEffect)))return;
		break;
	}

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(iPass);

	m_pMeshCom->Render_Mesh(pEffect);

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);*/
}

void CNPC::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
{
	switch (iPassIdx)
	{
	case 0:
		if (FAILED(Set_ConstantTable(pEffect)))return;
		break;

	case 1:
		if (FAILED(Set_ConstantTable(pEffect)))return;
		break;

	case 2:
		return;

	case 3:
		if (FAILED(Set_Shadow_ConstanTable(pEffect)))return;
		break;

	case 4:
		if (FAILED(Set_CACD_Shadow_ConstanTable(pEffect)))return;
		break;

	/*case 5:
		if (m_bOutlineDraw)
		{
			if (FAILED(Set_ConstantTable(pEffect)))return;
		}
		else
		{
			return;
		}
		break;*/
	}

	m_pMeshCom->Render_Mesh(pEffect);
}

HRESULT CNPC::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pMeshCom->Set_AnimationSet(0);

	m_pTransCom->Set_Position(0.0f, 0.0f, 0.0f);

	//m_pMouseCol = CMouseCol::Create();

	return S_OK;
}

HRESULT CNPC::Add_Component(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = Engine::CTransform::Create();
	if(pComponent == NULL)
		return E_FAIL;
	m_pTransCom = (Engine::CTransform*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Transform", m_pTransCom));

	//Mesh

	TCHAR szName[MAX_PATH] = L"";

	wstring::iterator iter = m_wstrName.begin();
	wstring::iterator iter_end = m_wstrName.end();

	int i = 0;

	for(; iter != iter_end; ++iter)
	{
		szName[i] = (*iter);
		++i;
	}

	pComponent = (Engine::CDynamicMesh*)Engine::Clone_Resource(RESOURCE_MESH_TOWN, szName);
	m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"DynamicMesh", m_pMeshCom));

	// For.Renderer Component
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	if(NULL == pComponent)
		return E_FAIL;
	pComponent->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));

	//Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_DynamicMesh"));
	if(NULL == pComponent) return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	return S_OK;
}

HRESULT CNPC::Set_ConstantTable( LPD3DXEFFECT pEffect )
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

	//const D3DLIGHT9* pLightInfo = Engine::Find_LightInfo(0, D3DLIGHT_DIRECTIONAL);

	//pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.0f));
	//pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	//pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);
	//pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfo->Specular);

	D3DMATERIAL9	MtrlInfo;
	ZeroMemory(&MtrlInfo, sizeof(D3DMATERIAL9));

	//???? ?? * (??.Diffuse * ???׸???.diffuse) + (??.Ambient * ???׸???.Ambient)
	MtrlInfo.Diffuse = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);
	MtrlInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	//MtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//MtrlInfo.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.f);
	//MtrlInfo.Power = 20.f;

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&MtrlInfo.Diffuse);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&MtrlInfo.Ambient);
	//pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&MtrlInfo.Specular);
	pEffect->SetFloat("g_fPower", 40.0f);

	D3DXMatrixInverse(&matView, NULL, &matView);
	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView._41);
	pEffect->SetFloat("g_fAlpha", m_fAlpha);

	float fFar = Engine::GetCurrentCamera()->GetCamFar();
	pEffect->SetFloat("g_fFar", fFar);

	pEffect->SetBool("g_bBlurCheck", false);
	
	pEffect->SetBool("g_bOutline", m_bOutlineCheck);
	pEffect->SetFloat("g_fColorCheck", 0.25f);
	pEffect->SetFloat("g_fOutlineThickness", 0.0005f);

	/*if (m_bOutlineCheck)
	{
		pEffect->SetVector("g_vColor", &_vec4(m_bOutlineColor, 0.0f));
		pEffect->SetFloat("g_bOutlinePower", 2.5f);
	}*/

	pEffect->SetBool("g_bLimLight", true);

	if (Engine::GetLimLight())
	{
		pEffect->SetFloat("g_bOutlinePower", 18.0f);

		pEffect->SetVector("g_vColor", &_vec4(m_bOutlineColor, 0.0f));
	}

	Engine::Safe_Release(pEffect);

	return S_OK;
}

HRESULT CNPC::Set_Shadow_ConstanTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld;

	m_pTransCom->Get_WorldMatrix(&matWorld);

	pEffect->SetMatrix("g_matWorld", &matWorld);

	_matrix		matLightView, matLightProj;

	matLightView = Engine::GetCamera(Engine::CAMERA_LIGHT)->GetMatView();
	matLightProj = Engine::GetCamera(Engine::CAMERA_LIGHT)->GetMatProj();

	pEffect->SetMatrix("g_matLightView", &matLightView);
	pEffect->SetMatrix("g_matLightProj", &matLightProj);

	Engine::Safe_Release(pEffect);

	return S_OK;
}

HRESULT CNPC::Set_CACD_Shadow_ConstanTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld;

	m_pTransCom->Get_WorldMatrix(&matWorld);

	pEffect->SetMatrix("g_matWorld", &matWorld);

	CLightCamera* pLightCam = (CLightCamera*)Engine::GetCamera(Engine::CAMERA_LIGHT);

	pEffect->SetMatrix("g_matLightView", &pLightCam->GetMatView());

	if (0 == m_pRendererCom->GetCACDCount())
		pEffect->SetMatrix("g_matLightProj", &pLightCam->Get_CACD_Proj(0));
	else if (1 == m_pRendererCom->GetCACDCount())
		pEffect->SetMatrix("g_matLightProj", &pLightCam->Get_CACD_Proj(1));
	else if (2 == m_pRendererCom->GetCACDCount())
		pEffect->SetMatrix("g_matLightProj", &pLightCam->Get_CACD_Proj(2));

	Engine::Safe_Release(pEffect);

	return S_OK;
}

CNPC* CNPC::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
{
	CNPC*		pInstance = new CNPC(pGraphicDev, wstrName);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CGrid Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CNPC::Free(void)
{
	Engine::CGameObject::Free();
//	Safe_Delete(m_pMouseCol);

}


Engine::CTransform* CNPC::GetTransCom( void )
{
	return m_pTransCom;
}


//OBJ_INFO* CNPC::GetObjectInfo( void )
//{
//	//m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &m_tInfo.m_vPos);
//	//m_pTransCom->Get_Angle(&m_tInfo.m_vAngle);
//	//m_pTransCom->Get_Scale(&m_tInfo.m_vScale);
//
//
//	//wstring::iterator iter = m_wstrName.begin();
//	//wstring::iterator iter_end = m_wstrName.end();
//
//	//int i = 0;
//
//	//for(; iter != iter_end; ++iter)
//	//{
//	//	m_tInfo.m_szName[i] = (*iter);
//	//	++i;
//	//}
//
//	////	m_tInfo.m_eID = m_eID;
//
//
//	return &m_tInfo;
//}

void CNPC::SetObjectInfo( _vec3* vPos, _vec3* vScale, _vec3* vAngle )
{
	m_pTransCom->Set_Position(vPos);
	m_pTransCom->Set_Scale(vScale->x, vScale->y, vScale->z);
	m_pTransCom->Rotation(Engine::ROT_Y, vAngle->y);
//	m_eMode = MODE_STOP;
}


