#include "stdafx.h"
#include "SpinningThrown.h"
#include "PlayerCamera.h"
#include "WitchBlade.h"
#include "SpinningBullet.h"
#include "Sprite.h"
#include "Wave.h"

CSpinningThrown::CSpinningThrown(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
:CEffect(pGraphicDev, wstrName)
, m_pBullet(NULL)
, m_dTimer(0)
, m_pColliderCom(NULL)
{
	m_vLook = _vec3(0.f, 0.0f, 1.0f);
	ZeroMemory(&m_pBombWave, sizeof(CWave*) * 20);

}

CSpinningThrown::~CSpinningThrown(void)
{
}

void CSpinningThrown::Update_Object(const _float & fTimeDelta)
{

	//Player 충돌
	if (m_pColliderCom->IsHit())
	{
		m_eState = Engine::STATE_DESTROY;
	}

	m_dTimer += fTimeDelta;

	_vec3 vPos;

	m_pTransCom->Move_Position(&m_vTargetLook);
	vPos = m_pTransCom->Get_Now_Position();

	m_pBullet->GetTransCom()->Set_Position(&vPos);
	m_pBullet->Update_Object(fTimeDelta);


	if(m_dTimer >= 0.1)
	{
		m_dTimer = 0;

		for (int i = 0; i < 20; ++i)
		{
			SpinningThrownStart(i);
			break;
		}
	}

	SpinningUpdate();

	for (int i = 0; i < 20; ++i)
	{
		m_pBombWave[i]->Update_Object(fTimeDelta);
	}

	CEffect::Update_Object(fTimeDelta);
}

void CSpinningThrown::Render_Object(void)
{
	m_pBullet->Render_Object();

	for (int i = 0; i < 20; ++i)
	{
		m_pBombWave[i]->Render_Object();
	}

	if (m_pColliderCom)
		m_pColliderCom->Render_Collider(g_bDebugRender);
}

HRESULT CSpinningThrown::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pBullet = CSpinningBullet::Create(m_pGraphicDev, L"Bullet");

	_vec4 vColor = _vec4(-1.f, -0.1f, 1.f, 2.f);
	//Sprite Dust
	for (int i = 0; i < 20; ++i)
	{
		m_pBombWave[i] = CWave::Create(m_pGraphicDev, L"SpinningWave");
		m_pBombWave[i]->SetAni(false);
		m_pBombWave[i]->SetPattern(CWave::PATTERN_END);
		m_pBombWave[i]->SetRendererCheck(true);
		m_pBombWave[i]->GetTransCom()->Set_Scale(1.f, 1.f, 1.f);
		m_pBombWave[i]->GetParticleInfo()->m_iPath = 2;
		m_pBombWave[i]->GetParticleInfo()->m_iTextureNumber = 8;
		m_pBombWave[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;

		m_pBombWave[i]->SetEffectBlurCheck(TRUE);
		m_pBombWave[i]->SetBlurPass(2);
		m_pBombWave[i]->SetBlurCnt(2);
		m_pBombWave[i]->SetBlurPower(2.f);
		m_pBombWave[i]->SetBlurValue(80.0f);
		m_pBombWave[i]->SetBlurType(Engine::BLUR_CROSS);
	}


	//=========================================================================
	m_pColliderCom->SetScale(5.f);
	m_pColliderCom->SetPos(_vec3(0.f, 0.f, -2.f));
	Engine::COLINFO tColInfo;
	tColInfo.eColInfo = Engine::COL_PUSH;
	tColInfo.fJumpForce = 0.f;
	tColInfo.fStampForce = 0.f;
	tColInfo.fForce = 10.f;
	tColInfo.vDir = m_vTargetLook;
	tColInfo.iDamage = 40;
	m_pColliderCom->Set_ColInfo(&tColInfo);
	Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pColliderCom);
	//======================================================================

	return S_OK;
}

