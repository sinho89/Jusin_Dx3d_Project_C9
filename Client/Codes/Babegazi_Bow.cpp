#include "stdafx.h"

#include "Monster.h"
#include "Babegazi_Bow.h"

#include "Export_Engine.h"
#include "VIBuffer.h"

#include "Map_Ice.h"

//#include "Camera.h"
#include "LightCamera.h"
#include "Environment.h"
#include "Player.h"
#include "Cube.h"
#include "NavigationMesh.h"
#include "Babegazi_Bow_Ani.h"

#include "AI.h"
#include "AI_Roaming_BBow.h"
#include "AI_Trace_BBow.h"
#include "AI_Discovery_BBow.h"
#include "AI_Combat_BBow.h"
#include "AI_Attack_BBow.h"
#include "AI_Down_BBow.h"
#include "AI_Idle_BBow.h"
#include "AI_Death_BBow.h"

#include "Sound.h"

#include "Sprite.h"
#include "World_HP_Bar.h"
#include "World_HP_Gage.h"
#include "Arrow.h"
#include "Thrown_Arrow.h"
#include "Monster_Name.h"

CBabegazi_Bow::CBabegazi_Bow(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
	: CMonster(pGraphicDev, wstrName)
	,m_pDust(NULL)
	,m_pArrow(NULL)
	,m_bArrowCreate(false)
	,m_bShot(false)
	,m_bAttack(false)
	,m_fAttackTime(0.f)
	,m_bTest(false)
{
	m_eMonsterID = MON_B_Bow;
}
CBabegazi_Bow::~CBabegazi_Bow(void)
{

}

HRESULT CBabegazi_Bow::Start_ForScene(void)
{
	CMonster::Start_ForScene();

	m_pSoundCom->Set_Sound(L"Discovery", L"Babe_Bow_Discovery.wav");
	m_pSoundCom->Set_Sound(L"Attack_1", L"Babe_Bow_Attack_1.wav");

	m_pSoundCom->Set_Sound(L"Dam_1", L"Babe_Bow_Dam_1.wav");
	m_pSoundCom->Set_Sound(L"Dam_2", L"Babe_Bow_Dam_2.wav");
	m_pSoundCom->Set_Sound(L"Dam_3", L"Babe_Bow_Dam_3.wav");

	m_pSoundCom->Set_Sound(L"Death", L"Babe_Bow_Death.wav");

	m_pSoundCom->Set_Sound(L"Floor_Hit", L"Floor_Hit.wav");
	// WorldUI
	Engine::CGameObject* pGameObject = CMonster_Name::Create(m_pGraphicDev, L"Monster_Name", this, MON_B_Bow);
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"Name_UI", pGameObject);

	pGameObject = CWorld_HP_Gage::Create(m_pGraphicDev, L"HP_Gage", this, MON_B_Bow);
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"Name_UI", pGameObject);

	pGameObject = CWorld_HP_Bar::Create(m_pGraphicDev, L"HP_Bar", this, MON_B_Bow);
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"Name_UI", pGameObject);

	m_pLightCam = Engine::GetCamera(Engine::CAMERA_LIGHT);
	m_pCurCam = Engine::GetCurrentCamera();

	//AI 세팅..

	Engine::CAI_State* pAI_State = NULL;
	pAI_State = CAI_Roaming_BBow::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pAICom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_ROAMING), pAI_State);

	pAI_State = CAI_Discovery_BBow::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pCalculatorCom, m_pRigidbodyCom,
		m_pColliderCom, &m_tColInfo, &m_bDamaged, m_pAICom, m_pSoundCom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_DISCOVERY), pAI_State);

	pAI_State = CAI_Trace_BBow::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pCalculatorCom, m_pRigidbodyCom,
		m_pColliderCom, &m_tColInfo, &m_bDamaged, m_pAICom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_TRACE), pAI_State);

	pAI_State = CAI_Combat_BBow::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pCalculatorCom, m_pRigidbodyCom,
		m_pColliderCom, &m_tColInfo, &m_bDamaged, m_pAICom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_COMBAT), pAI_State);

	pAI_State = CAI_Attack_BBow::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pCalculatorCom, m_pRigidbodyCom,
		m_pColliderCom, &m_tColInfo, &m_bDamaged, m_pAICom, m_pSoundCom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	pAI_State->Set_SphereCol(m_pSphereColCom);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_ATTACK), pAI_State);

	pAI_State = CAI_Down_BBow::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pCalculatorCom, m_pRigidbodyCom,
		m_pColliderCom, &m_tColInfo, &m_bDamaged, m_pAICom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_DOWN), pAI_State);

	pAI_State = CAI_Idle_BBow::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pCalculatorCom, m_pRigidbodyCom,
		m_pColliderCom, &m_tColInfo, &m_bDamaged, m_pAICom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_IDLE), pAI_State);

	pAI_State = CAI_Death_BBow::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pCalculatorCom, m_pRigidbodyCom,
		m_pColliderCom, &m_tColInfo, &m_bDamaged, m_pAICom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_DEATH), pAI_State);

	m_eAIState = AI_STATE::AI_ROAMING;
	m_pAICom->Set_State(m_eAIState);

	m_pAICom->Set_StartState();

	////Arrow..
	//CArrow* pWeaponR = CArrow::Create(m_pGraphicDev, L"Arrow", m_wstrName.c_str(), L"Arrow");
	//if (pWeaponR == NULL)
	//	return E_FAIL;
	//Engine::AddObject(Engine::LAYER_GAMELOGIC, L"Weapon", pWeaponR);

	return S_OK;
}

