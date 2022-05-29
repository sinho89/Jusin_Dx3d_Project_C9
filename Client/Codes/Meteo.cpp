#include "stdafx.h"
#include "Meteo.h"
#include "Export_Engine.h"
#include "Rock.h"
#include "Sprite.h"
#include "HitExplosion.h"

CMeteo::CMeteo(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
:CEffect(pGraphicDev, wstrName)
,m_pColliderCom(NULL)
, m_pRock(NULL)
, m_dLifeTime(0)
, m_fDownTime(0)
, m_pSprite(NULL)
, m_dExplosionTime(0)
, m_pExplosion(NULL)
, m_bBombCheck(false)
{
	m_bAni = true;

	ZeroMemory(&m_pMeteoDust, sizeof(CSprite*) * 10);
}

CMeteo::~CMeteo(void)
{
}

void CMeteo::Update_Object(const _float & fTimeDelta)
{
	//Player충돌
	if (m_pColliderCom->IsHit())
	{
		m_vBombPos = m_pTransCom->Get_Now_Position();

		m_vBombPos.y = m_vLimitPos.y;

		m_pExplosion = CHitExplosion::Create(m_pGraphicDev, L"Bomb");
		m_pExplosion->SetStartPos(m_vBombPos);
		m_pExplosion->SettingStartPos();

		Engine::AddObject(Engine::LAYER_GAMELOGIC, L"SoftEffect", m_pExplosion);
		
		m_eState = Engine::STATE_DESTROY;
	}


	m_dLifeTime += fTimeDelta;

	MoveParabola();

	_vec3 vPos = m_pTransCom->Get_Now_Position();

	//폭발
	if( m_vLimitPos.y > vPos.y)
	{
		if (!m_bBombCheck)
		{
			m_vBombPos = m_pTransCom->Get_Now_Position();
			m_vBombPos.y = m_vLimitPos.y;

			m_pExplosion = CHitExplosion::Create(m_pGraphicDev, L"Bomb");
			m_pExplosion->SetStartPos(m_vBombPos);
			m_pExplosion->SettingStartPos();
			Engine::AddObject(Engine::LAYER_GAMELOGIC, L"SoftEffect", m_pExplosion);

			m_bBombCheck = true;
		}	
	}




	if( ( (m_vLimitPos.y - 5.f) > vPos.y) || (m_dLifeTime >= 5.f))
	{
		m_eState = Engine::STATE_DESTROY;
	}


	m_dExplosionTime += fTimeDelta;

	if (m_dExplosionTime >= 0.1f)
	{
		m_dExplosionTime = 0;

		for (int i = 0; i < 10; ++i)
		{
			if(m_pMeteoDust[i]->GetAni())
				continue;

			SpriteDustStart(i);
			break;
		}
	}



	CEffect::Update_Object(fTimeDelta);

	m_pRock->Update_Object(fTimeDelta);

	m_pSprite->Update_Object(fTimeDelta);

	for (int i = 0; i < 10; ++i)
		m_pMeteoDust[i]->Update_Object(fTimeDelta);
}

void CMeteo::Render_Object(void)
{

	m_pRock->Render_Object();

	m_pSprite->Render_Object();

	for (int i = 0; i < 10; ++i)
		m_pMeteoDust[i]->Render_Object();

	if (m_pColliderCom)
		m_pColliderCom->Render_Collider(g_bDebugRender);
}

HRESULT CMeteo::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	//==========================================
	m_pColliderCom->SetScale(6.f);
	m_pColliderCom->SetPos(_vec3(0.f, 0.f, -2.f));
	Engine::COLINFO tColInfo;
	tColInfo.eColInfo = Engine::COL_PUSH;
	tColInfo.fJumpForce = 0.f;
	tColInfo.fStampForce = 0.f;
	tColInfo.fForce = 10.f;
	tColInfo.vDir = m_vShootingDir;
	tColInfo.iDamage = 40;
	m_pColliderCom->Set_ColInfo(&tColInfo);
	Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Monster", m_pColliderCom);
	//===========================================================

	m_pSprite = CSprite::LoadCreate(m_pGraphicDev, L"PillarMeteo");
	m_pSprite->SetAni(true);
	m_pSprite->GetTransCom()->Set_Scale(1.6f, 1.6f, 1.6f);
	m_pSprite->SetRenderCheck(true);
	_vec4 vSpriteColor = (_vec4)m_pSprite->GetParticleInfo()->m_dColor;
	vSpriteColor.w = 0.5f;
	m_pSprite->GetParticleInfo()->m_dColor = (D3DXCOLOR)vSpriteColor;


	m_pSprite->SetBlurCheck(TRUE);
	m_pSprite->SetBlurPass(2);
	m_pSprite->SetBlurCnt(2);

	m_pSprite->SetBlurPower(1.5f);  //높을수록 밝게
	m_pSprite->SetBlurValue(140.0f); //낮을수록 많이 흔들린다.
	m_pSprite->SetBlurType(Engine::BLUR_CROSS);


	m_pRock = CRock::Create(m_pGraphicDev, L"PillarRock");


	//Sprite Dust
	for (int i = 0; i < 10; ++i)
	{
		m_pMeteoDust[i] = CSprite::LoadCreate(m_pGraphicDev, L"MeteoDust");
		m_pMeteoDust[i]->SetAni(false);
		m_pMeteoDust[i]->GetTransCom()->Set_Scale(1.7f, 1.7f, 1.7f);
		m_pMeteoDust[i]->SetRenderCheck(true);
	}

	return S_OK;
}

CMeteo * CMeteo::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CMeteo* pInstance = new CMeteo(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("Meteo Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CMeteo::Free(void)
{
	//충돌체 삭제=======================================
	Engine::Delete_Collider(L"Monster", m_pColliderCom);

	CEffect::Free();

	Engine::Safe_Release(m_pRock);
	Engine::Safe_Release(m_pSprite);

	for (int i = 0; i < 10; ++i)
	{
		Engine::Safe_Release(m_pMeteoDust[i]);
	}
}

HRESULT CMeteo::Add_Component(void)
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

void CMeteo::SetShootingDir(_vec3 vDir)
{
	m_vShootingDir = vDir;
}

void CMeteo::SetLimitPos(_vec3 vLimitPos)
{
	m_vLimitPos = vLimitPos;
}

void CMeteo::MoveParabola(void)
{
	_vec3 vPos = m_pTransCom->Get_Now_Position();

	m_fSpeed = 1.2f;

	vPos.x += m_vShootingDir.x * m_fSpeed;
	vPos.z += m_vShootingDir.z * m_fSpeed;

	m_fDownTime += 0.05f;

	vPos.y += m_fTimeAdd + (-0.98f * m_fDownTime * m_fDownTime) / 2;


	m_pTransCom->Set_Position(&vPos);

	m_pRock->GetTransCom()->Set_Position(&vPos);
	m_pSprite->GetTransCom()->Set_Position(&vPos);
}

void CMeteo::SetTimeAdd(_float fTime)
{
	m_fTimeAdd = fTime;
}

void CMeteo::SetAngle(_vec3 vAngle)
{
	m_pTransCom->Set_Angle(&vAngle);
	m_pRock->GetTransCom()->Set_Angle(&vAngle);
}

void CMeteo::SpriteDustStart(int iIndex)
{
	_vec3 vPos = m_pTransCom->Get_Now_Position();

	m_pMeteoDust[iIndex]->SetAni(true);
	m_pMeteoDust[iIndex]->GetTransCom()->Set_Position(&vPos);

}