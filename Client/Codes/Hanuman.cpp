#include "stdafx.h"

#include "Monster.h"
#include "Hanuman.h"

#include "Export_Engine.h"
#include "VIBuffer.h"

#include "Map_Ice.h"

#include "Texture.h"

//#include "Camera.h"
#include "LightCamera.h"
#include "Environment.h"
#include "Player.h"
#include "Cube.h"
#include "NavigationMesh.h"
#include "Hanuman_Ani.h"

#include "AI.h"
#include "AI_CameraIntro_Hanuman.h"
#include "AI_Idle_Hanuman.h"
#include "AI_Trace_Hanuman.h"
#include "AI_RoundPunch_Hanuman.h"
#include "AI_Attack1_Hanuman.h"
#include "AI_Attack2_Hanuman.h"
#include "AI_Combo_Hanuman.h"
#include "AI_Roar_Hanuman.h"
#include "AI_CrazyFoot_Hanuman.h"
#include "AI_Breath_Hanuman.h"
#include "AI_Death_Hanuman.h"

#include "Sound.h"

#include "Sprite.h"
#include "World_HP_Bar.h"
#include "World_HP_Gage.h"
#include "BossBar.h"
#include "BossGage.h"
#include "BossOutBar.h"
#include "BossTex.h"
#include "BossBarEffect.h"
#include "BossName.h"

