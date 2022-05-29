#include "stdafx.h"
#include "Magma.h"
#include "Sprite.h"
#include "Export_Engine.h"

CMagma::CMagma(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
:CEffect(pGraphicDev, wstrName)
, m_dTimer(0)
{
	ZeroMemory(&m_pMagmaBall, sizeof(CSprite*) * 10);
}

CMagma::~CMagma(void)
{
}

void CMagma::Update_Object(const _float & fTimeDelta)
{
	CEffect::Update_Object(fTimeDelta);

	m_dTimer += fTimeDelta;

	if (m_dTimer >= 0.5f)
	{
		m_dTimer = 0;


	}



	for (int i = 0; i < 10; ++i)
	{
		m_pMagmaBall[i]->Update_Object(fTimeDelta);
	}
}

void CMagma::Render_Object(void)
{
	for (int i = 0; i < 10; ++i)
	{
		m_pMagmaBall[i]->Render_Object();
	}
}

HRESULT CMagma::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;


	for (int i = 0; i < 10; ++i)
	{
		m_pMagmaBall[i] = CSprite::LoadCreate(m_pGraphicDev, L"MagmaBall");
		m_pMagmaBall[i]->SetAni(true);
		m_pMagmaBall[i]->GetTransCom()->Set_Scale(1.f, 1.f, 1.f);
		m_pMagmaBall[i]->SetRenderCheck(true);

		m_pMagmaBall[i]->SetBlurCheck(TRUE);
		m_pMagmaBall[i]->SetBlurPass(2);
		m_pMagmaBall[i]->SetBlurCnt(2);
		m_pMagmaBall[i]->SetBlurPower(1.0f);  //³ôÀ»¼ö·Ï ¹à°Ô
		m_pMagmaBall[i]->SetBlurValue(200.0f); //³·À»¼ö·Ï ¸¹ÀÌ Èçµé¸°´Ù.
		m_pMagmaBall[i]->SetBlurType(Engine::BLUR_CROSS);
	}



	return S_OK;
}

CMagma* CMagma::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CMagma*		pInstance = new CMagma(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("Magma Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CMagma::Free(void)
{
	CEffect::Free();
	for (int i = 0; i < 10; ++i)
	{
		Engine::Safe_Release(m_pMagmaBall[i]);
	}
}

HRESULT CMagma::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}
