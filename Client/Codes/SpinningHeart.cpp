#include "stdafx.h"
#include "SpinningHeart.h"
#include "PlayerCamera.h"
#include "WitchBlade.h"
#include "Cyclone.h"
#include "Wave.h"
#include "CherryBlossom.h"
#include "SpinningHeartTrail.h"
#include "SpinningThrown.h"

CSpinningHeart::CSpinningHeart(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
:CEffect(pGraphicDev, wstrName)
, m_pvecAniInfo(NULL)
, m_pTargetMesh(NULL)
, m_pTargetTransform(NULL)
, m_pCyclone(NULL)
, m_pLightWave(NULL)
, m_bCherryCheck(false)
, m_pLightWaveTwo(NULL)
, m_pCherryBlossom(NULL)
, m_pTrail(NULL)
, m_pmatTarget(NULL)
, m_bTrailStart(false)
, m_bShooingState(false)
, m_bGroundWaveCheck(false)
, m_pSpinningBullet(NULL)
, m_bSpinningThrownCheck(false)
{
	m_bIsStatic = true;
	m_bAni = false;

	ZeroMemory(&m_pGroundWave, sizeof(CWave*) * 2);

}

CSpinningHeart::~CSpinningHeart(void)
{
}

void CSpinningHeart::Update_Object(const _float & fTimeDelta)
{

	//앞으로 쏘는 동작
	if (m_pPlayerAction[CWitchBlade::ACTION_SPINNINGHEART] == true)
	{
		m_bAni = true;

		//현재 동작이 모으는 동작일경우
		if (!m_bTrailStart)
		{
				m_pTrail->SetAni(true);
				m_pTrail->SettingTrail();
				m_bTrailStart = true;
		}

		if (m_pTargetAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_SKILLSPINNINGHEARTB)
		{

			_double dTargetTime = m_pTargetMesh->Get_TrackPos();

			if (dTargetTime >= 0.3f && (!m_bGroundWaveCheck))
			{
				GroundWaveStart();
				m_bGroundWaveCheck = true;
			}
		}
	}

	//백덤블링뒤 쏘는 동작
	if (m_pPlayerAction[CWitchBlade::ACTION_SPINNINGHEARTB] == true)
	{
		 m_bAni = true;		
		 
		 //벚꽃 발사
		if (!m_bCherryCheck)
		{
			LightStart();
			CycloneStart();
			m_bCherryCheck = true;
		}

		if (m_pTargetAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_SKILLSPINNINGHEARTB)
		{

			_double dTargetTime = m_pTargetMesh->Get_TrackPos();


			//GroundWave 발사
			if (dTargetTime >= 0.3f && (!m_bGroundWaveCheck))
			{
				GroundWaveStart();
				m_bGroundWaveCheck = true;
			}

			if (!m_bTrailStart)
			{
				m_pTrail->SetAni(true);
				m_pTrail->SettingTrail();
				m_bTrailStart = true;
			}
		}

	}

	//왼쪽으로 돈뒤 쏘는 동작
	if (m_pPlayerAction[CWitchBlade::ACTION_SPINNINGHEARTL] == true)
	{
		m_bAni = true;
		
		
		//벚꽃 발사
		if (!m_bCherryCheck)
		{
			LightStart();
			CycloneStart();
			m_bCherryCheck = true;
		}


		if (m_pTargetAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_SKILLSPINNINGHEARTB)
		{

			_double dTargetTime = m_pTargetMesh->Get_TrackPos();


			//GroundWave 발사
			if (dTargetTime >= 0.3f && (!m_bGroundWaveCheck))
			{
				GroundWaveStart();
				m_bGroundWaveCheck = true;
			}

			if (!m_bTrailStart)
			{
				m_pTrail->SetAni(true);
				m_pTrail->SettingTrail();
				m_bTrailStart = true;
			}
		}
	}


	//오른쪽으로 돈뒤 쏘는 동작
	if (m_pPlayerAction[CWitchBlade::ACTION_SPINNINGHEARTR] == true)
	{
		m_bAni = true;
		
		//벚꽃 발사
		if (!m_bCherryCheck)
		{
			LightStart();
			CycloneStart();
			m_bCherryCheck = true;
		}

		if (m_pTargetAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_SKILLSPINNINGHEARTB)
		{

			_double dTargetTime = m_pTargetMesh->Get_TrackPos();


			//GroundWave 발사
			if (dTargetTime >= 0.3f && (!m_bGroundWaveCheck))
			{
				GroundWaveStart();
				m_bGroundWaveCheck = true;
			}

			if (!m_bTrailStart)
			{
				m_pTrail->SetAni(true);
				m_pTrail->SettingTrail();
				m_bTrailStart = true;
			}
		}
	}




	//쏘는 동작
	if (m_pTargetAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_SKILLSPININGHEARTC)
	{
		m_bShooingState = true;
		m_bTrailStart = false;
		m_pTrail->SetAni(false);

		if (!m_bSpinningThrownCheck)
		{
			//m_pSpinningBullet = CSpinningThrown::Create(m_pGraphicDev, L"SpinningBullet");
			//m_pSpinningBullet->Setting(m_pTargetTransform);
			//Engine::AddObject(Engine::LAYER_GAMELOGIC, L"SoftEffect", m_pSpinningBullet);
			
			m_bSpinningThrownCheck = true;
		}
	}
	else
	{
		m_bSpinningThrownCheck = false;
	}


	if (!m_bAni)
	{
		m_bCherryCheck = false;
		m_bShooingState = false;
		m_bGroundWaveCheck = false;
		return;
	}

	EndAniCheck();


	CycloneUpdate();
	LightUpdate();

	GroundWaveUpdate();

	m_pCyclone->Update_Object(fTimeDelta);
	m_pCherryBlossom->Update_Object(fTimeDelta);
	
	m_pTrail->Update_Object(fTimeDelta);
	m_pLightWave->Update_Object(fTimeDelta);
	m_pLightWaveTwo->Update_Object(fTimeDelta);


	for (int i = 0; i < 2; ++i)
	{
		m_pGroundWave[i]->Update_Object(fTimeDelta);
	}

}

void CSpinningHeart::Render_Object(void)
{
	if (!m_bAni)
		return;

	m_pCyclone->Render_Object();
	m_pCherryBlossom->Render_Object();

	m_pTrail->Render_Object();
	m_pLightWave->Render_Object();
	m_pLightWaveTwo->Render_Object();


	for (int i = 0; i < 2; ++i)
	{
		m_pGroundWave[i]->Render_Object();
	}

}

HRESULT CSpinningHeart::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;


	//===============================================================
	m_pCyclone = CCyclone::LoadCreate(m_pGraphicDev, L"Cyclone");
	m_pCyclone->SetAni(false);
	m_pCyclone->GetTransCom()->Set_Scale(1.3f, 1.3f, 1.3f);
	_vec4 vColor = _vec4(1.f, 0.f, 0.f, 1.f);
	m_pCyclone->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
	m_pCyclone->GetParticleInfo()->m_iPath = 2;
	//================================================================

	//LightWave======================================================
	m_pLightWave = CWave::Create(m_pGraphicDev, L"LightWave");
	m_pLightWave->SetAni(false);
	m_pLightWave->SetPattern(CWave::PATTERN_END);
	m_pLightWave->SetBilBoardCheck(true);
	m_pLightWave->SetRendererCheck(true);
	m_pLightWave->GetTransCom()->Set_Scale(0.5f, 0.5f, 0.5f);
	_vec4 Color = _vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_pLightWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pLightWave->GetParticleInfo()->m_iPath = 2;
	m_pLightWave->GetParticleInfo()->m_iTextureNumber = 36;
	//===============================================================

	//LightWaveTwo==================================================
	m_pLightWaveTwo = CWave::Create(m_pGraphicDev, L"LightWave");
	m_pLightWaveTwo->SetAni(false);
	m_pLightWaveTwo->SetPattern(CWave::PATTERN_END);
	m_pLightWaveTwo->SetBilBoardCheck(true);
	m_pLightWaveTwo->SetRendererCheck(true);
	m_pLightWaveTwo->GetTransCom()->Set_Scale(0.8f, 0.8f, 0.8f);
	Color = _vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_pLightWaveTwo->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pLightWaveTwo->GetParticleInfo()->m_iPath = 2;
	m_pLightWaveTwo->GetParticleInfo()->m_iTextureNumber = 35;

	//=============================================================

	//CherryBlossom================================================
	m_pCherryBlossom = CCherryBlossom::LoadCreate(m_pGraphicDev, L"SpinningCherry");
	m_pCherryBlossom->SetAni(false);
	m_pCherryBlossom->SetBlurRenderCheck(true);
	_vec4 vCherryColor = _vec4(1.f, 0.f, 0.f, 1.f);
	m_pCherryBlossom->GetParticleInfo()->m_dColor = (D3DXCOLOR)vCherryColor;
	m_pCherryBlossom->SetEqual(true);
	m_pCherryBlossom->GetParticleInfo()->m_iPath = 2;
	m_pCherryBlossom->GetTransCom()->Set_Scale(1.3f, 1.3f, 1.3f);
	//============================================================



	//Wave=========================================================================
	for (int i = 0; i < 2; ++i)
	{
		m_pGroundWave[i] = CWave::Create(m_pGraphicDev, L"GroundWave");
		m_pGroundWave[i]->SetAni(false);
		m_pGroundWave[i]->SetPattern(CWave::PATTERN_END);
		m_pGroundWave[i]->SetBilBoardCheck(false);
		m_pGroundWave[i]->SetRendererCheck(true);
		m_pGroundWave[i]->GetTransCom()->Set_Scale((i * 1.5f), (i * 1.f), (i * 1.f));
		_vec4 Color = _vec4(1.0f, 1.0f, 1.0f, 2.0f);
		m_pGroundWave[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pGroundWave[i]->GetParticleInfo()->m_iPath = 2;
		m_pGroundWave[i]->GetParticleInfo()->m_iTextureNumber = 3;
	}
	//===============================================================================

	return S_OK;
}

CSpinningHeart * CSpinningHeart::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CSpinningHeart*		pInstance = new CSpinningHeart(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("SpinningHeart Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpinningHeart::Free(void)
{
	CEffect::Free();
	Engine::Safe_Release(m_pCyclone);
	Engine::Safe_Release(m_pLightWave);
	Engine::Safe_Release(m_pLightWaveTwo);
	Engine::Safe_Release(m_pCherryBlossom);
	Engine::Safe_Release(m_pTrail);

	for (int i = 0; i < 2; ++i)
	{
		Engine::Safe_Release(m_pGroundWave[i]);
	}
}

HRESULT CSpinningHeart::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CSpinningHeart::SkillStart(void)
{
}

void CSpinningHeart::EndAniCheck(void)
{
	if (m_bAni == true && m_pCyclone->GetAni() == false && m_pLightWave->GetAni() == false && 
		m_pLightWaveTwo->GetAni() == false && m_pCherryBlossom->GetAni() == false && m_pTrail->GetAni() == false && m_pGroundWave[0]->GetAni() == false &&
		m_pGroundWave[1]->GetAni() == false)
		m_bAni = false;
}

HRESULT CSpinningHeart::Start_ForScene(void)
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


	//Sword Object
	Engine::CGameObject* pTargetObject = Engine::Find(L"Sword1");

	Engine::CTransform* pSwordTransform = (Engine::CTransform*)pTargetObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

	//검의 월드행렬
	m_pmatTarget = pSwordTransform->Get_WorldMatrixPointer();

	//Traill=============================================================================
	m_pTrail = CSpinningHeartTrail::Create(m_pGraphicDev, L"SpinningHeartTrail");
	m_pTrail->SetAni(false);
	m_pTrail->SetTargetMatrix(m_pmatTarget);
	//====================================================================================
	
	Engine::Safe_Release(pTargetObject);
	Engine::Safe_Release(pTargetTransform);
	Engine::Safe_Release(pGameObject);
	return S_OK;
}

void CSpinningHeart::CycloneUpdate(void)
{
	if (m_pCyclone->GetAni() == true)
	{
		_vec4 vColor = (_vec4)m_pCyclone->GetParticleInfo()->m_dColor;

		if (vColor.w < 0.f)
		{
			vColor.w = 1.0f;
			m_pCyclone->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
			m_pCyclone->SetAni(false);
			m_pCyclone->ResetInfo();
		}
		else
		{
			vColor.w -= 0.02f;
			m_pCyclone->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
			m_pCyclone->GetTransCom()->Rotation(Engine::ROT_Y, 20.f);
		}

	}



	if (m_pCherryBlossom->GetAni() == true)
	{
		_vec4 vColor = (_vec4)m_pCherryBlossom->GetParticleInfo()->m_dColor;

		if (vColor.w < 0.f)
		{
			vColor.w = 1.0f;
			m_pCherryBlossom->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
			m_pCherryBlossom->SetAni(false);
		}
		else
		{
			vColor.w -= 0.01f;
			m_pCherryBlossom->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
		}

	}
}

void CSpinningHeart::CycloneStart(void)
{
	m_pCyclone->SetAni(true);
	_vec3 vPos = m_pTargetTransform->Get_Now_Position();
	m_pCyclone->GetTransCom()->Set_Position(&vPos);
	m_pCyclone->GetTransCom()->Set_Scale(1.f, 1.f, 1.f);
	D3DXCOLOR vColor = _vec4(1.f, 0.f, 0.f, 1.f);
	m_pCyclone->GetParticleInfo()->m_dColor = vColor;


	//CherryBlossom==========================================
	_vec3 Look;
	m_pTargetTransform->Get_Infomation(Engine::INFO_LOOK, &Look);
	D3DXVec3Normalize(&Look, &Look);

	Look *= 3.f;	
	vPos += Look;
	m_pCherryBlossom->SetAni(true);
	m_pCherryBlossom->GetTransCom()->Set_Position(&vPos);

	D3DXCOLOR vCherryColor = _vec4(1.f, 0.f, 0.f, 1.f);
	m_pCherryBlossom->GetParticleInfo()->m_dColor = vCherryColor;

	//=======================================================
}

void CSpinningHeart::LightStart(void)
{
	//LightWave==================================================
	m_pLightWave->SetAni(true);
	_vec3 vPos = m_pTargetTransform->Get_Now_Position();
	m_pLightWave->GetTransCom()->Set_Position(&vPos);
	m_pLightWave->GetTransCom()->Set_Scale(0.5f, 0.5f, 0.5f);
	_vec4 Color = _vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_pLightWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	//===========================================================

	//LightWaveTwo===============================================
	m_pLightWaveTwo->SetAni(true);
	m_pLightWaveTwo->GetTransCom()->Set_Position(&vPos);
	m_pLightWaveTwo->GetTransCom()->Set_Scale(0.8f, 0.8f, 0.8f);
	Color = _vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_pLightWaveTwo->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	//===========================================================


}

void CSpinningHeart::LightUpdate(void)
{
	if (m_pLightWave->GetAni() == true)
	{

		_vec4 vColor = (_vec4)m_pLightWave->GetParticleInfo()->m_dColor;

		if (vColor.w <= 0.f)
		{
			m_pLightWave->SetAni(false);
		}
		else
		{
			vColor.w -= 0.03f;
			m_pLightWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
			m_pLightWave->GetTransCom()->Add_Scale(0.2f);
		}

	}




	if(m_pLightWaveTwo->GetAni() == true)
	{
		_vec4 vColor = (_vec4)m_pLightWaveTwo->GetParticleInfo()->m_dColor;

		if (vColor.w <= 0.f)
		{
			m_pLightWaveTwo->SetAni(false);
		}
		else
		{
			vColor.w -= 0.03f;
			m_pLightWaveTwo->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
			m_pLightWaveTwo->GetTransCom()->Add_Scale(0.2f);
		}

	}
}

void CSpinningHeart::GroundWaveStart(void)
{
	_vec3 vPos = m_pTargetTransform->Get_Now_Position();
	vPos.y += 0.5f;

	for (int i = 0; i < 2; ++i)
	{
		m_pGroundWave[i]->SetAni(true);
		m_pGroundWave[i]->GetTransCom()->Set_Position(&vPos);
		m_pGroundWave[i]->GetTransCom()->Set_Scale((i * 1.5f), (i * 1.f), (i * 1.f));
		_vec3 vAngle = _vec3(90.f, 0.f, 0.f);
		m_pGroundWave[i]->GetTransCom()->Set_Angle(&vAngle);
		_vec4 Color = _vec4(1.0f, 1.0f, 1.0f, 2.f);
		m_pGroundWave[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	}
}

void CSpinningHeart::GroundWaveUpdate(void)
{
	_vec3 vPos = m_pTargetTransform->Get_Now_Position();
	vPos.y += 0.5f;

	for (int i = 0; i < 2; ++i)
	{
		if (!m_pGroundWave[i]->GetAni())
			continue ;

	
		_vec4 vColor = (_vec4)m_pGroundWave[i]->GetParticleInfo()->m_dColor;


		if (vColor.w <= 0.f)
		{
			if (m_bShooingState)
			{
				m_pGroundWave[i]->SetAni(false);
			}
			else
			{
				m_pGroundWave[i]->GetTransCom()->Set_Position(&vPos);
				m_pGroundWave[i]->GetTransCom()->Set_Scale((i * 1.5f), (i * 1.f), (i * 1.f));
				_vec4 Color = _vec4(1.0f, 1.0f, 1.0f, 2.0f);
				m_pGroundWave[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
			}
		}
		else
		{
			vColor.w -= 0.05f;
			m_pGroundWave[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
			m_pGroundWave[i]->GetTransCom()->Add_Scale(0.1f);
		}


	}


}
