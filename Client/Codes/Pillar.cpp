#include "stdafx.h"
#include "Pillar.h"
#include "Export_Engine.h"
#include "Bardiel.h"
#include "Wave.h"
#include "Curtain.h"
#include "Meteo.h"

CPillar::CPillar(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
:CEffect(pGraphicDev, wstrName)
, m_pvecAniInfo(NULL)
, m_pTargetMesh(NULL)
, m_pTargetTransform(NULL)
, m_pTargetAnimator(NULL)
, m_pStartWave(NULL)
, m_bStartWave(false)
, m_bStartGroundWave(false)
, m_bEndAction(false)
, m_pCurtain(NULL)
, m_bCloseCurtain(false)
, m_dShootingTimer(0)
, m_pRockBullet(NULL)
{
	m_bAni = false;
	ZeroMemory(&m_pGroundWave, sizeof(CWave*) * 3);
}

CPillar::~CPillar(void)
{
}

void CPillar::Update_Object(const _float & fTimeDelta)
{
	if (g_bBardielDead)
		return;

	//FIREPILLAR_START(필라 시작 단계)
	//FIREPILLAR_ING_1(1)
	//FIREPILLAR_ING2->내려 꽂는 단계
	//FIREPILLAR_ING3
	//FIREPILLAR_END

	if (m_pTargetAnimator->Get_AnimationIndex() == CBardiel::BARDIEL::CHANGEPHASE2)
	{
		m_bAni = false;
	}

	//Start
	if (m_pTargetAnimator->Get_AnimationIndex() == CBardiel::BARDIEL::FIREPILLAR_ING_1)
	{
		m_bAni = true;
		_double dTargetTime = m_pTargetMesh->Get_TrackPos();

		if (dTargetTime >= 0.0f && dTargetTime < 1.3f)
		{
			Engine::SetRadialBlurPower(-0.05f);
			Engine::SetRadialBlur(TRUE);
		}

		if (dTargetTime >= 1.3f && dTargetTime < 1.35f)
		{
			Engine::SetRadialBlurPower(-0.25f);
			Engine::SetRadialBlur(FALSE);
		}

		if (!m_bStartWave)
		{
			m_bStartWave = true;
			StartWaveStart();
		}

	}
	else
	{
		m_bStartWave = false;
	}

	//바닥에 망치를 내려찍는 과정
	if (m_pTargetAnimator->Get_AnimationIndex() == CBardiel::BARDIEL::FIREPILLAR_ING2)
	{
		_double dTargetTime = m_pTargetMesh->Get_TrackPos();

		if((0.54f <= dTargetTime) && (!m_bStartGroundWave)) //바닥 웨이브 생성
		{
			m_bStartGroundWave = true;
			GroundWaveStart();
			CurtainStart();
		}

	}
	else
	{
		m_bStartGroundWave = false;
	}

	//구체 발사
	if (m_pTargetAnimator->Get_AnimationIndex() == CBardiel::BARDIEL::FIREPILLAR_ING3)
	{
		_double dTargetTime = m_pTargetMesh->Get_TrackPos();


		m_dShootingTimer += fTimeDelta;

		if (m_dShootingTimer > 0.1f) //Bullet 발사
		{
			m_dShootingTimer = 0;

			_float m_fAngle = (_float)(rand() % 360) + 10.f;

			LookAtAngle(m_fAngle);


			//Bullet 생성=========================================================
			_vec3 vBulletLook = _vec3(0.f, 0.f, 1.f);

			_float Dot = D3DXVec3Dot(&vBulletLook, &m_vBardielDir);

			_float Radian = (_float)acos(Dot);
			Radian = D3DXToDegree(Radian);

			_vec3 vBulletDir;


			D3DXVec3Cross(&vBulletDir, &vBulletLook, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));

			if (D3DXVec3Dot(&vBulletDir, &m_vBardielDir) > 0)
			{
				Radian *= -1.f;
			}

			_vec3 Angle = _vec3(0.f, Radian, 0.0f);


			//Bullet 생성
			m_pRockBullet = CMeteo::Create(m_pGraphicDev, L"PillarBullet");

			_vec3 vPos = m_pTargetTransform->Get_Now_Position();

			m_pRockBullet->SetLimitPos(vPos);

			vPos.y += 10.f;

			m_pRockBullet->GetTransCom()->Set_Position(&vPos);

			m_pRockBullet->SetShootingDir(m_vBardielDir);

			m_pRockBullet->SetAngle(Angle);

			_float fResult = rand()%5 + 0.5f;

			m_pRockBullet->SetTimeAdd(fResult);

			Engine::AddObject(Engine::LAYER_GAMELOGIC, L"SoftEffect", m_pRockBullet);
		}

	}


	//마무리 동작
	if (m_pTargetAnimator->Get_AnimationIndex() == CBardiel::BARDIEL::FIREPILLAR_END)
	{
		_double dTargetTime = m_pTargetMesh->Get_TrackPos();

		if ((0.35f <= dTargetTime) && (!m_bEndAction)) //바닥 웨이브 생성
		{
			m_bEndAction = true;

			m_bCloseCurtain = true;
		}

		if(1.9f <= dTargetTime)
		{
			m_bAni = false;
		}

	}
	else
	{
		m_bEndAction = false;
	}



	if (!m_bAni)
		return;


	StartWaveUpdate();
	GroundWaveUpdate();
	CurtainUpdate();


	m_pCurtain->Update_Object(fTimeDelta);
	m_pStartWave->Update_Object(fTimeDelta);

	for (int i = 0; i < 3; ++i)
	{
		m_pGroundWave[i]->Update_Object(fTimeDelta);
	}


}

