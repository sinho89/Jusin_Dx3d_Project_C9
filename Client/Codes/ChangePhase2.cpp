#include "stdafx.h"
#include "ChangePhase2.h"
#include "Export_Engine.h"
#include "Bardiel.h"
#include "Wave.h"
#include "Curtain.h"

CChangePhase2::CChangePhase2(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
	:CEffect(pGraphicDev, wstrName)
	, m_pvecAniInfo(NULL)
	, m_pTargetMesh(NULL)
	, m_pTargetTransform(NULL)
	, m_pTargetAnimator(NULL)
	, m_pCurtain(NULL)
	, m_pCallHammerWave(NULL)
	, m_pWorldMat(NULL)
	, m_pLeftElbowMat(NULL)
	, m_bArmorCurtain(FALSE)
	, m_bCallHammerWave(FALSE)
{
}

CChangePhase2::~CChangePhase2(void)
{
}

void CChangePhase2::Update_Object(const _float & fTimeDelta)
{
	if (g_bBardielDead)
		return;

	if (m_pTargetAnimator->Get_AnimationIndex() == CBardiel::BARDIEL::CHANGEPHASE2)
	{
		_double dTargetTime = m_pTargetMesh->Get_TrackPos();

		if (2.8306667f <= dTargetTime && (!m_bCallHammerWave))
		{
			m_bCallHammerWave = TRUE;
			CurtainStart();
		}

		if (7.334f <= dTargetTime && (!m_bArmorCurtain))
		{
			m_bArmorCurtain = TRUE;
			CallHammerWaveStart();
		}

		if (dTargetTime >= 3.088f && dTargetTime < 3.12f)
		{
			Engine::SetRadialBlurPower(-0.1f);
			Engine::SetRadialBlur(TRUE);
		}

		if (dTargetTime >= 3.12f && dTargetTime < 3.17f)
		{
			Engine::SetRadialBlurPower(-0.25f);
			Engine::SetRadialBlur(FALSE);
		}

		if (dTargetTime >= 10.93666f && dTargetTime < 11.2f)
		{
			Engine::SetRadialBlurPower(-0.1f);
			Engine::SetRadialBlur(TRUE);
		}

		if (dTargetTime >= 11.2f && dTargetTime < 11.7f)
		{
			Engine::SetRadialBlurPower(-0.25f);
			Engine::SetRadialBlur(FALSE);
		}
	}
	else
	{
		m_bCallHammerWave = FALSE;
		m_bArmorCurtain = FALSE;
	}

	CallHammerWaveUpdate();
	CurtainUpdate();

	m_pCurtain->Update_Object(fTimeDelta);
	m_pCallHammerWave->Update_Object(fTimeDelta);
}

void CChangePhase2::Render_Object(void)
{
	if (g_bBardielDead)
		return;

	m_pCurtain->Render_Object();
	m_pCallHammerWave->Render_Object();
}

HRESULT CChangePhase2::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	_vec4 Color = _vec4(1.0f, -0.1f, -0.1f, 2.0f);
	m_pCallHammerWave = CWave::Create(m_pGraphicDev, L"CallHammerWave");
	m_pCallHammerWave->SetAni(false);
	m_pCallHammerWave->SetPattern(CWave::PATTERN_END);
	m_pCallHammerWave->SetRendererCheck(true);
	m_pCallHammerWave->GetTransCom()->Set_Scale(2.0f, 2.0f, 2.0f);
	m_pCallHammerWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pCallHammerWave->GetParticleInfo()->m_iPath = 2;
	m_pCallHammerWave->GetParticleInfo()->m_iTextureNumber = 69;
	m_pCallHammerWave->SetEffectBlurCheck(TRUE);
	m_pCallHammerWave->SetBlurPass(2);
	m_pCallHammerWave->SetBlurCnt(2);
	m_pCallHammerWave->SetBlurPower(1.3f);
	m_pCallHammerWave->SetBlurValue(128.0f);
	m_pCallHammerWave->SetBlurType(Engine::BLUR_CROSS);


	m_pCurtain = CCurtain::LoadCreate(m_pGraphicDev, L"GroundUpAura");
	m_pCurtain->SetYBillBoard(true);
	m_pCurtain->SetAni(false);
	m_pCurtain->SetEffectBlurCheck(TRUE);
	m_pCurtain->SetBlurPass(2);
	m_pCurtain->SetBlurCnt(2);
	m_pCurtain->SetBlurPower(1.3f);
	m_pCurtain->SetBlurValue(100.0f);
	m_pCurtain->SetBlurType(Engine::BLUR_HORIZON);

	return S_OK;
}

