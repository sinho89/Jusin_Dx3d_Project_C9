#include "stdafx.h"

#include "Monster.h"
#include "Bardiel.h"

#include "Export_Engine.h"
#include "VIBuffer.h"

#include "Map_Ice.h"

//#include "Camera.h"
#include "LightCamera.h"
#include "Environment.h"
#include "Player.h"
#include "Cube.h"
#include "NavigationMesh.h"
#include "Bardiel_Ani.h"
#include "NumberingMgr.h"
#include "DamParent.h"

#include "AI.h"
#include "AI_CameraIntro_Bardiel.h"
#include "AI_Idle1_Bardiel.h"
#include "AI_Combat1_Bardiel.h"
#include "AI_Penalty1_Bardiel.h"
#include "AI_Lava1_Bardiel.h"
#include "AI_Fireball1_Bardiel.h"
#include "AI_FireFrame1_Bardiel.h"
#include "AI_FirePillar1_Bardiel.h"
#include "AI_ChangePhase2_Bardiel.h"
#include "AI_Idle2_Bardiel.h"
#include "AI_Combat2_Bardiel.h"
#include "AI_WhirlWind2_Bardiel.h"
#include "AI_Volcano2_Bardiel.h"
#include "AI_ShortHammer2_Bardiel.h"
#include "AI_LFoot2_Bardiel.h"
#include "AI_Hammer2_Bardiel.h"
#include "AI_FireWall2_Bardiel.h"
#include "AI_FireRain2_Bardiel.h"
#include "AI_Divide2_Bardiel.h"
#include "AI_Clean2_Bardiel.h"

#include "Sound.h"

#include "Sprite.h"
#include "World_HP_Bar.h"
#include "World_HP_Gage.h"
#include "BossBarEffect.h"
#include "BossBar.h"
#include "BossGage.h"
#include "BossOutBar.h"
#include "BossTex.h"
#include "BossName.h"
#include "Boss_Base.h"

