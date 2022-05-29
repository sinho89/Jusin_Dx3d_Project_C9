#include "stdafx.h"
#include "TrickyRabbit.h"
#include "PlayerCamera.h"
#include "WitchBlade.h"
#include "Wave.h"
#include "TrickyRabbitTrail.h"

CTrickyRabbit::CTrickyRabbit(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
:CEffect(pGraphicDev, wstrName)
, m_pvecAniInfo(NULL)
, m_pTargetMesh(NULL)
, m_pTargetTransform(NULL)
, m_pTargetAnimator(NULL)
, m_pTrailWave(NULL)
, m_bTrailWaveCheck(false)
, pTargetSwordTransform(NULL)
, m_pGroundWave(NULL)
, m_bGroundWaveCheck(false)
, m_pTrailLeft(NULL)
, m_pTrailRight(NULL)
, m_bTrailLeft(false)
, m_bTrailRight(false)
, m_pKickImpact(NULL)
, m_bKickCheck(false)
{
	m_bIsStatic = true;
	m_bAni = false;
}

CTrickyRabbit::~CTrickyRabbit(void)
{
}

void CTrickyRabbit::Update_Object(const _float & fTimeDelta)
{
	if (m_pPlayerAction[CWitchBlade::ACTION_TRICKYRABBIT] == true)
	{
		 m_bAni = true;

		_double dTime = (*m_pvecAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->pMotionEventInfo[EVENT_EFFECT].dStartTime;
		_double dTargetTime = m_pTargetMesh->Get_TrackPos();


		if((dTargetTime >= 1.9f)  &&  (!m_bTrailRight)) //초기에 오른손 검 휘두르기
		{
			m_bTrailRight = true;
			
			m_pTrailRight->SetLightPatternCheck(false);
			m_pTrailRight->SetAni(true);
			m_pTrailRight->SettingTrail();
		}

		if((dTargetTime >= 2.97f) && (!m_bTrailLeft)) //왼손 검 휘두르기
		{
			m_bTrailLeft = true;
			m_pTrailLeft->SetAni(true);
			m_pTrailLeft->SettingTrail();

			m_pTrailRight->SetLightPatternCheck(true);
			m_pTrailRight->SetAni(true);
			m_pTrailRight->SettingTrail();
		}

		if((dTargetTime >= 4.3f)) //발차기 동작 전
		{
			m_pTrailLeft->SetAni(false);
			m_pTrailRight->SetAni(false);
			m_pTrailRight->SetLightPatternCheck(false);
		}
		

		if( (dTargetTime >= 4.68f) && (!m_bKickCheck))
		{
			m_bKickCheck = true;
			KickImpactStart();
		}



		if ((dTargetTime >= 0.38) && (!m_bGroundWaveCheck)) //그라운드 웨이브  발사
		{
			GroundWaveStart();

			m_bGroundWaveCheck = true;
		}

		if((dTargetTime >= dTime) && (!m_bTrailWaveCheck)) //검 아우라 트레일 발사
		{
			SkillStart();
			m_bTrailWaveCheck = true;
		}
	}
	else
	{
		m_bTrailWaveCheck = false;
		m_bGroundWaveCheck = false;
		m_bTrailRight = false;
		m_bTrailLeft = false;
		m_bKickCheck = false;
	}

	if (!m_bAni)
		return;

	EndAniCheck();


	TrailWaveUpdate();

	GroundWaveUpdate();


	KickImpactUpdate();


	m_pTrailWave->Update_Object(fTimeDelta);

	m_pGroundWave->Update_Object(fTimeDelta);

	m_pTrailLeft->Update_Object(fTimeDelta);
	m_pTrailRight->Update_Object(fTimeDelta);

	m_pKickImpact->Update_Object(fTimeDelta);

}

void CTrickyRabbit::Render_Object(void)
{
	if (!m_bAni)
		return;

	m_pTrailWave->Render_Object();

	m_pGroundWave->Render_Object();


	m_pTrailLeft->Render_Object();
	m_pTrailRight->Render_Object();


	m_pKickImpact->Render_Object();
}

HRESULT CTrickyRabbit::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	//===TrailWave======================================================//
	m_pTrailWave = CWave::Create(m_pGraphicDev, L"TrailWave");
	m_pTrailWave->SetAni(true);
	m_pTrailWave->SetPattern(CWave::PATTERN_END);
	m_pTrailWave->GetTransCom()->Set_Scale(3.f, 1.f, 1.f);
	m_pTrailWave->SetBilBoardCheck(false);	
	m_pTrailWave->SetRendererCheck(true);
	m_pTrailWave->GetParticleInfo()->m_iTextureNumber = 37;
	_vec4 vColor = _vec4(0.f, 0.f, 1.f, 1.f);
	m_pTrailWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
	m_pTrailWave->GetParticleInfo()->m_iPath = 2;

	m_pTrailWave->SetBlurCheck(true);
	m_pTrailWave->SetBlurPass(2);
	m_pTrailWave->SetBlurCnt(2);
	m_pTrailWave->SetBlurPower(1.2f);
	m_pTrailWave->SetBlurValue(128.0f);
	m_pTrailWave->SetBlurType(Engine::BLUR_VERTICAL);
	//===================================================================


	//===GroundWave======================================================
	m_pGroundWave = CWave::Create(m_pGraphicDev, L"GroundWave");
	m_pGroundWave->SetAni(false);
	m_pGroundWave->SetPattern(CWave::PATTERN_END);
	m_pGroundWave->SetBilBoardCheck(false);
	m_pGroundWave->SetRendererCheck(true);
	m_pGroundWave->GetTransCom()->Set_Scale(0.3f, 0.3f, 0.3f);
	_vec4 Color = _vec4(1.0f, 1.0f, 1.0f, 2.0f);
	m_pGroundWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pGroundWave->GetParticleInfo()->m_iPath = 2;
	m_pGroundWave->GetParticleInfo()->m_iTextureNumber = 1;

	m_pGroundWave->SetBlurCheck(true);
	m_pGroundWave->SetBlurPass(2);
	m_pGroundWave->SetBlurCnt(2);
	m_pGroundWave->SetBlurPower(1.2f);
	m_pGroundWave->SetBlurValue(128.0f);
	m_pGroundWave->SetBlurType(Engine::BLUR_X);
	//===================================================================


	//KickImpact=========================================================
	m_pKickImpact = CWave::Create(m_pGraphicDev, L"KickImpact");
	m_pKickImpact->SetAni(false);
	m_pKickImpact->SetPattern(CWave::PATTERN_END);
	m_pKickImpact->SetBilBoardCheck(true);
	m_pKickImpact->SetRendererCheck(true);
	m_pKickImpact->GetTransCom()->Set_Scale(0.5f, 0.5f, 0.5f);
	Color = _vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_pKickImpact->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pKickImpact->GetParticleInfo()->m_iPath = 2;
	m_pKickImpact->GetParticleInfo()->m_iTextureNumber = 36;

	m_pKickImpact->SetBlurCheck(true);
	m_pKickImpact->SetBlurPass(2);
	m_pKickImpact->SetBlurCnt(2);
	m_pKickImpact->SetBlurPower(1.2f);
	m_pKickImpact->SetBlurValue(128.0f);
	m_pKickImpact->SetBlurType(Engine::BLUR_X);
	//===================================================================


	return S_OK;
}

CTrickyRabbit* CTrickyRabbit::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CTrickyRabbit*		pInstance = new CTrickyRabbit(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("TrickyRabbit Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CTrickyRabbit::Free(void)
{
	CEffect::Free();
	Engine::Safe_Release(m_pTrailWave);
	Engine::Safe_Release(m_pGroundWave);
	Engine::Safe_Release(m_pTrailLeft);
	Engine::Safe_Release(m_pTrailRight);
	Engine::Safe_Release(m_pKickImpact);

}

HRESULT CTrickyRabbit::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CTrickyRabbit::SkillStart(void)
{
	//TrailWave========================================================
	_vec3 TargetPos;

	_vec3 vLocal = _vec3(0.2f, -3.0f, -2.0f);

	_matrix matSwordWorld;

	pTargetSwordTransform->Get_WorldMatrix(&matSwordWorld);

	D3DXVec3TransformCoord(&vLocal, &vLocal, &matSwordWorld);

	TargetPos = vLocal;

	TargetPos.y += 0.5f;


	_vec3 TargetLook;

	m_pTargetTransform->Get_Infomation(Engine::INFO_LOOK, &TargetLook);

	D3DXVec3Normalize(&TargetLook, &TargetLook);

	_vec3 vLook = _vec3(-1.f, 0.0f, 0.0f);

	_float Dot = D3DXVec3Dot(&vLook, &TargetLook);
	_float Radian = (_float)acos(Dot);
	 Radian = D3DXToDegree(Radian);

	 _vec3 vDir;

	 D3DXVec3Cross(&vDir, &vLook, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	 if (D3DXVec3Dot(&vDir, &TargetLook) > 0) //왼쪽
	 {
		 Radian *= -1.f;
	 }
	 _vec3 Angle = _vec3(90.f, Radian, 0.0f);

	 _vec3 vScale;
	 m_pTrailWave->GetTransCom()->Get_Scale(&vScale);

	 TargetPos += (-TargetLook * 10.f);

	 m_pTrailWave->GetTransCom()->Set_Angle(&Angle);
	 m_pTrailWave->GetTransCom()->Set_Position(&TargetPos);
	 m_pTrailWave->SetAni(true);
	 _vec4 vColor = _vec4(0.f, 0.f, 1.f, 1.f);
	 m_pTrailWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
	//===================================================================
}

void CTrickyRabbit::EndAniCheck(void)
{
	if (m_bAni == true && m_pTrailWave->GetAni() == false && m_pGroundWave->GetAni() == false && m_pTrailLeft->GetAni() == false  && 
		m_pTrailRight->GetAni() == false && m_pKickImpact->GetAni() == false)
		m_bAni = false;
}

HRESULT CTrickyRabbit::Start_ForScene(void)
{
	if (m_bOnceStart)
		return S_OK;

	m_bOnceStart = true;

	Engine::CGameObject* pGameObject = Engine::FindWithTag(L"Player");


	if (pGameObject == NULL)
		return E_FAIL;

	m_pPlayerAction = ((CWitchBlade*)pGameObject)->Get_IsSkill(PLAYER_FIGHT);

	m_pTargetAnimator = ((CWitchBlade*)pGameObject)->GetAnimatorInfo();

	//Target Mesh
	m_pTargetMesh = (Engine::CDynamicMesh*)pGameObject->GetComponent<Engine::CDynamicMesh>(Engine::COMID_STATIC);

	//애니메이션 정보를 가져온다.
	m_pvecAniInfo = ((CWitchBlade*)pGameObject)->Get_AniInfo();

	//Player TransCom
	Engine::CTransform* pTargetTransform = (Engine::CTransform*)pGameObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

	m_pTargetTransform = pTargetTransform;
	
	Engine::CGameObject* pTargetObject = Engine::Find(L"Sword1");
	if (pTargetObject == NULL)
		return E_FAIL;

	pTargetSwordTransform = (Engine::CTransform*)pTargetObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);
	if (pTargetSwordTransform == NULL)
		return E_FAIL;

	const _matrix*					m_pmatTarget;

	m_pmatTarget = pTargetSwordTransform->Get_WorldMatrixPointer();

	//Traill=============================================================================
	m_pTrailLeft = CTrickyRabbitTrail::Create(m_pGraphicDev, L"TrickyRabbitTrailLeft", CTrickyRabbitTrail::DIR_LEFT);
	m_pTrailLeft->SetAni(false);
	m_pTrailLeft->SetTargetMatrix(m_pmatTarget);

	m_pTrailLeft->SetEffectBlurCheck(TRUE);
	m_pTrailLeft->SetBlurCnt(4);
	m_pTrailLeft->SetBlurPass(0);
	m_pTrailLeft->SetBlurPower(1.5f);
	m_pTrailLeft->SetBlurValue(70.0f);
	m_pTrailLeft->SetBlurType(Engine::BLUR_VERTICAL);
	//====================================================================================



	Engine::CGameObject* pRightSwordObject = Engine::Find(L"SwordL1");
	if (pRightSwordObject == NULL)
		return E_FAIL;

	Engine::CTransform*   pRightSwordTransform = NULL;

	pRightSwordTransform = (Engine::CTransform*)pRightSwordObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);
	if (pRightSwordTransform == NULL)
		return E_FAIL;

	const _matrix*					m_pLightmatTarget;

	m_pLightmatTarget = pRightSwordTransform->Get_WorldMatrixPointer();

	//Traill=============================================================================
	m_pTrailRight = CTrickyRabbitTrail::Create(m_pGraphicDev, L"TrickyRabbitTrailRight", CTrickyRabbitTrail::DIR_LIGHT);
	m_pTrailRight->SetAni(false);
	m_pTrailRight->SetTargetMatrix(m_pLightmatTarget);

	m_pTrailRight->SetEffectBlurCheck(TRUE);
	m_pTrailRight->SetBlurCnt(4);
	m_pTrailRight->SetBlurPass(0);
	m_pTrailRight->SetBlurPower(1.5f);
	m_pTrailRight->SetBlurValue(70.0f);
	m_pTrailRight->SetBlurType(Engine::BLUR_VERTICAL);
	//====================================================================================

	Engine::Safe_Release(pRightSwordObject);
	Engine::Safe_Release(pTargetObject);
	Engine::Safe_Release(pTargetTransform);
	Engine::Safe_Release(pGameObject);
	return S_OK;
}


//TrailWave Update관련 함수
void CTrickyRabbit::TrailWaveUpdate(void)
{
	if (!m_pTrailWave->GetAni())
		return;

	_vec4 vColor = (_vec4)m_pTrailWave->GetParticleInfo()->m_dColor;

	if (vColor.w < 0.f)
	{
			vColor.w = 1.0f;
			m_pTrailWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
			m_pTrailWave->SetAni(false);
	}
	else
	{
			vColor.w -= 0.03f;
			m_pTrailWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
	}

}

void CTrickyRabbit::GroundWaveUpdate(void)
{
	if ((!m_pGroundWave->GetAni()))
		return;

	_vec4 vColor = (_vec4)m_pGroundWave->GetParticleInfo()->m_dColor;

	if (vColor.w < 0.f)
	{
		vColor.w = 2.0f;
		m_pGroundWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
		m_pGroundWave->SetAni(false);
	}
	else
	{
		vColor.w -= 0.1f;
		m_pGroundWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;

		m_pGroundWave->GetTransCom()->Add_Scale(0.4f);
	}

}

void CTrickyRabbit::GroundWaveStart(void)
{
	_vec3 vPos = m_pTargetTransform->Get_Now_Position();
	vPos.y += 0.5f;

	m_pGroundWave->SetAni(true);
	m_pGroundWave->GetTransCom()->Set_Position(&vPos);
	m_pGroundWave->GetTransCom()->Set_Scale(0.3f, 0.3f, 0.3f);
	_vec4 Color = _vec4(1.0f, 1.0f, 1.0f, 2.0f);
	m_pGroundWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	_vec3 vAngle = _vec3(90.f, 0.f, 0.f);
	m_pGroundWave->GetTransCom()->Set_Angle(&vAngle);

}

void CTrickyRabbit::KickImpactStart(void)
{
	_vec3 vPos = m_pTargetTransform->Get_Now_Position();

	_vec3 TargetLook;

	m_pTargetTransform->Get_Infomation(Engine::INFO_LOOK, &TargetLook);

	D3DXVec3Normalize(&TargetLook, &TargetLook);

	vPos += TargetLook * 3.f;
	vPos.y += 5.f;

	m_pKickImpact->SetAni(true);
	m_pKickImpact->GetTransCom()->Set_Position(&vPos);
	m_pKickImpact->GetTransCom()->Set_Scale(0.5f, 0.5f, 0.5f);
	_vec4 Color = _vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_pKickImpact->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
}

void CTrickyRabbit::KickImpactUpdate(void)
{
	if (m_pKickImpact->GetAni() == false)
		return;

	_vec4 vColor = (_vec4)m_pKickImpact->GetParticleInfo()->m_dColor;

	if (vColor.w < 0.f)
	{
		vColor.w = 1.0f;
		m_pKickImpact->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
		m_pKickImpact->SetAni(false);
	}
	else
	{
		vColor.w -= 0.03f;
		m_pKickImpact->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;

		m_pKickImpact->GetTransCom()->Add_Scale(0.3f);
	}
}