CHanuman::CHanuman(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
	: CMonster(pGraphicDev, wstrName)
	, m_pDust(NULL)
	, m_pBossUIEffect(NULL)
	, m_pEffectShaderCom(NULL)
	, m_fEffectPowerTime(0.0f)
	, m_fEffectPower(0.0f)
	, m_fDeadRadialBlur(0.0f)
{	
	m_eMonsterID = MON_HANUMAN;
}
CHanuman::~CHanuman(void)
{

}

void CHanuman::Set_CameraStart(const _bool & bStart)
{
	CAI_CameraIntro_Hanuman* pState = (CAI_CameraIntro_Hanuman*)m_pAICom->Get_State(AI_CAMERAINTRO);
	if (pState == NULL)
		return;

	pState->Set_CameraStart(bStart);
}

HRESULT CHanuman::Start_ForScene(void)
{
	CMonster::Start_ForScene();

	m_pSoundCom->Set_Sound(L"RoundPunch_Voice", L"Hanuman_RoundPunch_Voice.wav");
	m_pSoundCom->Set_Sound(L"RoundPunch_Attack_1", L"Hanuman_RoundPunch_Attack_1.wav");
	m_pSoundCom->Set_Sound(L"RoundPunch_Attack_2", L"Hanuman_RoundPunch_Attack_2.wav");
	m_pSoundCom->Set_Sound(L"RoundPunch_Attack_3", L"Hanuman_RoundPunch_Attack_3.wav");
	m_pSoundCom->Set_Sound(L"RoundPunch_Fall_1", L"Hanuman_RoundPunch_Fall_1.wav");

	m_pSoundCom->Set_Sound(L"Attack_1_Voice", L"Hanuman_Attack_1_Voice.wav");
	m_pSoundCom->Set_Sound(L"Attack_1_1", L"Hanuman_Attack_1_1.wav");
	m_pSoundCom->Set_Sound(L"Attack_1_2", L"Hanuman_Attack_1_2.wav");

	m_pSoundCom->Set_Sound(L"Attack_2_Voice", L"Hanuman_Attack_2_Voice.wav");
	m_pSoundCom->Set_Sound(L"Attack_2", L"Hanuman_Attack_2.wav");

	m_pSoundCom->Set_Sound(L"Combo_Voice", L"Hanuman_Combo_Voice.wav");
	m_pSoundCom->Set_Sound(L"Combo_Attack_1", L"Hanuman_Combo_Attack_1.wav");
	m_pSoundCom->Set_Sound(L"Combo_Attack_2", L"Hanuman_Combo_Attack_2.wav");
	m_pSoundCom->Set_Sound(L"Combo_Attack_3", L"Hanuman_Combo_Attack_3.wav");

	m_pSoundCom->Set_Sound(L"Roar_Voice", L"Hanuman_Roar_Voice.wav");
	m_pSoundCom->Set_Sound(L"Roar_Attack", L"Hanuman_Roar_Attack.wav");

	m_pSoundCom->Set_Sound(L"Breath_Voice", L"Hanuman_Breath_Voice.wav");

	m_pSoundCom->Set_Sound(L"CrazyFoot_Ice_Fall", L"Hanuman_CrazyFoot_Ice_Fall.wav");
	m_pSoundCom->Set_Sound(L"Death", L"Hanuman_Death.wav");
	m_pSoundCom->Set_Sound(L"Drop", L"Hanuman_Drop.wav");
	m_pSoundCom->Set_Sound(L"Floor_Hit_1", L"Hanuman_CrazyFoot_Floor_1.wav");
	m_pSoundCom->Set_Sound(L"Floor_Hit_2", L"Hanuman_CrazyFoot_Floor_2.wav");

	m_pSoundCom->Set_Sound(L"Walk1", L"HanumanWalk1.wav");
	m_pSoundCom->Set_Sound(L"Walk2", L"HanumanWalk2.wav");


	m_tColInfo.bCritical = false;

	Engine::CGameObject* pGameObject = CBossBarEffect::Create(m_pGraphicDev, L"Boss_Bar_Effect", this);
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"Name_UI", pGameObject);


	m_pBossUIEffect = ((CUI*)pGameObject);
	pGameObject = CBossBar::Create(m_pGraphicDev, L"Boss_Bar", this);
	if (pGameObject == NULL)
	return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"Name_UI", pGameObject);

	pGameObject = CBossGage::Create(m_pGraphicDev, L"Boss_Gage", this);
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"Name_UI", pGameObject);

	pGameObject = CBossOutBar::Create(m_pGraphicDev, L"Boss_OutBar", this);
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"Name_UI", pGameObject);

	pGameObject = CBossTex::Create(m_pGraphicDev, L"Boss_Tex", this);
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"Name_UI", pGameObject);

	pGameObject = CUI_BossName::Create(m_pGraphicDev, L"Boss_Name", this);
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"Name_UI", pGameObject);


	m_pLightCam = Engine::GetCamera(Engine::CAMERA_LIGHT);
	m_pCurCam = Engine::GetCurrentCamera();

	//AI 세팅..

	Engine::CAI_State* pAI_State = NULL;
	pAI_State = CAI_CameraIntro_Hanuman::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pCalculatorCom, m_pRigidbodyCom,
		m_pColliderCom, &m_tColInfo, &m_bDamaged, m_pAICom, m_pSoundCom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_CAMERAINTRO), pAI_State);

	pAI_State = CAI_Idle_Hanuman::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pCalculatorCom, m_pRigidbodyCom,
		m_pColliderCom, &m_tColInfo, &m_bDamaged, m_pAICom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_IDLE), pAI_State);

	pAI_State = CAI_Trace_Hanuman::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pCalculatorCom, m_pRigidbodyCom,
		m_pColliderCom, &m_tColInfo, &m_bDamaged, m_pAICom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_TRACE), pAI_State);

	pAI_State = CAI_RoundPunch_Hanuman::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pCalculatorCom, m_pRigidbodyCom,
		m_pColliderCom, &m_tColInfo, &m_bDamaged, m_pAICom, m_pSoundCom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	pAI_State->Set_SphereCol(m_pSphereColCom);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_ROUNDPUNCH), pAI_State);

	pAI_State = CAI_Attack1_Hanuman::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pCalculatorCom, m_pRigidbodyCom,
		m_pColliderCom, &m_tColInfo, &m_bDamaged, m_pAICom, m_pSoundCom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	pAI_State->Set_SphereCol(m_pSphereColCom);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_ATTACK1), pAI_State);

	pAI_State = CAI_Attack2_Hanuman::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pCalculatorCom, m_pRigidbodyCom,
		m_pColliderCom, &m_tColInfo, &m_bDamaged, m_pAICom, m_pSoundCom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	pAI_State->Set_SphereCol(m_pSphereColCom);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_ATTACK2), pAI_State);

	pAI_State = CAI_Roar_Hanuman::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pCalculatorCom, m_pRigidbodyCom,
		m_pColliderCom, &m_tColInfo, &m_bDamaged, m_pAICom, m_pSoundCom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	pAI_State->Set_SphereCol(m_pSphereColCom);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_ROAR), pAI_State);

	pAI_State = CAI_Combo_Hanuman::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pCalculatorCom, m_pRigidbodyCom,
		m_pColliderCom, &m_tColInfo, &m_bDamaged, m_pAICom, m_pSoundCom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	pAI_State->Set_SphereCol(m_pSphereColCom);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_COMBO), pAI_State);

	pAI_State = CAI_CrazyFoot_Hanuman::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pCalculatorCom, m_pRigidbodyCom,
		m_pColliderCom, &m_tColInfo, &m_bDamaged, m_pAICom, m_pSoundCom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	pAI_State->Set_SphereCol(m_pSphereColCom);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_CRAZYFOOT), pAI_State);

	pAI_State = CAI_Breath_Hanuman::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pCalculatorCom, m_pRigidbodyCom,
		m_pColliderCom, &m_tColInfo, &m_bDamaged, m_pAICom, m_pSoundCom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	pAI_State->Set_SphereCol(m_pSphereColCom);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_BREATH), pAI_State);

	pAI_State = CAI_Death_Hanuman::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pCalculatorCom, m_pRigidbodyCom,
		m_pColliderCom, &m_tColInfo, &m_bDamaged, m_pAICom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	pAI_State->Set_SphereCol(m_pSphereColCom);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_DEATH), pAI_State);

	m_eAIState = AI_STATE::AI_CAMERAINTRO;
	m_pAnimatorCom->Set_State(MONSTER_NORMAL);
	m_pAICom->Set_State(m_eAIState);

	m_iBlurCnt = 1;
	m_fBlurPower = 1.7f;
	m_fBlurValue = 2000.0f;
	m_eBlurType = Engine::BLUR_CROSS;

	//m_pAICom->Set_StartState();

	return S_OK;
}

