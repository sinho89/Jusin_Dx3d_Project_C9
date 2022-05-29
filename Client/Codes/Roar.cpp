#include "stdafx.h"
#include "Roar.h"
#include "PlayerCamera.h"
#include "Wave.h"
#include "Reflect.h"
#include "Hanuman.h"

CRoar::CRoar(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
	:CEffect(pGraphicDev, wstrName)
	, m_pvecAniInfo(NULL)
	, m_pTargetMesh(NULL)
	, m_pTargetTransform(NULL)
	, m_pTargetAnimator(NULL)
	, m_pWorldMat(NULL)
	, m_bGroundWave(FALSE)
	, m_bRadialBlurCheck(FALSE)
	, m_bRadialBlur_0(FALSE)
	, m_bRadialBlur_1(FALSE)
	, m_pHanuman(NULL)
	, m_fRadialBlurTime(0.0f)
	, m_fRadialBlurPower(-0.1f)
{
	ZeroMemory(&m_pGroundWave, sizeof(CWave*));
	ZeroMemory(&m_pReflect, sizeof(CReflect*) * 8);
	ZeroMemory(&m_pRoarWave, sizeof(CWave*) * 8);

	for (int i = 0; i < 8; ++i)
		m_bReflect[i] = FALSE;
}

CRoar::~CRoar(void)
{
}

void CRoar::Update_Object(const _float & fTimeDelta)
{
	if (g_bHanumanDead)
		return;

	if (m_pTargetAnimator->Get_AnimationIndex() == CHanuman::HANUMAN::ROAR)
	{
		_double dTargetTime = m_pTargetMesh->Get_TrackPos();
		
		if (dTargetTime >= 0.733f && (!m_bReflect[0]))
		{			
			m_bReflect[0] = TRUE;
			ReflectStart(0);
			//RoarWaveStart(0);

			m_bRadialBlurCheck = TRUE;
			m_bRadialBlur_0 = TRUE;
			m_bRadialBlur_1 = FALSE;
		}

		if (dTargetTime >= 0.983f && (!m_bReflect[1]))
		{
			m_bReflect[1] = TRUE;
			ReflectStart(1);
			//RoarWaveStart(1);

			m_bRadialBlur_0 = FALSE;
			m_bRadialBlur_1 = TRUE;
		}

		if (dTargetTime >= 1.233f && (!m_bReflect[2]))
		{
			m_bReflect[2] = TRUE;
			ReflectStart(2);
			//RoarWaveStart(2);

			m_bRadialBlur_0 = TRUE;
			m_bRadialBlur_1 = FALSE;
		}

		if (dTargetTime >= 1.483f && (!m_bReflect[3]))
		{
			m_bReflect[3] = TRUE;
			ReflectStart(3);
			//RoarWaveStart(3);

			m_bRadialBlur_0 = FALSE;
			m_bRadialBlur_1 = TRUE;
		}

		if (dTargetTime >= 1.733f && (!m_bReflect[4]))
		{
			m_bReflect[4] = TRUE;
			ReflectStart(4);
			//RoarWaveStart(4);

			m_bRadialBlur_0 = TRUE;
			m_bRadialBlur_1 = FALSE;
		}

		if (dTargetTime >= 1.983f && (!m_bReflect[5]))
		{
			m_bReflect[5] = TRUE;
			ReflectStart(5);
			//RoarWaveStart(5);

			m_bRadialBlur_0 = FALSE;
			m_bRadialBlur_1 = TRUE;			
		}

		if (dTargetTime >= 2.233f && (!m_bReflect[6]))
		{
			m_bReflect[6] = TRUE;
			ReflectStart(6);
			//RoarWaveStart(6);

			GroundWaveStart();

			m_bRadialBlur_0 = TRUE;
			m_bRadialBlur_1 = FALSE;
		}

		if (dTargetTime >= 2.483f && (!m_bReflect[7]))
		{
			m_bReflect[7] = TRUE;

			ReflectStart(7);
			//RoarWaveStart(7);

			m_bRadialBlur_0 = FALSE;
			m_bRadialBlur_1 = TRUE;			
		}

		if (dTargetTime >= 2.5f)
		{
			m_bRadialBlurCheck = FALSE;
			Engine::SetRadialBlur(FALSE);
		}		
	}
	else
	{
		for (int i = 0; i < 8; ++i)
			m_bReflect[i] = FALSE;

		m_bRadialBlurCheck = FALSE;
		m_bRadialBlur_0 = FALSE;
		m_bRadialBlur_1 = FALSE;

		m_bGroundWave = FALSE;

		m_fRadialBlurPower = -0.1f;
	}

	if (m_bRadialBlurCheck)
	{
		Engine::SetRadialBlur(TRUE);
		Engine::SetRadialBlurPower(m_fRadialBlurPower);
		if (m_bRadialBlur_0)
		{			
			m_fRadialBlurPower -= 0.02f;
		}

		if (m_bRadialBlur_1)
		{
			m_fRadialBlurPower += 0.02f;
			if (m_fRadialBlurPower > 0.0f)
				m_fRadialBlurPower = 0.0f;
		}
	}
	/*else
	{
		Engine::SetRadialBlur(FALSE);
	}*/

	ReflectUpdate();
	//RoarWaveUpdate();
	GroundWaveUpdate();

	for (int i = 0; i < 8; ++i)
		m_pReflect[i]->Update_Object(fTimeDelta);

	/*for (int i = 0; i < 8; ++i)
		m_pRoarWave[i]->Update_Object(fTimeDelta);*/

	m_pGroundWave->Update_Object(fTimeDelta);
}

void CRoar::Render_Object(void)
{
	if (g_bHanumanDead)
		return;

	for (int i = 0; i < 8; ++i)
		m_pReflect[i]->Render_Object();

	/*for (int i = 0; i < 8; ++i)
		m_pRoarWave[i]->Render_Object();*/

	m_pGroundWave->Render_Object();
}

HRESULT CRoar::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	for (int i = 0; i < 8; ++i)
	{
		m_pReflect[i] = CReflect::Create(m_pGraphicDev, L"RoarReflect");
		m_pReflect[i]->SetAni(FALSE);
		m_pReflect[i]->SetTextureNumber(0);
		m_pReflect[i]->SetNormalTextureNumber(0);
		m_pReflect[i]->SetReflectIndex(0.1f);
		m_pReflect[i]->SetShaderPath(7);
		//m_pReflect[i]->GetTransCom()->Set_Scale(15.0f, 15.0f, 15.0f);
		D3DXCOLOR vColor = _vec4(0.04908f * 0.75f, 0.1f * 0.75f, 0.08313f * 0.75f, 1.0f);
		m_pReflect[i]->GetParticleInfo()->m_dColor = vColor;
		m_pReflect[i]->GetParticleInfo()->m_bBillBoard = TRUE;
	}

	/*for (int i = 0; i < 8; ++i)
	{
		m_pRoarWave[i] = CWave::Create(m_pGraphicDev, L"RoarWave");
		m_pRoarWave[i]->SetAni(FALSE);
		m_pRoarWave[i]->SetPattern(CWave::PATTERN_END);
		m_pRoarWave[i]->SetBilBoardCheck(FALSE);
		m_pRoarWave[i]->SetRendererCheck(TRUE);
		m_pRoarWave[i]->GetParticleInfo()->m_iPath = 2;
		m_pRoarWave[i]->GetParticleInfo()->m_iTextureNumber = 4;
		D3DXCOLOR vColor = _vec4(0.04908f * 0.75f, 0.1f * 0.75f, 0.08313f * 0.75f, 1.0f);
		m_pRoarWave[i]->GetParticleInfo()->m_dColor = vColor;
		m_pRoarWave[i]->GetParticleInfo()->m_bBillBoard = TRUE;
	}*/

	m_pGroundWave = CWave::Create(m_pGraphicDev, L"RoarGroundWave");
	m_pGroundWave->SetAni(FALSE);
	m_pGroundWave->SetPattern(CWave::PATTERN_END);
	m_pGroundWave->SetBilBoardCheck(FALSE);
	m_pGroundWave->SetRendererCheck(TRUE);
	m_pGroundWave->GetParticleInfo()->m_iPath = 2;
	m_pGroundWave->GetParticleInfo()->m_iTextureNumber = 36;
	_vec3 vAngle = _vec3(90.f, 0.f, 0.f);
	m_pGroundWave->GetTransCom()->Set_Angle(&vAngle);
	D3DXCOLOR vColor = _vec4(0.4908f, 1.0f, 0.8313f, 1.0f);
	m_pGroundWave->GetParticleInfo()->m_dColor = vColor;
	m_pGroundWave->SetBlurCheck(TRUE);
	m_pGroundWave->SetBlurPass(2);
	m_pGroundWave->SetBlurCnt(2);
	m_pGroundWave->SetBlurPower(1.3f);
	m_pGroundWave->SetBlurValue(100.0f);
	m_pGroundWave->SetBlurType(Engine::BLUR_X);

	return S_OK;
}

