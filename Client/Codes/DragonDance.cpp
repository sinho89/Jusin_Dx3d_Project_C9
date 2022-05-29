#include "stdafx.h"
#include "DragonDance.h"
#include "Export_Engine.h"
#include "WitchBlade.h"
#include "CherryBlossom.h"
#include "PledgeRing.h"
#include "Wave.h"

CDragonDance::CDragonDance(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
:CEffect(pGraphicDev, wstrName)
,m_pRightLongTrail(NULL)
,m_pRightSmallTrail(NULL)
,m_pLeftLongTrail(NULL)
,m_pLeftSmallTrail(NULL)
,m_bRightTrailCheck(false)
,m_fRightLongGage(0.0f)
,m_fRightSmallGage(0.0f)
,m_bLeftTrailCheck(false)
,m_pTargetAnimator(NULL)
,m_fLeftLongGage(0.0f)
,m_fLeftSmallGage(0.0f)
{
	m_bIsStatic = true;
	m_bAni = false;
}

CDragonDance::~CDragonDance(void)
{

}


void CDragonDance::Update_Object(const _float & fTimeDelta)
{

	//Attack6 오른손 휘두르기
	//Attack7 왼손   휘두르기
	if (m_pPlayerAction[CWitchBlade::ACTION_DRAGONDANCE] == true)
	{
		m_bAni = true;

		_double dTime = (*m_pvecAniInfo)[CWitchBlade::WITCH_ATTACK6]->pMotionEventInfo[EVENT_EFFECT].dStartTime;
		_double dTargetTime = m_pTargetMesh->Get_TrackPos();

		//오른손으로 휘두르기
		if ( (1.21f <= dTargetTime) && (!m_bRightTrailCheck))
		{
			m_bRightTrailCheck = true;
			RightTrailStart(); //스킬 시작을 위한 초기 세팅
		}

		if (m_pTargetAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_ATTACK7)
		{
			//왼손으로 휘두르기
			if ( (0.952 <= dTargetTime) && (!m_bLeftTrailCheck))
			{
				m_bLeftTrailCheck = true;
				LeftTrailStart();
			}
		}
	}
	else //해당 스킬이 끝난경우
	{
		m_bRightTrailCheck = false;
		m_bLeftTrailCheck = false;
	}



	if (!m_bAni)
		return;

	EndAniCheck();

	RightTrailUpdate(fTimeDelta);
	RightTrailSmallUpdate(fTimeDelta);

	LeftTrailUpdate(fTimeDelta);
	LeftTrailSmallUpdate(fTimeDelta);
}

void CDragonDance::Render_Object(void)
{
	if (!m_bAni)
		return;

	m_pRightLongTrail->Render_Object();
	m_pRightSmallTrail->Render_Object();


	m_pLeftLongTrail->Render_Object();
	m_pLeftSmallTrail->Render_Object();

}

HRESULT CDragonDance::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	AllRightTrailInit();
	AllLeftTrailInit();

	return S_OK;
}


