#include "stdafx.h"
#include "Select_Char_Model.h"
#include "Export_Engine.h"
#include "Light.h"
#include "UICamera.h"
#include "LightCamera.h"
#include "Sound.h"

CSelect_Char_Model::CSelect_Char_Model(LPDIRECT3DDEVICE9 pDevice, const wstring& wstrName)
: CUI(pDevice, wstrName)
, m_pTransCom(NULL)
, m_pRendererCom(NULL)
, m_pShaderCom(NULL)
, m_fFrame(0.0f)
, m_iAniIndex(1)
, m_pLightCam(NULL)
, m_pCurCam(NULL)
, m_bCanTalk(true)
{
	ZeroMemory(&m_pMeshCom, NULL);
}

CSelect_Char_Model::~CSelect_Char_Model(void)
{

}

HRESULT CSelect_Char_Model::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pMeshCom[m_iIndex]->Set_AnimationSet(m_iAniIndex);

	m_pTransCom->Set_Position(-0.f, -1.1f, 0.1f);
	m_pTransCom->Set_Scale(0.25f, 0.25f, 0.25f);
	m_pTransCom->Rotation(Engine::ROT_Y, 180.f);
	m_iIndex = -1;

	return S_OK;
}

HRESULT CSelect_Char_Model::Start_ForScene(void)
{
	m_pLightCam = Engine::GetCamera(Engine::CAMERA_LIGHT);
	m_pCurCam = Engine::GetCurrentCamera();

	return S_OK;
}

void CSelect_Char_Model::Update_Object(const _float& fTimeDelta)
{
	if(m_iIndex < 0 || m_iIndex > 4)
		return;

	Engine::CGameObject::Update_Object(fTimeDelta);

	m_pMeshCom[m_iIndex]->Play_AnimationSet(fTimeDelta);

	if(m_pMeshCom[m_iIndex]->is_AnimationSetEnd())
	{
		if(m_iAniIndex == 1)
			m_iAniIndex = 0;

		m_pMeshCom[m_iIndex]->Set_AnimationSet(m_iAniIndex);
		m_pMeshCom[m_iIndex]->Set_TrackPos(0.0);
	}
	
	//m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_DYNAMIC, this);
}

void CSelect_Char_Model::Render_Object(void)
{
	if(m_iIndex < 0 || m_iIndex > 4)
		return;

	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if(NULL == pEffect) return;

	pEffect->AddRef();

	//_uint iPass = 0;

	////그림자를 그릴 때
	//if (Engine::MRT_DEFERRED_0 == m_pRendererCom->GetMRTID())
	//{
	//	if (FAILED(Set_Shadow_ConstanTable(pEffect))) return;	
	//	iPass = 2;
	//}
	////디퓨즈, 노말, 스펙큘러, 깊이맵을 그릴 때
	//else if (Engine::MRT_DEFERRED_1 == m_pRendererCom->GetMRTID())
	//{
	//	if (FAILED(Set_ConstantTable(pEffect))) return;
	//	iPass = 0;
	//}

	_uint iPass = 0;

	if (Engine::MRT_DEFERRED_1 == m_pRendererCom->GetMRTID())	iPass = 0;	//디퓨즈, 깊이, 캐릭터 카피
	else if (Engine::MRT_DEFERRED_2 == m_pRendererCom->GetMRTID())	iPass = 1;	//노말, 스펙큘러

	if (FAILED(Set_ConstantTable(pEffect))) return;

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(iPass);

	m_pMeshCom[m_iIndex]->Render_Mesh(pEffect);

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

void CSelect_Char_Model::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
{
	if (m_iIndex < 0 || m_iIndex > 4)
		return;

	switch (iPassIdx)
	{
	case 0:
		if (FAILED(Set_ConstantTable(pEffect)))return;
		break;

	case 1:
		if (FAILED(Set_ConstantTable(pEffect)))return;
		break;

	case 2:
		if (FAILED(Set_ConstantTable(pEffect)))return;
		break;

	case 3:
		if (FAILED(Set_Shadow_ConstanTable(pEffect)))return;
		break;
	}

	m_pMeshCom[m_iIndex]->Render_Mesh(pEffect);
}

void CSelect_Char_Model::SetMotionStart()
{
	if(m_iIndex < 0 || m_iIndex > 4)
		return;
	m_iAniIndex = 1;
	m_pMeshCom[m_iIndex]->Set_AnimationSet(m_iAniIndex);
	m_pMeshCom[m_iIndex]->Set_TrackPos(0.0);
	m_pTransCom->Set_Angle(&_vec3(0.f, 180.f, 0.f));

	switch (m_iIndex)
	{
	case 0:
		m_pSoundCom->MyPlaySound(L"Fighter");
		break;
	case 1:
		m_pSoundCom->MyPlaySound(L"Hunter");
		break;
	case 2:
		m_pSoundCom->MyPlaySound(L"Shaman");
		break;
	case 3:
		m_pSoundCom->MyPlaySound(L"Witch");
		break;
	case 4:
		m_pSoundCom->MyPlaySound(L"Mistic");
		break;
	}
}

HRESULT CSelect_Char_Model::Add_Component(void)
{
	Engine::CComponent*	pComponent = NULL;

	//Transform
	pComponent = m_pTransCom	=	Engine::CTransform::Create();
	if(NULL == pComponent) return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	//Mesh
	pComponent = m_pMeshCom[0]		=	static_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Mesh_Fighter"));
	if(NULL == pComponent) return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Model1", pComponent));

	pComponent = m_pMeshCom[1]		=	static_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Mesh_Hunter"));
	if(NULL == pComponent) return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Model2", pComponent));

	pComponent = m_pMeshCom[2]		=	static_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Mesh_Shaman"));
	if(NULL == pComponent) return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Model3", pComponent));

	pComponent = m_pMeshCom[3]		=	static_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Mesh_WitchBlade"));
	if(NULL == pComponent) return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Model4", pComponent));

	pComponent = m_pMeshCom[4]		=	static_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Mesh_Mistic"));
	if(NULL == pComponent) return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Model5", pComponent));

	//Renderer
	pComponent = m_pRendererCom	=	Engine::Get_Renderer();
	if(NULL == pComponent) return E_FAIL;
	pComponent->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));

	//Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_DynamicMesh"));
	if(NULL == pComponent) return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	//Sound
	pComponent = CSound::Create(m_pTransCom);
	if (pComponent == NULL)
		return E_FAIL;
	m_pSoundCom = (CSound*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Sound", pComponent));

	m_pSoundCom->Set_Sound(L"Witch", L"SelectWitchVoice.wav");
	m_pSoundCom->Set_Sound(L"Shaman", L"SelectShamanvoice.wav");
	m_pSoundCom->Set_Sound(L"Mistic", L"SelectMisticVoice.wav");
	m_pSoundCom->Set_Sound(L"Hunter", L"SelectHunterVoice.wav");
	m_pSoundCom->Set_Sound(L"Fighter", L"SelectFighterVoice.wav");
	

	return S_OK;
}