void CHanuman::Update_Object(const _float& fTimeDelta)
{

	if (m_pAnimatorCom->Get_AnimationIndex() == MOVE_RUN)
	{
		if (m_pAnimatorCom->IsPlaying_ThisSection(m_vecAniInfo[MOVE_RUN]->tMotionInfo.dEndTime * 0.0,
			m_vecAniInfo[MOVE_RUN]->tMotionInfo.dEndTime * 0.1))
		{
			m_pSoundCom->MyPlaySound(L"Walk1");
		}
		if (m_pAnimatorCom->IsPlaying_ThisSection(m_vecAniInfo[MOVE_RUN]->tMotionInfo.dEndTime * 0.1,
			m_vecAniInfo[MOVE_RUN]->tMotionInfo.dEndTime * 0.2))
		{
			m_pSoundCom->MyPlaySound(L"Walk2");
		}
	}

	//피격후 지면과의 충돌 이펙트
	CollideEffectDust(fTimeDelta);

	if (!m_bDeath)
	{
		if (m_tMonInfo.iHp < 0)
		{
			if (m_pAnimatorCom->Set_AnimationSet(STAND_DOWN))
			{
				m_pSoundCom->MyPlaySound(L"Death");
				m_pRigidbodyCom->Set_AccMotion(&m_tColInfo.vDir, 0.f);
				//m_pAICom->Set_State(AI_DEATH);				
				m_bDeath = true;
			}
		}
	}

	if (m_bDeath)
	{
		//if (m_pAnimatorCom->Set_AnimationSet(BATTLE_IDLE))
		//{
		//	m_pAICom->Set_State(AI_DEATH);
		//}

		//레디얼 블러=======
				
		//=================
		m_fClearTime += fTimeDelta;

		if (1.5f > m_fClearTime)
		{
			m_fDeadRadialBlur -= 0.005f;
			if (m_fDeadRadialBlur < -0.5f)
				m_fDeadRadialBlur = -0.5f;

			Engine::SetRadialBlur(TRUE);
			Engine::SetRadialBlurPower(m_fDeadRadialBlur);
		}

		if (1.5f <= m_fClearTime && 3.0f > m_fClearTime)
		{
			m_fDeadRadialBlur += 0.005f;
			if (m_fDeadRadialBlur > 0.0f)
				m_fDeadRadialBlur = 0.0f;

			Engine::SetRadialBlurPower(m_fDeadRadialBlur);
		}

		
		if (m_fClearTime > 3.f)
		{
			Engine::SetRadialBlur(FALSE);
			Engine::SetRadialBlurPower(-0.25f);
			m_eState = Engine::STATE_DESTROY;
		}
	}
	
	if (m_tColInfo.bCritical)
		m_pBossUIEffect->SetRender(true);
	else
		m_pBossUIEffect->SetRender(false);

	//m_pAICom->Get_CurState() == AI_ATTACK1

	//_vec3	vScale;

	//Compute_ViewZ(m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &vScale));

	//Motion Blur============================================================
	if (m_pRendererCom->GetMotionBlurBuffer())
	{
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_MOTION_BLUR, this);
		m_pMeshCom->UpdateTimeDelta(fTimeDelta);
	}
	//=======================================================================

	CMonster::Update_Object(fTimeDelta);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_POSTEFFECT_BLUR, this);

	_matrix matScale, matRot_X, matRot_Y, matRot_Z, matTrans;

	D3DXMatrixScaling(&matScale, 1.0075f, 1.0075f, 1.0075f);
	D3DXMatrixRotationX(&matRot_X, D3DXToRadian(m_pTransCom->Get_Now_Angle().x));
	D3DXMatrixRotationY(&matRot_Y, D3DXToRadian(m_pTransCom->Get_Now_Angle().y));
	D3DXMatrixRotationZ(&matRot_Z, D3DXToRadian(m_pTransCom->Get_Now_Angle().z));
	D3DXMatrixTranslation(&matTrans, m_pTransCom->Get_Now_Position().x, m_pTransCom->Get_Now_Position().y, m_pTransCom->Get_Now_Position().z);

	m_matEffectWorld = matScale * matRot_X * matRot_Y * matRot_Z * matTrans;

	BlurPowerControl(fTimeDelta);
}