void CBabegazi_Bow::Update_Object(const _float& fTimeDelta)
{
	/*if (!m_bArrowCreate)
	{
//		m_pArrow = CArrow::Create(m_pGraphicDev, L"Arrow", m_wstrName.c_str(), L"Arrow");
	//	Engine::AddObject(Engine::LAYER_GAMELOGIC, L"Weapon", m_pArrow);
		m_bArrowCreate = true;
	}*/

	if (m_pAnimatorCom->Get_AnimationIndex() == ATTACK_A)
	{
		if (m_pAnimatorCom->IsPlaying_ThisSection((m_vecAniInfo)[ATTACK_A]->tMotionInfo.dStartTime + (m_vecAniInfo)[ATTACK_A]->tMotionInfo.dEndTime * 0.4,
			(m_vecAniInfo)[ATTACK_A]->tMotionInfo.dStartTime + (m_vecAniInfo)[ATTACK_A]->tMotionInfo.dEndTime * 0.7))
		{
			//m_pArrow->SetRender(true);
		}
	}



	if (m_pAnimatorCom->Is_AnimationEnd(ATTACK_B, 0.1))
	{
		//m_pArrow->SetRender(false);
		if (m_pAnimatorCom->IsPlaying_ThisSection((m_vecAniInfo)[ATTACK_B]->tMotionInfo.dStartTime + (m_vecAniInfo)[ATTACK_B]->tMotionInfo.dEndTime * 0.9,
			(m_vecAniInfo)[ATTACK_B]->tMotionInfo.dStartTime + (m_vecAniInfo)[ATTACK_B]->tMotionInfo.dEndTime * 1.0))
		{
			m_bShot = true;
		}
	}
	if (m_pAnimatorCom->Get_AnimationIndex() != ATTACK_B)
	{
		m_bShot = false;
		m_bAttack = false;
	}

	if (!m_bAttack)
	{
		if (m_bShot)
		{
			m_bAttack = true;
			_vec3 vLook = m_pTransCom->Get_Now_Look();
			_vec3 vPos = m_pTransCom->Get_Now_Position();
			_vec3 vTargetPos = m_pTargetTransCom->Get_Now_Position();
			vPos.y += 3.f;
			vTargetPos.y += 3.f;
			CGameObject* pGameObject = CThrown_Arrow::Create(m_pGraphicDev, L"Mesh_Arrow", &vPos, &vTargetPos, &vLook, 100.f);
			Engine::AddObject(Engine::LAYER_GAMELOGIC, L"Weapon", pGameObject);
			m_bShot = false;
			m_bTest = true;
		}
	}

	if (!m_bDeath)
	{
		if (m_tMonInfo.iHp < 0)
		{
			if (m_pAnimatorCom->Set_AnimationSet(STAND_DOWN))
			{
				m_bDeath = true;
				m_pRigidbodyCom->Set_AccMotion(&m_tColInfo.vDir, 0.f);
				m_pAICom->Set_State(AI_DEATH);
			}
		}
	}

	if (m_bDeath)
	{
		m_fClearTime += fTimeDelta;
		if (m_fClearTime > 5.f)
		{
			m_eState = Engine::STATE_DESTROY;
		}
	}

	if (m_pAICom->Get_CurState() == AI_DOWN)
	{
		m_bDown = true;
	}
	else
		m_bDown = false;

	CMonster::Update_Object(fTimeDelta);

	//피격후 지면과의 충돌 이펙트
	CollideEffectDust(fTimeDelta);

	DamagedSound();


	//_vec3	vScale;

	//Compute_ViewZ(m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &vScale));
}