HRESULT CSelect_Char_Model::Set_ConstantTable(LPD3DXEFFECT pEffect)
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

	D3DMATERIAL9	MtrlInfo;
	ZeroMemory(&MtrlInfo, sizeof(D3DMATERIAL9));

	////지형 색 * (빛.Diffuse * 마테리얼.diffuse) + (빛.Ambient * 마테리얼.Ambient)
	MtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	MtrlInfo.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f);
	//MtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//MtrlInfo.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.f);
	//MtrlInfo.Power = 20.f;

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&MtrlInfo.Diffuse);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&MtrlInfo.Ambient);
	//pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&MtrlInfo.Specular);
	pEffect->SetFloat("g_fPower", 70.0f);

	D3DXMatrixInverse(&matView, NULL, &matView);
	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView._41);

	float fFar = Engine::GetCurrentCamera()->GetCamFar();
	pEffect->SetFloat("g_fFar", fFar);

	_vec3 vCamDir = Engine::GetCurrentCamera()->GetEyePosition() - m_pTransCom->Get_Now_Position();
	D3DXVec3Normalize(&vCamDir, &vCamDir);
	pEffect->SetVector("g_vCamDir", &_vec4(vCamDir, 0.0f));

	pEffect->SetBool("g_bOutline", false);

	pEffect->SetBool("g_bLimLight", true);
	pEffect->SetFloat("g_bOutlinePower", 10.0f);

	if (Engine::SCENE_SELECT == Engine::GetSceneID())
		pEffect->SetVector("g_vColor", &_vec4(1.0f, 1.0f, 1.0f, 0.0f));
	else if (Engine::SCENE_TOWN == Engine::GetSceneID())
		pEffect->SetVector("g_vColor", &_vec4(0.9607f, 0.8705f, 0.7019f, 0.0f));
	else if (Engine::SCENE_STAGE_ICE == Engine::GetSceneID())
		pEffect->SetVector("g_vColor", &_vec4(0.5294f, 0.8078f, 0.9215f, 0.0f));
	else if (Engine::SCENE_STAGE_FIRE == Engine::GetSceneID())
		pEffect->SetVector("g_vColor", &_vec4(1.0f, 0.6274f, 0.4784f, 0.0f));

	Engine::Safe_Release(pEffect);

	return S_OK;
}

HRESULT CSelect_Char_Model::Set_Shadow_ConstanTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld;
	/*D3DXMatrixIdentity(&matWorld);

	matWorld = m_pTransCom->Get_Now_ScaleRotMatrix();

	matWorld._41 = m_pTransCom->Get_Now_Position().x;
	matWorld._42 = m_pTransCom->Get_Now_Position().y;
	matWorld._43 = m_pTransCom->Get_Now_Position().z;*/

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

CSelect_Char_Model* CSelect_Char_Model::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
{
	CSelect_Char_Model*	pPlayer = new CSelect_Char_Model(pGraphicDev, wstrName);

	if(FAILED(pPlayer->Ready_Object()))
	{
		MSG_BOX("CPlayer Created Failed");
		::Safe_Release(pPlayer);
	}

	return pPlayer;
}

void CSelect_Char_Model::Free(void)
{
	Engine::CGameObject::Free();
}

