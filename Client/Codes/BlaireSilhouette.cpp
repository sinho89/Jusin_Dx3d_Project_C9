#include "stdafx.h"
#include "BlaireSilhouette.h"
#include "Export_Engine.h"
#include "Sprite.h"
#include "Ring.h"
#include "WitchBlade.h"
#include "Sprite.h"

CBlaireSilhouette::CBlaireSilhouette(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
:CEffect(pGraphicDev, wstrName)
, m_pOutLineRing(NULL)
,pTargetTransform(NULL)
,m_pvecAniInfo(NULL)
,m_pTargetMesh(NULL)
,m_bSkillCheck(false)
{
	m_bIsStatic = true;
	m_bAni = false;

	ZeroMemory(&m_pSmallRing, sizeof(CRing*) * 4);
	ZeroMemory(&m_pDust, sizeof(CSprite*) * 2);
}

CBlaireSilhouette::~CBlaireSilhouette(void)
{
}

void CBlaireSilhouette::Update_Object(const _float & fTimeDelta)
{
	//L : 왼쪽 블레어 실루엣
	if (m_pPlayerAction[CWitchBlade::ACTION_BLAIRESILHOUETTEL] == true)
	{
		_double dTime = (*m_pvecAniInfo)[CWitchBlade::WITCH_SKILLEVASIONL]->pMotionEventInfo[EVENT_EFFECT].dStartTime;
		_double dTargetTime = m_pTargetMesh->Get_TrackPos();

		if (dTargetTime >= dTime && (!m_bSkillCheck))
		{
			SkillStart();
			m_bSkillCheck = true;

			//=외곽 트레일==================
			m_pOutLineRing->SetTrailY(-0.4f);
			m_pOutLineRing->SettingTrail();
			//=============================

			for (int i = 0; i != 4; ++i)
			{
				m_pSmallRing[i]->SetTrailY(0.4f);
				m_pSmallRing[i]->SettingTrail();

			}

		}

	}

	if (m_pPlayerAction[CWitchBlade::ACTION_BLAIRESILHOUETTER] == true)
	{
		_double dTime = (*m_pvecAniInfo)[CWitchBlade::WITCH_SKILLEVASIONR]->pMotionEventInfo[EVENT_EFFECT].dStartTime;
		_double dTargetTime = m_pTargetMesh->Get_TrackPos();

		if (dTargetTime >= dTime && (!m_bSkillCheck))
		{
			SkillStart();
			m_bSkillCheck = true;

			//=외곽 트레일==================
			m_pOutLineRing->SetTrailY(0.4f);
			m_pOutLineRing->SettingTrail();
			//==============================

			for (int i = 0; i != 4; ++i)
			{
				m_pSmallRing[i]->SetTrailY(-0.4f);
				m_pSmallRing[i]->SettingTrail();

			}
		}
	}

	if (m_pPlayerAction[CWitchBlade::ACTION_BLAIRESILHOUETTEL] == false &&
		m_pPlayerAction[CWitchBlade::ACTION_BLAIRESILHOUETTER] == false)
	{
		m_bSkillCheck = false;
		EndAniCheck();
	}
	
	if (!m_bAni)
		return;

	for (int i = 0; i < 2; ++i)
	{
		m_pDust[i]->Update_Object(fTimeDelta);
	}
	
	m_pOutLineRing->Update_Object(fTimeDelta);
	m_pOutLineRing->GetParticleInfo()->m_dColor.a = max(m_pOutLineRing->GetParticleInfo()->m_dColor.a - 0.0145f, 0.0f);
	_vec3 vScale;
	m_pOutLineRing->GetTransCom()->Get_Scale(&vScale);

	if (vScale.x >= 1.f)
	{
		m_pOutLineRing->GetTransCom()->Set_Scale(1.f, 1.f, 1.f);
	}
	else
	{
		m_pOutLineRing->GetTransCom()->Add_Scale(0.02f);
	}

	for (int i = 0; i != 4; ++i)
	{
		m_pSmallRing[i]->GetParticleInfo()->m_dColor.a = max(m_pSmallRing[i]->GetParticleInfo()->m_dColor.a - 0.0145f, 0.0f);

		_vec3 vScale;
		m_pSmallRing[i]->GetTransCom()->Get_Scale(&vScale);

		if (vScale.x >= 1.f)
		{
			m_pSmallRing[i]->GetTransCom()->Set_Scale(1.f, 1.f, 1.f);
		}
		else
		{
			m_pSmallRing[i]->GetTransCom()->Add_Scale(0.02f);
		}

		m_pSmallRing[i]->Update_Object(fTimeDelta);
	}
}

void CBlaireSilhouette::Render_Object(void)
{
	if(!m_bAni)
		return;

	for (int i = 0; i < 2; ++i)
	{
		m_pDust[i]->Render_Object();
	}

	m_pOutLineRing->Render_Object();

	for (int i = 0; i != 4; ++i)
	{
		m_pSmallRing[i]->Render_Object();
	}
}

HRESULT CBlaireSilhouette::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	//생성 및 설정

	//=외곽 트레일=========================================
	_vec4 Color = _vec4(1.f, 0.2f, 0.f, 2.f);
	m_pOutLineRing = CRing::Create(m_pGraphicDev, L"Ring", 40);
	m_pOutLineRing->SetAni(false);

	m_pOutLineRing->SetRenderCheck(true);
	m_pOutLineRing->SetEffectBlurCheck(true);
	m_pOutLineRing->SetBlurPass(8);
	m_pOutLineRing->SetBlurCnt(6);	
	m_pOutLineRing->SetBlurPower(1.2f);
	m_pOutLineRing->SetBlurType(Engine::BLUR_HORIZON);
	m_pOutLineRing->SetBlurValue(100.0f);
	
	m_pOutLineRing->SetRotSpeed(30.f);
	m_pOutLineRing->SetRadius(10.0f, 7.0f);
	m_pOutLineRing->SetTextureNumber(11);
	m_pOutLineRing->SetTrailY(0.4f);
	m_pOutLineRing->SetRingPos(_vec3(0.f, 4.f, 0.f));
	m_pOutLineRing->GetParticleInfo()->m_dColor =  (D3DXCOLOR)Color;
	m_pOutLineRing->GetParticleInfo()->m_iPath = 1;
	m_pOutLineRing->SettingTrail();
	m_pOutLineRing->GetTransCom()->Set_Scale(0.7f, 0.7f, 0.7f);
	//=====================================================================

	for (int i = 0; i != 4; ++i)
	{
		if (i == 0)
		{
			m_pSmallRing[i] = CRing::Create(m_pGraphicDev, L"SmallRing", 10);
		}
		else if (i == 1)
		{
			
			m_pSmallRing[i] = CRing::Create(m_pGraphicDev, L"SmallRing2", 10);
		}
		else if (i == 2)
		{
			m_pSmallRing[i] = CRing::Create(m_pGraphicDev, L"SmallRing3", 10);
		}
		else
		{
			m_pSmallRing[i] = CRing::Create(m_pGraphicDev, L"SmallRing4", 10);
		}

	
		if (i >= 2)
		{
			Color = _vec4(0.8f, 0.8f, 0.0f, 2.f);

			m_pSmallRing[i]->SetAni(false);

			m_pSmallRing[i]->SetRenderCheck(true);
			m_pSmallRing[i]->SetBlurCnt(6);
			m_pSmallRing[i]->SetBlurPass(8);
			m_pSmallRing[i]->SetBlurPower(1.2f);
			m_pSmallRing[i]->SetBlurType(Engine::BLUR_HORIZON);
			m_pSmallRing[i]->SetBlurValue(100.0f);

			m_pSmallRing[i]->SetRotSpeed(-10.f);
			m_pSmallRing[i]->SetRadius(9.9f, 2.0f);
			m_pSmallRing[i]->SetTextureNumber(9);
			m_pSmallRing[i]->SetTrailY(-0.4f);
			m_pSmallRing[i]->SetRingPos(_vec3(0.f, 4.4f, 0.f));
			m_pSmallRing[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
			m_pSmallRing[i]->GetParticleInfo()->m_iPath = 1;
			m_pSmallRing[i]->SetIntervalAngle(20.f);
			m_pSmallRing[i]->SettingTrail();
			m_pSmallRing[i]->GetTransCom()->Set_Scale(0.7f, 0.7f, 0.7f);
		}
		else
		{
			Color = _vec4(0.9f, 0.1f, 0.9f, 2.f);

			m_pSmallRing[i]->SetAni(false);
			
			m_pSmallRing[i]->SetRenderCheck(true);
			m_pSmallRing[i]->SetBlurPass(8);
			m_pSmallRing[i]->SetBlurCnt(6);
			m_pSmallRing[i]->SetBlurPower(1.2f);
			m_pSmallRing[i]->SetBlurType(Engine::BLUR_HORIZON);
			m_pSmallRing[i]->SetBlurValue(100.0f);

			m_pSmallRing[i]->SetRotSpeed(-10.f);
			m_pSmallRing[i]->SetRadius(9.7f, 2.0f);
			m_pSmallRing[i]->SetTextureNumber(12);
			m_pSmallRing[i]->SetTrailY(-0.4f);
			m_pSmallRing[i]->SetRingPos(_vec3(0.f, 4.3f, 0.f));
			m_pSmallRing[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
			m_pSmallRing[i]->GetParticleInfo()->m_iPath = 1;
			m_pSmallRing[i]->SetIntervalAngle(9.f);
			m_pSmallRing[i]->SettingTrail();
			m_pSmallRing[i]->GetTransCom()->Set_Scale(0.7f, 0.7f, 0.7f);
		}
	}
	
//========================================================================
	Color = _vec4(0.3f, 0.3f, 0.3f, 2.f);
	for (int i = 0; i < 2; ++i)
	{
		m_pDust[i] = CSprite::LoadCreate(m_pGraphicDev, L"BlaireDust");
		m_pDust[i]->SetAni(false);
		m_pDust[i]->SetRenderCheck(true);
		m_pDust[i]->GetTransCom()->Set_Scale(1.1f, 1.1f, 1.1f);
		m_pDust[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pDust[i]->SetEffectBlurCheck(TRUE);
		m_pDust[i]->SetBlurPass(2);
		m_pDust[i]->SetBlurCnt(2);
		m_pDust[i]->SetBlurPower(1.5f);
		m_pDust[i]->SetBlurValue(100.0f);
		m_pDust[i]->SetBlurType(Engine::BLUR_CROSS);
	}

	return S_OK;
}

CBlaireSilhouette * CBlaireSilhouette::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CBlaireSilhouette*   pInstance = new CBlaireSilhouette(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("BlaireSilhouette Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CBlaireSilhouette::Free(void)
{
	CEffect::Free();
	Engine::Safe_Release(m_pOutLineRing);

	for (int i = 0; i != 4; ++i)
	{
		Engine::Safe_Release(m_pSmallRing[i]);
	}


	for (int i = 0; i < 2; ++i)
	{
		Engine::Safe_Release(m_pDust[i]);
	}
}

HRESULT CBlaireSilhouette::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CBlaireSilhouette::SkillStart(void)
{
	if (m_bAni) //m_bAni가 True이면 실행되지 않는다.
		return;


	if(m_pPlayerAction[CWitchBlade::ACTION_BLAIRESILHOUETTEL] == true)
	{
		_vec3 Rot = _vec3(0.0f, 0.0f, 180.0f);
		_vec3 DecoRot = _vec3(0.0f, 0.0f, 180.0f);
		//=외곽 트레일=========================================
		m_pOutLineRing->SetAngle(&Rot);
		m_pOutLineRing->GetTransCom()->Set_Scale(0.7f, 0.7f, 0.7f);
		m_pOutLineRing->GetParticleInfo()->m_dColor.a = 1.0f;
		//=====================================================
		Rot = _vec3(0.0f, 0.0f, 0.0f);


		for (int i = 0; i != 4; ++i)
		{
			m_pSmallRing[i]->SetAngle(&Rot);
			m_pSmallRing[i]->GetParticleInfo()->m_dColor.a = 1.0f;
			m_pSmallRing[i]->GetTransCom()->Set_Scale(0.7f, 0.7f, 0.7f);

		}

		m_pSmallRing[1]->GetTransCom()->Rotation(Engine::ROT_Y, 180.f);
		m_pSmallRing[3]->GetTransCom()->Rotation(Engine::ROT_Y, 180.f);

		//Dust===============================================
		for (int i = 0; i < 2; ++i)
		{
			m_pDust[i]->SetYRotCheck(true);
			m_pDust[i]->SetAni(true);

			_vec3 Pos = pTargetTransform->Get_Now_Position();
			_vec3 Look;
			Pos.y -= 0.3f;
			pTargetTransform->Get_Infomation(Engine::INFO_LOOK, &Look);
			Look *= -1.5f;
			Pos += Look;

			m_pDust[i]->GetTransCom()->Set_Position(&Pos);
		}
	}
	else if(m_pPlayerAction[CWitchBlade::ACTION_BLAIRESILHOUETTER] == true)
	{
		_vec3 Rot = _vec3(0.0f, 0.0f, 0.0f);
		_vec3 DecoRot = _vec3(0.0f, 0.0f, 0.0f);
		//=외곽 트레일=========================================
		m_pOutLineRing->SetAngle(&Rot);
		m_pOutLineRing->GetTransCom()->Set_Scale(0.7f, 0.7f, 0.7f);
		m_pOutLineRing->GetParticleInfo()->m_dColor.a = 1.0f;
		//====================================================
		Rot = _vec3(0.0f, 0.0f, 180.0f);

		for (int i = 0; i != 4; ++i)
		{
			m_pSmallRing[i]->SetAngle(&Rot);
			m_pSmallRing[i]->GetTransCom()->Set_Scale(0.7f, 0.7f, 0.7f);
			m_pSmallRing[i]->GetParticleInfo()->m_dColor.a = 1.0f;

		}
		m_pSmallRing[1]->GetTransCom()->Rotation(Engine::ROT_Y, 180.f);
		m_pSmallRing[3]->GetTransCom()->Rotation(Engine::ROT_Y, 180.f);



		//Dust===============================================
		for (int i = 0; i < 2; ++i)
		{
			m_pDust[i]->SetYRotCheck(false);
			m_pDust[i]->SetAni(true);

			_vec3 Pos = pTargetTransform->Get_Now_Position();
			_vec3 Look;
			Pos.y -= 0.3f;
			pTargetTransform->Get_Infomation(Engine::INFO_LOOK, &Look);
			Look *= -1.5f;
			Pos += Look;

			m_pDust[i]->GetTransCom()->Set_Position(&Pos);
		}
	}

	m_bAni = true;

	m_pOutLineRing->SetAni(true);

	for (int i = 0; i != 4; ++i)
	{
		m_pSmallRing[i]->SetAni(true);
	}
}

void CBlaireSilhouette::EndAniCheck(void)
{
	m_bAni = false;
	m_pOutLineRing->SetAni(false);

	for (int i = 0; i != 4; ++i)
	{
		m_pSmallRing[i]->SetAni(false);
		
	}

	for (int i = 0; i < 2; ++i)
	{
		m_pDust[i]->SetAni(false);
	}
}

HRESULT CBlaireSilhouette::Start_ForScene(void)
{
	if (m_bOnceStart)
		return S_OK;

	m_bOnceStart = true;

	Engine::CGameObject* pGameObject = Engine::FindWithTag(L"Player");

	if (pGameObject == NULL)
		return S_OK;

	Engine::CTransform* pTransform = (Engine::CTransform*)pGameObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

	if (pTransform == NULL)
		return S_OK;


	m_pPlayerAction = ((CWitchBlade*)pGameObject)->Get_IsSkill(PLAYER_FIGHT);

	m_pOutLineRing->SetTargetTransCom(pTransform);

	for (int i = 0; i != 4; ++i)
	{
		m_pSmallRing[i]->SetTargetTransCom(pTransform);
	}

	pTargetTransform = pTransform;


	//Target Mesh
	m_pTargetMesh = (Engine::CDynamicMesh*)pGameObject->GetComponent<Engine::CDynamicMesh>(Engine::COMID_STATIC);

	//애니메이션 정보를 가져온다.
	m_pvecAniInfo = ((CWitchBlade*)pGameObject)->Get_AniInfo();

	Engine::Safe_Release(pTransform);
	Engine::Safe_Release(pGameObject);
	return S_OK;
}