void CBabegazi_Bow::Render_Object(void)
{
	if (m_pSphereColCom)
		m_pSphereColCom->Render_Collider(g_bDebugRender);

	if (m_pColliderCom)
		m_pColliderCom->Render_Collider(g_bDebugRender);
}

void CBabegazi_Bow::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
{
	m_pMeshCom->Play_AnimationSet(0.f);

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

	if (2 == iPassIdx) //모션블러 일 때.
		m_pMeshCom->Render_BlurMesh(pEffect);
	else
		m_pMeshCom->Render_Mesh(pEffect);

	/*if (m_pColliderCom)
		m_pColliderCom->Render_Collider(g_bDebugRender);*/

	//if (m_pSphereColCom)
	//	m_pSphereColCom->Render_Collider(g_bDebugRender);

	if (m_bDamageEffectCheck)
	{
		m_pDust->Render_Object();
	}
}

HRESULT CBabegazi_Bow::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	if (FAILED(LoadAniInfo(L"Babegazi_Bow_Ani")))
		return E_FAIL;

	if (FAILED(LoadCollider(L"Babegazi_Bow_Collider")))
		return E_FAIL;

	m_iMonsterAniState = MONSTER_NORMAL;

	m_eLayerType = Engine::LAYER_GAMELOGIC;
	m_pTransCom->Set_Position(0.0f, 0.0f, 0.0f);

	m_tMonInfo.iMaxHp = 100000;
	m_tMonInfo.iHp = m_tMonInfo.iMaxHp;
	m_tMonInfo.iAtt = 10;
	m_fAlpha = 1.f;
	m_tMonInfo.m_eMonID = MON_B_Bow;

	////애니메이션 상태 준비
	Engine::CAnimationState* pState = NULL;

	pState = CBabegazi_Bow_Ani::Create(m_pMeshCom, &m_pmapNextMotion[MONSTER_NORMAL], STAND_IDLE);
	if (pState == NULL)
		return E_FAIL;
	m_pAnimatorCom->Set_State(MONSTER_NORMAL, pState);

	pState = CBabegazi_Bow_Ani::Create(m_pMeshCom, &m_pmapNextMotion[MONSTER_FIGHT], DISCOVERY);
	if (pState == NULL)
		return E_FAIL;
	m_pAnimatorCom->Set_State(MONSTER_FIGHT, pState);

	//	m_iAniIdx = STAND_IDLE;
	//Start_ForScene();

	//애니메이터 세팅
	m_pAnimatorCom->Set_Animator(&m_vecAniInfo);
	m_pAnimatorCom->Set_State(m_iMonsterAniState);
	m_pAnimatorCom->Set_AnimationSet(m_iAniIdx);

	//m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &m_vStartPos);

	//==피격 이펙트 Init==//
	InitEffectDust();
	return S_OK;
}

