#include "stdafx.h"
#include "Mistic.h"
#include "Mistic_Town.h"
#include "Mistic_Dungeon.h"
#include "Mistic_Fight.h"
#include "PlayerCamera.h"
#include "Light.h"
#include "Map_Store.h"
#include "Map_Ice.h"
#include "ScreenSpaceDecal.h"
#include "Layer.h"
#include "MisticWeapon.h"
#include "ObjectName.h"
#include "MisticBullet.h"


CMistic::CMistic(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
	: CPlayer(pGraphicDev, wstrName)
{
	
}

CMistic::~CMistic(void)
{

}


void CMistic::Set_ActionEnd(void)
{
}

const _bool* CMistic::Get_IsSkill(const _uint& iState) const
{
	const CMistic_State* pState = NULL;
	pState = (const CMistic_State*)m_pAnimatorCom->Get_State(iState);
	if (pState == NULL)
		return NULL;

	return pState->Get_CurAction();
}

_bool CMistic::Get_CameraMove(void)
{
	return m_pPlayerCamera->Get_Move();
}

vector<CMisticBullet*>* CMistic::Get_Bullet(void)
{
	return &m_vecBullet;
}

void CMistic::Set_Skill(const SKILL_ID eID)
{
	if (eID > SKILL_END)
		return;

	if (m_pAnimatorCom->Get_State() != PLAYER_FIGHT)
		return;

	CMistic_Fight* pState = (CMistic_Fight*)m_pAnimatorCom->Get_CurState();
	pState->Set_Skill(eID);
}

void CMistic::Compute_Camera(void)
{
	const _vec3* pCamPos = m_pPlayerCamera->Get_Pos();
	Compute_CameraLook(pCamPos);
	m_pPlayerCamera->Set_Move();
}

void CMistic::Shot_Bullet(const _vec3 & vPos, const _vec3 & vDir, const Engine::COLINFO & tColInfo)
{
	_uint iSize = m_vecBullet.size();

	for (_uint i = 0; i < iSize; i++)
	{
		if (!m_vecBullet[i]->GetActive())
		{
			m_vecBullet[i]->Shot_Bullet(vPos, vDir, tColInfo);
			return;
		}
	}
}

HRESULT CMistic::Start_ForScene(void)
{
	CPlayer::Start_ForScene();

	if (m_bOnceStart)
	{
		


		Engine::CGameObject* pGameObject = Engine::FindWithTag(L"Map");
		if (pGameObject == NULL)
			return E_FAIL;

		//Bullet
		_uint iSize = m_vecBullet.size();
		for (_uint i = 0; i < iSize; i++)
		{
			if (dynamic_cast<CMap_Store*>(pGameObject) != NULL)
			{
				_int iSize = 0;
				Engine::CStaticMesh** ppMesh = ((CMap_Store*)pGameObject)->GetMapMesh(&iSize);

				for (_int j = 0; j < iSize; j++)
				{
					m_vecBullet[i]->Set_MapMesh(ppMesh[j]->Get_Mesh());
				}
			}

			if (dynamic_cast<CMap_Ice*>(pGameObject) != NULL)
			{
				_int iSize = 0;
				Engine::CStaticMesh** ppMesh = ((CMap_Ice*)pGameObject)->GetMapMesh(&iSize);

				for (_int j = 0; j < iSize; j++)
				{
					m_vecBullet[i]->Set_MapMesh(ppMesh[j]->Get_Mesh());
				}
			}
		}

		Engine::Safe_Release(pGameObject);

		return S_OK;
	}

	m_bOnceStart = true;

	// NameUI



	CMisticWeapon* pWeapon = CMisticWeapon::Create(m_pGraphicDev, L"MisticWeapon", m_wstrName.c_str(), L"MisticWeapon1");
	if (pWeapon == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_GAMELOGIC, L"Weapon", pWeapon);

	CMisticWeapon* pWeapon_In = CMisticWeapon::Create(m_pGraphicDev, L"MisticWeapon", m_wstrName.c_str(), L"MisticWeapon1_In");
	if (pWeapon_In == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_GAMELOGIC, L"Weapon", pWeapon_In);


	//애니메이션 상태 준비
	Engine::CAnimationState* pState = NULL;

	pState = CMistic_Town::Create(m_pMeshCom, &m_pmapNextMotion[PLAYER_TOWN], this, MISTIC_IDLE, pWeapon_In);
	if (pState == NULL)
		return E_FAIL;
	m_pAnimatorCom->Set_State(PLAYER_TOWN, pState);

	pState = CMistic_Dungeon::Create(m_pMeshCom, &m_pmapNextMotion[PLAYER_DUNGEON], this, MISTIC_IDLE, pWeapon_In);
	if (pState == NULL)
		return E_FAIL;

	m_pAnimatorCom->Set_State(PLAYER_DUNGEON, pState);

	pState = CMistic_Fight::Create(m_pMeshCom, &m_pmapNextMotion[PLAYER_FIGHT], this, MISTIC_BATTLEIDLE, pWeapon);
	if (pState == NULL)
		return E_FAIL;

	m_pAnimatorCom->Set_State(PLAYER_FIGHT, pState);

	//애니메이터 세팅
	m_pAnimatorCom->Set_Animator(&m_vecAniInfo);
	m_pAnimatorCom->Set_State(m_iPlayerAniState);
	if (m_iPlayerAniState == PLAYER_FIGHT)
		m_pAnimatorCom->Set_AnimationSet(MISTIC_BATTLEIDLE);
	else
		m_pAnimatorCom->Set_AnimationSet(MISTIC_IDLE);

	//Light Cam
	m_pLightCam = Engine::GetCamera(Engine::CAMERA_LIGHT);
	m_pPlayerCamera = (CPlayerCamera*)Engine::GetCamera(Engine::CAMERA_PLAYER);


	Engine::CGameObject* pGameObject = Engine::FindWithTag(L"Map");
	if (pGameObject == NULL)
		return E_FAIL;

	//Bullet
	for (_int i = 0; i < 10; i++)
	{
		CMisticBullet* pBullet = CMisticBullet::Create(m_pGraphicDev, L"Bullet");
		Engine::AddObject(Engine::LAYER_GAMELOGIC, L"Weapon", pBullet);
		m_vecBullet.push_back(pBullet);

		if (dynamic_cast<CMap_Store*>(pGameObject) != NULL)
		{
			_int iSize = 0;
			Engine::CStaticMesh** ppMesh = ((CMap_Store*)pGameObject)->GetMapMesh(&iSize);

			for (_int j = 0; j < iSize; j++)
			{
				pBullet->Set_MapMesh(ppMesh[j]->Get_Mesh());
			}
		}

		if (dynamic_cast<CMap_Ice*>(pGameObject) != NULL)
		{
			_int iSize = 0;
			Engine::CStaticMesh** ppMesh = ((CMap_Ice*)pGameObject)->GetMapMesh(&iSize);

			for (_int j = 0; j < iSize; j++)
			{
				pBullet->Set_MapMesh(ppMesh[j]->Get_Mesh());
			}
		}
	}

	Engine::Safe_Release(pGameObject);

	return S_OK;
}

void CMistic::Update_Object(const _float& fTimeDelta)
{
	CPlayer::Update_Object(fTimeDelta);
	m_pMeshCom->Play_AnimationSet(fTimeDelta);

	if (m_pCalculatorCom->Get_IsCol()) //지형충돌
		m_pRigidbodyCom->Set_Gravity(false); //중력을 false
	else
		m_pRigidbodyCom->Set_Gravity(true); //중력을 true

											//m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_DYNAMIC, this);

	//Motion Blur============================================================
	if (m_pRendererCom->GetMotionBlurBuffer())
	{
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_MOTION_BLUR, this);
		m_pMeshCom->UpdateTimeDelta(fTimeDelta);
	}
	//=======================================================================

	//Shadow=================================================================
	if (m_pRendererCom->GetShadowBuffer() ||
		m_pRendererCom->GetSoftShadow())
	{
		m_pRendererCom->Add_RenderGroup(Engine::RENEDER_DYANAMIC_SHADOW, this);

		_vec3 EyePos = m_pTransCom->Get_Now_Position() + _vec3(-2.5f, 50.5f, -2.5f);
		//_vec3 EyePos = _vec3(1.0f, 85.0f, 1.0f);
		//EyePos.y = 50.5f;

		m_pLightCam->SetEyePosition(EyePos);
		m_pLightCam->SetAtPosition(m_pPlayerCamera->GetAtPosition());

		m_pLightCam->MakeViewMatrix();
		m_pLightCam->MakeProjOrthoLHMatrix(_float(WINSIZEX / 10.0f), _float(WINSIZEY / 10.0f));
	}
	else if (m_pRendererCom->GetCACDShadow())
	{
		m_pRendererCom->Add_RenderGroup(Engine::RENEDER_DYANAMIC_SHADOW, this);

		_vec3 EyePos = m_pTransCom->Get_Now_Position() + _vec3(-2.5f, 50.5f, -2.5f);

		m_pLightCam->SetEyePosition(EyePos);
		m_pLightCam->SetAtPosition(m_pPlayerCamera->GetAtPosition());
		m_pLightCam->MakeViewMatrix();

		_float fNear = m_pLightCam->GetNear();
		_float fFar = m_pLightCam->GetCamFar();
		_matrix matProj;

		D3DXMatrixOrthoLH(&matProj, _float(WINSIZEX / 80.0f), _float(WINSIZEY / 80.0f), fNear, fFar);
		m_pLightCam->Set_CACD_Proj(matProj, 0);
		D3DXMatrixOrthoLH(&matProj, _float(WINSIZEX / 30.0f), _float(WINSIZEY / 30.0f), fNear, fFar);
		m_pLightCam->Set_CACD_Proj(matProj, 1);
		D3DXMatrixOrthoLH(&matProj, _float(WINSIZEX / 10.0f), _float(WINSIZEY / 10.0f), fNear, fFar);
		m_pLightCam->Set_CACD_Proj(matProj, 2);
	}
	//=======================================================================

}