CBardiel::CBardiel(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
	: CMonster(pGraphicDev, wstrName)
	, m_pDust(NULL)
	, m_pBossUIEffect(NULL)
	,m_bGravityStart(false)
	,m_bPhase2(false)
	,m_bWeapon1(false)
	,m_bWeapon2(false)
	,m_bArmor(false)
	, m_bDeath(FALSE)
	, m_fDeathRadialBlur(0.0f)
	, m_fRadialTime(0.0f)
	, m_fEffectPowerTime(0.0f)
	, m_fEffectPower(0.0f)
	, m_pBlur_Body_TransCom(NULL)
	, m_pBlur_Shield_TransCom(NULL)
	, m_pBlur_HandGun_TransCom(NULL)
	, m_pEffectShaderCom(NULL)
{
	m_eMonsterID = MON_BARDIEL;
	m_bWalk[0] = false;
	m_bWalk[1] = false;
}
CBardiel::~CBardiel(void)
{

}

void CBardiel::Set_CameraStart(const _bool & bStart)
{
	CAI_CameraIntro_Bardiel* pState = (CAI_CameraIntro_Bardiel*)m_pAICom->Get_State(AI_CAMERAINTRO);
	if (pState == NULL)
		return;

	pState->Set_CameraStart(bStart);
}

HRESULT CBardiel::Start_ForScene(void)
{
	CMonster::Start_ForScene();

	m_pSoundCom->Set_Sound(L"Walk1", L"BardielWalk.wav");
	m_pSoundCom->Set_Sound(L"Walk2", L"BardielWalk.wav");
	m_pSoundCom->Set_Sound(L"CameraIntro_SpinCircle_2", L"Bardiel_CameraIntro_SpinCircle_2");
	m_pSoundCom->Set_Sound(L"CameraIntro_EarthCrack", L"Bardiel_CameraIntro_EarthCrack.wav");
	m_pSoundCom->Set_Sound(L"CameraIntro_SpinCircle_2", L"Bardiel_CameraIntro_SpinCircle_2.wav");
	m_pSoundCom->Set_Sound(L"CameraIntro_Ball", L"Bardiel_CameraIntro_Ball.wav");
	m_pSoundCom->Set_Sound(L"CameraIntro_Voice_1", L"Bardiel_CameraIntro_Voice_1.wav");
	m_pSoundCom->Set_Sound(L"CameraIntro_Voice_2", L"Bardiel_CameraIntro_Voice_2.wav");
	m_pSoundCom->Set_Sound(L"CameraIntro_Voice_3", L"Bardiel_CameraIntro_Voice_3.wav");
	m_pSoundCom->Set_Sound(L"CameraIntro_Weapon", L"Bardiel_CameraIntro_Weapon.wav");

	m_pSoundCom->Set_Sound(L"Bardiel_Penalty_FootStep", L"Bardiel_Penalty_FootStep.wav");
	m_pSoundCom->Set_Sound(L"Bardiel_Penalty_FootStep_2", L"Bardiel_Penalty_FootStep_2.wav");
	m_pSoundCom->Set_Sound(L"Bardiel_Penalty_Explosion", L"Bardiel_Penalty_Explosion.wav");
	m_pSoundCom->Set_Sound(L"Bardiel_Penalty_Voice", L"Bardiel_Penalty_Voice.wav");

	m_pSoundCom->Set_Sound(L"Bardiel_Lava_Voice", L"Bardiel_Lava_Voice.wav");
	m_pSoundCom->Set_Sound(L"Bardiel_Lava_Fire", L"Bardiel_Lava_Fire.wav");
	m_pSoundCom->Set_Sound(L"Bardiel_Lava_Fire_2", L"Bardiel_Lava_Fire_2.wav");

	m_pSoundCom->Set_Sound(L"Bardiel_FireBall_Fire", L"Bardiel_FireBall_Fire.wav");
	m_pSoundCom->Set_Sound(L"Bardiel_FireBall_Voice", L"Bardiel_FireBall_Voice.wav");

	m_pSoundCom->Set_Sound(L"Bardiel_FireFrame_Voice", L"Bardiel_FireFrame_Voice.wav");
	m_pSoundCom->Set_Sound(L"Bardiel_FireFrame_Fire_1", L"Bardiel_FireFrame_Fire_1.wav");
	m_pSoundCom->Set_Sound(L"Bardiel_FireFrame_Fire_2", L"Bardiel_FireFrame_Fire_2.wav");
	m_pSoundCom->Set_Sound(L"Bardiel_FireFrame_Fire_3", L"Bardiel_FireFrame_Fire_3.wav");

	m_pSoundCom->Set_Sound(L"Bardiel_FirePillar_Voice", L"Bardiel_FirePillar_Voice.wav");
	m_pSoundCom->Set_Sound(L"Bardiel_FirePillar_Fire_1", L"Bardiel_FirePillar_Fire_1.wav");
	m_pSoundCom->Set_Sound(L"Bardiel_FirePillar_Fire_2", L"Bardiel_FirePillar_Fire_2.wav");
	m_pSoundCom->Set_Sound(L"Bardiel_FirePillar_Fire_3", L"Bardiel_FirePillar_Fire_3.wav");
	m_pSoundCom->Set_Sound(L"Bardiel_FirePillar_EarthCrack", L"Bardiel_FirePillar_EarthCrack.wav");

	m_pSoundCom->Set_Sound(L"Bardiel_ChangePhase2_Voice", L"Bardiel_ChangePhase2_Voice.wav");
	m_pSoundCom->Set_Sound(L"Bardiel_ChangePhase2_Stamp", L"Bardiel_ChangePhase2_Stamp.wav");
	m_pSoundCom->Set_Sound(L"Bardiel_ChangePhase2_Weapon", L"Bardiel_ChangePhase2_Weapon.wav");
	m_pSoundCom->Set_Sound(L"Bardiel_ChangePhase2_Star", L"Bardiel_ChangePhase2_Star.wav");
	m_pSoundCom->Set_Sound(L"Bardiel_ChangePhase2_Burf", L"Bardiel_ChangePhase2_Burf.wav");

	m_pSoundCom->Set_Sound(L"Bardiel_Hammer_Voice", L"Bardiel_Hammer_Voice.wav");
	m_pSoundCom->Set_Sound(L"Bardiel_Hammer_Stamp_1", L"Bardiel_Hammer_Stamp_1.wav");
	m_pSoundCom->Set_Sound(L"Bardiel_Hammer_Stamp_2", L"Bardiel_Hammer_Stamp_2.wav");
	m_pSoundCom->Set_Sound(L"Bardiel_Hammer_Stamp_3", L"Bardiel_Hammer_Stamp_3.wav");
	
	m_pSoundCom->Set_Sound(L"Bardiel_WhirlWind_Swing", L"Bardiel_WhirlWind_Swing.wav");
	m_pSoundCom->Set_Sound(L"Bardiel_WhirlWind_Voice", L"Bardiel_WhirlWind_Voice.wav");

	m_pSoundCom->Set_Sound(L"Bardiel_Volcano_Voice", L"Bardiel_Volcano_Voice.wav");

	m_pSoundCom->Set_Sound(L"Bardiel_ShortHammer_Voice", L"Bardiel_ShortHammer_Voice.wav");

	m_pSoundCom->Set_Sound(L"Bardiel_LFoot_1", L"Bardiel_LFoot_1.wav");
	m_pSoundCom->Set_Sound(L"Bardiel_LFoot_2", L"Bardiel_LFoot_2.wav");

	m_pSoundCom->Set_Sound(L"Bardiel_FireWall_Stone_1", L"Bardiel_FireWall_Stone_1.wav");
	m_pSoundCom->Set_Sound(L"Bardiel_FireWall_Stone_2", L"Bardiel_FireWall_Stone_2.wav");
	m_pSoundCom->Set_Sound(L"Bardiel_FireWall_Voice", L"Bardiel_FireWall_Voice.wav");

	m_pSoundCom->Set_Sound(L"Bardiel_Divide_Voice", L"Bardiel_Divide_Voice.wav");

	m_pSoundCom->Set_Sound(L"Bardiel_Clean_Voice_1", L"Bardiel_Clean_Voice_1.wav");
	m_pSoundCom->Set_Sound(L"Bardiel_Clean_Voice_2", L"Bardiel_Clean_Voice_2.wav");


	m_pSoundCom->Set_Sound(L"Bardiel_Swing_1", L"Bardiel_Swing_1.wav");
	m_pSoundCom->Set_Sound(L"Bardiel_Swing_2", L"Bardiel_Swing_2.wav");
	m_pSoundCom->Set_Sound(L"Bardiel_Swing_3", L"Bardiel_Swing_3.wav");
	m_pSoundCom->Set_Sound(L"Bardiel_Swing_4", L"Bardiel_Swing_4.wav");
	m_pSoundCom->Set_Sound(L"Bardiel_Swing_5", L"Bardiel_Swing_5.wav");

	m_pSoundCom->Set_Sound(L"Bardiel_Stamp_1", L"Bardiel_Stamp_1.wav");

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
	pAI_State = CAI_CameraIntro_Bardiel::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pAICom, m_pSoundCom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_CAMERAINTRO), pAI_State);

	pAI_State = CAI_Idle1_Bardiel::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pCalculatorCom, m_pRigidbodyCom,
		m_pColliderCom, &m_tColInfo, &m_bDamaged, m_pAICom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_IDLE_1), pAI_State);

	pAI_State = CAI_Combat1_Bardiel::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pCalculatorCom, m_pRigidbodyCom,
		m_pColliderCom, &m_tColInfo, &m_bDamaged, m_pAICom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_COMBAT), pAI_State);

	pAI_State = CAI_Penalty1_Bardiel::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pCalculatorCom, m_pRigidbodyCom,
		m_pColliderCom, &m_tColInfo, &m_bDamaged, m_pAICom, m_pSoundCom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	pAI_State->Set_SphereCol(m_pSphereColCom);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_PENALTY), pAI_State);

	pAI_State = CAI_Lava1_Bardiel::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pCalculatorCom, m_pRigidbodyCom,
		m_pColliderCom, &m_tColInfo, &m_bDamaged, m_pAICom, m_pSoundCom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	pAI_State->Set_SphereCol(m_pSphereColCom);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_LAVA), pAI_State);

	pAI_State = CAI_FireBall1_Bardiel::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pCalculatorCom, m_pRigidbodyCom,
		m_pColliderCom, &m_tColInfo, &m_bDamaged, m_pAICom, m_pSoundCom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	pAI_State->Set_SphereCol(m_pSphereColCom);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_FIREBALL), pAI_State);
	
	pAI_State = CAI_FireFrame1_Bardiel::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pCalculatorCom, m_pRigidbodyCom,
		m_pColliderCom, &m_tColInfo, &m_bDamaged, m_pAICom, m_pSoundCom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	pAI_State->Set_SphereCol(m_pSphereColCom);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_FIREFRAME), pAI_State);

	pAI_State = CAI_FirePillar1_Bardiel::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pCalculatorCom, m_pRigidbodyCom,
		m_pColliderCom, &m_tColInfo, &m_bDamaged, m_pAICom, m_pSoundCom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	pAI_State->Set_SphereCol(m_pSphereColCom);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_FIREPILLAR), pAI_State);

	pAI_State = CAI_ChangePhase2_Bardiel::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pAICom, m_pSoundCom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_CHANGEPHASE2), pAI_State);

	pAI_State = CAI_Idle2_Bardiel::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pCalculatorCom, m_pRigidbodyCom,
		m_pColliderCom, &m_tColInfo, &m_bDamaged, m_pAICom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_IDLE_2), pAI_State);

	pAI_State = CAI_Combat2_Bardiel::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pCalculatorCom, m_pRigidbodyCom,
		m_pColliderCom, &m_tColInfo, &m_bDamaged, m_pAICom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_COMBAT_2), pAI_State);


	pAI_State = CAI_WhirlWind2_Bardiel::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pCalculatorCom, m_pRigidbodyCom,
		m_pColliderCom, &m_tColInfo, &m_bDamaged, m_pAICom, m_pSoundCom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	pAI_State->Set_SphereCol(m_pSphereColCom);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_WHIRLWIND), pAI_State);

	pAI_State = CAI_Volcano2_Bardiel::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pCalculatorCom, m_pRigidbodyCom,
		m_pColliderCom, &m_tColInfo, &m_bDamaged, m_pAICom, m_pSoundCom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	pAI_State->Set_SphereCol(m_pSphereColCom);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_VOLCANO), pAI_State);

	pAI_State = CAI_ShortHammer2_Bardiel::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pCalculatorCom, m_pRigidbodyCom,
		m_pColliderCom, &m_tColInfo, &m_bDamaged, m_pAICom, m_pSoundCom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	pAI_State->Set_SphereCol(m_pSphereColCom);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_SHORTHAMMER), pAI_State);

	pAI_State = CAI_LFoot2_Bardiel::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pCalculatorCom, m_pRigidbodyCom,
		m_pColliderCom, &m_tColInfo, &m_bDamaged, m_pAICom, m_pSoundCom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	pAI_State->Set_SphereCol(m_pSphereColCom);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_LFOOT), pAI_State);

	pAI_State = CAI_Hammer2_Bardiel::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pCalculatorCom, m_pRigidbodyCom,
		m_pColliderCom, &m_tColInfo, &m_bDamaged, m_pAICom, m_pSoundCom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	pAI_State->Set_SphereCol(m_pSphereColCom);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_HAMMER), pAI_State);

	pAI_State = CAI_FireWall2_Bardiel::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pCalculatorCom, m_pRigidbodyCom,
		m_pColliderCom, &m_tColInfo, &m_bDamaged, m_pAICom, m_pSoundCom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	pAI_State->Set_SphereCol(m_pSphereColCom);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_FIREWALL), pAI_State);

	pAI_State = CAI_FireRain2_Bardiel::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pCalculatorCom, m_pRigidbodyCom,
		m_pColliderCom, &m_tColInfo, &m_bDamaged, m_pAICom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	pAI_State->Set_SphereCol(m_pSphereColCom);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_FIRERAIN), pAI_State);

	pAI_State = CAI_Divide2_Bardiel::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pCalculatorCom, m_pRigidbodyCom,
		m_pColliderCom, &m_tColInfo, &m_bDamaged, m_pAICom, m_pSoundCom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	pAI_State->Set_SphereCol(m_pSphereColCom);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_DIVIDE), pAI_State);

	pAI_State = CAI_Clean2_Bardiel::Create(m_pTransCom, m_pAnimatorCom, m_pNaviCom, m_pCalculatorCom, m_pRigidbodyCom,
		m_pColliderCom, &m_tColInfo, &m_bDamaged, m_pAICom, m_pSoundCom);
	pAI_State->Set_AniInfo(&m_vecAniInfo);
	pAI_State->Set_SphereCol(m_pSphereColCom);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_CLEAN), pAI_State);

	//AI셋팅
	m_eAIState = AI_STATE::AI_CAMERAINTRO;
	m_pAnimatorCom->Set_AnimationSet(IDLE_1);
	m_pAnimatorCom->Set_State(MONSTER_NORMAL);
	m_pAnimatorCom->Set_AnimationSet(IDLE_1);
	m_pAICom->Set_State(m_eAIState);

	m_pAICom->Set_StartState();

	m_bFadeName = false;
	//----------------
	m_pMeshCom->SetBardiel_HandGunBlurTex(m_pBlur_HandGun_TransCom);
	m_pMeshCom->SetBardiel_BodyBlurTex(m_pBlur_Body_TransCom);
	m_pMeshCom->SetBardiel_ShieldBlurTex(m_pBlur_Shield_TransCom);
	//----------------	

	m_iBlurCnt = 1;
	m_fBlurPower = 1.1f;
	m_fBlurValue = 2000.0f;
	m_eBlurType = Engine::BLUR_CROSS;


	return S_OK;
}