CChangePhase2 * CChangePhase2::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CChangePhase2*		pInstance = new CChangePhase2(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("ChangePhase2 Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CChangePhase2::Free(void)
{
}

HRESULT CChangePhase2::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CChangePhase2::SkillStart(void)
{
}

void CChangePhase2::EndAniCheck(void)
{
}

HRESULT CChangePhase2::Start_ForScene(void)
{
	if (m_bOnceStart)
		return S_OK;

	m_bOnceStart = true;

	Engine::CGameObject* pGameObject = Engine::Find(L"Mesh_Bardiel");
	if (pGameObject == NULL)
		return E_FAIL;

	m_pTargetAnimator = ((CBardiel*)pGameObject)->GetAnimatorInfo();

	//Target Mesh
	m_pTargetMesh = (Engine::CDynamicMesh*)pGameObject->GetComponent<Engine::CDynamicMesh>(Engine::COMID_STATIC);

	//Bardiel TransCom
	m_pTargetTransform = (Engine::CTransform*)pGameObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

	m_pWorldMat = ((CBardiel*)pGameObject)->GetTransCom()->Get_WorldMatrixPointer();
	m_pLeftElbowMat = m_pTargetMesh->Get_FrameByName("_35");

	Engine::Safe_Release(pGameObject);
	return S_OK;
}

void CChangePhase2::CurtainStart(void)
{
	m_pCurtain->SetAni(true);

	_vec3 vBardielPos = m_pTargetTransform->Get_Now_Position();
	m_pCurtain->GetTransCom()->Set_Position(&vBardielPos);
	m_pCurtain->GetTransCom()->Set_Scale(2.5f, 2.5f, 2.5f);

	_vec4 vColor = m_pCurtain->GetParticleInfo()->m_dColor;

	vColor.w = 1.f;

	m_pCurtain->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
}

void CChangePhase2::CurtainUpdate(void)
{
	if (m_pCurtain->GetAni())
	{
		_vec4 vColor = (_vec4)m_pCurtain->GetParticleInfo()->m_dColor;

		if (vColor.w <= 0.f)
		{
			m_pCurtain->SetAni(false);
		}
		else
		{
			vColor.w -= 0.009f;
			m_pCurtain->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
			_vec3 vPos = m_pCurtain->GetTransCom()->Get_Now_Position();
			vPos.y += 0.5f;
			m_pCurtain->GetTransCom()->Set_Position(&vPos);
		}
	}	
}

void CChangePhase2::CallHammerWaveStart(void)
{
	_vec3 vAngle = _vec3(90.f, 0.f, 0.f);
	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vPos, &vPos, m_pLeftElbowMat);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	vPos.y = m_pTargetTransform->Get_Now_Position().y + 3.0f;

	m_pCallHammerWave->SetAni(TRUE);
	m_pCallHammerWave->GetTransCom()->Set_Position(&vPos);
	m_pCallHammerWave->GetTransCom()->Set_Angle(&vAngle);
}

void CChangePhase2::CallHammerWaveUpdate(void)
{
	if (m_pCallHammerWave->GetAni())
	{
		_vec4 vColor = (_vec4)m_pCallHammerWave->GetParticleInfo()->m_dColor;

		if (vColor.w <= 0.f)
		{
			m_pCallHammerWave->SetAni(FALSE);
		}
		else
		{
			vColor.w -= 0.0075f;
			m_pCallHammerWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
			m_pCallHammerWave->GetTransCom()->Rotation(Engine::ROT_Y, 7.5f);
		}
	}
}