CSpinningThrown * CSpinningThrown::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CSpinningThrown*		pInstance = new CSpinningThrown(pGraphicDev, wstrName);

		if (FAILED(pInstance->Ready_Object()))
		{
			MSG_BOX("SpinningThrown Created Failed");
			::Safe_Release(pInstance);
		}

		return pInstance;
}

void CSpinningThrown::SetAni(_bool bAni)
{
	m_bAni = bAni;
}

_bool CSpinningThrown::GetAni(void)
{
	return m_bAni;
}

void CSpinningThrown::Setting(Engine::CTransform* TargetTransform)
{
	_vec3 Look, vPos;
	TargetTransform->Get_Infomation(Engine::INFO_LOOK, &Look); 
	//Player의 LookVector
	D3DXVec3Normalize(&Look, &Look);
	m_vTargetLook = Look;
	m_vTargetLook *= 1.5f;

	_float Dot = D3DXVec3Dot(&m_vLook, &Look);

	_float Radian = (_float)acos(Dot);

	Radian = D3DXToDegree(Radian);

	_vec3 vDir;

	D3DXVec3Cross(&vDir, &m_vLook, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	if (D3DXVec3Dot(&vDir, &Look) > 0) //왼쪽
	{
		Radian *= -1.f;
		Radian -= 4.f;
	}
	else
	{
		Radian *= 1.f;
		Radian -= 4.f;
	}

	_vec3 Angle = _vec3(0.f, Radian, 0.0f);

	m_vTargetAngle = Angle;
	
	vPos = TargetTransform->Get_Now_Position();
	vPos.y += 5.f;
	m_pTransCom->Set_Position(&vPos);
	m_pTransCom->Set_Angle(&Angle);

	m_pBullet->GetTransCom()->Set_Position(&vPos);
	_vec3 vBulletAngle = _vec3(90.f, Angle.y, 0.f);
	m_pBullet->GetTransCom()->Set_Angle(&vBulletAngle);

}

void CSpinningThrown::Free(void)
{
	//충돌체 삭제=======================================
	Engine::Delete_Collider(L"Player", m_pColliderCom);

	CEffect::Free();
	Engine::Safe_Release(m_pBullet);

	for (int i = 0; i < 20; ++i)
	{
		Engine::Safe_Release(m_pBombWave[i]);
	}
}

HRESULT CSpinningThrown::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));


	//For.Collider Component
	pComponent = Engine::Create_SphereCollider(m_pGraphicDev, m_pTransCom, L"Player", _vec3(0.f, 0.f, 0.f), 1.f);
	if (pComponent == NULL)
		return E_FAIL;

	m_pColliderCom = (Engine::CSphereCollider*)pComponent;
	m_pColliderCom->Set_Render(true);
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Collider", pComponent));

	return S_OK;
}

void CSpinningThrown::SpinningThrownStart(int iIndex)
{
	_vec3 vPos = m_pTransCom->Get_Now_Position();
	_vec4 vColor = _vec4(-1.f, -0.1f, 1.f, 2.f);

	m_pBombWave[iIndex]->SetAni(true);
	m_pBombWave[iIndex]->GetTransCom()->Set_Position(&vPos);
	m_pBombWave[iIndex]->GetTransCom()->Set_Angle(&m_vTargetAngle);
	m_pBombWave[iIndex]->GetTransCom()->Set_Scale(2.f, 2.f, 2.f);
	m_pBombWave[iIndex]->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
}

void CSpinningThrown::SpinningUpdate(void)
{
	for (int i = 0; i < 20; ++i)
	{
		if (!m_pBombWave[i]->GetAni())
			continue;

		_vec4 vColor = (_vec4)m_pBombWave[i]->GetParticleInfo()->m_dColor;

		if (vColor.w <= 0.f)
		{
			m_pBombWave[i]->SetAni(false);
		}
		else
		{
			vColor.w -= 0.01f;
			m_pBombWave[i]->GetTransCom()->Add_Scale(0.1f);
			m_pBombWave[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
		}
	}
}
