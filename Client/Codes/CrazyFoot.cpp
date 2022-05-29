#include "stdafx.h"
#include "CrazyFoot.h"
#include "PlayerCamera.h"
#include "Wave.h"
#include "Sprite.h"
#include "Hanuman.h"
#include "Icicle.h"

CCrazyFoot::CCrazyFoot(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
	:CEffect(pGraphicDev, wstrName)
, m_pvecAniInfo(NULL)
, m_pTargetMesh(NULL)
, m_pTargetTransform(NULL)
, m_pTargetAnimator(NULL)
, m_pWorldMat(NULL)
, m_pLeftFootMat(NULL)
, m_pRightFootMat(NULL)
, m_pCrazyFootWave_0(NULL)
, m_pCrazyFootWave_1(NULL)
	, m_pHanuman(NULL)
	, m_bStartSetting(FALSE)
, m_bCrazyFootWave_0(FALSE)
, m_bCrazyFootWave_1(FALSE)

, m_pIcicle(NULL)
{
	m_bAni = false;
	ZeroMemory(&m_pDust, sizeof(CSprite*) * 2);
	for (int i = 0; i < 2; ++i)
		m_bDustCheck[i] = FALSE;
}

CCrazyFoot::~CCrazyFoot(void)
{
}

void CCrazyFoot::Update_Object(const _float & fTimeDelta)
{
	if (g_bHanumanDead)
		return;

	if (m_pTargetAnimator->Get_AnimationIndex() == CHanuman::HANUMAN::CRAZYFOOT)
	{
		_double dTargetTime = m_pTargetMesh->Get_TrackPos();

		if (dTargetTime >= 1.368f && (!m_bCrazyFootWave_0)) //¶¥ µó±â
		{
			m_bCrazyFootWave_0 = TRUE;
			CrazyFootWaveStart(0);
			m_pCrazyFootWave_0->SetAni(TRUE);

			CrazyFootDustStart(0);
			m_pDust[0]->SetAni(TRUE);

			//°íµå¸§ »ý¼º
			CreateIcicle();
		}
		
		if (dTargetTime >= 2.196f && (!m_bCrazyFootWave_1))
		{
			m_bCrazyFootWave_1 = TRUE;
			CrazyFootWaveStart(1);
			m_pCrazyFootWave_1->SetAni(TRUE);

			CrazyFootDustStart(1);
			m_pDust[1]->SetAni(TRUE);
		}

		if (!m_bStartSetting)
		{
			m_bStartSetting = TRUE;
			SkillStart();
		}
	}
	else
	{
		m_bStartSetting = FALSE;
		m_bCrazyFootWave_0 = FALSE;
		m_bCrazyFootWave_1 = FALSE;
	}

	CrazyFootDustUpdate();
	CrazyFootWaveUpdate();

	m_pCrazyFootWave_0->Update_Object(fTimeDelta);
	m_pCrazyFootWave_1->Update_Object(fTimeDelta);

	for (int i = 0; i < 2; ++i)
		m_pDust[i]->Update_Object(fTimeDelta);
}

void CCrazyFoot::Render_Object(void)
{
	if (g_bHanumanDead)
		return;

	m_pCrazyFootWave_0->Render_Object();
	m_pCrazyFootWave_1->Render_Object();

	for (int i = 0; i < 2; ++i)
		m_pDust[i]->Render_Object();
}

HRESULT CCrazyFoot::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pCrazyFootWave_0 = CWave::Create(m_pGraphicDev, L"CrazyFootWave_0");
	m_pCrazyFootWave_0->SetAni(false);
	m_pCrazyFootWave_0->SetPattern(CWave::PATTERN_END);
	m_pCrazyFootWave_0->SetBilBoardCheck(false);
	m_pCrazyFootWave_0->SetRendererCheck(true);
	m_pCrazyFootWave_0->GetParticleInfo()->m_iPath = 2;
	m_pCrazyFootWave_0->GetParticleInfo()->m_iTextureNumber = 0;
	m_pCrazyFootWave_0->SetBlurCheck(true);
	m_pCrazyFootWave_0->SetBlurPass(2);
	m_pCrazyFootWave_0->SetBlurCnt(4);
	m_pCrazyFootWave_0->SetBlurPower(1.3f);
	m_pCrazyFootWave_0->SetBlurValue(100.0f);
	m_pCrazyFootWave_0->SetBlurType(Engine::BLUR_X);


	m_pCrazyFootWave_1 = CWave::Create(m_pGraphicDev, L"CrazyFootWave_1");
	m_pCrazyFootWave_1->SetAni(false);
	m_pCrazyFootWave_1->SetPattern(CWave::PATTERN_END);
	m_pCrazyFootWave_1->SetBilBoardCheck(false);
	m_pCrazyFootWave_1->SetRendererCheck(true);
	m_pCrazyFootWave_1->GetParticleInfo()->m_iPath = 2;
	m_pCrazyFootWave_1->GetParticleInfo()->m_iTextureNumber = 0;
	m_pCrazyFootWave_1->SetBlurCheck(true);
	m_pCrazyFootWave_1->SetBlurPass(2);
	m_pCrazyFootWave_1->SetBlurCnt(4);
	m_pCrazyFootWave_1->SetBlurPower(1.3f);
	m_pCrazyFootWave_1->SetBlurValue(100.0f);
	m_pCrazyFootWave_1->SetBlurType(Engine::BLUR_X);

	for (int i = 0; i < 2; ++i)
	{
		m_pDust[i] = CSprite::LoadCreate(m_pGraphicDev, L"HanumanHQDust");
		m_pDust[i]->SetAni(false);
		m_pDust[i]->GetTransCom()->Set_Scale(1.f, 1.f, 1.f);
		_vec4 Color = _vec4(0.2f, 0.2f, 0.2f, 2.f);
		m_pDust[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pDust[i]->GetParticleInfo()->m_iPath = 3;
		m_pDust[i]->SetRenderCheck(true);
		m_pDust[i]->GetTransCom()->Set_Scale(4.f, 4.f, 4.f);
		m_pDust[i]->GetParticleInfo()->m_tSpriteInfo.m_fSpriteSpeed = 150.0f;

		m_pDust[i]->SetBlurCheck(true);
		m_pDust[i]->SetBlurCnt(2);
		m_pDust[i]->SetBlurPass(3);
		m_pDust[i]->SetBlurPower(1.05f);
		m_pDust[i]->SetBlurType(Engine::BLUR_CROSS);
		m_pDust[i]->SetBlurValue(80.0f);
	}

	return S_OK;
}

CCrazyFoot * CCrazyFoot::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CCrazyFoot*		pInstance = new CCrazyFoot(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CrazyFoot Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CCrazyFoot::Free(void)
{
	::Safe_Release(m_pCrazyFootWave_0);
	::Safe_Release(m_pCrazyFootWave_1);
	::Safe_Release(m_pHanuman);

	for (int i = 0; i < 2; ++i)
	{
		Engine::Safe_Release(m_pDust[i]);
	}

	CEffect::Free();
}

HRESULT CCrazyFoot::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CCrazyFoot::SkillStart(void)
{
	
}

void CCrazyFoot::EndAniCheck(void)
{

}

HRESULT CCrazyFoot::Start_ForScene(void)
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

	m_pWorldMat = ((CHanuman*)m_pHanuman)->GetTransCom()->Get_WorldMatrixPointer();
	m_pLeftFootMat = m_pTargetMesh->Get_FrameByName("_54");
	m_pRightFootMat = m_pTargetMesh->Get_FrameByName("_50");

	return S_OK;
}

void CCrazyFoot::CrazyFootWaveStart(int iNum)
{
	if (0 == iNum)
	{
		_vec4 Color = _vec4(0.4980f, 1.0f, 0.8313f, 1.5f);
		_vec3 vAngle = _vec3(90.f, 0.f, 0.f);

		_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

		D3DXVec3TransformCoord(&vPos, &vPos, m_pLeftFootMat);
		D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

		m_pCrazyFootWave_0->GetTransCom()->Set_Position(&vPos);
		m_pCrazyFootWave_0->GetTransCom()->Set_Scale(1.0f, 1.0f, 1.0f);
		m_pCrazyFootWave_0->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pCrazyFootWave_0->GetTransCom()->Set_Angle(&vAngle);
	}
	else if (1 == iNum)
	{
		_vec4 Color = _vec4(0.4980f, 1.0, 0.8313f, 1.5f);
		_vec3 vAngle = _vec3(90.f, 0.f, 0.f);

		_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

		D3DXVec3TransformCoord(&vPos, &vPos, m_pRightFootMat);
		D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

		m_pCrazyFootWave_1->GetTransCom()->Set_Position(&vPos);
		m_pCrazyFootWave_1->GetTransCom()->Set_Scale(1.0f, 1.0f, 1.0f);
		m_pCrazyFootWave_1->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pCrazyFootWave_1->GetTransCom()->Set_Angle(&vAngle);
	}
}

void CCrazyFoot::CrazyFootWaveUpdate(void)
{
	if (m_pCrazyFootWave_0->GetAni())
	{
		_vec4 vColor = (_vec4)m_pCrazyFootWave_0->GetParticleInfo()->m_dColor;

		if (vColor.w <= 0.f)
		{
			m_pCrazyFootWave_0->SetAni(FALSE);
		}
		else
		{
			vColor.w -= 0.08f;
			m_pCrazyFootWave_0->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
			m_pCrazyFootWave_0->GetTransCom()->Add_Scale(0.6f);
		}
	}

	if (m_pCrazyFootWave_1->GetAni())
	{
		_vec4 vColor = (_vec4)m_pCrazyFootWave_1->GetParticleInfo()->m_dColor;

		if (vColor.w <= 0.f)
		{
			m_pCrazyFootWave_1->SetAni(FALSE);
		}
		else
		{
			vColor.w -= 0.08f;
			m_pCrazyFootWave_1->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
			m_pCrazyFootWave_1->GetTransCom()->Add_Scale(0.6f);
		}
	}
}

void CCrazyFoot::CrazyFootDustStart(int iNum)
{
	if (0 == iNum)
	{
		_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

		D3DXVec3TransformCoord(&vPos, &vPos, m_pLeftFootMat);
		D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

		m_pDust[0]->GetTransCom()->Set_Position(&vPos);
	}
	else if (1 == iNum)
	{
		_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

		D3DXVec3TransformCoord(&vPos, &vPos, m_pRightFootMat);
		D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

		m_pDust[1]->GetTransCom()->Set_Position(&vPos);
	}
}

void CCrazyFoot::CrazyFootDustUpdate(void)
{
	for (int i = 0; i < 2; ++i)
	{
		if (m_pDust[i]->GetAni() == false)
			continue;

		_vec3 vPos = m_pDust[i]->GetTransCom()->Get_Now_Position();

		vPos.y += 0.05f;

		m_pDust[i]->GetTransCom()->Set_Position(&vPos);
	}
}

void CCrazyFoot::CreateIcicle(void)
{
	_vec3 vPos = m_pTargetTransform->Get_Now_Position();

	for (int i = 0; i < 15; ++i)
	{
		//Icle
		m_pIcicle = CIcicle::Create(m_pGraphicDev, L"Icicle");

		_vec3 vResultPos;

		_float	fPhi; // °¢µµ ¥õ
		_float  fRadius = (rand() % 30) + 10.f;

		fPhi = (_float)(rand() % 360);

		fPhi = D3DXToRadian(fPhi);


		vResultPos.x = vPos.x + (fRadius * cosf(fPhi));
		vResultPos.y = vPos.y + rand()%70 + 40.f;
		vResultPos.z = vPos.z + (fRadius * sinf(fPhi));

		_vec3 vZAngle;

		vZAngle = _vec3(-90.f, (_float)(rand() % 360), 0.0f);
		m_pIcicle->GetTransCom()->Set_Angle(&vZAngle);

		m_pIcicle->GetTransCom()->Set_Position(&vResultPos);

		_vec3 vDustPos;
		vDustPos = _vec3(vResultPos.x, vPos.y, vResultPos.z);
		m_pIcicle->SetDustStartPoint(vDustPos);

		_vec3 vLimitPos;
		vLimitPos = _vec3(vResultPos.x, vPos.y - (rand()%5 + 2), vResultPos.z);
		m_pIcicle->SetLimitPos(vLimitPos);

		Engine::AddObject(Engine::LAYER_GAMELOGIC, L"SoftEffect", m_pIcicle);
	}

}