void CBardiel::Update_Object(const _float& fTimeDelta)
{
	//Engine::CGameObject::Update_Object(fTimeDelta);
	CMonster::Update_Object(fTimeDelta);

	if (m_pAnimatorCom->Get_AnimationIndex() == WALK_F)
	{
		if (m_pAnimatorCom->IsPlaying_ThisSection(m_vecAniInfo[WALK_F]->tMotionInfo.dEndTime * 0.2
			, m_vecAniInfo[WALK_F]->tMotionInfo.dEndTime * 0.3))
		{
			m_pSoundCom->MyPlaySound(L"Walk1");
		}
		if (m_pAnimatorCom->IsPlaying_ThisSection(m_vecAniInfo[WALK_F]->tMotionInfo.dEndTime * 0.7
			, m_vecAniInfo[WALK_F]->tMotionInfo.dEndTime * 0.8))
		{
			m_pSoundCom->MyPlaySound(L"Walk2");
		}
	}

	if (m_pAnimatorCom->Get_AnimationIndex() == WALK_F_2)
	{
		if (m_pAnimatorCom->IsPlaying_ThisSection(m_vecAniInfo[WALK_F_2]->tMotionInfo.dEndTime * 0.2
			, m_vecAniInfo[WALK_F_2]->tMotionInfo.dEndTime * 0.3))
		{
			m_pSoundCom->MyPlaySound(L"Walk1");
		}
		if (m_pAnimatorCom->IsPlaying_ThisSection(m_vecAniInfo[WALK_F_2]->tMotionInfo.dEndTime * 0.7
			, m_vecAniInfo[WALK_F_2]->tMotionInfo.dEndTime * 0.8))
		{
			m_pSoundCom->MyPlaySound(L"Walk2");
		}
	}

	if (m_pAnimatorCom->Get_AnimationIndex() == WALK_L)
	{
		if (m_pAnimatorCom->IsPlaying_ThisSection(m_vecAniInfo[WALK_L]->tMotionInfo.dEndTime * 0.2
			, m_vecAniInfo[WALK_L]->tMotionInfo.dEndTime * 0.3))
		{
			m_pSoundCom->MyPlaySound(L"Walk1");
		}
		if (m_pAnimatorCom->IsPlaying_ThisSection(m_vecAniInfo[WALK_L]->tMotionInfo.dEndTime * 0.7
			, m_vecAniInfo[WALK_L]->tMotionInfo.dEndTime * 0.8))
		{
			m_pSoundCom->MyPlaySound(L"Walk2");
		}
	}

	if (m_pAnimatorCom->Get_AnimationIndex() == WALK_R)
	{
		if (m_pAnimatorCom->IsPlaying_ThisSection(m_vecAniInfo[WALK_R]->tMotionInfo.dEndTime * 0.2
			, m_vecAniInfo[WALK_R]->tMotionInfo.dEndTime * 0.3))
		{
			m_pSoundCom->MyPlaySound(L"Walk1");
		}
		if (m_pAnimatorCom->IsPlaying_ThisSection(m_vecAniInfo[WALK_R]->tMotionInfo.dEndTime * 0.7
			, m_vecAniInfo[WALK_R]->tMotionInfo.dEndTime * 0.8))
		{
			m_pSoundCom->MyPlaySound(L"Walk2");
		}
	}
	if (m_pAnimatorCom->Get_AnimationIndex() == RUN_WEAPON)
	{
		if (m_pAnimatorCom->IsPlaying_ThisSection(m_vecAniInfo[RUN_WEAPON]->tMotionInfo.dEndTime * 0.0
			, m_vecAniInfo[RUN_WEAPON]->tMotionInfo.dEndTime * 0.1))
		{
			if (!m_bWalk[0])
			{
				m_pSoundCom->MyPlaySound(L"Walk1", false, true);
				m_bWalk[0] = true;
			}

		}
		else
			m_bWalk[0] = false;
		if (m_pAnimatorCom->IsPlaying_ThisSection(m_vecAniInfo[RUN_WEAPON]->tMotionInfo.dEndTime * 0.5
			, m_vecAniInfo[RUN_WEAPON]->tMotionInfo.dEndTime * 0.6))
		{
			if (!m_bWalk[1])
			{
				m_pSoundCom->MyPlaySound(L"Walk2", false, true);
				m_bWalk[1] = true;
			}
		}
		else
			m_bWalk[1] = false;

	}
	
	_vec3 vTargetPos;
	vTargetPos = m_pTargetTransCom->Get_Now_Position();
	_vec3 vMyPos;
	vMyPos = m_pTransCom->Get_Now_Position();

	_vec3 vDis;
	vDis = vTargetPos - vMyPos;

	//m_pMeshCom->Play_AnimationSet(fTimeDelta);

	if (m_bGravityStart)
	{
		if (m_pCalculatorCom->Get_IsCol())
		{
			m_pRigidbodyCom->Set_Gravity(false);
		}
		else
		{
			m_pRigidbodyCom->Set_Gravity(true);
		}
	}

	if (m_tMonInfo.iHp <= 0)
	{
		m_bDeath = TRUE;
	}

	if (m_bDeath)
	{
		m_fRadialTime += fTimeDelta;
		if (1.5f > m_fRadialTime)
		{
			m_fDeathRadialBlur -= 0.005f;
			if (m_fDeathRadialBlur < -0.5f)
				m_fDeathRadialBlur = -0.5f;

			Engine::SetRadialBlur(TRUE);
			Engine::SetRadialBlurPower(m_fDeathRadialBlur);
		}

		if (1.5f <= m_fRadialTime && 3.0f > m_fRadialTime)
		{
			m_fDeathRadialBlur += 0.005f;
			if (m_fDeathRadialBlur > 0.0f)
				m_fDeathRadialBlur = 0.0f;

			Engine::SetRadialBlurPower(m_fDeathRadialBlur);
		}

		if (m_fRadialTime > 3.f)
		{
			Engine::SetRadialBlur(FALSE);
			Engine::SetRadialBlurPower(-0.25f);
			m_eState = Engine::STATE_DESTROY;
		}
	}

	//
	/*if (m_pAnimatorCom->Get_AnimationIndex == WAKEUP)
	{
		if (m_pAnimatorCom->IsPlaying_ThisSection((m_vecAniInfo)[WAKEUP]->tMotionInfo.dStartTime + (m_vecAniInfo)[WAKEUP]->tMotionInfo.dEndTime * 0.1,
			(m_vecAniInfo)[WAKEUP]->tMotionInfo.dStartTime + (m_vecAniInfo)[WAKEUP]->tMotionInfo.dEndTime * 0.3))
		{
			
		}
	}*/
	if (!m_bWeapon1)
	{
		if (m_pAnimatorCom->Get_AnimationIndex() == SUMMON)
		{
			if (m_pAnimatorCom->IsPlaying_ThisSection((m_vecAniInfo)[SUMMON]->tMotionInfo.dStartTime + (m_vecAniInfo)[SUMMON]->tMotionInfo.dEndTime * 0.86f,
				(m_vecAniInfo)[SUMMON]->tMotionInfo.dStartTime + (m_vecAniInfo)[SUMMON]->tMotionInfo.dEndTime * 0.93f))
			{
				m_bWeapon1 = true;
				m_pMeshCom->SetBardiel_Weapon_1_Check(m_bWeapon1);
			}
		}
	}

	if (!m_bArmor)
	{
		if (m_pAnimatorCom->Get_AnimationIndex() == CHANGEPHASE2)
		{
			m_bWeapon1 = false;
			m_bWeapon2 = true;

			m_pMeshCom->SetBardiel_Weapon_1_Check(m_bWeapon1);
			m_pMeshCom->SetBardiel_Weapon_2_Check(m_bWeapon2);

			if (m_pAnimatorCom->IsPlaying_ThisSection((m_vecAniInfo)[CHANGEPHASE2]->tMotionInfo.dStartTime + (m_vecAniInfo)[CHANGEPHASE2]->tMotionInfo.dEndTime * 0.2f,
				(m_vecAniInfo)[CHANGEPHASE2]->tMotionInfo.dStartTime + (m_vecAniInfo)[CHANGEPHASE2]->tMotionInfo.dEndTime * 0.31f))
			{
				m_bArmor = true;

				m_pMeshCom->SetBardiel_Armor_Check(m_bArmor);
			}
		}
	}

	//phase2
	if (!m_bPhase2)
	{
		if (m_tMonInfo.iHp < m_tMonInfo.iMaxHp * 0.5f)
		{
			if (m_pAnimatorCom->Set_AnimationSet(CHANGEPHASE2))
			{
				m_pAICom->Set_State(AI_CHANGEPHASE2);
				m_bPhase2 = true;
			}
		}
	}

	if (Engine::IsCollision(L"Player", &m_tColInfo, m_pColliderCom))
	{
		m_tMonInfo.iHp -= m_tColInfo.iDamage;
		Collision_CameraWork();
		CNumbering_Manager::GetInstance()->Increase_ComboCount();
		m_bDamaged = true;
		((CDamParent*)m_pDamParent)->SetDamageNumber(m_tColInfo.iDamage, m_tColInfo.bCritical);
	}
	else
		m_bDamaged = false;

	if (m_pRendererCom->GetShadowBuffer() ||
		m_pRendererCom->GetSoftShadow() ||
		m_pRendererCom->GetCACDShadow())
	{
		m_pRendererCom->Add_RenderGroup(Engine::RENEDER_DYANAMIC_SHADOW, this);
	}

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_DYNAMIC, this);


	Engine::Add_ColliderGroup(Engine::COL_BOX, L"Monster", m_pColliderCom);


	/*if (m_bCriRadialBlur)
	{
		Engine::SetRadialBlur(TRUE);

		m_bCriRadialTime += fTimeDelta;
		if (m_bCriRadialTime >= 0.25f)
		{
			m_bCriRadialTime = 0.0f;
			Engine::SetRadialBlur(FALSE);
			m_bCriRadialBlur = FALSE;
		}
	}*/

	//피격후 지면과의 충돌 이펙트
	CollideEffectDust(fTimeDelta);

	if (!m_bGravityStart)
	{
		if (m_pAnimatorCom->Get_AnimationIndex() == SUMMON)
			m_bGravityStart = true;
	}
	
	//Motion Blur============================================================
	if (m_pRendererCom->GetMotionBlurBuffer())
	{
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_MOTION_BLUR, this);
		m_pMeshCom->UpdateTimeDelta(fTimeDelta);
	}
	//=======================================================================

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_POSTEFFECT_BLUR, this);

	_matrix matScale, matRot_X, matRot_Y, matRot_Z, matTrans;

	D3DXMatrixScaling(&matScale, 1.0075f, 1.0075f, 1.0075f);
	D3DXMatrixRotationX(&matRot_X, D3DXToRadian(m_pTransCom->Get_Now_Angle().x));
	D3DXMatrixRotationY(&matRot_Y, D3DXToRadian(m_pTransCom->Get_Now_Angle().y));
	D3DXMatrixRotationZ(&matRot_Z, D3DXToRadian(m_pTransCom->Get_Now_Angle().z));
	D3DXMatrixTranslation(&matTrans, m_pTransCom->Get_Now_Position().x, m_pTransCom->Get_Now_Position().y, m_pTransCom->Get_Now_Position().z);

	m_matEffectWorld = matScale * matRot_X * matRot_Y * matRot_Z * matTrans;

	//_vec3	vScale;

	//Compute_ViewZ(m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &vScale));

	BlurPowerControl(fTimeDelta);
}

