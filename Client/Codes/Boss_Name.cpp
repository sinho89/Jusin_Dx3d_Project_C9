#include "stdafx.h"
#include "Boss_Name.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "Sound.h"

CBossName::CBossName(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
	:CUI(pGraphicDev, wstrName, eState, bIsActive)
	, m_pTransCom(NULL)
	, m_pBufferCom(NULL)
	, m_pTextureCom(NULL)
	, m_pRendererCom(NULL)
	, m_pShaderCom(NULL)
{
}

CBossName::~CBossName(void)
{

}

HRESULT CBossName::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	switch (m_eFadeType)
	{
	case FADE_HA:
		m_fX = WINSIZEX - 400.f;
		m_fY = WINSIZEY - 200.f;
		m_fAlphaTime = 0.25f;
		break;
	case FADE_NOO:
		m_fX = WINSIZEX - 335.f;
		m_fY = WINSIZEY - 200.f;
		m_fAlphaTime = 0.75f;
		break;
	case FADE_MAN:
		m_fX = WINSIZEX - 270.f;
		m_fY = WINSIZEY - 200.f;
		m_fAlphaTime = 1.25f;
		break;
	case FADE_BA:
		m_fX = WINSIZEX - 400.f;
		m_fY = WINSIZEY - 200.f;
		m_fAlphaTime = 0.25f;
		break;
	case FADE_R:
		m_fX = WINSIZEX - 340.f;
		m_fY = WINSIZEY - 200.f;
		m_fAlphaTime = 0.75f;
		break;
	case FADE_DI:
		m_fX = WINSIZEX - 280.f;
		m_fY = WINSIZEY - 200.f;
		m_fAlphaTime = 1.25f;
		break;
	case FADE_EL:
		m_fX = WINSIZEX - 220.f;
		m_fY = WINSIZEY - 200.f;
		m_fAlphaTime = 1.75f;
		break;
	}


	m_fSizeX = 300.f;
	m_fSizeY = 300.f;

	m_fAlpha = 0.f;
	m_bAlpha_Increase = false;

	m_fTime = 10.f;
	m_bRender = false;

	m_fFadeTime = 0.f;

	m_fStartUV = 0.f;
	m_bSound = false;

	return S_OK;
}

HRESULT CBossName::Start_ForScene(void)
{

	return S_OK;
}

void CBossName::Update_Object(const _float& fTimeDelta)
{
	m_fStartUV += fTimeDelta;

	if (Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
	{
		if (m_fStartUV >= 4.f)
			m_eState = Engine::STATE_DESTROY;
	}
	else if (Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE)
	{
		if (m_fStartUV >= 6.f)
		{
			m_fAlpha -= 0.05f;

			if (m_fAlpha <= 0.f)
				m_eState = Engine::STATE_DESTROY;
		}
	}
	
	if (!m_bRender)
	{
		m_fFadeTime += fTimeDelta;

		if (m_fFadeTime >= m_fAlphaTime)
			m_bRender = true;
	}

	if (m_bRender)
		m_fTime -= fTimeDelta;
	else
		m_fTime += fTimeDelta;


	if (m_bRender)
	{
		if (!m_bAlpha_Increase)
		{
			m_fAlpha += 0.05f;

			if (m_fAlpha < 2.f)
			{
				m_fSizeX -= m_fTime * 5.f;
				m_fSizeY -= m_fTime * 5.f;

				if (m_fSizeX <= 90.f)
					m_fSizeX = 90.f;
				if (m_fSizeY <= 95.f)
					m_fSizeY = 95.f;

				if (m_fSizeX <= 90.f && m_fSizeY <= 95.f)
				{
					if (!m_bSound)
					{
						m_bSound = true;
						m_pSound->MyPlaySound(L"FadeName");
					}
				}
			}
			if (m_fAlpha >= 2.f)
			{
				m_fAlpha = 1.f;
				m_bAlpha_Increase = true;
			}

		}

		CUI::Update_Object(fTimeDelta);

		m_pRendererCom->Add_WindowUI(21002, this);

		D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
	}


}


void CBossName::Render_Object(void)
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (NULL == pEffect)
		return;

	_uint			iPassCnt;

	_matrix				matWorld, matView;
	_matrix				matOldView, matOldProj;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matOldProj);

	matWorld._11 = m_fSizeX;
	matWorld._22 = m_fSizeY;
	matWorld._33 = 1.f;
	matWorld._41 = m_fX - WINSIZEX * 0.5f;
	matWorld._42 = -m_fY + WINSIZEY * 0.5f;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &m_matProj);

	pEffect->SetFloat("g_fAlpha", m_fAlpha);
	pEffect->SetFloat("g_fNumColor", m_fAngle);
	pEffect->SetFloat("g_fStartUV", m_fStartUV);
	pEffect->SetFloat("g_fEndUV", m_fEndUV);

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture");

	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);

}


HRESULT CBossName::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.Buffer Component
	m_pBufferCom = (Engine::CRect_Texture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_RcTex");
	pComponent = (Engine::CComponent*)m_pBufferCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));

	// For.Texture Component 
	switch (m_eFadeType)
	{
	case FADE_HA:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Fade_Ha");
		break;
	case FADE_NOO:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Fade_Noo");
		break;
	case FADE_MAN:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Fade_Man");
		break;
	case FADE_BA:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Fade_Ba");
		break;
	case FADE_R:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Fade_R");
		break;
	case FADE_DI:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Fade_Di");
		break;
	case FADE_EL:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Fade_El");
		break;
	}
	pComponent = (Engine::CComponent*)m_pTextureCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));

	// For.Renderer Component
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	if (NULL == pComponent)
		return E_FAIL;
	pComponent->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));

	pComponent = m_pShaderCom = (Engine::CShader*)Engine::Clone_Prototype(L"Shader_UI");
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	pComponent = m_pSound = CSound::Create(NULL);
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Sound", pComponent));
	m_pSound->Set_Sound(L"FadeName", L"FadeName.wav");

	return S_OK;
}


CBossName* CBossName::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CBossName*		pInstance = new CBossName(pGraphicDev, wstrName, eState, bIsActive);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}


CBossName* CBossName::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, _uint iIndex, FADE_ID eType, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CBossName*		pInstance = new CBossName(pGraphicDev, wstrName, eState, bIsActive);

	pInstance->SetIndex(iIndex);
	pInstance->SetFadeType(eType);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CBossName::Free(void)
{
	CUI::Free();
}