CRoar * CRoar::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CRoar*		pInstance = new CRoar(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("Roar Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CRoar::Free(void)
{
	::Safe_Release(m_pHanuman);
	::Safe_Release(m_pGroundWave);

	for (int i = 0; i < 8; ++i)
		::Safe_Release(m_pReflect[i]);

	CEffect::Free();
}

HRESULT CRoar::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CRoar::SkillStart(void)
{
}

void CRoar::EndAniCheck(void)
{
}

HRESULT CRoar::Start_ForScene(void)
{
	if (m_bOnceStart)
		return S_OK;

	m_bOnceStart = true;

	m_pHanuman = Engine::Find(L"Mesh_Hanuman");
	if (m_pHanuman == NULL)
		return E_FAIL;

	m_pTargetAnimator = ((CHanuman*)m_pHanuman)->GetAnimatorInfo();

	//Target Mesh
	m_pTargetMesh = (Engine::CDynamicMesh*)m_pHanuman->GetComponent<Engine::CDynamicMesh>(Engine::COMID_STATIC);

	//Hanuman TransCom
	m_pTargetTransform = (Engine::CTransform*)m_pHanuman->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

	const _matrix* pMatWorld = ((CHanuman*)m_pHanuman)->GetTransCom()->Get_WorldMatrixPointer();

	//Engine::Safe_Release(pGameObject);
	return S_OK;
}

void CRoar::ReflectStart(int iNum)
{
	_vec3 vPos = m_pTargetTransform->Get_Now_Position();
	vPos.y += 1.0f;

	_vec3 vScale = _vec3(15.0f, 15.0f, 15.0f);

	switch (iNum)
	{
	case 0:
		m_pReflect[0]->SetAni(TRUE);
		m_pReflect[0]->GetTransCom()->Set_Position(&vPos);
		m_pReflect[0]->GetTransCom()->Set_Scale(vScale.x, vScale.y, vScale.z);
		m_pReflect[0]->SetReflectIndex(0.1f);
		break;

	case 1:
		m_pReflect[1]->SetAni(TRUE);
		m_pReflect[1]->GetTransCom()->Set_Position(&vPos);
		m_pReflect[1]->GetTransCom()->Set_Scale(vScale.x, vScale.y, vScale.z);
		m_pReflect[1]->SetReflectIndex(0.1f);
		break;

	case 2:
		m_pReflect[2]->SetAni(TRUE);
		m_pReflect[2]->GetTransCom()->Set_Position(&vPos);
		m_pReflect[2]->GetTransCom()->Set_Scale(vScale.x, vScale.y, vScale.z);
		m_pReflect[2]->SetReflectIndex(0.1f);
		break;

	case 3:
		m_pReflect[3]->SetAni(TRUE);
		m_pReflect[3]->GetTransCom()->Set_Position(&vPos);
		m_pReflect[3]->GetTransCom()->Set_Scale(vScale.x, vScale.y, vScale.z);
		m_pReflect[3]->SetReflectIndex(0.1f);
		break;

	case 4:
		m_pReflect[4]->SetAni(TRUE);
		m_pReflect[4]->GetTransCom()->Set_Position(&vPos);
		m_pReflect[4]->GetTransCom()->Set_Scale(vScale.x, vScale.y, vScale.z);
		m_pReflect[4]->SetReflectIndex(0.1f);
		break;

	case 5:
		m_pReflect[5]->SetAni(TRUE);
		m_pReflect[5]->GetTransCom()->Set_Position(&vPos);
		m_pReflect[5]->GetTransCom()->Set_Scale(vScale.x, vScale.y, vScale.z);
		m_pReflect[5]->SetReflectIndex(0.1f);
		break;

	case 6:
		m_pReflect[6]->SetAni(TRUE);
		m_pReflect[6]->GetTransCom()->Set_Position(&vPos);
		m_pReflect[6]->GetTransCom()->Set_Scale(vScale.x, vScale.y, vScale.z);
		m_pReflect[6]->SetReflectIndex(0.1f);
		break;

	case 7:
		m_pReflect[7]->SetAni(TRUE);
		m_pReflect[7]->GetTransCom()->Set_Position(&vPos);
		m_pReflect[7]->GetTransCom()->Set_Scale(vScale.x, vScale.y, vScale.z);
		m_pReflect[7]->SetReflectIndex(0.1f);
		break;

	default:
		break;
	}
}

void CRoar::ReflectUpdate(void)
{
	for (int i = 0; i < 8; ++i)
	{
		if (m_pReflect[i]->GetAni())
		{
			_vec3 vScale;
			m_pReflect[i]->GetTransCom()->Get_Scale(&vScale);

			if (vScale.x <= 0.0f)
			{
				m_pReflect[i]->SetAni(FALSE);
			}
			else
			{
				m_pReflect[i]->GetTransCom()->Add_Scale(-0.8f);
			}
		}
	}
}

void CRoar::RoarWaveStart(int iNum)
{
	_vec3 vPos = m_pTargetTransform->Get_Now_Position();
	vPos.y += 1.0f;

	_vec3 vScale = _vec3(15.0f, 15.0f, 15.0f);

	switch (iNum)
	{
	case 0:
		m_pRoarWave[0]->SetAni(TRUE);
		m_pRoarWave[0]->GetTransCom()->Set_Position(&vPos);
		m_pRoarWave[0]->GetTransCom()->Set_Scale(vScale.x, vScale.y, vScale.z);
		break;

	case 1:
		m_pRoarWave[1]->SetAni(TRUE);
		m_pRoarWave[1]->GetTransCom()->Set_Position(&vPos);
		m_pRoarWave[1]->GetTransCom()->Set_Scale(vScale.x, vScale.y, vScale.z);
		break;

	case 2:
		m_pRoarWave[2]->SetAni(TRUE);
		m_pRoarWave[2]->GetTransCom()->Set_Position(&vPos);
		m_pRoarWave[2]->GetTransCom()->Set_Scale(vScale.x, vScale.y, vScale.z);
		break;

	case 3:
		m_pRoarWave[3]->SetAni(TRUE);
		m_pRoarWave[3]->GetTransCom()->Set_Position(&vPos);
		m_pRoarWave[3]->GetTransCom()->Set_Scale(vScale.x, vScale.y, vScale.z);
		break;

	case 4:
		m_pRoarWave[4]->SetAni(TRUE);
		m_pRoarWave[4]->GetTransCom()->Set_Position(&vPos);
		m_pRoarWave[4]->GetTransCom()->Set_Scale(vScale.x, vScale.y, vScale.z);
		break;

	case 5:
		m_pRoarWave[5]->SetAni(TRUE);
		m_pRoarWave[5]->GetTransCom()->Set_Position(&vPos);
		m_pRoarWave[5]->GetTransCom()->Set_Scale(vScale.x, vScale.y, vScale.z);
		break;

	case 6:
		m_pRoarWave[6]->SetAni(TRUE);
		m_pRoarWave[6]->GetTransCom()->Set_Position(&vPos);
		m_pRoarWave[6]->GetTransCom()->Set_Scale(vScale.x, vScale.y, vScale.z);
		break;

	case 7:
		m_pRoarWave[7]->SetAni(TRUE);
		m_pRoarWave[7]->GetTransCom()->Set_Position(&vPos);
		m_pRoarWave[7]->GetTransCom()->Set_Scale(vScale.x, vScale.y, vScale.z);
		break;

	default:
		break;
	}
}

void CRoar::RoarWaveUpdate(void)
{
	for (int i = 0; i < 8; ++i)
	{
		if (m_pRoarWave[i]->GetAni())
		{
			_vec3 vScale;
			m_pRoarWave[i]->GetTransCom()->Get_Scale(&vScale);

			if (vScale.x <= 0.0f)
			{
				m_pRoarWave[i]->SetAni(FALSE);
			}
			else
			{
				m_pRoarWave[i]->GetTransCom()->Add_Scale(-0.8f);
			}
		}
	}
}

void CRoar::GroundWaveStart(void)
{
	_vec3 vPos = m_pTargetTransform->Get_Now_Position();
	vPos.y += 1.5f;

	_vec3 vScale = _vec3(0.0f, 0.0, 0.0);

	D3DXCOLOR vColor = _vec4(0.4908f, 1.0f, 0.8313f, 1.0f);

	m_pGroundWave->SetAni(TRUE);
	m_pGroundWave->GetTransCom()->Set_Position(&vPos);
	m_pGroundWave->GetTransCom()->Set_Scale(vScale.x, vScale.y, vScale.z);
	m_pGroundWave->GetParticleInfo()->m_dColor = vColor;
}

void CRoar::GroundWaveUpdate(void)
{
	if (m_pGroundWave->GetAni())
	{
		_vec4 vColor = (_vec4)m_pGroundWave->GetParticleInfo()->m_dColor;

		if (vColor.w <= 0.f)
		{
			m_pGroundWave->SetAni(FALSE);
		}
		else
		{
			vColor.w -= 0.01f;
			m_pGroundWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
			m_pGroundWave->GetTransCom()->Add_Scale(0.4f);
		}
	}
}