void CBardiel::Render_Object(void)
{
	if (m_pSphereColCom)
		m_pSphereColCom->Render_Collider(g_bDebugRender);

	if (m_pColliderCom)
		m_pColliderCom->Render_Collider(g_bDebugRender);
}

void CBardiel::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
{
	if (Engine::GetMRTID() == Engine::MRT_POSTEFFECT_BLUR)
	{
		pEffect->AddRef();

		if (FAILED(Set_ConstantTable(pEffect)))
			return;

		Engine::SetUp_OnShader(L"Target_DepthMap", pEffect, "g_DepthTexture");
		pEffect->SetVector("g_vColor", &_vec4((1.0f), (-0.3f), (-0.9f), 0.0f));
		
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

HRESULT CBardiel::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	if (FAILED(LoadAniInfo(L"Bardiel_Ani")))//
		return E_FAIL;

	if (FAILED(LoadCollider(L"Bardiel_Collider")))
		return E_FAIL;

	m_iMonsterAniState = MONSTER_NORMAL;

	m_eLayerType = Engine::LAYER_GAMELOGIC;
	m_pTransCom->Set_Position(0.0f, 0.0f, 0.0f);

	m_fAlpha = 1.f;

	m_tMonInfo.iMaxHp = 3500000;
	m_tMonInfo.iHp = m_tMonInfo.iMaxHp;
	m_tMonInfo.iAtt = 10;
	m_tMonInfo.m_eMonID = MON_BARDIEL;
	////애니메이션 상태 준비
	Engine::CAnimationState* pState = NULL;

	pState = CBardiel_Ani::Create(m_pMeshCom, &m_pmapNextMotion[MONSTER_NORMAL], IDLE_1);
	if (pState == NULL)
		return E_FAIL;
	m_pAnimatorCom->Set_State(MONSTER_NORMAL, pState);

	pState = CBardiel_Ani::Create(m_pMeshCom, &m_pmapNextMotion[MONSTER_FIGHT], BATTLE_IDLE_1);
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

HRESULT CBardiel::Add_Component(void)
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

	pComponent = (Engine::CDynamicMesh*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, szName);
	m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"DynamicMesh", m_pMeshCom));
	static_cast<Engine::CDynamicMesh*>(m_pMeshCom)->SetstrName(m_wstrName);

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
	//pComponent = Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Navigation");
	//if (pComponent == NULL)
	//	return E_FAIL;
	//m_pNaviCom = (Engine::CNavigationMesh*)pComponent;
	//m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"NaviCom", pComponent));

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
	m_pBlur_Body_TransCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Bardiel_Body_Blur_Effect");
	pComponent = (Engine::CComponent*)m_pBlur_Body_TransCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Body_Blur_Texture", pComponent));

	m_pBlur_Shield_TransCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Bardiel_Shield_Blur_Effect");
	pComponent = (Engine::CComponent*)m_pBlur_Shield_TransCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Shield_Blur_Texture", pComponent));

	m_pBlur_HandGun_TransCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Bardiel_HandGun_Blur_Effect");
	pComponent = (Engine::CComponent*)m_pBlur_HandGun_TransCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_HandGun_Blur_Texture", pComponent));

	//Sound
	pComponent = CSound::Create(m_pTransCom);
	m_pSoundCom = (CSound*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Sound", pComponent));

	return S_OK;
}