void CHanuman::Render_Object(void)
{
	if (Engine::GetMRTID() == Engine::MRT_POSTEFFECT)
	{
		LPD3DXEFFECT pEffect = m_pEffectShaderCom->Get_EffectHandle();
		if (pEffect == NULL)
			return;

		pEffect->AddRef();

		if (FAILED(Set_ConstantTable(pEffect)))
			return;
				
		m_pBlurTransCom->Set_Texture(pEffect, "g_BlurTexture");
		Engine::SetUp_OnShader(L"Target_DepthMap", pEffect, "g_DepthTexture");
		pEffect->SetVector("g_vColor", &_vec4(0.6862f / 2.0f, 0.9333f / 2.0f, 0.9333f /2.0f, 0.0f));
		
		pEffect->Begin(NULL, 0);
		pEffect->BeginPass(12);

		m_pMeshCom->Render_Mesh(pEffect);

		pEffect->EndPass();
		pEffect->End();

		Engine::Safe_Release(pEffect);
	}
	else
	{
		if (m_pSphereColCom)
			m_pSphereColCom->Render_Collider(g_bDebugRender);

		if (m_pColliderCom)
			m_pColliderCom->Render_Collider(g_bDebugRender);
	}
}

void CHanuman::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
{
	if (Engine::GetMRTID() == Engine::MRT_POSTEFFECT_BLUR)
	{
		pEffect->AddRef();

		if (FAILED(Set_ConstantTable(pEffect)))
			return;

		m_pBlurTransCom->Set_Texture(pEffect, "g_BlurTexture");
		Engine::SetUp_OnShader(L"Target_DepthMap", pEffect, "g_DepthTexture");
		pEffect->SetVector("g_vColor", &_vec4((0.1176f), (0.5647f), (1.0f), 0.0f));

		m_fBlurPower = m_fEffectPower;
				
		pEffect->BeginPass(12);

		m_pMeshCom->Render_Mesh(pEffect);

		pEffect->EndPass();

		Engine::Safe_Release(pEffect);
	}
	else
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
}