HRESULT CBabegazi_Bow::Add_Component(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;
	m_pTransCom = (Engine::CTransform*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"3", m_pTransCom));


	//Mesh..
	wstring::iterator iter = m_wstrName.begin();
	wstring::iterator iter_end = m_wstrName.end();

	TCHAR szName[MAX_PATH] = L"";

	int i = 0;

	for (; iter != iter_end; ++iter)
	{
		szName[i] = (*iter);
		++i;
	}

	pComponent = (Engine::CDynamicMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, szName);
	m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"DynamicMesh", m_pMeshCom));

	// For.Renderer Component
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	if (NULL == pComponent)
		return E_FAIL;
	pComponent->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));

	//Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_DynamicMesh"));
	if (NULL == pComponent) return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	//Navi
	pComponent = Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Navigation");
	if (pComponent == NULL)
		return E_FAIL;
	m_pNaviCom = (Engine::CNavigationMesh*)pComponent;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"NaviCom", pComponent));

	//Animator
	pComponent = Engine::CAnimator::Create();
	if (pComponent == NULL)
		return E_FAIL;
	m_pAnimatorCom = (Engine::CAnimator*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"3Animator", pComponent));

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
	pComponent = Engine::Create_SphereCollider(m_pGraphicDev, m_pTransCom, L"Monster", _vec3(0.f, 0.f, 0.f), 10.f);
	if (pComponent == NULL)
		return E_FAIL;
	m_pSphereColCom = (Engine::CSphereCollider*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"SphereCol", pComponent));

	//AI
	pComponent = Engine::CAI::Create();
	if (pComponent == NULL)
		return E_FAIL;
	m_pAICom = (Engine::CAI*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"4AI", pComponent));

	//Sound
	pComponent = CSound::Create(m_pTransCom);
	m_pSoundCom = (CSound*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Sound", pComponent));

	return S_OK;
}

HRESULT CBabegazi_Bow::Set_ConstantTable(LPD3DXEFFECT pEffect)
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

	//지형 색 * (빛.Diffuse * 마테리얼.diffuse) + (빛.Ambient * 마테리얼.Ambient)
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

	pEffect->SetBool("g_bBlurCheck", false);
	//pEffect->SetBool("g_bDynamicDepth", false);
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
		pEffect->SetVector("g_vColor", &_vec4(0.5294f, 0.8078f, 0.9215f, 0.0f));
	}

	Engine::Safe_Release(pEffect);

	return S_OK;
}

HRESULT CBabegazi_Bow::Set_Shadow_ConstanTable(LPD3DXEFFECT pEffect)
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

HRESULT CBabegazi_Bow::Set_CACD_Shadow_ConstanTable(LPD3DXEFFECT pEffect)
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


void CBabegazi_Bow::Render_Ray(void)
{
	//_vec3			vPoint[2];

	//vPoint[0] = m_vPivotPos;
	//vPoint[1] = m_vTargetPos;

	//_matrix			matView, matProj;

	//m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	//m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	//for (_ulong i = 0; i < 2; ++i)
	//{
	//	D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matView);
	//	if (vPoint[i].z <= 0.1f)
	//		vPoint[i].z = 0.1f;

	//	D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matProj);
	//}

	//m_pGraphicDev->EndScene();

	//m_pGraphicDev->BeginScene();

	//_matrix				matTmp;

}

void CBabegazi_Bow::DamagedSound(void)
{
	if (m_pAnimatorCom->Get_AnimationIndex() == DAM_B
		|| m_pAnimatorCom->Get_AnimationIndex() == DAM_F
		|| m_pAnimatorCom->Get_AnimationIndex() == DAM_R
		|| m_pAnimatorCom->Get_AnimationIndex() == DAM_F
		|| m_pAnimatorCom->Get_AnimationIndex() == DAM_H)
	{
		if (m_pAnimatorCom->IsPlaying_ThisSection((m_vecAniInfo)[DAM_B]->tMotionInfo.dStartTime + (m_vecAniInfo)[DAM_B]->tMotionInfo.dEndTime * 0.0,
			(m_vecAniInfo)[DAM_B]->tMotionInfo.dStartTime + (m_vecAniInfo)[DAM_B]->tMotionInfo.dEndTime * 0.1))
		{
			m_pSoundCom->MyPlaySound(L"Dam_1");
		}
	}

	if (m_pAnimatorCom->Get_AnimationIndex() == DAM_UPPER_A
		|| m_pAnimatorCom->Get_AnimationIndex() == DAM_HARD)
	{
		if (m_pAnimatorCom->IsPlaying_ThisSection((m_vecAniInfo)[DAM_UPPER_A]->tMotionInfo.dStartTime + (m_vecAniInfo)[DAM_UPPER_A]->tMotionInfo.dEndTime * 0.0,
			(m_vecAniInfo)[DAM_UPPER_A]->tMotionInfo.dStartTime + (m_vecAniInfo)[DAM_UPPER_A]->tMotionInfo.dEndTime * 0.1))
		{
			m_pSoundCom->MyPlaySound(L"Dam_2");
		}
	}

	if (m_pAnimatorCom->Get_AnimationIndex() == SPIN_A)
	{
		if (m_pAnimatorCom->IsPlaying_ThisSection((m_vecAniInfo)[SPIN_A]->tMotionInfo.dStartTime + (m_vecAniInfo)[SPIN_A]->tMotionInfo.dEndTime * 0.0,
			(m_vecAniInfo)[SPIN_A]->tMotionInfo.dStartTime + (m_vecAniInfo)[SPIN_A]->tMotionInfo.dEndTime * 0.1))
		{
			m_pSoundCom->MyPlaySound(L"Dam_3");
		}
	}

	if (m_pAnimatorCom->Get_AnimationIndex() == STAND_DOWN)
	{
		if (m_pAnimatorCom->IsPlaying_ThisSection((m_vecAniInfo)[STAND_DOWN]->tMotionInfo.dStartTime + (m_vecAniInfo)[STAND_DOWN]->tMotionInfo.dEndTime * 0.1,
			(m_vecAniInfo)[STAND_DOWN]->tMotionInfo.dStartTime + (m_vecAniInfo)[STAND_DOWN]->tMotionInfo.dEndTime * 0.2))
		{
			m_pSoundCom->MyPlaySound(L"Death");
		}
	}
	if (m_pAnimatorCom->Get_AnimationIndex() == DAM_UPPER_C)
	{
		if (m_pAnimatorCom->IsPlaying_ThisSection((m_vecAniInfo)[DAM_UPPER_C]->tMotionInfo.dStartTime + (m_vecAniInfo)[DAM_UPPER_C]->tMotionInfo.dEndTime * 0.0,
			(m_vecAniInfo)[DAM_UPPER_C]->tMotionInfo.dStartTime + (m_vecAniInfo)[DAM_UPPER_C]->tMotionInfo.dEndTime * 0.1))
		{
			m_pSoundCom->MyPlaySound(L"Floor_Hit");
		}
	}
}