void CPillar::Render_Object(void)
{
	if (!m_bAni)
		return;


	m_pCurtain->Render_Object();
	m_pStartWave->Render_Object();

	for (int i = 0; i < 3; ++i)
	{
		m_pGroundWave[i]->Render_Object();
	}
}

HRESULT CPillar::Ready_Object(void)
{
	//StartWave=====================================================
	_vec4 Color = _vec4(1.0f, -0.1f, -0.1f, 2.0f);
	m_pStartWave = CWave::Create(m_pGraphicDev, L"StartWave");
	m_pStartWave->SetAni(false);
	m_pStartWave->SetPattern(CWave::PATTERN_END);
	m_pStartWave->SetRendererCheck(true);
	m_pStartWave->GetTransCom()->Set_Scale(0.f, 0.f, 0.f);
	m_pStartWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pStartWave->GetParticleInfo()->m_iPath = 2;
	m_pStartWave->GetParticleInfo()->m_iTextureNumber = 68;
	//===========================================================


	for (int i = 0; i < 3; ++i)
	{
		m_pGroundWave[i] = CWave::Create(m_pGraphicDev, L"GroundWave");
		m_pGroundWave[i]->SetAni(false);
		m_pGroundWave[i]->SetPattern(CWave::PATTERN_END);
		m_pGroundWave[i]->SetRendererCheck(true);
		m_pGroundWave[i]->GetTransCom()->Set_Scale(i * 4.f + 3.f, i * 4.f + 3.f, i * 4.f + 3.f);
		m_pGroundWave[i]->GetParticleInfo()->m_iPath = 2;
		m_pGroundWave[i]->GetParticleInfo()->m_iTextureNumber = 7;
	}


	//RedCurtain
	m_pCurtain = CCurtain::LoadCreate(m_pGraphicDev, L"GroundUpAura");
	m_pCurtain->SetYBillBoard(true);
	m_pCurtain->SetAni(false);
	m_pCurtain->SetEffectBlurCheck(TRUE);
	m_pCurtain->SetBlurPass(2);
	m_pCurtain->SetBlurCnt(2);
	m_pCurtain->SetBlurPower(1.2f);
	m_pCurtain->SetBlurValue(128.0f);
	m_pCurtain->SetBlurType(Engine::BLUR_HORIZON);

	return S_OK;
}