HRESULT CBardiel::Set_ConstantTable(LPD3DXEFFECT pEffect)
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

	//pEffect->SetMatrix("g_matWorld", &matWorld);
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
	pEffect->SetFloat("g_fPower", 130.0f);

	D3DXMatrixInverse(&matView, NULL, &matView);
	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView._41);
	pEffect->SetFloat("g_fAlpha", m_fAlpha);

	float fFar = Engine::GetCurrentCamera()->GetCamFar();
	pEffect->SetFloat("g_fFar", fFar);

	pEffect->SetBool("g_bBlurCheck", Engine::GetMotionBlurBuffer());
	//pEffect->SetBool("g_bDynamicDepth", true);
	pEffect->SetBool("g_bOutline", m_bOutlineCheck);
	pEffect->SetFloat("g_fColorCheck", 0.0f);
	pEffect->SetFloat("g_fOutlineThickness", 0.00045f);

	/*if (m_bOutlineCheck)
	{
		pEffect->SetVector("g_vColor", &_vec4(m_bOutlineColor, 0.0f));
		pEffect->SetFloat("g_bOutlinePower", 2.5f);

	}*/

	pEffect->SetBool("g_bLimLight", false);

	if (Engine::GetLimLight())
	{
		pEffect->SetFloat("g_bOutlinePower", 150.0f);
		pEffect->SetVector("g_vColor", &_vec4(0.4f, 0.0f, 0.0f, 0.0f));
	}

	Engine::Safe_Release(pEffect);

	return S_OK;
}