//충돌 이펙트 설정
void CBabegazi_Bow::InitEffectDust(void)
{
	m_pDust = CSprite::LoadCreate(m_pGraphicDev, L"MonsterDust");
	m_pDust->SetAni(false);
	m_pDust->GetTransCom()->Set_Scale(1.0f, 1.0f, 1.0f);

	D3DXCOLOR color = D3DXVECTOR4(0.2f, 0.2f, 0.2f, 0.6f);
	m_pDust->GetParticleInfo()->m_dColor = color;
	m_pDust->GetParticleInfo()->m_iPath = 3;
}

//충돌 이펙트 관리 함수
void CBabegazi_Bow::CollideEffectDust(const _float& fTimeDelta)
{

	if (m_pAnimatorCom->Get_AnimationIndex() == DAM_UPPER_C
		|| m_pAnimatorCom->Get_AnimationIndex() == SPIN_B
		|| m_pAnimatorCom->Get_AnimationIndex() == DAM_UPPER_B
		|| m_pAnimatorCom->Get_AnimationIndex() == JUMP_C)
	{
		if (m_pCalculatorCom->Get_IsCol() && m_bDamageEffectCheck == false)
		{
			m_bDamageEffectCheck = true;

			m_pDust->SetAni(true);

			_vec3 Pos;

			Pos = m_pTransCom->Get_Now_Position();

			Pos.y += 1.0f;

			m_pDust->GetTransCom()->Set_Position(&Pos);
		}
	}
	else
	{
		m_bDamageEffectCheck = false;
		_vec4 Alpha = (_vec4)m_pDust->GetParticleInfo()->m_dColor;
		Alpha.w = 0.6f;
		m_pDust->GetParticleInfo()->m_dColor = (D3DXCOLOR)Alpha;
	}

	if (m_bDamageEffectCheck)
	{
		_vec4 Alpha = (_vec4)m_pDust->GetParticleInfo()->m_dColor;

		Alpha.w = max(Alpha.w - 0.03f, 0.0f);

		m_pDust->GetParticleInfo()->m_dColor = (D3DXCOLOR)Alpha;

		m_pDust->Update_Object(fTimeDelta);
	}


}