CDragonDance * CDragonDance::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CDragonDance*   pInstance = new CDragonDance(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("DragonDance Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CDragonDance::Free(void)
{
	CEffect::Free();
	Engine::Safe_Release(m_pRightLongTrail);
	Engine::Safe_Release(m_pRightSmallTrail);
	Engine::Safe_Release(m_pLeftLongTrail);
	Engine::Safe_Release(m_pLeftSmallTrail);

}

HRESULT CDragonDance::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CDragonDance::SkillStart(void)
{
}

void CDragonDance::EndAniCheck(void)
{
	if (m_bAni == true && m_pRightLongTrail->GetAni() == false && m_pRightSmallTrail->GetAni() == false &&
		m_pLeftLongTrail->GetAni() == false && m_pLeftSmallTrail->GetAni() == false)
		m_bAni = false;
}

HRESULT CDragonDance::Start_ForScene(void)
{
	if (m_bOnceStart)
		return S_OK;

	m_bOnceStart = true;

	Engine::CGameObject* pGameObject = Engine::FindWithTag(L"Player");

	if (pGameObject == NULL)
		return S_OK;

	m_pPlayerAction = ((CWitchBlade*)pGameObject)->Get_IsSkill(PLAYER_FIGHT);


	m_pTargetAnimator = ((CWitchBlade*)pGameObject)->GetAnimatorInfo();


	Engine::CTransform* pTransform = (Engine::CTransform*)pGameObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

	m_pTargetTransform = pTransform;

	//Target Mesh
	m_pTargetMesh = (Engine::CDynamicMesh*)pGameObject->GetComponent<Engine::CDynamicMesh>(Engine::COMID_STATIC);

	//애니메이션 정보를 가져온다.
	m_pvecAniInfo = ((CWitchBlade*)pGameObject)->Get_AniInfo();


	Engine::Safe_Release(pTransform);
	Engine::Safe_Release(pGameObject);
	return S_OK;
}

//오른쪽 가장 큰 Trail Update
void CDragonDance::RightTrailUpdate(const _float & fTimeDelta)
{
	if (m_pRightLongTrail->GetAni() == false)
		return;

	m_pRightLongTrail->Update_Object(fTimeDelta);


	_matrix mMat, RightLongMat;

	_vec3 vPosition;
	//로컬상의 위치를잡아준다. //Player에 붙어서 이동하기 때문에....
	vPosition.x = 0.f;
	vPosition.y = 3.f;
	vPosition.z = 0.f;

	//회전==============================================================
	m_pRightLongTrail->GetTransCom()->Rotation(Engine::ROT_Y, 20.f);
	m_fRightLongGage += 20.f;
	//================================================================

	m_pRightLongTrail->GetTransCom()->Set_Position(&vPosition);
	m_pRightLongTrail->GetTransCom()->Get_WorldMatrix(&RightLongMat);
	m_pTargetTransform->Get_WorldMatrix(&mMat);

	RightLongMat = RightLongMat * mMat;

	m_pRightLongTrail->GetTransCom()->Set_WorldMatrix(&RightLongMat);

	_float fGage = m_pRightLongTrail->GetGage();

	fGage = max(fGage - 0.5f, 0.f);


	m_pRightLongTrail->SetGage(fGage);

	//회전게이지가 300이상이면 Ani = false;
	//트레일을 종료하기 위한 변수
	if (m_fRightLongGage >= 300.f)
	{
		m_pRightLongTrail->SetGage(1.f);
		m_pRightLongTrail->SetAni(false);
		m_fRightLongGage = 0.0f;
	}
}

//오른쪽 가장 작은 Trail Update
void CDragonDance::RightTrailSmallUpdate(const _float & fTImeDelta)
{
	if (m_pRightSmallTrail->GetAni() == false)
		return;

	m_pRightSmallTrail->Update_Object(fTImeDelta);


	_matrix mMat, RightSmallMat;

	_vec3 vPosition;

	vPosition.x = 0.f;
	vPosition.y = 4.f;
	vPosition.z = 0.f;

	//회전==============================================================
	m_pRightSmallTrail->GetTransCom()->Rotation(Engine::ROT_Y, 20.f);
	m_fRightSmallGage += 20.f;
	//================================================================

	m_pRightSmallTrail->GetTransCom()->Set_Position(&vPosition);
	m_pRightSmallTrail->GetTransCom()->Get_WorldMatrix(&RightSmallMat);
	m_pTargetTransform->Get_WorldMatrix(&mMat);

	RightSmallMat = RightSmallMat * mMat;

	m_pRightSmallTrail->GetTransCom()->Set_WorldMatrix(&RightSmallMat);



	_float fGage = m_pRightSmallTrail->GetGage();

	fGage = max(fGage - 0.5f, 0.f);


	m_pRightSmallTrail->SetGage(fGage);


	if (m_fRightSmallGage >= 300.f)
	{
		m_pRightSmallTrail->SetGage(1.f);
		m_pRightSmallTrail->SetAni(false);
		m_fRightSmallGage = 0.0f;
	}

}

void CDragonDance::AllRightTrailInit(void)
{
	//오른손방향에서 나가는 Long Trail=====================================
	_vec4 Color = _vec4(0.5411f, 0.1686f, 0.8862f, 2.f);
	m_pRightLongTrail = CPledgeRing::Create(m_pGraphicDev, L"RightLongTrail", 40);
	m_pRightLongTrail->SetAni(false);	
	m_pRightLongTrail->SetRadius(10.0f, 2.0f);
	m_pRightLongTrail->SetTrailY(0.0f);
	m_pRightLongTrail->SetTextureNumber(19);
	m_pRightLongTrail->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pRightLongTrail->GetParticleInfo()->m_iPath = 3;
	m_pRightLongTrail->SetRenderCheck(true);
	m_pRightLongTrail->SetIntervalAngle(10.f);//각도
	m_pRightLongTrail->SettingTrail();
	_vec3 vAngle = _vec3(0.f, -200.f, 180.f);
	m_pRightLongTrail->GetTransCom()->Set_Angle(&vAngle);
	m_pRightLongTrail->GetTransCom()->Set_Scale(1.5f, 1.5f, 1.5f);

	m_pRightLongTrail->SetBlurRenderCheck(true);
	m_pRightLongTrail->SetBlurCnt(4);
	m_pRightLongTrail->SetBlurPass(3);
	m_pRightLongTrail->SetBlurPower(1.2f);
	m_pRightLongTrail->SetBlurType(Engine::BLUR_HORIZON);
	m_pRightLongTrail->SetBlurValue(128.0f);
	//============================================================================

	//오른손방향에서 나가는 Small Trail===========================================
	Color = _vec4(0.5411f, 0.1686f, 0.8862f, 2.f);
	m_pRightSmallTrail = CPledgeRing::Create(m_pGraphicDev, L"RightSmallTrail", 40);
	m_pRightSmallTrail->SetAni(false);
	m_pRightSmallTrail->SetRadius(10.0f, 2.0f);
	m_pRightSmallTrail->SetTrailY(0.0f);
	m_pRightSmallTrail->SetTextureNumber(19);
	m_pRightSmallTrail->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pRightSmallTrail->GetParticleInfo()->m_iPath = 3;
	m_pRightSmallTrail->SetRenderCheck(true);
	m_pRightSmallTrail->SetIntervalAngle(10.f);
	m_pRightSmallTrail->SettingTrail();
	vAngle = _vec3(0.f, -200.f, 180.f);
	m_pRightSmallTrail->GetTransCom()->Set_Angle(&vAngle);
	m_pRightSmallTrail->GetTransCom()->Set_Scale(0.5f, 0.5f, 0.5f);

	m_pRightSmallTrail->SetBlurRenderCheck(true);
	m_pRightSmallTrail->SetBlurCnt(4);
	m_pRightSmallTrail->SetBlurPass(3);
	m_pRightSmallTrail->SetBlurPower(1.1f);
	m_pRightSmallTrail->SetBlurType(Engine::BLUR_HORIZON);
	m_pRightSmallTrail->SetBlurValue(128.0f);
	//============================================================================
}

void CDragonDance::RightTrailStart(void)
{
	//오른손방향에서 나가는 Long Trail=====================================
	_vec4 Color = _vec4(0.5411f, 0.1686f, 0.8862f, 2.f);
	m_pRightLongTrail->SetAni(true);
	m_pRightLongTrail->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	_vec3 vAngle = _vec3(0.f, -200.f, 180.f);
	m_pRightLongTrail->GetTransCom()->Set_Angle(&vAngle);
	m_pRightLongTrail->GetTransCom()->Set_Scale(1.5f, 1.5f, 1.5f);
	//============================================================================

	//오른손방향에서 나가는 Small Trail===========================================
	Color = _vec4(0.5411f, 0.1686f, 0.8862f, 2.f);
	m_pRightSmallTrail->SetAni(true);
	m_pRightSmallTrail->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	vAngle = _vec3(0.f, -200.f, 180.f);
	m_pRightSmallTrail->GetTransCom()->Set_Angle(&vAngle);
	m_pRightSmallTrail->GetTransCom()->Set_Scale(0.5f, 0.5f, 0.5f);
	//============================================================================
}

void CDragonDance::AllLeftTrailInit(void)
{
	//왼손방향에서 나가는 Trail=====================================
	_vec4 Color = _vec4(0.5411f, 0.1686f, 0.8862f, 2.f);
	m_pLeftLongTrail = CPledgeRing::Create(m_pGraphicDev, L"LeftLongTrail", 40);
	m_pLeftLongTrail->SetAni(false);
	m_pLeftLongTrail->SetBlurRenderCheck(false);
	m_pLeftLongTrail->SetRadius(10.0f, 2.0f);
	m_pLeftLongTrail->SetTrailY(0.0f);
	m_pLeftLongTrail->SetTextureNumber(19);
	m_pLeftLongTrail->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pLeftLongTrail->GetParticleInfo()->m_iPath = 3;
	m_pLeftLongTrail->SetRenderCheck(true);
	m_pLeftLongTrail->SetIntervalAngle(10.f);
	m_pLeftLongTrail->SettingTrail();
	_vec3 vAngle = _vec3(0.f, -200.f, 0.f);
	m_pLeftLongTrail->GetTransCom()->Set_Angle(&vAngle);
	m_pLeftLongTrail->GetTransCom()->Set_Scale(1.5f, 1.5f, 1.5f);

	m_pLeftLongTrail->SetBlurRenderCheck(true);
	m_pLeftLongTrail->SetBlurCnt(4);
	m_pLeftLongTrail->SetBlurPass(3);
	m_pLeftLongTrail->SetBlurPower(1.2f);
	m_pLeftLongTrail->SetBlurType(Engine::BLUR_HORIZON);
	m_pLeftLongTrail->SetBlurValue(128.0f);
	//============================================================================



	//왼손방향에서 나가는 Small Trail===========================================
	Color = _vec4(0.5411f, 0.1686f, 0.8862f, 2.f);
	m_pLeftSmallTrail = CPledgeRing::Create(m_pGraphicDev, L"LeftSmallTrail", 40);
	m_pLeftSmallTrail->SetAni(false);
	m_pLeftSmallTrail->SetBlurRenderCheck(false);
	m_pLeftSmallTrail->SetRadius(10.0f, 2.0f);
	m_pLeftSmallTrail->SetTrailY(0.0f);
	m_pLeftSmallTrail->SetTextureNumber(19);
	m_pLeftSmallTrail->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pLeftSmallTrail->GetParticleInfo()->m_iPath = 3;
	m_pLeftSmallTrail->SetRenderCheck(true);
	m_pLeftSmallTrail->SetIntervalAngle(10.f);
	m_pLeftSmallTrail->SettingTrail();
	vAngle = _vec3(0.f, -200.f, 0.f);
	m_pLeftSmallTrail->GetTransCom()->Set_Angle(&vAngle);
	m_pLeftSmallTrail->GetTransCom()->Set_Scale(0.5f, 0.5f, 0.5f);

	m_pLeftSmallTrail->SetBlurRenderCheck(true);
	m_pLeftSmallTrail->SetBlurCnt(4);
	m_pLeftSmallTrail->SetBlurPass(3);
	m_pLeftSmallTrail->SetBlurPower(1.2f);
	m_pLeftSmallTrail->SetBlurType(Engine::BLUR_HORIZON);
	m_pLeftSmallTrail->SetBlurValue(128.0f);
	//============================================================================


}

void CDragonDance::LeftTrailStart(void)
{
	//왼손방향에서 나가는 Trail================================================
	_vec4 Color = _vec4(0.5411f, 0.1686f, 0.8862f, 2.f);
	m_pLeftLongTrail->SetAni(true);
	m_pLeftLongTrail->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	_vec3 vAngle = _vec3(0.f, -200.f, 0.f);
	m_pLeftLongTrail->GetTransCom()->Set_Angle(&vAngle);
	//============================================================================

	//왼손방향에서 나가는 Small Trail===========================================
	Color = _vec4(0.5411f, 0.1686f, 0.8862f, 2.f);
	m_pLeftSmallTrail->SetAni(true);
	m_pLeftSmallTrail->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pLeftSmallTrail->GetTransCom()->Set_Angle(&vAngle);
	//===========================================================================
}



void CDragonDance::LeftTrailUpdate(const _float & fTimeDelta)
{

	if (m_pLeftLongTrail->GetAni() == false)
		return;

	m_pLeftLongTrail->Update_Object(fTimeDelta);


	_matrix mMat, LeftLongMat;

	_vec3 vPosition;

	vPosition.x = 0.f;
	vPosition.y = 3.f;
	vPosition.z = 0.f;

	//회전==============================================================
	m_pLeftLongTrail->GetTransCom()->Rotation(Engine::ROT_Y, 20.f);
	m_fLeftLongGage += 20.f;
	//================================================================


	m_pLeftLongTrail->GetTransCom()->Set_Position(&vPosition);
	m_pLeftLongTrail->GetTransCom()->Get_WorldMatrix(&LeftLongMat);
	m_pTargetTransform->Get_WorldMatrix(&mMat);

	LeftLongMat = LeftLongMat * mMat;

	m_pLeftLongTrail->GetTransCom()->Set_WorldMatrix(&LeftLongMat);


	//===============================================
	_float fGage = m_pLeftLongTrail->GetGage();

	fGage = max(fGage - 0.5f, 0.f);


	m_pLeftLongTrail->SetGage(fGage);


	if (m_fLeftLongGage >= 300.f)
	{
		m_pLeftLongTrail->SetGage(1.f);
		m_pLeftLongTrail->SetAni(false);
		m_fLeftLongGage = 0.0f;
	}
	//==================================================
}




void CDragonDance::LeftTrailSmallUpdate(const _float & fTImeDelta)
{
	if (m_pLeftSmallTrail->GetAni() == false)
		return;

	m_pLeftSmallTrail->Update_Object(fTImeDelta);


	_matrix mMat, LeftSmallMat;

	_vec3 vPosition;

	vPosition.x = 0.f; 
	vPosition.y = 4.f;
	vPosition.z = 0.f;


	//회전==============================================================
	m_pLeftSmallTrail->GetTransCom()->Rotation(Engine::ROT_Y, 20.f);
	m_fLeftSmallGage += 20.f;
	//================================================================

	m_pLeftSmallTrail->GetTransCom()->Set_Position(&vPosition);
	m_pLeftSmallTrail->GetTransCom()->Get_WorldMatrix(&LeftSmallMat);
	m_pTargetTransform->Get_WorldMatrix(&mMat);

	LeftSmallMat = LeftSmallMat * mMat;

	m_pLeftSmallTrail->GetTransCom()->Set_WorldMatrix(&LeftSmallMat);


	//===============================================
	_float fGage = m_pLeftLongTrail->GetGage();

	fGage = max(fGage - 0.5f, 0.f);


	m_pLeftSmallTrail->SetGage(fGage);


	if (m_fLeftSmallGage >= 300.f)
	{
		m_pLeftSmallTrail->SetGage(1.f);
		m_pLeftSmallTrail->SetAni(false);
		m_fLeftSmallGage = 0.0f;
	}
	//==================================================
}