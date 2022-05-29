#include "stdafx.h"
#include "Lava.h"
#include "Bardiel.h"
#include "Wave.h"
#include "Sprite.h"
#include "LaveTrail.h"

CLava::CLava(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
	:CEffect(pGraphicDev, wstrName)
	, m_pvecAniInfo(NULL)
	, m_pTargetMesh(NULL)
	, m_pTargetTransform(NULL)
	, m_pTargetAnimator(NULL)
	, m_pRightLeg_Trail(NULL)
	, m_pWorldMat(NULL)
	, m_pRightFootMat(NULL)
	, m_bRightLeg_Swing_Start(FALSE)
	, m_bRightLeg_Swing_End(FALSE)
{
	m_bAni = false;

	ZeroMemory(&m_pGroundFireWave, sizeof(CWave*) * 40);
	ZeroMemory(&m_pGroundFire, sizeof(CSprite*) * 40);
	ZeroMemory(&m_pGroundFireSmokeSprite, sizeof(CSprite*) * 30);

	for (int i = 0; i < 40; ++i)
		m_bGroundFireWave[i] = FALSE;

	for (int i = 0; i < 30; ++i)
		m_bGroundFireSmokeSprite[i] = FALSE;
}

CLava::~CLava(void)
{
}

void CLava::Update_Object(const _float & fTimeDelta)
{
	if (g_bBardielDead)
		return;

	if (m_pTargetAnimator->Get_AnimationIndex() == CBardiel::BARDIEL::CHANGEPHASE2)
	{
		m_bAni = false;
	}

	if (m_pTargetAnimator->Get_AnimationIndex() == CBardiel::BARDIEL::LAVA)
	{
		m_bAni = true;
		_double dTargetTime = m_pTargetMesh->Get_TrackPos();

		//트레일=========================================================
		if (dTargetTime >= 2.267999f && (!m_bRightLeg_Swing_Start))
		{
			m_bRightLeg_Swing_Start = TRUE;

			m_pRightLeg_Trail->SetAni(TRUE);
			m_pRightLeg_Trail->SetColor(_vec4(0.5450f, 0.0f, 0.0f, 1.0f));
			m_pRightLeg_Trail->SettingTrail();
		}

		if (dTargetTime >= 3.0f && (!m_bRightLeg_Swing_End))
		{
			_vec4 vColor = m_pRightLeg_Trail->GetColor();
			vColor.w -= 0.095f;
			m_pRightLeg_Trail->SetColor(vColor);

			if (m_pRightLeg_Trail->GetColor().w <= 0.0f)
			{
				m_pRightLeg_Trail->SetAni(FALSE);

				m_bRightLeg_Swing_End = TRUE;
			}
		}
		//==============================================================

		//먼지================================================================
		for (int i = 0; i < 30; ++i)
		{
			if (dTargetTime >= (2.3f + (0.06f * i)) && (!m_bGroundFireSmokeSprite[i]))
			{
				m_bGroundFireSmokeSprite[i] = TRUE;
				GroundFireSmokeSpriteStart(i);
			}
		}
		//================================================================

		//불 웨이브======================================================
		for (int i = 0; i < 40; ++i)
		{
			if (dTargetTime >= (2.3f + (0.03f * i)) && (!m_bGroundFireWave[i]))
			{
				m_bGroundFireWave[i] = TRUE;
				GroundFireWaveStart(i);
				GroundFireSpriteStart(i);
			}
		}
		//==============================================================
	}
	else
	{
		m_bRightLeg_Swing_Start = FALSE;
		m_bRightLeg_Swing_End = FALSE;
		
		for (int i = 0; i < 30; ++i)
			m_bGroundFireSmokeSprite[i] = FALSE;

		for (int i = 0; i < 40; ++i)
			m_bGroundFireWave[i] = FALSE;
	}

	if (!m_bAni)
		return;

	GroundFireWaveUpdate();
	GroundFireSmokeSpriteUpdate();

	m_pRightLeg_Trail->Update_Object(fTimeDelta);

	for (int i = 0; i < 30; ++i)
		m_pGroundFireSmokeSprite[i]->Update_Object(fTimeDelta);

	for (int i = 0; i < 40; ++i)
		m_pGroundFire[i]->Update_Object(fTimeDelta);

	for (int i = 0; i < 40; ++i)
		m_pGroundFireWave[i]->Update_Object(fTimeDelta);
}

void CLava::Render_Object(void)
{
	if (g_bBardielDead)
		return;

	if (!m_bAni)
		return;

	m_pRightLeg_Trail->Render_Object();

	for (int i = 0; i < 30; ++i)
		m_pGroundFireSmokeSprite[i]->Render_Object();

	for (int i = 0; i < 40; ++i)
		m_pGroundFire[i]->Render_Object();

	for (int i = 0; i < 40; ++i)
		m_pGroundFireWave[i]->Render_Object();
}

HRESULT CLava::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	for (int i = 0; i < 40; ++i)
	{
		m_pGroundFireWave[i] = CWave::Create(m_pGraphicDev, L"FireGround");
		m_pGroundFireWave[i]->SetAni(FALSE);
		m_pGroundFireWave[i]->SetPattern(CWave::PATTERN_END);
		m_pGroundFireWave[i]->SetBilBoardCheck(FALSE);
		m_pGroundFireWave[i]->SetRendererCheck(TRUE);
		m_pGroundFireWave[i]->GetParticleInfo()->m_iPath = 2;
		m_pGroundFireWave[i]->GetParticleInfo()->m_iTextureNumber = 58;
		D3DXCOLOR vColor = _vec4(0.5450f, 0.0f, 0.0f, 1.0f);
		m_pGroundFireWave[i]->GetParticleInfo()->m_dColor = vColor;
		m_pGroundFireWave[i]->GetParticleInfo()->m_bBillBoard = TRUE;
		m_pGroundFireWave[i]->SetBlurCheck(true);
		m_pGroundFireWave[i]->SetBlurPass(2);
		m_pGroundFireWave[i]->SetBlurCnt(2);
		m_pGroundFireWave[i]->SetBlurPower(1.1f);
		m_pGroundFireWave[i]->SetBlurValue(128.0f);
		m_pGroundFireWave[i]->SetBlurType(Engine::BLUR_CROSS);
	}

	for (int i = 0; i < 30; ++i)
	{
		m_pGroundFireSmokeSprite[i] = CSprite::LoadCreate(m_pGraphicDev, L"GroundFireSmoke");
		m_pGroundFireSmokeSprite[i]->SetAni(false);
		_vec4 Color = _vec4(0.3f, 0.3f, 0.3f, 1.f);
		m_pGroundFireSmokeSprite[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pGroundFireSmokeSprite[i]->GetParticleInfo()->m_iPath = 3;
		m_pGroundFireSmokeSprite[i]->SetRenderCheck(true);
		m_pGroundFireSmokeSprite[i]->GetTransCom()->Set_Scale(1.0f, 1.0f, 1.0f);
		m_pGroundFireSmokeSprite[i]->GetParticleInfo()->m_tSpriteInfo.m_fSpriteSpeed = 150.0f;

		m_pGroundFireSmokeSprite[i]->SetBlurCheck(true);
		m_pGroundFireSmokeSprite[i]->SetBlurCnt(2);
		m_pGroundFireSmokeSprite[i]->SetBlurPass(3);
		m_pGroundFireSmokeSprite[i]->SetBlurPower(1.05f);
		m_pGroundFireSmokeSprite[i]->SetBlurType(Engine::BLUR_CROSS);
		m_pGroundFireSmokeSprite[i]->SetBlurValue(128.0f);
	}

	for (int i = 0; i < 40; ++i)
	{
		m_pGroundFire[i] = CSprite::LoadCreate(m_pGraphicDev, L"GroundFire");
		m_pGroundFire[i]->SetAni(false);
		_vec4 Color = _vec4(0.8039f, 0.3607f, 0.3607f, 1.f);
		m_pGroundFire[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pGroundFire[i]->GetParticleInfo()->m_iPath = 2;
		m_pGroundFire[i]->SetRenderCheck(true);
		m_pGroundFire[i]->GetTransCom()->Set_Scale(1.0f, 1.0f, 1.0f);
		m_pGroundFire[i]->GetParticleInfo()->m_tSpriteInfo.m_fSpriteSpeed = 60.0f;

		m_pGroundFire[i]->SetBlurCheck(true);
		m_pGroundFire[i]->SetBlurCnt(2);
		m_pGroundFire[i]->SetBlurPass(3);
		m_pGroundFire[i]->SetBlurPower(1.05f);
		m_pGroundFire[i]->SetBlurType(Engine::BLUR_CROSS);
		m_pGroundFire[i]->SetBlurValue(200.0f);
	}

	return S_OK;
}

CLava * CLava::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CLava*		pInstance = new CLava(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("Rava Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CLava::Free(void)
{
	::Safe_Release(m_pRightLeg_Trail);

	for (int i = 0; i < 30; ++i)
		::Safe_Release(m_pGroundFireSmokeSprite[i]);

	for (int i = 0; i < 40; ++i)
		::Safe_Release(m_pGroundFireWave[i]);

	for (int i = 0; i < 40; ++i)
		::Safe_Release(m_pGroundFire[i]);

	CEffect::Free();
}

HRESULT CLava::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CLava::SkillStart(void)
{
}

void CLava::EndAniCheck(void)
{
}

HRESULT CLava::Start_ForScene(void)
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

	//Hanuman TransCom
	m_pTargetTransform = (Engine::CTransform*)pGameObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

	m_pWorldMat = ((CBardiel*)pGameObject)->GetTransCom()->Get_WorldMatrixPointer();

	m_pRightFootMat = m_pTargetMesh->Get_FrameByName("_55");
	const _matrix* pEndpMat = m_pTargetMesh->Get_FrameByName("_52");

	m_pRightLeg_Trail = CLaveTrail::Create(m_pGraphicDev, L"RaveTrail_RightLeg_Trail");
	m_pRightLeg_Trail->SetAni(FALSE);
	m_pRightLeg_Trail->SetTargetMatrix(m_pRightFootMat, pEndpMat);
	m_pRightLeg_Trail->SetTargetWorldMat(m_pWorldMat);
	m_pRightLeg_Trail->SetTextureNum(23);
	m_pRightLeg_Trail->SetColor(_vec4(0.5450f, 0.0f, 0.0f, 1.0f));

	::Safe_Release(pGameObject);

	return S_OK;
}

void CLava::GroundFireWaveStart(int iIndex)
{
	_vec4 Color = _vec4(0.5450f, 0.0f, 0.0f, 1.0f);
	_vec3 vAngle = _vec3(90.f, 0.f, 0.f);
	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vPos, &vPos, m_pRightFootMat);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	vPos.y -= 1.5f;

	m_pGroundFireWave[iIndex]->SetAni(TRUE);
	m_pGroundFireWave[iIndex]->GetTransCom()->Set_Position(&vPos);
	m_pGroundFireWave[iIndex]->GetTransCom()->Set_Scale(5.0f, 5.0f, 5.0f);
	m_pGroundFireWave[iIndex]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pGroundFireWave[iIndex]->GetTransCom()->Set_Angle(&vAngle);
}

void CLava::GroundFireWaveUpdate(void)
{
	for (int i = 0; i < 40; ++i)
	{
		if (m_pGroundFireWave[i]->GetAni())
		{
			_vec4 vColor = (_vec4)m_pGroundFireWave[i]->GetParticleInfo()->m_dColor;

			if (vColor.w <= 0.f)
			{
				m_pGroundFireWave[i]->SetAni(FALSE);
			}
			else
			{
				vColor.w -= 0.008f;
				m_pGroundFireWave[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
				//m_pGroundFireWave[i]->GetTransCom()->Add_Scale(0.01f);
			}
		}
	}
}

void CLava::GroundFireSmokeSpriteStart(int iIndex)
{
	_vec4 Color = _vec4(0.5f, 0.5f, 0.5f, 1.f);
	_vec3 vPos = _vec3(0.0f, 1.0f, 0.0f);

	D3DXVec3TransformCoord(&vPos, &vPos, m_pRightFootMat);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	vPos.y -= 8.0f;

	m_pGroundFireSmokeSprite[iIndex]->SetAni(TRUE);
	m_pGroundFireSmokeSprite[iIndex]->GetTransCom()->Set_Position(&vPos);
	m_pGroundFireSmokeSprite[iIndex]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
}

void CLava::GroundFireSmokeSpriteUpdate(void)
{
	for (int i = 0; i < 30; ++i)
	{
		if (m_pGroundFireSmokeSprite[i]->GetAni())
		{
			_vec3 vPos = m_pGroundFireSmokeSprite[i]->GetTransCom()->Get_Now_Position();
			vPos.y += 0.1f;
			m_pGroundFireSmokeSprite[i]->GetTransCom()->Set_Position(&vPos);
		}
	}
}

void CLava::GroundFireSpriteStart(int iIndex)
{
	_vec4 Color = _vec4(100.0f / 255.f, 0.0f / 255.f, 0.f / 255.f, 1.0f);
	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vPos, &vPos, m_pRightFootMat);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	vPos.y += 15.0f;

	m_pGroundFire[iIndex]->SetAni(TRUE);
	m_pGroundFire[iIndex]->GetTransCom()->Set_Position(&vPos);
	m_pGroundFire[iIndex]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
}

void CLava::GroundFireSpriteUpdate(void)
{
}