CBabegazi_Bow* CBabegazi_Bow::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
{
	CBabegazi_Bow*		pInstance = new CBabegazi_Bow(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CBabegazi_Bow Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CBabegazi_Bow::Free(void)
{
	//Engine::Safe_Release(m_pArrow);

	_uint iSize = m_vecAniInfo.size();
	for (_uint i = 0; i < iSize; i++)
	{
		Engine::Safe_Delete_Array(m_vecAniInfo[i]->pMotionEventInfo);
		Engine::Safe_Delete_Array(m_vecAniInfo[i]->pUse);
		Engine::Safe_Delete(m_vecAniInfo[i]);
	}
	m_vecAniInfo.clear();
	for (_uint i = 0; i < MONSTER_END; i++)
	{
		map<_uint, list<Engine::ANIMATORINFO*>>::iterator iter = m_pmapNextMotion[i].begin();
		map<_uint, list<Engine::ANIMATORINFO*>>::iterator iter_end = m_pmapNextMotion[i].end();

		for (iter; iter != iter_end; iter++)
		{
			list<Engine::ANIMATORINFO*>::iterator iter_list = iter->second.begin();
			list<Engine::ANIMATORINFO*>::iterator iter_list_end = iter->second.end();
			for (iter_list; iter_list != iter_list_end; iter_list++)
			{
				Engine::Safe_Delete((*iter_list));
			}
		}
		m_pmapNextMotion[i].clear();
	}
	Engine::Safe_Delete_Array(m_pmapNextMotion);
	Engine::Safe_Release(m_pDust);

	CMonster::Free();	
}

HRESULT CBabegazi_Bow::LoadAniInfo(const _tchar* pFileName)
{
	if (pFileName == NULL)
		return E_FAIL;

	_tchar szFullPath[MAX_PATH] = L"";
	lstrcpy(szFullPath, L"../Bin/Data/MonsterAnimationData/");
	lstrcat(szFullPath, pFileName);
	lstrcat(szFullPath, L".dat");


	DWORD dwByte = 0;
	HANDLE hFile = CreateFile(szFullPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == NULL)
		return E_FAIL;

	_uint iMaxAniCnt = 0;
	_uint iContainerSize = 0;

	ReadFile(hFile, &iMaxAniCnt, sizeof(_uint), &dwByte, NULL);
	ReadFile(hFile, &iContainerSize, sizeof(_uint), &dwByte, NULL);

	if (iMaxAniCnt == 0)
		return E_FAIL;

	m_vecAniInfo.reserve(iMaxAniCnt);
	m_pmapNextMotion = new	map<_uint, list<Engine::ANIMATORINFO*>>[iContainerSize];

	for (_uint i = 0; i < iMaxAniCnt; i++)
	{
		Engine::ANIINFO* pAniInfo = new Engine::ANIINFO;
		ReadFile(hFile, &pAniInfo->tMotionInfo, sizeof(Engine::MOTIONINFO), &dwByte, NULL);
		_uint iEventSize = 0;
		ReadFile(hFile, &iEventSize, sizeof(_uint), &dwByte, NULL);
		pAniInfo->pMotionEventInfo = new Engine::MOTIONEVENTINFO[iEventSize];
		for (_uint j = 0; j < iEventSize; j++)
		{
			ReadFile(hFile, &pAniInfo->pMotionEventInfo[j], sizeof(Engine::MOTIONEVENTINFO), &dwByte, NULL);
		}
		for (_uint j = 0; j < iContainerSize; j++)
		{
			list<Engine::ANIMATORINFO*> listAnimator;
			_uint iNextMotionSize = 0;
			ReadFile(hFile, &iNextMotionSize, sizeof(_uint), &dwByte, NULL);
			for (_uint k = 0; k < iNextMotionSize; k++)
			{
				Engine::ANIMATORINFO* pAnimatorInfo = new Engine::ANIMATORINFO;
				ReadFile(hFile, pAnimatorInfo, sizeof(Engine::ANIMATORINFO), &dwByte, NULL);
				listAnimator.push_back(pAnimatorInfo);
			}
			m_pmapNextMotion[j].insert(map<_uint, list<Engine::ANIMATORINFO*>>::value_type(i, listAnimator));
		}
		pAniInfo->pUse = new _bool[iContainerSize];
		for (_uint j = 0; j < iContainerSize; j++)
		{
			ReadFile(hFile, &pAniInfo->pUse[j], sizeof(_bool), &dwByte, NULL);
		}

		m_vecAniInfo.push_back(pAniInfo);
	}

	CloseHandle(hFile);

	return S_OK;
}