HRESULT CHanuman::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	if (FAILED(LoadAniInfo(L"Hanuman_Ani")))
		return E_FAIL;

	if (FAILED(LoadCollider(L"Hanuman_Collider")))
		return E_FAIL;

	m_bOutlineCheck = FALSE;

	m_iMonsterAniState = MONSTER_NORMAL;

	m_eLayerType = Engine::LAYER_GAMELOGIC;
	m_pTransCom->Set_Position(0.0f, 0.0f, 0.0f);

	m_fAlpha = 1.f;

	m_tMonInfo.iMaxHp = 1500000;
	m_tMonInfo.iHp = m_tMonInfo.iMaxHp;
	m_tMonInfo.iAtt = 10;
	m_tMonInfo.m_eMonID = MON_HANUMAN;
	////애니메이션 상태 준비
	Engine::CAnimationState* pState = NULL;

	pState = CHanuman_Ani::Create(m_pMeshCom, &m_pmapNextMotion[MONSTER_NORMAL], STAND_IDLE);
	if (pState == NULL)
		return E_FAIL;
	m_pAnimatorCom->Set_State(MONSTER_NORMAL, pState);

	pState = CHanuman_Ani::Create(m_pMeshCom, &m_pmapNextMotion[MONSTER_FIGHT], BATTLE_IDLE);
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

HRESULT CHanuman::Add_Component(void)
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

	pComponent = m_pEffectShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Effect"));
	if (NULL == pComponent) return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_EffectShader", pComponent));

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

	// For.Texture Component 
	m_pBlurTransCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"HANUMAN_Blur_Effect");
	pComponent = (Engine::CComponent*)m_pBlurTransCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));

	//Sound
	pComponent = CSound::Create(m_pTransCom);
	m_pSoundCom = (CSound*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Sound", pComponent));

	return S_OK;
}

HRESULT CHanuman::Set_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld, matView, matProj;

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	if (Engine::GetMRTID() == Engine::MRT_POSTEFFECT || Engine::GetMRTID() == Engine::MRT_POSTEFFECT_BLUR)
		pEffect->SetMatrix("g_matWorld", &m_matEffectWorld);
	else
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
	
	pEffect->SetBool("g_bBlurCheck", Engine::GetMotionBlurBuffer());
	//pEffect->SetBool("g_bDynamicDepth", true);
	pEffect->SetBool("g_bOutline", m_bOutlineCheck);
	pEffect->SetFloat("g_fColorCheck", 0.15f);
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
		pEffect->SetVector("g_vColor", &_vec4(0.5294f, 0.8078f, 0.9215f, 0.0f));
	}

	Engine::Safe_Release(pEffect);

	return S_OK;
}

HRESULT CHanuman::Set_Shadow_ConstanTable(LPD3DXEFFECT pEffect)
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

HRESULT CHanuman::Set_CACD_Shadow_ConstanTable(LPD3DXEFFECT pEffect)
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


void CHanuman::Render_Ray(void)
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