HRESULT CBardiel::Set_Shadow_ConstanTable(LPD3DXEFFECT pEffect)
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

HRESULT CBardiel::Set_CACD_Shadow_ConstanTable(LPD3DXEFFECT pEffect)
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


void CBardiel::Render_Ray(void)
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
void CBardiel::InitEffectDust(void)
{
	m_pDust = CSprite::LoadCreate(m_pGraphicDev, L"MonsterDust");
	m_pDust->SetAni(false);
	m_pDust->GetTransCom()->Set_Scale(1.0f, 1.0f, 1.0f);

	D3DXCOLOR color = D3DXVECTOR4(0.2f, 0.2f, 0.2f, 0.6f);
	m_pDust->GetParticleInfo()->m_dColor = color;
	m_pDust->GetParticleInfo()->m_iPath = 3;
}

//충돌 이펙트 관리 함수
void CBardiel::CollideEffectDust(const _float& fTimeDelta)
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

CBardiel* CBardiel::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
{
	CBardiel*		pInstance = new CBardiel(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CBardiel Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CBardiel::Free(void)
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

HRESULT CBardiel::LoadAniInfo(const _tchar* pFileName)
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

void CBardiel::BlurPowerControl(const _float & fTimeDelta)
{
	if (m_pAnimatorCom->Get_AnimationIndex() == IDLE_1)
	{
		m_fEffectPower = 0.0f;
	}
	else if (m_pAnimatorCom->Get_AnimationIndex() == WAKEUP)
	{
		m_fEffectPowerTime += fTimeDelta;

		if (m_fEffectPowerTime >= 0.0f && m_fEffectPowerTime < 23.996309f)
		{
			m_fEffectPower = 0.0f;
		}

		if (m_fEffectPowerTime >= 23.996309f)
		{
			m_fEffectPower += 0.02f;
			if (m_fEffectPower > 1.1f)
				m_fEffectPower = 1.1f;
		}
	}
	else if (m_pAnimatorCom->Get_AnimationIndex() == SUMMON)
	{
		m_fEffectPowerTime += fTimeDelta;

		if (m_fEffectPowerTime >= 4.0343334f && m_fEffectPowerTime < 16.686767f)
		{
			m_fEffectPower += 0.1f;
			if (m_fEffectPower > 6.5f)
				m_fEffectPower = 6.5f;
		}
		
		if (m_fEffectPowerTime >= 16.686767f)
		{
			m_fEffectPower -= 0.1f;
			if (m_fEffectPower < 1.1f)
				m_fEffectPower = 1.1f;
		}

		if (m_fEffectPowerTime >= 19.534666f && m_fEffectPowerTime < 20.171666f)
		{
			m_fEffectPower += 1.0f;
			if (m_fEffectPower > 6.5f)
				m_fEffectPower = 6.5f;
		}

		if (m_fEffectPowerTime >= 20.171666f)
		{
			m_fEffectPower -= 0.3f;
			if (m_fEffectPower < 1.1f)
				m_fEffectPower = 1.1f;
		}
	}
	//Phase 1 ------------------------------------------------------------------------------
	else if (m_pAnimatorCom->Get_AnimationIndex() == LAVA)
	{
		m_fEffectPowerTime += fTimeDelta;

		if (m_fEffectPowerTime >= 0.756f && m_fEffectPowerTime < 3.1679999f)
		{
			m_fEffectPower += 0.1f;
			if (m_fEffectPower > 6.5f)
				m_fEffectPower = 6.5f;
		}

		if (m_fEffectPowerTime >= 3.1679999f)
		{
			m_fEffectPower -= 0.1f;
			if (m_fEffectPower < 1.1f)
				m_fEffectPower = 1.1f;
		}
	}
	else if (m_pAnimatorCom->Get_AnimationIndex() == FIREBALL)
	{
		m_fEffectPowerTime += fTimeDelta;

		if (m_fEffectPowerTime >= 1.9763333f && m_fEffectPowerTime < 2.984666f)
		{
			m_fEffectPower += 0.1f;
			if (m_fEffectPower > 6.5f)
				m_fEffectPower = 6.5f;
		}

		if (m_fEffectPowerTime >= 2.984666f)
		{
			m_fEffectPower -= 0.1f;
			if (m_fEffectPower < 1.1f)
				m_fEffectPower = 1.1f;
		}
	}
	else if (m_pAnimatorCom->Get_AnimationIndex() == FIREPILLAR_ING_1)
	{
		m_fEffectPowerTime += fTimeDelta;

		if (m_fEffectPowerTime >= 0.0f && m_fEffectPowerTime < 1.566667f)
		{
			m_fEffectPower += 0.1f;
			if (m_fEffectPower > 6.5f)
				m_fEffectPower = 6.5f;
		}
	}
	else if (m_pAnimatorCom->Get_AnimationIndex() == FIREPILLAR_ING2)
	{
		m_fEffectPowerTime += fTimeDelta;

		if (m_fEffectPowerTime >= 0.0f && m_fEffectPowerTime < 0.8f)
		{
			m_fEffectPower += 0.1f;
			if (m_fEffectPower > 6.5f)
				m_fEffectPower = 6.5f;
		}
	}
	else if (m_pAnimatorCom->Get_AnimationIndex() == FIREPILLAR_ING3)
	{
		m_fEffectPowerTime += fTimeDelta;

		if(m_fEffectPowerTime >= 0.0f && m_fEffectPowerTime < 1.53333f)
		{
			m_fEffectPower += 0.1f;
			if (m_fEffectPower > 6.5f)
				m_fEffectPower = 6.5f;
		}
	}
	else if (m_pAnimatorCom->Get_AnimationIndex() == FIREPILLAR_END)
	{
		m_fEffectPowerTime += fTimeDelta;

		if (m_fEffectPowerTime >= 0.0f)
		{
			m_fEffectPower -= 0.1f;
			if (m_fEffectPower < 1.1f)
				m_fEffectPower = 1.1f;
		}
	}
	else if (m_pAnimatorCom->Get_AnimationIndex() == FIREFRAME_A)
	{
		m_fEffectPowerTime += fTimeDelta;

		if (m_fEffectPowerTime >= 2.288f && m_fEffectPowerTime < 4.400001f)
		{
			m_fEffectPower += 0.1f;
			if (m_fEffectPower > 6.5f)
				m_fEffectPower = 6.5f;
		}
	}
	else if (m_pAnimatorCom->Get_AnimationIndex() == FIREFRAME_B)
	{
		m_fEffectPowerTime += fTimeDelta;
	}
	else if (m_pAnimatorCom->Get_AnimationIndex() == FIREFRAME_C)
	{
		m_fEffectPowerTime += fTimeDelta;
	}
	else if (m_pAnimatorCom->Get_AnimationIndex() == FIREFRAME_D)
	{
		m_fEffectPowerTime += fTimeDelta;
	}
	else if (m_pAnimatorCom->Get_AnimationIndex() == FIREFRAME_E)
	{
		m_fEffectPowerTime += fTimeDelta;

		if (m_fEffectPowerTime >= 0.475f)
		{
			m_fEffectPower -= 0.1f;
			if (m_fEffectPower < 1.1f)
				m_fEffectPower = 1.1f;
		}
	}
	else if (m_pAnimatorCom->Get_AnimationIndex() == PENALTY)
	{
		m_fEffectPowerTime += fTimeDelta;

		if (m_fEffectPowerTime >= 1.276666f && m_fEffectPowerTime < 2.936333f)
		{
			m_fEffectPower += 0.25f;
			if (m_fEffectPower > 6.5f)
				m_fEffectPower = 6.5f;
		}

		if (m_fEffectPowerTime >= 2.936333f)
		{
			m_fEffectPower -= 0.25f;
			if (m_fEffectPower < 1.1f)
				m_fEffectPower = 1.1f;
		}

		if (m_fEffectPowerTime >= 4.08533f && m_fEffectPowerTime < 5.106666f)
		{
			m_fEffectPower += 0.25f;
			if (m_fEffectPower > 6.5f)
				m_fEffectPower = 6.5f;
		}

		if (m_fEffectPowerTime >= 5.106666f)
		{
			m_fEffectPower -= 0.25f;
			if (m_fEffectPower < 1.1f)
				m_fEffectPower = 1.1f;
		}

		if (m_fEffectPowerTime >= 7.659998f && m_fEffectPowerTime < 8.298332f)
		{
			m_fEffectPower += 0.25f;
			if (m_fEffectPower > 6.5f)
				m_fEffectPower = 6.5f;
		}

		if (m_fEffectPowerTime >= 8.298332f)
		{
			m_fEffectPower -= 0.25f;
			if (m_fEffectPower < 1.1f)
				m_fEffectPower = 1.1f;
		}

		if (m_fEffectPowerTime >= 9.7026665f && m_fEffectPowerTime < 11.106999f)
		{
			m_fEffectPower += 0.25f;
			if (m_fEffectPower > 6.5f)
				m_fEffectPower = 6.5f;
		}

		if (m_fEffectPowerTime >= 11.106999f)
		{
			m_fEffectPower -= 0.25f;
			if (m_fEffectPower < 1.1f)
				m_fEffectPower = 1.1f;
		}
	}
	//--------------------------------------------------------------------------------------
	//Phase 2 ------------------------------------------------------------------------------	
	else if (m_pAnimatorCom->Get_AnimationIndex() == ATT_WHIRLWIND)
	{
	}
	else if (m_pAnimatorCom->Get_AnimationIndex() == ATT_VOLCANO)
	{
	}
	else if (m_pAnimatorCom->Get_AnimationIndex() == ATT_SHORTHAMMER)
	{
	}
	else if (m_pAnimatorCom->Get_AnimationIndex() == ATT_LFOOT)
	{
	}
	else if (m_pAnimatorCom->Get_AnimationIndex() == ATT_HAMMER)
	{
	}
	else if (m_pAnimatorCom->Get_AnimationIndex() == ATT_FIRE_WALL)
	{
	}
	else if (m_pAnimatorCom->Get_AnimationIndex() == ATT_DIVIDE)
	{
	}
	else if (m_pAnimatorCom->Get_AnimationIndex() == ATT_CLEAN)
	{
	}
	//--------------------------------------------------------------------------------------
	else
	{
		m_fEffectPowerTime = 0.0f;
		m_fEffectPower = 1.1f;
	}
}







