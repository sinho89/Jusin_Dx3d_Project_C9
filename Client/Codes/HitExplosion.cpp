#include "stdafx.h"
#include "HitExplosion.h"
#include "Export_Engine.h"
#include "Sprite.h"
#include "Wave.h"


CHitExplosion::CHitExplosion(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
:CEffect(pGraphicDev, wstrName)
,m_pWave(NULL)
,m_pSprite(NULL)
{
	m_bAni = true;
	m_pStartPos = _vec3(0.f, 0.f, 0.f);
}

CHitExplosion::~CHitExplosion(void)
{
}

void CHitExplosion::Update_Object(const _float & fTimeDelta)
{
	if (m_pWave->GetAni() == false && m_pSprite->GetAni() == false
		&& m_pDust->GetAni() == false)
	{
		m_bAni = false;
	}

	if (!m_bAni)
	{
		m_eState = Engine::STATE_DESTROY;
	}

	WaveUpdate();

	m_pWave->Update_Object(fTimeDelta);

	m_pSprite->Update_Object(fTimeDelta);

	m_pDust->Update_Object(fTimeDelta);
}

void CHitExplosion::Render_Object(void)
{
	m_pWave->Render_Object();

	m_pSprite->Render_Object();

	m_pDust->Render_Object();
}

HRESULT CHitExplosion::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	WaveReady();

	m_pSprite = CSprite::LoadCreate(m_pGraphicDev, L"ExplosionDust");
	m_pSprite->SetAni(true);
	m_pSprite->SetRenderCheck(true);
	m_pSprite->GetTransCom()->Set_Scale(5.f, 5.f, 5.f);

	m_pSprite->SetBlurCheck(TRUE);
	m_pSprite->SetBlurPass(0);
	m_pSprite->SetBlurCnt(2);
	m_pSprite->SetBlurPower(1.1f);  //³ôÀ»¼ö·Ï ¹à°Ô
	m_pSprite->SetBlurValue(200.0f); //³·À»¼ö·Ï ¸¹ÀÌ Èçµé¸°´Ù.
	m_pSprite->SetBlurType(Engine::BLUR_CROSS);
	//====================================================

	m_pDust = CSprite::LoadCreate(m_pGraphicDev, L"HanumanHQDust");
	m_pDust->SetAni(true);
	m_pDust->SetRenderCheck(true);
	m_pDust->GetTransCom()->Set_Scale(6.f, 6.f, 6.f);

	m_pDust->SetBlurCheck(TRUE);
	m_pDust->SetBlurPass(2);
	m_pDust->SetBlurCnt(2);
	m_pDust->SetBlurPower(1.2f);  //³ôÀ»¼ö·Ï ¹à°Ô
	m_pDust->SetBlurValue(180.0f); //³·À»¼ö·Ï ¸¹ÀÌ Èçµé¸°´Ù.
	m_pDust->SetBlurType(Engine::BLUR_CROSS);
	return S_OK;
}

CHitExplosion * CHitExplosion::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CHitExplosion* pInstance = new CHitExplosion(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("HitExplosion Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CHitExplosion::Free(void)
{
	CEffect::Free();

	Engine::Safe_Release(m_pWave);
	Engine::Safe_Release(m_pSprite);
	Engine::Safe_Release(m_pDust);
}

HRESULT CHitExplosion::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));


	return S_OK;
}

void CHitExplosion::SetStartPos(_vec3 vPos)
{
	m_pStartPos = vPos;
}

void CHitExplosion::WaveReady(void)
{

	//Wave
	m_pWave = CWave::Create(m_pGraphicDev, L"HitExplosionWave");
	m_pWave->SetAni(true);
	m_pWave->SetRendererCheck(true);
	m_pWave->SetPattern(CWave::PATTERN_END);
	_vec4 vColor = _vec4(1.0f, -0.3f, -0.9f, 1.0f);
	m_pWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
	m_pWave->GetParticleInfo()->m_iPath = 2;
	m_pWave->GetParticleInfo()->m_iTextureNumber = 7;
	m_pWave->GetTransCom()->Set_Scale(2.f, 2.f, 2.f);

	_vec3 vAngle = _vec3(90.f, 0.f, 0.f);
	m_pWave->GetTransCom()->Set_Angle(&vAngle);

	m_pWave->SetBlurCheck(TRUE);
	m_pWave->SetBlurPass(2);
	m_pWave->SetBlurCnt(2);
	m_pWave->SetBlurPower(1.7f);  //³ôÀ»¼ö·Ï ¹à°Ô
	m_pWave->SetBlurValue(140.0f); //³·À»¼ö·Ï ¸¹ÀÌ Èçµé¸°´Ù.
	m_pWave->SetBlurType(Engine::BLUR_CROSS);
}

void CHitExplosion::WaveUpdate(void)
{
	if (!m_pWave->GetAni())
		return;

	_vec4 vColor = (_vec4)m_pWave->GetParticleInfo()->m_dColor;

	if (vColor.w <= 0.f)
		m_pWave->SetAni(false);
	else
	{
		m_pWave->GetTransCom()->Add_Scale(0.4f);
		vColor.w -= 0.05f;
		m_pWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
	}
}

void CHitExplosion::SettingStartPos(void)
{
	_vec3 vPos = m_pStartPos;
	vPos.y += 2.3f;

	m_pWave->GetTransCom()->Set_Position(&vPos);
	m_pSprite->GetTransCom()->Set_Position(&vPos);

	vPos.y += 4.f;
	m_pDust->GetTransCom()->Set_Position(&vPos);
}
