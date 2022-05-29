#include "stdafx.h"
#include "FireBall.h"
#include "Export_Engine.h"
#include "Bardiel.h"
#include "Wave.h"
#include "Sprite.h"
#include "FireBallParticle.h"

CFireBall::CFireBall(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
	:CEffect(pGraphicDev, wstrName)
	, m_pvecAniInfo(NULL)
	, m_pTargetMesh(NULL)
	, m_pTargetTransform(NULL)
	, m_pTargetAnimator(NULL)
	, m_pWorldMat(NULL)
	, m_pRightFootMat(NULL)
	, m_pFireBallParticle(NULL)
	, m_pFireBallSprite(NULL)
	, m_pBardiel(NULL)
	, m_pLeftHandMat(NULL)
	, m_pFireBallShotDustSprite(NULL)
	, m_bFireBallShotDustSprite(FALSE)
	, m_bFireBallParticle(FALSE)
	, m_bFireBallSprite(FALSE)
{
	m_bAni = false;

	ZeroMemory(&m_pFrontFireWave, sizeof(CWave*) * 3);
	ZeroMemory(&m_pMiddleFireWave, sizeof(CWave*) * 4);
	ZeroMemory(&m_pBackFireWave, sizeof(CWave*) * 5);
	ZeroMemory(&m_pFireBallGroundWave, sizeof(CSprite*) * 30);
	ZeroMemory(&m_pFireBallGroundDustSprite, sizeof(CSprite*) * 60);
	
	for (int i = 0; i < 30; ++i)
		m_bFireBallGroundWave[i] = FALSE;
}

CFireBall::~CFireBall(void)
{
}

void CFireBall::Update_Object(const _float & fTimeDelta)
{
	if (g_bBardielDead)
		return;

	if (m_pTargetAnimator->Get_AnimationIndex() == CBardiel::BARDIEL::FIREBALL)
	{
		_double dTargetTime = m_pTargetMesh->Get_TrackPos();

		if (dTargetTime >= 2.863666f && (!m_bFireBallParticle))
		{
			m_bFireBallParticle = TRUE;
			FireBallParticleStart();
			FireBallSpriteStart();
			FireWaveSart();
			FireBallShotDustSpriteStart();
		}

		/*if (dTargetTime >= 2.87f && (!m_bFireBallShotDustSprite))
		{
			m_bFireBallShotDustSprite = FALSE;
			FireBallShotDustSpriteStart();
		}*/

		for (int i = 0; i < 30; ++i)
		{
			if (dTargetTime >= 2.863666f + (0.1f * i) && (!m_bFireBallGroundWave[i]))
			{
				m_bFireBallGroundWave[i] = TRUE;
				FireBallGroundWaveStart(i);
				FireBallGroundDustSpriteStart(i * 2);
			}
		}

		if (dTargetTime >= 2.823333f && dTargetTime < 2.9f)
		{
			Engine::SetRadialBlurPower(-0.2f);
			Engine::SetRadialBlur(TRUE);
		}

		if (dTargetTime >= 2.9f && dTargetTime < 3.0f)
		{
			Engine::SetRadialBlurPower(-0.25f);
			Engine::SetRadialBlur(FALSE);
		}
	}
	else
	{
		m_bFireBallParticle = FALSE;
		m_bFireBallSprite = FALSE;
		m_bFireBallShotDustSprite = FALSE;

		for (int i = 0; i < 30; ++i)
			m_bFireBallGroundWave[i] = FALSE;
	}

	FireBallParticleUpdate();
	FireWaveUpdate();
	FireBallGroundDustSpriteUpdate();
	FireBallShotDustSpriteUpdate();

	m_pFireBallParticle->Update_Object(fTimeDelta);
	m_pFireBallSprite->Update_Object(fTimeDelta);
	m_pFireBallShotDustSprite->Update_Object(fTimeDelta);
	
	for (int i = 0; i < 3; ++i)
		m_pFrontFireWave[i]->Update_Object(fTimeDelta);

	for (int i = 0; i < 4; ++i)
		m_pMiddleFireWave[i]->Update_Object(fTimeDelta);

	for (int i = 0; i < 5; ++i)
		m_pBackFireWave[i]->Update_Object(fTimeDelta);

	for (int i = 0; i < 30; ++i)
		m_pFireBallGroundWave[i]->Update_Object(fTimeDelta);

	for (int i = 0; i < 60; ++i)
		m_pFireBallGroundDustSprite[i]->Update_Object(fTimeDelta);
}

void CFireBall::Render_Object(void)
{
	if (g_bBardielDead)
		return;

	m_pFireBallParticle->Render_Object();
	m_pFireBallSprite->Render_Object();
	m_pFireBallShotDustSprite->Render_Object();

	for (int i = 0; i < 3; ++i)
		m_pFrontFireWave[i]->Render_Object();

	for (int i = 0; i < 4; ++i)
		m_pMiddleFireWave[i]->Render_Object();

	for (int i = 0; i < 5; ++i)
		m_pBackFireWave[i]->Render_Object();

	for (int i = 0; i < 30; ++i)
		m_pFireBallGroundWave[i]->Render_Object();

	for (int i = 0; i < 60; ++i)
		m_pFireBallGroundDustSprite[i]->Render_Object();
}

HRESULT CFireBall::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pFireBallParticle = CFireBallParticle::Create(m_pGraphicDev, L"SphereParticle2", 500);
	m_pFireBallParticle->SetAni(FALSE);
	//m_pFireBallParticle->GetParticleInfo()->m_iPath = 2;
	m_pFireBallParticle->SetRenderGroupCheck(TRUE);

	m_pFireBallSprite = CSprite::LoadCreate(m_pGraphicDev, L"BardielBall");
	m_pFireBallSprite->SetAni(FALSE);
	m_pFireBallSprite->GetTransCom()->Set_Scale(1.0f, 1.0f, 1.0f);
	m_pFireBallSprite->GetParticleInfo()->m_tSpriteInfo.m_fSpriteSpeed = 60.0f;
	m_pFireBallSprite->SetBlurCheck(TRUE);
	m_pFireBallSprite->SetBlurPass(2);
	m_pFireBallSprite->SetBlurCnt(2);
	m_pFireBallSprite->SetBlurPower(2.0f);
	m_pFireBallSprite->SetBlurValue(128.0f);
	m_pFireBallSprite->SetBlurType(Engine::BLUR_CROSS);

	m_pFireBallShotDustSprite = CSprite::LoadCreate(m_pGraphicDev, L"BardielShotDust");
	m_pFireBallShotDustSprite->SetAni(FALSE);
	m_pFireBallShotDustSprite->GetTransCom()->Set_Scale(1.0f, 1.0f, 1.0f);
	m_pFireBallShotDustSprite->GetParticleInfo()->m_tSpriteInfo.m_fSpriteSpeed = 15.0f;
	m_pFireBallShotDustSprite->SetBlurCheck(TRUE);
	m_pFireBallShotDustSprite->SetBlurPass(2);
	m_pFireBallShotDustSprite->SetBlurCnt(2);
	m_pFireBallShotDustSprite->SetBlurPower(2.0f);
	m_pFireBallShotDustSprite->SetBlurValue(128.0f);
	m_pFireBallShotDustSprite->SetBlurType(Engine::BLUR_CROSS);
	

	for (int i = 0; i < 3; ++i)
	{
		m_pFrontFireWave[i] = CWave::Create(m_pGraphicDev, L"FrontFireWave");
		m_pFrontFireWave[i]->SetAni(true);
		m_pFrontFireWave[i]->SetRendererCheck(true);
		m_pFrontFireWave[i]->GetParticleInfo()->m_iPath = 2;
		m_pFrontFireWave[i]->GetTransCom()->Set_Position(&_vec3(0.0f, 0.0f, 0.0f));

		if(i == 0)
			m_pFrontFireWave[i]->GetParticleInfo()->m_iTextureNumber = 65;
		else
			m_pFrontFireWave[i]->GetParticleInfo()->m_iTextureNumber = 56;

		_vec3 vAngle = _vec3(0.f, 0.f, rand() % 360 + 30.f);
		m_pFrontFireWave[i]->GetTransCom()->Set_Angle(&vAngle);

		m_pFrontFireWave[i]->SetBlurCheck(true);
		m_pFrontFireWave[i]->SetBlurPass(2);
		m_pFrontFireWave[i]->SetBlurCnt(2);
		m_pFrontFireWave[i]->SetBlurPower(1.3f);
		m_pFrontFireWave[i]->SetBlurValue(100.0f);
		m_pFrontFireWave[i]->SetBlurType(Engine::BLUR_CROSS);
	}

	for (int i = 0; i < 4; ++i)
	{
		m_pMiddleFireWave[i] = CWave::Create(m_pGraphicDev, L"MiddleFireWave");
		m_pMiddleFireWave[i]->SetAni(true);
		m_pMiddleFireWave[i]->SetRendererCheck(true);
		m_pMiddleFireWave[i]->GetParticleInfo()->m_iPath = 2;
		m_pMiddleFireWave[i]->GetTransCom()->Set_Position(&_vec3(0.0f, 0.0f, 0.0f));

		if (i == 0 || i == 1)
			m_pMiddleFireWave[i]->GetParticleInfo()->m_iTextureNumber = 62;
		else
			m_pMiddleFireWave[i]->GetParticleInfo()->m_iTextureNumber = 51;

		_vec3 vAngle = _vec3(0.f, 0.f, rand() % 360 + 30.f);
		m_pMiddleFireWave[i]->GetTransCom()->Set_Angle(&vAngle);

		m_pMiddleFireWave[i]->SetBlurCheck(true);
		m_pMiddleFireWave[i]->SetBlurPass(2);
		m_pMiddleFireWave[i]->SetBlurCnt(2);
		m_pMiddleFireWave[i]->SetBlurPower(1.3f);
		m_pMiddleFireWave[i]->SetBlurValue(100.0f);
		m_pMiddleFireWave[i]->SetBlurType(Engine::BLUR_CROSS);
	}

	for (int i = 0; i < 5; ++i)
	{
		m_pBackFireWave[i] = CWave::Create(m_pGraphicDev, L"BackFireWave");
		m_pBackFireWave[i]->SetAni(true);
		m_pBackFireWave[i]->SetRendererCheck(true);
		m_pBackFireWave[i]->GetParticleInfo()->m_iPath = 2;
		m_pBackFireWave[i]->GetTransCom()->Set_Position(&_vec3(0.0f, 0.0f, 0.0f));

		if (i == 0 || i == 1)
			m_pBackFireWave[i]->GetParticleInfo()->m_iTextureNumber = 54;
		else if(i == 2 || i ==3)
			m_pBackFireWave[i]->GetParticleInfo()->m_iTextureNumber = 53;
		else
			m_pBackFireWave[i]->GetParticleInfo()->m_iTextureNumber = 57;

		_vec3 vAngle = _vec3(0.f, 0.f, rand() % 360 + 30.f);
		m_pBackFireWave[i]->GetTransCom()->Set_Angle(&vAngle);

		m_pBackFireWave[i]->SetBlurCheck(TRUE);
		m_pBackFireWave[i]->SetBlurPass(2);
		m_pBackFireWave[i]->SetBlurCnt(2);
		m_pBackFireWave[i]->SetBlurPower(1.3f);
		m_pBackFireWave[i]->SetBlurValue(100.0f);
		m_pBackFireWave[i]->SetBlurType(Engine::BLUR_CROSS);
	}

	for (int i = 0; i < 30; ++i)
	{
		m_pFireBallGroundWave[i] = CSprite::LoadCreate(m_pGraphicDev, L"FireBallGround");
		m_pFireBallGroundWave[i]->SetAni(FALSE);
		m_pFireBallGroundWave[i]->SetBillBoardCheck(TRUE);
		m_pFireBallGroundWave[i]->GetTransCom()->Set_Scale(1.1f, 1.1f, 1.1f);
		m_pFireBallGroundWave[i]->GetParticleInfo()->m_tSpriteInfo.m_fSpriteSpeed = 20.0f;
		_vec4 vColor = _vec4(1.0f, 0.2f, 0.0f, 1.0f);
		m_pFireBallGroundWave[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
		m_pFireBallGroundWave[i]->GetParticleInfo()->m_iPath = 2;
		m_pFireBallGroundWave[i]->SetRenderCheck(TRUE);

		m_pFireBallGroundWave[i]->SetBlurCheck(TRUE);
		m_pFireBallGroundWave[i]->SetBlurPass(2);
		m_pFireBallGroundWave[i]->SetBlurCnt(2);
		m_pFireBallGroundWave[i]->SetBlurPower(1.3f);
		m_pFireBallGroundWave[i]->SetBlurValue(100.0f);
		m_pFireBallGroundWave[i]->SetBlurType(Engine::BLUR_CROSS);
	}

	for (int i = 0; i < 60; ++i)
	{
		m_pFireBallGroundDustSprite[i] = CSprite::LoadCreate(m_pGraphicDev, L"HanumanHQDust");
		m_pFireBallGroundDustSprite[i]->SetAni(FALSE);
		m_pFireBallGroundDustSprite[i]->SetBillBoardCheck(TRUE);
		m_pFireBallGroundDustSprite[i]->GetTransCom()->Set_Scale(8.0f, 8.0f, 8.0f);
		m_pFireBallGroundDustSprite[i]->GetParticleInfo()->m_tSpriteInfo.m_fSpriteSpeed = 50.0f;
		_vec4 vColor = _vec4(0.3f, 0.3f, 0.3f, 1.0f);
		m_pFireBallGroundDustSprite[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
		m_pFireBallGroundDustSprite[i]->GetParticleInfo()->m_iPath = 2;
		m_pFireBallGroundDustSprite[i]->SetRenderCheck(TRUE);

		m_pFireBallGroundDustSprite[i]->SetBlurCheck(TRUE);
		m_pFireBallGroundDustSprite[i]->SetBlurPass(2);
		m_pFireBallGroundDustSprite[i]->SetBlurCnt(6);
		m_pFireBallGroundDustSprite[i]->SetBlurPower(1.4f);
		m_pFireBallGroundDustSprite[i]->SetBlurValue(80.0f);
		m_pFireBallGroundDustSprite[i]->SetBlurType(Engine::BLUR_HORIZON);
	}
	
	
	return S_OK;
}

CFireBall * CFireBall::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CFireBall*		pInstance = new CFireBall(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("FireBall Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CFireBall::Free(void)
{
	::Safe_Release(m_pFireBallParticle);
	::Safe_Release(m_pFireBallSprite);
	::Safe_Release(m_pFireBallShotDustSprite);

	for (int i = 0; i < 3; ++i)
		::Safe_Release(m_pFrontFireWave[i]);

	for (int i = 0; i < 4; ++i)
		::Safe_Release(m_pMiddleFireWave[i]);

	for (int i = 0; i < 5; ++i)
		::Safe_Release(m_pBackFireWave[i]);

	for (int i = 0; i < 30; ++i)
		::Safe_Release(m_pFireBallGroundWave[i]);

	for (int i = 0; i < 60; ++i)
		::Safe_Release(m_pFireBallGroundDustSprite[i]);

	CEffect::Free();
}

HRESULT CFireBall::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CFireBall::SkillStart(void)
{	
}

void CFireBall::EndAniCheck(void)
{
}

HRESULT CFireBall::Start_ForScene(void)
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

	m_pRightFootMat = m_pTargetMesh->Get_FrameByName("_55");
	m_pLeftHandMat = m_pTargetMesh->Get_FrameByName("_36");
	const _matrix* pEndpMat = m_pTargetMesh->Get_FrameByName("_52");
	
	::Safe_Release(pGameObject);

	return S_OK;
}

void CFireBall::FireBallParticleStart(void)
{
	m_pFireBallParticle->SetAni(TRUE);

	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vPos, &vPos, m_pLeftHandMat);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	m_pFireBallParticle->GetTransCom()->Set_Position(&vPos);
}

void CFireBall::FireBallParticleUpdate(void)
{
	if (m_pFireBallParticle->GetAni())
	{
		_vec3 vPos = m_pFireBallParticle->GetTransCom()->Get_Now_Position();
		_vec3 vBardielPos = m_pBardiel->GetTransCom()->Get_Now_Position();

		float fDist = D3DXVec3Length(&(vPos - vBardielPos));

		if (fDist > 150.0f)
		{
			m_pFireBallParticle->SetAni(FALSE);
			m_pFireBallSprite->SetAni(FALSE);
		}
		else
		{
			vPos += (m_pBardiel->GetTransCom()->Get_Now_Look() * 2.0f);
			m_pFireBallParticle->GetTransCom()->Set_Position(&vPos);
			m_pFireBallSprite->GetTransCom()->Set_Position(&vPos);
			
			_vec3 vRot = m_pFireBallParticle->GetTransCom()->Get_Now_Angle();
			vRot.y += 5.0f;
			m_pFireBallParticle->GetTransCom()->Set_Angle(&vRot);
		}		
	}
}

void CFireBall::FireBallSpriteStart(void)
{
	m_pFireBallSprite->SetAni(TRUE);
	_vec4 Color = _vec4(2.0f, 0.0f, 0.0f, 1.f);
	m_pFireBallSprite->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
}

void CFireBall::FireWaveSart(void)
{
	_vec4 Color = _vec4(0.0f, 0.0f, 0.0f, 1.f);

	_vec3 vWaveLook = _vec3(0.0f, 0.0f, 1.0f);
	_float Dot = D3DXVec3Dot(&vWaveLook, &m_pBardiel->GetTransCom()->Get_Now_Look());
	_float Radian = (_float)acos(Dot);
	Radian = D3DXToDegree(Radian);

	if (D3DXVec3Dot(&_vec3(-1.0f, 0.0f, 0.0f), &m_pBardiel->GetTransCom()->Get_Now_Look()) > 0.0f)
		Radian *= -1.0f;

	_vec3 Angle = _vec3(0.0f, Radian, 0.0f);

	for (int i = 0; i < 3; ++i)
	{
		m_pFrontFireWave[i]->SetAni(TRUE);
		
		_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);
		D3DXVec3TransformCoord(&vPos, &vPos, m_pLeftHandMat);
		D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

		m_pFrontFireWave[i]->GetTransCom()->Set_Position(&vPos);
		m_pFrontFireWave[i]->GetTransCom()->Set_Angle(&Angle);
		m_pFrontFireWave[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	}

	for (int i = 0; i < 4; ++i)
	{
		m_pMiddleFireWave[i]->SetAni(TRUE);

		_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

		D3DXVec3TransformCoord(&vPos, &vPos, m_pLeftHandMat);
		D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

		m_pMiddleFireWave[i]->GetTransCom()->Set_Position(&vPos);
		m_pMiddleFireWave[i]->GetTransCom()->Set_Angle(&Angle);
		m_pMiddleFireWave[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	}

	for (int i = 0; i < 5; ++i)
	{
		m_pBackFireWave[i]->SetAni(TRUE);

		_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

		D3DXVec3TransformCoord(&vPos, &vPos, m_pLeftHandMat);
		D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

		m_pBackFireWave[i]->GetTransCom()->Set_Position(&vPos);
		m_pBackFireWave[i]->GetTransCom()->Set_Angle(&Angle);
		m_pBackFireWave[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	}
}

void CFireBall::FireWaveUpdate(void)
{
	_vec3 vScale;

	for (int i = 0; i < 3; ++i)
	{
		if (m_pFrontFireWave[i]->GetAni())
		{
			_vec4 vColor = (_vec4)m_pFrontFireWave[i]->GetParticleInfo()->m_dColor;
			if (vColor.w < 0.f)
			{
				m_pFrontFireWave[i]->SetAni(false);
			}
			else
			{
				vColor.w -= 0.005f;
				m_pFrontFireWave[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
				
				_vec3 vPos = m_pFrontFireWave[i]->GetTransCom()->Get_Now_Position();
				_vec3 vDir = m_pBardiel->GetTransCom()->Get_Now_Look();

				vPos += (vDir + _vec3(i * 1.5f, i * 1.5f, i * 1.5f));

				//m_pFrontFireWave[i]->GetTransCom()->Rotation(Engine::ROT_Z, 10.0f);

				m_pFrontFireWave[i]->GetTransCom()->Get_Scale(&vScale);
				vScale *= 1.05f;
				m_pFrontFireWave[i]->GetTransCom()->Set_Scale(vScale.x, vScale.y, vScale.z);
			}
		}
	}

	for (int i = 0; i < 4; ++i)
	{
		if (m_pMiddleFireWave[i]->GetAni())
		{
			_vec4 vColor = (_vec4)m_pMiddleFireWave[i]->GetParticleInfo()->m_dColor;
			if (vColor.w < 0.f)
			{
				m_pMiddleFireWave[i]->SetAni(false);
			}
			else
			{
				vColor.w -= 0.005f;
				m_pMiddleFireWave[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;

				_vec3 vPos = m_pMiddleFireWave[i]->GetTransCom()->Get_Now_Position();
				_vec3 vDir = m_pBardiel->GetTransCom()->Get_Now_Look();

				vPos += (vDir + _vec3(i * 1.2f, i * 1.2f, i * 1.2f));

				//m_pMiddleFireWave[i]->GetTransCom()->Rotation(Engine::ROT_Z, 6.0f);

				m_pMiddleFireWave[i]->GetTransCom()->Get_Scale(&vScale);
				vScale *= 1.2f;
				m_pMiddleFireWave[i]->GetTransCom()->Set_Scale(vScale.x, vScale.y, vScale.z);
			}
		}
	}

	for (int i = 0; i < 5; ++i)
	{
		if (m_pBackFireWave[i]->GetAni())
		{
			_vec4 vColor = (_vec4)m_pBackFireWave[i]->GetParticleInfo()->m_dColor;
			if (vColor.w < 0.f)
			{
				m_pBackFireWave[i]->SetAni(false);
			}
			else
			{
				vColor.w -= 0.005f;
				m_pBackFireWave[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;

				_vec3 vPos = m_pBackFireWave[i]->GetTransCom()->Get_Now_Position();
				_vec3 vDir = m_pBardiel->GetTransCom()->Get_Now_Look();

				vPos += (vDir + _vec3(i * 0.9f, i * 0.9f, i * 0.9f));

				//m_pBackFireWave[i]->GetTransCom()->Rotation(Engine::ROT_Z, 3.0f);

				m_pBackFireWave[i]->GetTransCom()->Get_Scale(&vScale);
				vScale *= 1.4f;
				m_pBackFireWave[i]->GetTransCom()->Set_Scale(vScale.x, vScale.y, vScale.z);
			}
		}
	}
}

void CFireBall::FireBallGroundWaveStart(int iIndex)
{
	m_pFireBallGroundWave[iIndex]->SetAni(TRUE);

	_vec4 vColor = _vec4(1.0f, 0.2f, 0.0f, 1.0f);
	m_pFireBallGroundWave[iIndex]->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;

	_vec3 vPos = m_pFireBallSprite->GetTransCom()->Get_Now_Position();
	vPos.y -= 10.0f;
	m_pFireBallGroundWave[iIndex]->GetTransCom()->Set_Position(&vPos);
}

void CFireBall::FireBallGroundWaveUpdate(void)
{
}

void CFireBall::FireBallGroundDustSpriteStart(int iIndex)
{
	m_pFireBallGroundDustSprite[iIndex]->SetAni(TRUE);
	m_pFireBallGroundDustSprite[iIndex + 1]->SetAni(TRUE);

	_vec4 vColor = _vec4(0.3f, 0.3f, 0.3f, 1.0f);
	m_pFireBallGroundDustSprite[iIndex]->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
	m_pFireBallGroundDustSprite[iIndex + 1]->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;


	_vec3 vPos = m_pFireBallSprite->GetTransCom()->Get_Now_Position();
	vPos.y -= 8.0f;
	
	_vec3 vLeft; 
	D3DXVec3Cross(&vLeft, &m_pBardiel->GetTransCom()->Get_Now_Look(), &_vec3(0.0f, 1.0f, 0.0f));
	vLeft *= 20.0f;
			
	m_pFireBallGroundDustSprite[iIndex]->GetTransCom()->Set_Position(&(vPos + vLeft));
	m_pFireBallGroundDustSprite[iIndex + 1]->GetTransCom()->Set_Position(&(vPos - vLeft));
}

void CFireBall::FireBallGroundDustSpriteUpdate(void)
{
	for (int i = 0; i < 60; ++i)
	{
		if (m_pFireBallGroundDustSprite[i]->GetAni())
		{
			_vec3 vPos = m_pFireBallGroundDustSprite[i]->GetTransCom()->Get_Now_Position();

			_vec3 vLeft;
			D3DXVec3Cross(&vLeft, &m_pBardiel->GetTransCom()->Get_Now_Look(), &_vec3(0.0f, 1.0f, 0.0f));
			vLeft *= 1.005f;

			if ((i % 2) == 0)
				m_pFireBallGroundDustSprite[i]->GetTransCom()->Set_Position(&(vPos + vLeft));
			else
				m_pFireBallGroundDustSprite[i]->GetTransCom()->Set_Position(&(vPos - vLeft));
		}
	}
}

void CFireBall::FireBallShotDustSpriteStart(void)
{
	_vec3 vWaveLook = _vec3(0.0f, 0.0f, 1.0f);
	_float Dot = D3DXVec3Dot(&vWaveLook, &m_pBardiel->GetTransCom()->Get_Now_Look());
	_float Radian = (_float)acos(Dot);
	Radian = D3DXToDegree(Radian);

	if (D3DXVec3Dot(&_vec3(-1.0f, 0.0f, 0.0f), &m_pBardiel->GetTransCom()->Get_Now_Look()) > 0.0f)
		Radian *= -1.0f;

	_vec3 Angle = _vec3(0.0f, Radian, 0.0f);

	m_pFireBallShotDustSprite->SetAni(TRUE);
	_vec4 Color = _vec4(0.2f, 0.2f, 0.2f, 1.f);
	m_pFireBallShotDustSprite->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;

	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pLeftHandMat);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	m_pFireBallShotDustSprite->GetTransCom()->Set_Position(&vPos);
	m_pFireBallShotDustSprite->GetTransCom()->Set_Angle(&Angle);
}

void CFireBall::FireBallShotDustSpriteUpdate(void)
{
	if (m_pFireBallShotDustSprite->GetAni())
	{
		_vec3 vPos = m_pFireBallShotDustSprite->GetTransCom()->Get_Now_Position();
		_vec3 vDir = m_pBardiel->GetTransCom()->Get_Now_Look();

		vPos += (vDir * 1.2f);

		_vec3 vScale;
		m_pFireBallShotDustSprite->GetTransCom()->Get_Scale(&vScale);
		vScale *= 1.01f;
		m_pFireBallShotDustSprite->GetTransCom()->Set_Scale(vScale.x, vScale.y, vScale.z);
	}
}
