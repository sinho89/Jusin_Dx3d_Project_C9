#include "stdafx.h"
#include "BreathBullet.h"
#include "Wave.h"
#include "Sprite.h"

CBreathBullet::CBreathBullet(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
:CEffect(pGraphicDev, wstrName)
,m_pCore(NULL)
,m_pElectric(NULL)
,m_pTwist(NULL)
, m_pColliderCom(NULL)
,m_bDestroyCheck(false)
, m_dTimer(0)
{
	ZeroMemory(&m_pTwistMiddle, sizeof(CWave*) * 2);
	ZeroMemory(&m_pTwistGroup, sizeof(CWave*) * 10);
}

CBreathBullet::~CBreathBullet(void)
{
}

void CBreathBullet::Update_Object(const _float & fTimeDelta)
{
	DestroyCheck();


	//Player 충돌
	if (m_pColliderCom->IsHit())
	{
		m_eState = Engine::STATE_DESTROY;
	}


	//Twist Wave의 Update
	TwistUpdate();
	

	//Twist 관련 Update
	m_pTwist->Update_Object(fTimeDelta);
	for (int i = 0; i < 2; ++i)
	{
		m_pTwistMiddle[i]->Update_Object(fTimeDelta);
	}

	//Twist Group관련 Update
	for (int i = 0; i < 10; ++i)
	{
		m_pTwistGroup[i]->Update_Object(fTimeDelta);
	}

	//Core
	m_pCore->Update_Object(fTimeDelta);

	//Core주변에 Spark
	m_pElectric->Update_Object(fTimeDelta);

	Shooting();

	CEffect::Update_Object(fTimeDelta);

	SettingRotate();

	m_dTimer += fTimeDelta;

	if (m_dTimer >= 0.2f)
	{
		m_bDestroyCheck = true;
	}

}

void CBreathBullet::Render_Object(void)
{
	//Twist 관련 Render
	m_pTwist->Render_Object();

	for (int i = 0; i < 2; ++i)
	{
		m_pTwistMiddle[i]->Render_Object();
	}

	//Twist Group관련 Update
	for (int i = 0; i < 10; ++i)
	{
		m_pTwistGroup[i]->Render_Object();
	}

	m_pCore->Render_Object();
	m_pElectric->Render_Object();

	if (m_pColliderCom)
		m_pColliderCom->Render_Collider(g_bDebugRender);
}

HRESULT CBreathBullet::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	//Core===========================================================
	m_pCore = CWave::Create(m_pGraphicDev, L"BulletCore");
	m_pCore->SetAni(true);
	m_pCore->SetPattern(CWave::PATTERN_END);
	m_pCore->SetRendererCheck(true);
	m_pCore->SetBilBoardCheck(true);
	m_pCore->GetTransCom()->Set_Scale(0.5f, 0.5f, 0.5f);
	_vec4 vColor = _vec4(1.f, 1.f, 1.f, 1.f);
	m_pCore->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
	m_pCore->GetParticleInfo()->m_iPath = 2;
	m_pCore->GetParticleInfo()->m_iTextureNumber = 49;
	_vec3 vPos = _vec3(0.f, 2.f, 0.f);
	m_pCore->GetTransCom()->Set_Position(&vPos);

	const _matrix* matCoreWorld = NULL;
	_matrix* matCoreNotConst = NULL;
	matCoreWorld = m_pTransCom->Get_WorldMatrixPointer();
	matCoreNotConst = const_cast<_matrix*>(matCoreWorld);
	m_pCore->SetTargetWorld(matCoreNotConst);
	m_pCore->SetBillboardRotate(true);
	//==================================================================


	//==================================================================
	//코어주변에서 도는 스파크
	m_pElectric = CSprite::LoadCreate(m_pGraphicDev, L"Electric");
	m_pElectric->SetAni(true);
	m_pElectric->SetRenderCheck(true);
	m_pElectric->GetTransCom()->Set_Scale(0.5f, 0.5f, 0.5f);
	vColor = _vec4(1.f, 1.f, 1.f, 1.f);
	m_pElectric->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
	m_pElectric->GetParticleInfo()->m_iPath = 2;
	m_pElectric->GetTransCom()->Set_Position(&vPos);

	m_pElectric->SetBlurCheck(TRUE);
	m_pElectric->SetBlurPass(2);
	m_pElectric->SetBlurCnt(2);
	m_pElectric->SetBlurPower(1.3f);
	m_pElectric->SetBlurValue(100.0f);
	m_pElectric->SetBlurType(Engine::BLUR_CROSS);

	const _matrix* matElectricWorld = NULL;
	_matrix* matElectricNotConst = NULL;
	matElectricWorld = m_pTransCom->Get_WorldMatrixPointer();
	matElectricNotConst = const_cast<_matrix*>(matElectricWorld);
	m_pElectric->SetTargetWorld(matElectricNotConst);
	m_pElectric->SetBillboardRotate(true);
	//=================================================================


	//=================================================================
	m_pTwist = CWave::Create(m_pGraphicDev, L"Twist");
	m_pTwist->SetAni(true);
	m_pTwist->SetRendererCheck(true);
	m_pTwist->GetTransCom()->Set_Scale(0.4f, 0.4f, 0.4f);
	m_pTwist->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
	m_pTwist->GetParticleInfo()->m_iPath = 2;
	_vec3 vTwistPos = _vec3(0.f, 2.f, -0.5f);
	m_pTwist->GetTransCom()->Set_Position(&vTwistPos);
	m_pTwist->GetParticleInfo()->m_iTextureNumber = 52;

	m_pTwist->SetBlurCheck(true);
	m_pTwist->SetBlurPass(2);
	m_pTwist->SetBlurCnt(2);
	m_pTwist->SetBlurPower(1.3f);
	m_pTwist->SetBlurValue(100.0f);
	m_pTwist->SetBlurType(Engine::BLUR_CROSS);
	//=================================================================


	//================================================================
	for (int i = 0; i < 2; ++i)
	{
		m_pTwistMiddle[i] = CWave::Create(m_pGraphicDev, L"TwisthMiddle");
		m_pTwistMiddle[i]->SetAni(true);
		m_pTwistMiddle[i]->SetRendererCheck(true);
		m_pTwistMiddle[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
		m_pTwistMiddle[i]->GetTransCom()->Set_Scale(0.6f, 0.6f, 0.6f);
		m_pTwistMiddle[i]->GetParticleInfo()->m_iPath = 2;
		_vec3 vTwistMiddlePos = _vec3(0.f, 2.f, -1.0f);
		m_pTwistMiddle[i]->GetTransCom()->Set_Position(&vTwistMiddlePos);
		m_pTwistMiddle[i]->GetParticleInfo()->m_iTextureNumber = 55;

		if (i == 1)
		{
			_vec3 vAngle = _vec3(0.f, 0.f, 180.f);
			m_pTwistMiddle[i]->GetTransCom()->Set_Angle(&vAngle);
		}

		m_pTwistMiddle[i]->SetBlurCheck(true);
		m_pTwistMiddle[i]->SetBlurPass(2);
		m_pTwistMiddle[i]->SetBlurCnt(2);
		m_pTwistMiddle[i]->SetBlurPower(1.3f);
		m_pTwistMiddle[i]->SetBlurValue(100.0f);
		m_pTwistMiddle[i]->SetBlurType(Engine::BLUR_CROSS);
	}
	//=======================================================================


	for (int i = 0; i < 10; ++i)
	{
		m_pTwistGroup[i] = CWave::Create(m_pGraphicDev, L"Twist");
		m_pTwistGroup[i]->SetAni(true);
		m_pTwistGroup[i]->SetRendererCheck(true);
		_vec3 vScale = _vec3(0.7f + (i * 0.05f), 0.7f + (i * 0.05f), 0.7f + (i * 0.05f));
		m_pTwistGroup[i]->GetTransCom()->Set_Scale(vScale.x, vScale.y, vScale.z);
		m_pTwistGroup[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
		m_pTwistGroup[i]->GetParticleInfo()->m_iPath = 2;
		_vec3 vTwistPos = _vec3(0.f, 2.f, -1.2f + (-i * 0.4f));
		m_pTwistGroup[i]->GetTransCom()->Set_Position(&vTwistPos);
		m_pTwistGroup[i]->GetParticleInfo()->m_iTextureNumber = 54;

		_vec3 vAngle = _vec3(0.f, 0.f, rand()%360 + 30.f );
		m_pTwistGroup[i]->GetTransCom()->Set_Angle(&vAngle);

		m_pTwistGroup[i]->SetBlurCheck(true);
		m_pTwistGroup[i]->SetBlurPass(2);
		m_pTwistGroup[i]->SetBlurCnt(2);
		m_pTwistGroup[i]->SetBlurPower(1.3f);
		m_pTwistGroup[i]->SetBlurValue(100.0f);
		m_pTwistGroup[i]->SetBlurType(Engine::BLUR_CROSS);
	}

	m_pColliderCom->SetScale(4.f);
	m_pColliderCom->SetPos(_vec3(0.f, 3.f, 0.f));
	Engine::COLINFO tColInfo;
	tColInfo.eColInfo = Engine::COL_PUSH;
	tColInfo.fJumpForce = 0.f;
	tColInfo.fStampForce = 0.f;
	tColInfo.fForce = 10.f;
	tColInfo.vDir = m_vShootingDir;
	tColInfo.iDamage = 50;
	m_pColliderCom->Set_ColInfo(&tColInfo);
	Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Monster", m_pColliderCom);

	return S_OK;
}

CBreathBullet * CBreathBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CBreathBullet*		pInstance = new CBreathBullet(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("BreathBullet Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CBreathBullet::Free(void)
{
	//충돌체 삭제=======================================
	Engine::Delete_Collider(L"Monster", m_pColliderCom);

	CEffect::Free();
	Engine::Safe_Release(m_pCore);
	Engine::Safe_Release(m_pElectric);
	Engine::Safe_Release(m_pTwist);


	for (int i = 0; i < 2; ++i)
	{
		Engine::Safe_Release(m_pTwistMiddle[i]);
	}

	for (int i = 0; i < 10; ++i)
	{
		Engine::Safe_Release(m_pTwistGroup[i]);
	}
}

HRESULT CBreathBullet::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	//For.Collider Component
	pComponent = Engine::Create_SphereCollider(m_pGraphicDev, m_pTransCom, L"Monster", _vec3(0.f, 0.f, 0.f), 1.f);
	if (pComponent == NULL)
		return E_FAIL;

	m_pColliderCom = (Engine::CSphereCollider*)pComponent;
	m_pColliderCom->Set_Render(true);
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Collider", pComponent));

	return S_OK;
}

void CBreathBullet::SkillStart(void)
{
}

void CBreathBullet::EndAniCheck(void)
{
}

void CBreathBullet::TwistUpdate(void)
{
	//Twisth 회전률
	if (m_pTwist->GetAni())
	{
		m_pTwist->GetTransCom()->Rotation(Engine::ROT_Z, 10.f);
	}


	if (m_pTwistMiddle[0]->GetAni())
	{
		for (int i = 0; i < 2; ++i)
		{
			m_pTwistMiddle[i]->GetTransCom()->Rotation(Engine::ROT_Z, 8.f);
		}
	}

	for (int i = 0; i < 10; ++i)
	{
		m_pTwistGroup[i]->GetTransCom()->Rotation(Engine::ROT_Z, 20.f);
	}

}

void CBreathBullet::SettingRotate(void)
{
	_matrix matWorld;

	m_pTransCom->Get_WorldMatrix(&matWorld);

	m_pCore->GetTransCom()->Set_ParentMatrix(&matWorld);

	m_pElectric->GetTransCom()->Set_ParentMatrix(&matWorld);


	m_pTwist->GetTransCom()->Set_ParentMatrix(&matWorld);
	
	for (int i = 0; i < 2; ++i)
	{
		m_pTwistMiddle[i]->GetTransCom()->Set_ParentMatrix(&matWorld);
	}

	for (int i = 0; i < 10; ++i)
	{
		m_pTwistGroup[i]->GetTransCom()->Set_ParentMatrix(&matWorld);
	}

}

void CBreathBullet::SetShootingDir(_vec3 vDir)
{
	m_vShootingDir = vDir;

	m_vShootingDir *= 1.5f;
}

void CBreathBullet::Shooting(void)
{
	m_pTransCom->Move_Position(&m_vShootingDir);
}

void CBreathBullet::SetRotate(_vec3 vRotate)
{
	m_pTransCom->Set_Angle(&vRotate);
}

void CBreathBullet::DestroyCheck(void)
{
	if (!m_bDestroyCheck)
		return;

	_float fMinusAlpha = 0.01f;

	_vec4 vCoreColor = (_vec4)m_pCore->GetParticleInfo()->m_dColor;

	if (vCoreColor.w <= 0.f)
	{
		m_pCore->SetAni(false);
	}
	else
	{
		vCoreColor.w -= fMinusAlpha;
		m_pCore->GetParticleInfo()->m_dColor = (D3DXCOLOR)vCoreColor;
	}

	//========================================================================
	_vec4 vElectricColor = (_vec4)m_pElectric->GetParticleInfo()->m_dColor;

	if (vElectricColor.w <= 0.f)
	{
		m_pElectric->SetAni(false);
	}
	else
	{
		vElectricColor.w -= fMinusAlpha;
		m_pElectric->GetParticleInfo()->m_dColor = (D3DXCOLOR)vElectricColor;
	}
	//==========================================================================


		
	_vec4 vTwistColor = (_vec4)m_pTwist->GetParticleInfo()->m_dColor;


	if (m_pElectric->GetAni() == false)
	{
		if (vTwistColor.w <= 0.f)
		{
			m_pTwist->SetAni(false);
		}
		else
		{
			vTwistColor.w -= fMinusAlpha;
			m_pTwist->GetParticleInfo()->m_dColor = (D3DXCOLOR)vTwistColor;
		}
	}


	_vec4 TwistMiddle[2];
	_vec4 TwistGroup[10];

	_float fAlphaTwist;

	fAlphaTwist = 0.006f;

		for (int i = 0; i < 2; ++i)
		{
			if (m_pTwistMiddle[i]->GetAni() == false)
				continue;


			TwistMiddle[i] = (_vec4)m_pTwistMiddle[i]->GetParticleInfo()->m_dColor;

			if (TwistMiddle[i].w <= 0.f)
			{
				m_pTwistMiddle[i]->SetAni(false);
			}
			else
			{
				TwistMiddle[i].w -= fAlphaTwist;
				m_pTwistMiddle[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)TwistMiddle[i];
			}
		}


		for (int i = 0; i < 10; ++i)
		{
			if (m_pTwistGroup[i]->GetAni() == false)
				continue;

			TwistGroup[i] = (_vec4)m_pTwistGroup[i]->GetParticleInfo()->m_dColor;

			if (TwistGroup[i].w <= 0.f)
			{
				m_pTwistGroup[i]->SetAni(false);
			}
			else
			{
				TwistGroup[i].w -= fAlphaTwist;
				m_pTwistGroup[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)TwistGroup[i];
			}

		}


	if(m_pTwistGroup[9]->GetAni() == false)
	{
		m_eState = Engine::STATE_DESTROY;
	}

}