void CMistic::Render_Object(void)
{
	if (m_pColliderCom)
		m_pColliderCom->Render_Collider(g_bDebugRender);
	if (m_pSphereColCom)
		m_pSphereColCom->Render_Collider(g_bDebugRender);
}

void CMistic::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
{
	switch (iPassIdx)
	{
	case 0:
		if (FAILED(SetUp_ConstantTable(pEffect)))return;
		break;

	case 1:
		if (FAILED(SetUp_ConstantTable(pEffect)))return;
		break;

	case 2:
		if (FAILED(SetUp_ConstantTable(pEffect)))return;
		break;

	case 3:
		if (FAILED(Set_Shadow_ConstanTable(pEffect)))return;
		break;

	case 4:
		if (FAILED(Set_CACD_Shadow_ConstanTable(pEffect)))return;
		break;

	/*case 5:
		if (m_bOutlineDraw)
		{
			if (FAILED(SetUp_ConstantTable(pEffect)))return;
		}
		else
		{
			return;
		}
		break;*/
	}

	if (2 == iPassIdx) //모션블러 일 때.
		m_pMeshCom->Render_BlurMesh(pEffect);
	else
		m_pMeshCom->Render_Mesh(pEffect);
}

HRESULT CMistic::Ready_Object(SCENE_TYPE eSceneType)
{
	if (FAILED(Add_Component()))
		return E_FAIL;
	if (FAILED(LoadAniInfo(L"MisticAni")))
		return E_FAIL;
	if (FAILED(LoadCollider(L"MisticCollider")))
		return E_FAIL;

	m_eSceneType = eSceneType;
	if (m_eSceneType == SCENE_TOWN)
		m_iPlayerAniState = PLAYER_TOWN;
	if (m_eSceneType == SCENE_DUNGEON)
		m_iPlayerAniState = PLAYER_FIGHT;

	return S_OK;
}