//충돌 이펙트 설정
void CHanuman::InitEffectDust(void)
{
	m_pDust = CSprite::LoadCreate(m_pGraphicDev, L"MonsterDust");
	m_pDust->SetAni(false);
	m_pDust->GetTransCom()->Set_Scale(1.0f, 1.0f, 1.0f);

	D3DXCOLOR color = D3DXVECTOR4(0.2f, 0.2f, 0.2f, 0.6f);
	m_pDust->GetParticleInfo()->m_dColor = color;
	m_pDust->GetParticleInfo()->m_iPath = 3;
}

//충돌 이펙트 관리 함수
void CHanuman::CollideEffectDust(const _float& fTimeDelta)
{

	//if (m_pAnimatorCom->Get_AnimationIndex() == DAM_UPPER_C
	//	|| m_pAnimatorCom->Get_AnimationIndex() == SPIN_B
	//	|| m_pAnimatorCom->Get_AnimationIndex() == DAM_UPPER_B
	//	|| m_pAnimatorCom->Get_AnimationIndex() == JUMP_C)
	//{
	//	if (m_pCalculatorCom->Get_IsCol() && m_bDamageEffectCheck == false)
	//	{
	//		m_bDamageEffectCheck = true;

	//		m_pDust->SetAni(true);

	//		_vec3 Pos;

	//		Pos = m_pTransCom->Get_Now_Position();

	//		Pos.y += 1.0f;

	//		m_pDust->GetTransCom()->Set_Position(&Pos);
	//	}
	//}
	//else
	//{
	//	m_bDamageEffectCheck = false;
	//	_vec4 Alpha = (_vec4)m_pDust->GetParticleInfo()->m_dColor;
	//	Alpha.w = 0.6f;
	//	m_pDust->GetParticleInfo()->m_dColor = (D3DXCOLOR)Alpha;
	//}

	//if (m_bDamageEffectCheck)
	//{
	//	_vec4 Alpha = (_vec4)m_pDust->GetParticleInfo()->m_dColor;

	//	Alpha.w = max(Alpha.w - 0.03f, 0.0f);

	//	m_pDust->GetParticleInfo()->m_dColor = (D3DXCOLOR)Alpha;

	//	m_pDust->Update_Object(fTimeDelta);
	//}


}