CPillar * CPillar::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CPillar*		pInstance = new CPillar(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("Pillar Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CPillar::Free(void)
{
	CEffect::Free();
	Engine::Safe_Release(m_pStartWave);

	for (int i = 0; i < 3; ++i)
	{
		Engine::Safe_Release(m_pGroundWave[i]);
	}
	Engine::Safe_Release(m_pCurtain);
}

HRESULT CPillar::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CPillar::SkillStart(void)
{
}

void CPillar::EndAniCheck(void)
{
}

HRESULT CPillar::Start_ForScene(void)
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

	Engine::Safe_Release(pGameObject);
	return S_OK;
}

void CPillar::StartWaveStart(void)
{
	_vec3 vPos = m_pTargetTransform->Get_Now_Position();
	vPos.y += 0.4f;

	m_pStartWave->SetAni(true);
	m_pStartWave->GetTransCom()->Set_Position(&vPos);
	_vec3 vAngle = _vec3(90.f, 0.f, 0.f);
	m_pStartWave->GetTransCom()->Set_Angle(&vAngle);
	m_pStartWave->GetTransCom()->Set_Scale(0.0f, 0.0f, 0.0f);
	_vec4 vColor = _vec4(1.0f, -0.1f, -0.1f, 2.0f);
	m_pStartWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;

}

void CPillar::StartWaveUpdate(void)
{
	if (!m_pStartWave->GetAni())
		return;

	_vec3 vScale;

	m_pStartWave->GetTransCom()->Get_Scale(&vScale);

	if (!m_bEndAction)
	{
		if (vScale.x < 5.f)
		{
			m_pStartWave->GetTransCom()->Add_Scale(0.2f);
		}
	}
	else
	{
		if (vScale.x <= 0.f)
		{
			m_pStartWave->SetAni(false);
		}
		else
		{
			m_pStartWave->GetTransCom()->Add_Scale(-0.2f);
		}
	}
	
	m_pStartWave->GetTransCom()->Rotation(Engine::ROT_Y, 10.f);
}

void CPillar::GroundWaveStart(void)
{
	_vec3 vPos = m_pTargetTransform->Get_Now_Position();
	vPos.y += 0.4f;
	_vec3 vAngle = _vec3(90.f, 0.f, 0.f);

	for (int i = 0; i < 3; ++i)
	{
		m_pGroundWave[i]->SetAni(true);
		m_pGroundWave[i]->GetTransCom()->Set_Position(&vPos);
		m_pGroundWave[i]->GetTransCom()->Set_Angle(&vAngle);
		m_pGroundWave[i]->GetTransCom()->Set_Scale(i * 4.f + 3.f, i * 4.f + 3.f, i * 4.f + 3.f);

		_vec4 Color = _vec4(1.f, 0.0f, 0.0f, 1.f);

		m_pGroundWave[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	}
}

void CPillar::GroundWaveUpdate(void)
{
	for (int i = 0; i < 3; ++i)
	{
		_vec3 vPos = m_pTargetTransform->Get_Now_Position();
		vPos.y += 0.5f;

		if (!m_pGroundWave[i]->GetAni())
			continue;

		_vec4 vColor = (_vec4)m_pGroundWave[i]->GetParticleInfo()->m_dColor;

		if (vColor.w <= 0.f)
		{
			if (m_bEndAction)
			{
				m_pGroundWave[i]->SetAni(false);
			}
			else
			{
				m_pGroundWave[i]->GetTransCom()->Set_Position(&vPos);
				m_pGroundWave[i]->GetTransCom()->Set_Scale(i * 4.f + 3.f, i * 4.f + 3.f, i * 4.f + 3.f);
				_vec4 Color = _vec4(1.f, 0.0f, 0.0f, 1.f);
				m_pGroundWave[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
			}

		}
		else
		{
			vColor.w -= 0.03f;
			m_pGroundWave[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
			m_pGroundWave[i]->GetTransCom()->Add_Scale(0.2f);
		}

	}
}

void CPillar::CurtainStart(void)
{
	m_pCurtain->SetAni(true);

	_vec3 vBardielPos = m_pTargetTransform->Get_Now_Position();
	m_pCurtain->GetTransCom()->Set_Position(&vBardielPos);
	m_pCurtain->GetTransCom()->Set_Scale(2.3f, 2.5f, 2.3f);

	_vec4 vColor = m_pCurtain->GetParticleInfo()->m_dColor;
	
	vColor.w = 1.f;

	m_pCurtain->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;

}

void CPillar::CurtainUpdate(void)
{
	if (!m_bCloseCurtain || (!m_pCurtain->GetAni()))
		return;


	_vec4 vColor = (_vec4)m_pCurtain->GetParticleInfo()->m_dColor;


	if (vColor.w <= 0.f)
	{
		m_pCurtain->SetAni(false);
		m_bCloseCurtain = false;
	}
	else
	{
		vColor.w -= 0.02f;
		m_pCurtain->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
	}
}

void CPillar::LookAtAngle(_float fAngle)
{
	_float	fPhi;		// 각도 φ

	fPhi = fAngle;

	fPhi = D3DXToRadian(fPhi);

	_vec3 vLocal;
	_matrix Trans;
	_matrix Result;
	_matrix BardielWorld;
	_vec3   vDir;
	_vec3	HanumanPos;
	vLocal.x = 5.f * cosf(fPhi);
	vLocal.z = 5.f * sinf(fPhi);
	vLocal.y = 0.f;

	D3DXMatrixTranslation(&Trans, vLocal.x, vLocal.y, vLocal.z);

	m_pTargetTransform->Get_WorldMatrix(&BardielWorld);

	Result = Trans * BardielWorld;

	_vec3 vWorldTargetPos;

	memcpy(&vWorldTargetPos, &Result.m[3][0], sizeof(_vec3));


	HanumanPos = m_pTargetTransform->Get_Now_Position();

	vDir = vWorldTargetPos - HanumanPos;

	D3DXVec3Normalize(&vDir, &vDir);

	m_vBardielDir = vDir;
}
