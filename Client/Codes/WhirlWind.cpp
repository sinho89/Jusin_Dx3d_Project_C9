#include "stdafx.h"
#include "WhirlWind.h"
#include "Export_Engine.h"
#include "Bardiel.h"
#include "Wave.h"
#include "Sprite.h"
#include "Phase2_Trail.h"

CWhirlWind::CWhirlWind(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
:CEffect(pGraphicDev, wstrName)
, m_pvecAniInfo(NULL)
, m_pTargetMesh(NULL)
, m_pTargetTransform(NULL)
, m_pTargetAnimator(NULL)
, m_pHammerTrail(NULL)
, m_pWorldMat(NULL)
, m_pHammerDust(NULL)
, m_pHammerEnd(NULL)	
, m_bFirstSwing_Start(FALSE)
, m_bFirstSwing_End(FALSE)
, m_bSecondSwing_Start(FALSE)
, m_bSecondSwing_End(FALSE)
, m_bHammerDust(FALSE)
, m_dDustTime(0)
{	
	ZeroMemory(&m_pActionDust, sizeof(CSprite*) * 40);
	
}

CWhirlWind::~CWhirlWind(void)
{
}

void CWhirlWind::Update_Object(const _float & fTimeDelta)
{
	if (g_bBardielDead)
		return;

	if (m_pTargetAnimator->Get_AnimationIndex() == CBardiel::BARDIEL::ATT_WHIRLWIND)
	{
		_double dTargetTime = m_pTargetMesh->Get_TrackPos();

		//Trail----------------------------------------------
		if (dTargetTime >= 2.183334f && (!m_bFirstSwing_Start))
		{
			m_bFirstSwing_Start = TRUE;
			m_pHammerTrail->SetAni(TRUE);
			m_pHammerTrail->SetColor(_vec4(0.5450f, 0.0f, 0.0f, 1.0f));
			m_pHammerTrail->SettingTrail();
		}

		if (dTargetTime >= 4.454001f && (!m_bFirstSwing_End))
		{
			_vec4 vColor = m_pHammerTrail->GetColor();
			vColor.w -= 0.095f;
			m_pHammerTrail->SetColor(vColor);

			if (m_pHammerTrail->GetColor().w <= 0.0f)
			{
				m_pHammerTrail->SetAni(FALSE);
				m_bFirstSwing_End = TRUE;
			}
		}

		if (dTargetTime >= 7.161335f && (!m_bSecondSwing_Start))
		{
			//해머
			m_bSecondSwing_Start = TRUE;
			m_pHammerTrail->SetAni(TRUE);
			m_pHammerTrail->SetColor(_vec4(0.5450f, 0.0f, 0.0f, 1.0f));
			m_pHammerTrail->SettingTrail();
		}

		if (dTargetTime >= 8.2093336f && (!m_bSecondSwing_End))
		{
			_vec4 vColor = m_pHammerTrail->GetColor();
			vColor.w -= 0.095f;
			m_pHammerTrail->SetColor(vColor);

			if (m_pHammerTrail->GetColor().w <= 0.0f)
			{
				m_pHammerTrail->SetAni(FALSE);
				m_bSecondSwing_End = TRUE;
			}
		}
		//--------------------------------------------------------------
		//Dust-----------------------------------------------------------
		if (dTargetTime >= 2.183334f &&  (4.366f >= dTargetTime))
		{
			m_dDustTime += fTimeDelta;

			if (m_dDustTime >= 0.03f)
			{
				m_dDustTime = 0;

				for (int i = 0; i < 40; ++i)
				{
					if (m_pActionDust[i]->GetAni())
						continue;
					
					ActionDustStart(i);
					break ;
				}
			}
		}


		//해머 Dust
		if (dTargetTime >= 7.10f && (!m_bHammerDust))
		{
			m_bHammerDust = TRUE;
			HammerDustStart();
		}
		//----------------------------------------------------------------		

		if (dTargetTime >= 2.5181818f && dTargetTime < 3.8426668f)
		{
			Engine::SetRadialBlurPower(-0.1f);
			Engine::SetRadialBlur(TRUE);
		}

		if (dTargetTime >= 3.8426668f && dTargetTime < 3.9f)
		{
			Engine::SetRadialBlurPower(-0.25f);
			Engine::SetRadialBlur(FALSE);
		}
	}
	else
	{
		m_bFirstSwing_Start = FALSE;
		m_bFirstSwing_End = FALSE;
		m_bSecondSwing_Start = FALSE;
		m_bSecondSwing_End = FALSE;
		m_bHammerDust = FALSE;
		m_dDustTime = 0;
	}

	ActionDustUpdate();

	m_pHammerTrail->Update_Object(fTimeDelta);
	m_pHammerDust->Update_Object(fTimeDelta);

	for (int i = 0; i < 40; ++i)
		m_pActionDust[i]->Update_Object(fTimeDelta);	
}

void CWhirlWind::Render_Object(void)
{
	if (g_bBardielDead)
		return;

	m_pHammerTrail->Render_Object();
	m_pHammerDust->Render_Object();

	for (int i = 0; i < 40; ++i)
		m_pActionDust[i]->Render_Object();	
}

HRESULT CWhirlWind::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	for (int i = 0; i < 40; ++i)
	{
		m_pActionDust[i] = CSprite::LoadCreate(m_pGraphicDev, L"PenaltyActionDust");
		m_pActionDust[i]->SetAni(false);
		_vec4 Color = _vec4(0.3f, 0.3f, 0.3f, 0.6f);
		m_pActionDust[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pActionDust[i]->GetParticleInfo()->m_iPath = 3;
		m_pActionDust[i]->SetRenderCheck(true);
		m_pActionDust[i]->GetTransCom()->Set_Scale(2.0f, 2.0f, 2.0f);
		m_pActionDust[i]->SetBlurCheck(true);
		m_pActionDust[i]->SetBlurCnt(2);
		m_pActionDust[i]->SetBlurPass(3);
		m_pActionDust[i]->SetBlurPower(1.3f);
		m_pActionDust[i]->SetBlurType(Engine::BLUR_CROSS);
		m_pActionDust[i]->SetBlurValue(180.0f);
	}

	m_pHammerDust = CSprite::LoadCreate(m_pGraphicDev, L"PenaltyActionDust");
	m_pHammerDust->SetAni(false);
	_vec4 Color = _vec4(0.3f, 0.3f, 0.3f, 1.f);
	m_pHammerDust->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pHammerDust->GetParticleInfo()->m_iPath = 3;
	m_pHammerDust->SetRenderCheck(true);
	m_pHammerDust->GetTransCom()->Set_Scale(2.0f, 2.0f, 2.0f);
	m_pHammerDust->GetParticleInfo()->m_tSpriteInfo.m_fSpriteSpeed = 20.0f;
	m_pHammerDust->SetBlurCheck(true);
	m_pHammerDust->SetBlurCnt(2);
	m_pHammerDust->SetBlurPass(3);
	m_pHammerDust->SetBlurPower(1.1f);
	m_pHammerDust->SetBlurType(Engine::BLUR_CROSS);
	m_pHammerDust->SetBlurValue(128.0f);

	return S_OK;
}

CWhirlWind * CWhirlWind::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CWhirlWind*		pInstance = new CWhirlWind(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("WhirlWind Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CWhirlWind::Free(void)
{
	::Safe_Release(m_pHammerTrail);
	::Safe_Release(m_pHammerDust);

	for (int i = 0; i < 40; ++i)
		::Safe_Release(m_pActionDust[i]);
	
	CEffect::Free();
}

HRESULT CWhirlWind::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CWhirlWind::SkillStart(void)
{
}

void CWhirlWind::EndAniCheck(void)
{
}

HRESULT CWhirlWind::Start_ForScene(void)
{
	if (m_bOnceStart)
		return S_OK;

	m_bOnceStart = true;

	Engine::CGameObject* pGameObject = Engine::Find(L"Mesh_Bardiel");
	if (pGameObject == NULL)
		return E_FAIL;

	m_pBardiel = ((CBardiel*)pGameObject);
	m_pTargetAnimator = ((CBardiel*)pGameObject)->GetAnimatorInfo();

	//Target Mesh
	m_pTargetMesh = (Engine::CDynamicMesh*)pGameObject->GetComponent<Engine::CDynamicMesh>(Engine::COMID_STATIC);

	//Hanuman TransCom
	m_pTargetTransform = (Engine::CTransform*)pGameObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

	m_pWorldMat = ((CBardiel*)pGameObject)->GetTransCom()->Get_WorldMatrixPointer();

	m_pHammerEnd = m_pTargetMesh->Get_FrameByName("_63");
	const _matrix* pHammerEnd = m_pTargetMesh->Get_FrameByName("_62");

	m_pHammerTrail = CPhase2_Trail::Create(m_pGraphicDev, L"WhirlWind_Trail");
	m_pHammerTrail->SetAni(FALSE);
	m_pHammerTrail->SetTargetMatrix(m_pHammerEnd, pHammerEnd);
	m_pHammerTrail->SetTargetWorldMat(m_pWorldMat);
	m_pHammerTrail->SetTextureNum(23);
	m_pHammerTrail->SetColor(_vec4(0.5450f, 0.0f, 0.0f, 1.0f));

	::Safe_Release(pGameObject);

	return S_OK;
}

void CWhirlWind::ActionDustStart(int iIndex)
{
	_vec3 vLocalPos; //해머의 로컬 위치
	memcpy(&vLocalPos, &m_pHammerEnd->m[3][0], sizeof(_vec3));

	//m_pWorldMat (바르디엘의 월드행렬 포인터)

	D3DXVec3TransformCoord(&vLocalPos, &vLocalPos, m_pWorldMat);
	
	vLocalPos.y = m_pBardiel->GetTransCom()->Get_Now_Position().y;

	m_pActionDust[iIndex]->SetAni(true);
	m_pActionDust[iIndex]->GetTransCom()->Set_Position(&vLocalPos);
	m_pActionDust[iIndex]->GetTransCom()->Set_Scale(1.0f, 1.0f, 1.0f);

}

void CWhirlWind::ActionDustUpdate(void)
{
	for (int i = 0; i < 40; ++i)
	{
		if (!(m_pActionDust[i]->GetAni()))
			continue;

		m_pActionDust[i]->GetTransCom()->Add_Scale(0.1f);

	}
}

void CWhirlWind::HammerDustStart(void)
{
	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);


	D3DXVec3TransformCoord(&vPos, &vPos, m_pHammerEnd);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	m_pHammerDust->SetAni(TRUE);
	m_pHammerDust->GetTransCom()->Set_Position(&vPos);
}