CHanuman* CHanuman::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
{
	CHanuman*		pInstance = new CHanuman(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CHanuman Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CHanuman::Free(void)
{
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

HRESULT CHanuman::LoadAniInfo(const _tchar* pFileName)
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

void CHanuman::BlurPowerControl(const _float& fTimeDelta)
{
	if (m_pAnimatorCom->Get_AnimationIndex() == ATTACK1)
	{
		m_fEffectPowerTime += fTimeDelta;

		if (m_fEffectPowerTime >= 0.692f && m_fEffectPowerTime < 1.3f)
		{
			m_fEffectPower += 0.25f;
			if (m_fEffectPower > 5.5f)
				m_fEffectPower = 5.5f;
		}

		if (m_fEffectPowerTime >= 1.3f && m_fEffectPowerTime < 3.11f)
		{
			m_fEffectPower -= 0.1f;
			if (m_fEffectPower < 1.7f)
				m_fEffectPower = 1.7f;
		}

		if (m_fEffectPowerTime >= 3.11f && m_fEffectPowerTime < 3.72f)
		{
			m_fEffectPower += 0.25f;
			if (m_fEffectPower > 5.5f)
				m_fEffectPower = 5.5f;
		}

		if (m_fEffectPowerTime >= 3.72f)
		{
			m_fEffectPower -= 0.1f;
			if (m_fEffectPower < 1.7f)
				m_fEffectPower = 1.7f;
		}
	}
	else if (m_pAnimatorCom->Get_AnimationIndex() == ATTACK2)
	{
		m_fEffectPowerTime += fTimeDelta;

		if (m_fEffectPowerTime >= 2.10f && m_fEffectPowerTime < 2.89f)
		{
			m_fEffectPower += 0.25f;
			if (m_fEffectPower > 5.5f)
				m_fEffectPower = 5.5f;
		}

		if (m_fEffectPowerTime >= 2.89f)
		{
			m_fEffectPower -= 0.1f;
			if (m_fEffectPower < 1.7f)
				m_fEffectPower = 1.7f;
		}
	}
	else if (m_pAnimatorCom->Get_AnimationIndex() == COMBO2)
	{
		m_fEffectPowerTime += fTimeDelta;

		if (m_fEffectPowerTime >= 0.1f && m_fEffectPowerTime < 0.68f)
		{
			m_fEffectPower += 0.25f;
			if (m_fEffectPower > 5.5f)
				m_fEffectPower = 5.5f;
		}

		if (m_fEffectPowerTime >= 0.68f)
		{
			m_fEffectPower -= 0.1f;
			if (m_fEffectPower < 1.7f)
				m_fEffectPower = 1.7f;
		}
	}
	else if (m_pAnimatorCom->Get_AnimationIndex() == CRAZYFOOT)
	{
		m_fEffectPowerTime += fTimeDelta;

		if (m_fEffectPowerTime >= 1.2f && m_fEffectPowerTime < 2.196f)
		{
			m_fEffectPower += 0.25f;
			if (m_fEffectPower > 5.5f)
				m_fEffectPower = 5.5f;
		}

		if (m_fEffectPowerTime >= 2.196f)
		{
			m_fEffectPower -= 0.1f;
			if (m_fEffectPower < 1.7f)
				m_fEffectPower = 1.7f;
		}
	}
	else if (m_pAnimatorCom->Get_AnimationIndex() == BREATH_1)
	{
		m_fEffectPowerTime += fTimeDelta;

		if (m_fEffectPowerTime >= 0.0f && m_fEffectPowerTime < 1.5f)
		{
			m_fEffectPower += 0.25f;
			if (m_fEffectPower > 5.5f)
				m_fEffectPower = 5.5f;
		}

		if (m_fEffectPowerTime >= 1.5f)
		{
			m_fEffectPower -= 0.1f;
			if (m_fEffectPower < 1.7f)
				m_fEffectPower = 1.7f;
		}
	}
	else if (m_pAnimatorCom->Get_AnimationIndex() == ROUNDPUNCH_B)
	{
		m_fEffectPowerTime += fTimeDelta;

		if (m_fEffectPowerTime >= 0.0f)
		{
			m_fEffectPower += 0.25f;
			if (m_fEffectPower > 5.5f)
				m_fEffectPower = 5.5f;
		}
	}
	else if (m_pAnimatorCom->Get_AnimationIndex() == ROUNDPUNCH_C)
	{
		m_fEffectPowerTime += fTimeDelta;

		if (m_fEffectPowerTime >= 0.0f)
		{
			m_fEffectPower -= 0.1f;
			if (m_fEffectPower < 1.7f)
				m_fEffectPower = 1.7f;
		}
	}
	else if (m_pAnimatorCom->Get_AnimationIndex() == ROUNDPUNCH_C1_START)
	{
		m_fEffectPowerTime += fTimeDelta;

		if (m_fEffectPowerTime >= 0.0f)
		{
			m_fEffectPower -= 0.1f;
			if (m_fEffectPower < 1.7f)
				m_fEffectPower = 1.7f;
		}
	}
	else if (m_pAnimatorCom->Get_AnimationIndex() == ROAR)
	{
		m_fEffectPowerTime += fTimeDelta;		

		if (m_fEffectPowerTime >= 0.733f && m_fEffectPowerTime < 2.5f)
		{
			m_fEffectPower += 0.1f;
			if (m_fEffectPower > 5.5f)
				m_fEffectPower = 5.5f;
		}

		if (m_fEffectPowerTime >= 2.5f)
		{
			m_fEffectPower -= 0.1f;
			if (m_fEffectPower < 1.7f)
				m_fEffectPower = 1.7f;
		}
	}
	else
	{
		m_fEffectPowerTime = 0.0f;	
		m_fEffectPower = 1.7f;
	}
}