HRESULT CMistic::Add_Component(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;
	m_pTransCom = (Engine::CTransform*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"0", pComponent));

	//Mesh
	pComponent = Engine::Clone_Resource(RESOURCE_MESH, L"Mesh_Mistic");
	if (pComponent == NULL)
		return E_FAIL;
	m_pMeshCom = (Engine::CDynamicMesh*)pComponent;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh", pComponent));

	//Renderer
	pComponent = Engine::Get_Renderer();
	if (pComponent == NULL)
		return E_FAIL;
	m_pRendererCom = (Engine::CRenderer*)pComponent;
	m_pRendererCom->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Renderer", pComponent));

	//Shader
	pComponent = Engine::Clone_Prototype(L"Shader_DynamicMesh");
	if (pComponent == NULL)
		return E_FAIL;
	m_pShaderCom = (Engine::CShader*)pComponent;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Shader", pComponent));

	//Animator
	pComponent = Engine::CAnimator::Create();
	if (pComponent == NULL)
		return E_FAIL;
	m_pAnimatorCom = (Engine::CAnimator*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"3", pComponent));

	//Calculator
	pComponent = Engine::CCalculator::Create(m_pGraphicDev);
	if (pComponent == NULL)
		return E_FAIL;
	m_pCalculatorCom = (Engine::CCalculator*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"2", pComponent));
	m_pCalculatorCom->Set_Transform(m_pTransCom);

	//Rigidbody
	pComponent = Engine::CRigidbody::Create(m_pTransCom, m_pCalculatorCom);
	if (pComponent == NULL)
		return E_FAIL;
	m_pRigidbodyCom = (Engine::CRigidbody*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"1", pComponent));
	m_pRigidbodyCom->Set_Gravity(false);

	//SphereCollider
	pComponent = Engine::Create_SphereCollider(m_pGraphicDev, m_pTransCom, L"Player", _vec3(0.f, 0.f, 0.f), 10.f);
	if (pComponent == NULL)
		return E_FAIL;
	m_pSphereColCom = (Engine::CSphereCollider*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"SphereCol", pComponent));

	return S_OK;
}

