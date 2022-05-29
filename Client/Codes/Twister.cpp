#include "stdafx.h"
#include "Twister.h"
#include "Export_Engine.h"
#include "Wind.h"


CTwister::CTwister(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
:CEffect(pGraphicDev, wstrName)
, m_pWindOne(NULL)
, m_pWindTwo(NULL)
, WorldMat(NULL)
, m_fTwisterSpeed(10.f)
{
	m_bAni = false;
}

CTwister::~CTwister(void)
{
}

void CTwister::Update_Object(const _float & fTimeDelta)
{
	if (!m_bAni)
		return;

	m_pTransCom->Rotation(Engine::ROT_Y, m_fTwisterSpeed);

	CEffect::Update_Object(fTimeDelta);

	m_pWindOne->Update_Object(fTimeDelta);
	m_pWindTwo->Update_Object(fTimeDelta);

	WorldMat = m_pTransCom->Get_WorldMatrixPointer();

	m_pWindOne->GetTransCom()->Set_ParentMatrix(WorldMat);
	m_pWindTwo->GetTransCom()->Set_ParentMatrix(WorldMat);
}

void CTwister::Render_Object(void)
{
	if (!m_bAni)
		return;

	m_pWindOne->Render_Object();
	m_pWindTwo->Render_Object();
}

HRESULT CTwister::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;
	//Wind=====================================================
	_vec4 Color = _vec4(0.0f, 0.0f, 0.0f, 1.0f);
	m_pWindOne = CWind::Create(m_pGraphicDev, L"Wind", 40);
	m_pWindOne->SetAni(true);
	m_pWindOne->SetRotSpeed(0.f);
	m_pWindOne->SetRadius(30.0f);
	m_pWindOne->SetTrailY(10.0f);
	m_pWindOne->SetTextureNumber(1);
	m_pWindOne->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pWindOne->GetParticleInfo()->m_iPath = 2;
	m_pWindOne->SetRenderCheck(true);
	m_pWindOne->SetIntervalAngle(10.f);
	m_pWindOne->SettingTrail();
	m_pWindOne->GetTransCom()->Set_Scale(1.7f, 1.7f, 1.7f);
	_vec3 vAngle = _vec3(0, 220.f, -10.f);
	m_pWindOne->GetTransCom()->Set_Angle(&vAngle);

	m_pWindOne->SetEffectBlurCheck(TRUE);
	m_pWindOne->SetBlurCnt(4);
	m_pWindOne->SetBlurPass(3);
	m_pWindOne->SetBlurValue(70.0f);
	m_pWindOne->SetBlurPower(1.3f);
	m_pWindOne->SetBlurType(Engine::BLUR_VERTICAL);
	//=====================================================
	//Two==================================================
	m_pWindTwo = CWind::Create(m_pGraphicDev, L"WindTwo", 40);
	m_pWindTwo->SetAni(true);
	m_pWindTwo->SetRotSpeed(0.f);
	m_pWindTwo->SetRadius(30.0f);
	m_pWindTwo->SetTrailY(10.0f);
	m_pWindTwo->SetTextureNumber(1);
	m_pWindTwo->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pWindTwo->GetParticleInfo()->m_iPath = 2;
	m_pWindTwo->SetRenderCheck(true);
	m_pWindTwo->SetIntervalAngle(10.f);
	m_pWindTwo->SettingTrail();
	m_pWindTwo->GetTransCom()->Set_Scale(1.7f, 1.7f, 1.7f);
	vAngle = _vec3(0, 30.f, 10.f);
	m_pWindTwo->GetTransCom()->Set_Angle(&vAngle);

	m_pWindTwo->SetEffectBlurCheck(TRUE);
	m_pWindTwo->SetBlurCnt(6);
	m_pWindTwo->SetBlurPass(3);
	m_pWindTwo->SetBlurValue(50.0f);
	m_pWindTwo->SetBlurPower(1.3f);
	m_pWindTwo->SetBlurType(Engine::BLUR_VERTICAL);
	//=====================================================


	return S_OK;
}

CTwister * CTwister::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CTwister*		pInstance = new CTwister(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("Twister Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CTwister::Free(void)
{
	CEffect::Free();
	Engine::Safe_Release(m_pWindOne);
	Engine::Safe_Release(m_pWindTwo);
}

HRESULT CTwister::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CTwister::SetAni(_bool bAni)
{
	m_bAni = bAni;
}

_bool CTwister::GetAni(void)
{
	return m_bAni;
}

void CTwister::SetSpeed(_float fSpeed)
{
	m_fTwisterSpeed = fSpeed;
}

void CTwister::AddScaleAlpha(void)
{
	if (!m_bAni)
		return;


	_vec4 vColor = (_vec4)m_pWindOne->GetParticleInfo()->m_dColor;


	if (vColor.w <= 0.f)
	{
		m_bAni = false;

		m_pWindOne->SetAni(false);
		m_pWindTwo->SetAni(false);
	}
	else
	{
		m_pWindOne->GetTransCom()->Add_Scale(0.3f);
		m_pWindTwo->GetTransCom()->Add_Scale(0.3f);
		vColor.w -= 0.02f;
		m_pWindOne->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
		m_pWindTwo->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
	}



}