HRESULT CMistic::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld, matView, matProj;

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	//========================
	//For.MotionBlur
	//이전에 저장했던 것을 쉐이더로 전달
	if (Engine::MRT_MOTION_BLUR == m_pRendererCom->GetMRTID())
	{
		//if(Engine::GetHDR())
		//	Engine::SetUp_OnShader(L"Target_Char_Sampler_Shade_HDR", pEffect, "g_BaseTexture");
		//else
		Engine::SetUp_OnShader(L"Target_Char_Sampler_Shade", pEffect, "g_BaseTexture");

		pEffect->SetMatrix("g_matOldWorld", &m_matOldWorld);
		pEffect->SetMatrix("g_matOldView", &m_matOldView);
		pEffect->SetMatrix("g_matOldProj", &m_matOldProj);

		float fTime = m_pMeshCom->GetTimeDelta();

		if (0.07f <= fTime)
		{
			//다음을 위해 지금의 것을 저장한다.
			m_matOldWorld = matWorld;
			m_matOldView = matView;
			m_matOldProj = matProj;
		}
	}
	//========================

	//Engine::SetUp_OnShader(L"Target_Shade", pEffect, "g_Light_Texture");

	pEffect->SetVector("g_vLightDir", &_vec4(1.0f, -1.0f, 1.0f, 0.0f));

	//const D3DLIGHT9* pLightInfo = Engine::Find_LightInfo(0, D3DLIGHT_DIRECTIONAL);

	//pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.0f));
	//pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	//pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);
	//pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfo->Specular);

	D3DMATERIAL9	MtrlInfo;
	ZeroMemory(&MtrlInfo, sizeof(D3DMATERIAL9));

	////지형 색 * (빛.Diffuse * 마테리얼.diffuse) + (빛.Ambient * 마테리얼.Ambient)
	MtrlInfo.Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.f);
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
	pEffect->SetFloat("g_fAlpha", m_fAlpha);

	float fFar = Engine::GetCurrentCamera()->GetCamFar();
	pEffect->SetFloat("g_fFar", fFar);

	pEffect->SetBool("g_bBlurCheck", Engine::GetMotionBlurBuffer());
	
	pEffect->SetBool("g_bOutline", m_bOutlineCheck);
	pEffect->SetFloat("g_fColorCheck", 0.0f);
	pEffect->SetFloat("g_fOutlineThickness", 0.00045f);

	/*if (m_bOutlineCheck)
	{
		pEffect->SetVector("g_vColor", &_vec4(m_bOutlineColor, 0.0f));
		pEffect->SetFloat("g_bOutlinePower", 2.5f);
	}*/

	pEffect->SetBool("g_bLimLight", true);

	if (Engine::GetLimLight())
	{
		pEffect->SetFloat("g_bOutlinePower", 18.0f);

		if (Engine::SCENE_SELECT == Engine::GetSceneID())
			pEffect->SetVector("g_vColor", &_vec4(1.0f, 1.0f, 1.0f, 0.0f));
		else if (Engine::SCENE_TOWN == Engine::GetSceneID())
			pEffect->SetVector("g_vColor", &_vec4(1.0f, 0.8549f, 0.7254f, 0.0f));
		else if (Engine::SCENE_STAGE_ICE == Engine::GetSceneID())
			pEffect->SetVector("g_vColor", &_vec4(0.5294f, 0.8078f, 0.9215f, 0.0f));
		else if (Engine::SCENE_STAGE_FIRE == Engine::GetSceneID())
			pEffect->SetVector("g_vColor", &_vec4(1.0f, 0.6274f, 0.4784f, 0.0f));
	}

	Engine::Safe_Release(pEffect);

	return S_OK;
}

HRESULT CMistic::Set_Shadow_ConstanTable(LPD3DXEFFECT pEffect)
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

HRESULT CMistic::Set_CACD_Shadow_ConstanTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld;

	m_pTransCom->Get_WorldMatrix(&matWorld);

	pEffect->SetMatrix("g_matWorld", &matWorld);

	pEffect->SetMatrix("g_matLightView", &m_pLightCam->GetMatView());

	if (0 == m_pRendererCom->GetCACDCount())
		pEffect->SetMatrix("g_matLightProj", &m_pLightCam->Get_CACD_Proj(0));
	else if (1 == m_pRendererCom->GetCACDCount())
		pEffect->SetMatrix("g_matLightProj", &m_pLightCam->Get_CACD_Proj(1));
	else if (2 == m_pRendererCom->GetCACDCount())
		pEffect->SetMatrix("g_matLightProj", &m_pLightCam->Get_CACD_Proj(2));

	Engine::Safe_Release(pEffect);

	return S_OK;
}

CMistic* CMistic::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, SCENE_TYPE eSceneType)
{
	CMistic*		pInstance = new CMistic(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object(eSceneType)))
	{
		MSG_BOX("CMistic Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CMistic::Free(void)
{
	CPlayer::Free();
}