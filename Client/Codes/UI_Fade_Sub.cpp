#include "stdafx.h"
#include "UI_Fade_Sub.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"

CUI_Fade_Sub::CUI_Fade_Sub(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
	:CUI(pGraphicDev, wstrName, eState, bIsActive)
	, m_pTransCom(NULL)
	, m_pBufferCom(NULL)
	, m_pTextureCom(NULL)
	, m_pRendererCom(NULL)
	, m_pShaderCom(NULL)
	, m_fAniTime(0.0f)
	, m_fMagicScale(5.f)
	, m_bFadeIn(false)
	, m_bFadeOut(false)
{

}

CUI_Fade_Sub::~CUI_Fade_Sub(void)
{

}

HRESULT CUI_Fade_Sub::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;


	switch (m_eFadeType)
	{
	case FADE_SANG:
		m_fX = (WINSIZEX >> 1) - 600.f;
		m_fY = (WINSIZEY >> 1) + 250.f;
		break;
	case FADE_JUM:
		m_fX = (WINSIZEX >> 1) - 565.f;
		m_fY = (WINSIZEY >> 1) + 250.f;
		break;
	case FADE_JI:
		m_fX = (WINSIZEX >> 1) - 515.f;
		m_fY = (WINSIZEY >> 1) + 250.f;
		break;
	case FADE_GOO:
		m_fX = (WINSIZEX >> 1) - 480.f;
		m_fY = (WINSIZEY >> 1) + 250.f;
		break;

		// ICE
	case FADE_HA:
		m_fX = (WINSIZEX >> 1) - 640.f;
		m_fY = (WINSIZEY >> 1) + 250.f;
		break;
	case FADE_NOO:
		m_fX = (WINSIZEX >> 1) - 610.f;
		m_fY = (WINSIZEY >> 1) + 250.f;
		break;
	case FADE_MAN:
		m_fX = (WINSIZEX >> 1) - 580.f;
		m_fY = (WINSIZEY >> 1) + 250.f;
		break;
	case FADE_EU:
		m_fX = (WINSIZEX >> 1) - 550.f;
		m_fY = (WINSIZEY >> 1) + 250.f;
		break;
	case FADE_DONG:
		m_fX = (WINSIZEX >> 1) - 500.f;
		m_fY = (WINSIZEY >> 1) + 250.f;
		break;
	case FADE_GOOL:
		m_fX = (WINSIZEX >> 1) - 470.f;
		m_fY = (WINSIZEY >> 1) + 250.f;
		break;
		// Fire
	case FADE_BA:
		m_fX = (WINSIZEX >> 1) - 640.f;
		m_fY = (WINSIZEY >> 1) + 250.f;
		break;
	case FADE_R:
		m_fX = (WINSIZEX >> 1) - 610.f;
		m_fY = (WINSIZEY >> 1) + 250.f;
		break;
	case FADE_DI:
		m_fX = (WINSIZEX >> 1) - 580.f;
		m_fY = (WINSIZEY >> 1) + 250.f;
		break;
	case FADE_EL:
		m_fX = (WINSIZEX >> 1) - 550.f;
		m_fY = (WINSIZEY >> 1) + 250.f;
		break;
	case FADE_JU:
		m_fX = (WINSIZEX >> 1) - 500.f;
		m_fY = (WINSIZEY >> 1) + 250.f;
		break;
	case FADE_DOON:
		m_fX = (WINSIZEX >> 1) - 470.f;
		m_fY = (WINSIZEY >> 1) + 250.f;
		break;
	case FADE_FIRE_JI:
		m_fX = (WINSIZEX >> 1) - 440.f;
		m_fY = (WINSIZEY >> 1) + 250.f;
		break;
	}

	m_fSizeX = 50.f;
	m_fSizeY = 55.f;

	m_fAlpha = 0.f;

	return S_OK;
}

HRESULT CUI_Fade_Sub::Start_ForScene(void)
{
	return S_OK;
}

void CUI_Fade_Sub::Update_Object(const _float& fTimeDelta)
{
	CUI::Update_Object(fTimeDelta);
	FadeUpdate(fTimeDelta);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);

	D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);


}
void CUI_Fade_Sub::FadeUpdate(const _float& fTimeDelta)
{
	m_fAniTime += fTimeDelta;

	if (!m_bFadeIn && !m_bFadeOut)
	{
		switch (m_eFadeType)
		{
		case FADE_SANG:
			if (m_fAniTime >= 2.5f)
				m_bFadeIn = true;
			break;
		case FADE_JUM:
			if (m_fAniTime >= 2.75f)
				m_bFadeIn = true;
			break;
		case FADE_JI:
			if (m_fAniTime >= 3.f)
				m_bFadeIn = true;
			break;
		case FADE_GOO:
			if (m_fAniTime >= 3.25f)
				m_bFadeIn = true;
			break;
			// ICE
		case FADE_HA:
			if (m_fAniTime >= 2.5f)
				m_bFadeIn = true;
			break;
		case FADE_NOO:
			if (m_fAniTime >= 2.75f)
				m_bFadeIn = true;
			break;
		case FADE_MAN:
			if (m_fAniTime >= 3.f)
				m_bFadeIn = true;
			break;
		case FADE_EU:
			if (m_fAniTime >= 3.25f)
				m_bFadeIn = true;
			break;
		case FADE_DONG:
			if (m_fAniTime >= 3.5f)
				m_bFadeIn = true;
			break;
		case FADE_GOOL:
			if (m_fAniTime >= 3.75f)
				m_bFadeIn = true;
			break;
			// Fire
		case FADE_BA:
			if (m_fAniTime >= 2.5f)
				m_bFadeIn = true;
			break;
		case FADE_R:
			if (m_fAniTime >= 2.75f)
				m_bFadeIn = true;
			break;
		case FADE_DI:
			if (m_fAniTime >= 3.f)
				m_bFadeIn = true;
			break;
		case FADE_EL:
			if (m_fAniTime >= 3.25f)
				m_bFadeIn = true;
			break;
		case FADE_JU:
			if (m_fAniTime >= 3.5f)
				m_bFadeIn = true;
			break;
		case FADE_DOON:
			if (m_fAniTime >= 3.75f)
				m_bFadeIn = true;
			break;
		case FADE_FIRE_JI:
			if (m_fAniTime >= 4.f)
				m_bFadeIn = true;
			break;
		}
	}

	if (m_bFadeIn && !m_bFadeOut)
	{
		m_fAlpha += 0.02f;

		if ((m_fAlpha > 0.f) && (m_fAlpha <= 0.5f))
		{
			m_fSizeX += 0.2f;
			m_fSizeY += 0.2f;
		}
		else if ((m_fAlpha > 0.5f) && (m_fAlpha <= 1.f))
		{
			m_fSizeX -= 0.2f;
			m_fSizeY -= 0.2f;
		}

		if (m_fAlpha > 1.f)
		{
			m_bFadeIn = false;
			m_bFadeOut = true;
		}
	}

	if (!m_bFadeIn && m_bFadeOut)
	{
		switch (m_eFadeType)
		{
		case FADE_SANG:
			if (m_fAniTime >= 6.9f)
			{
				m_fAlpha -= 0.02f;

				if ((m_fAlpha > 0.f) && (m_fAlpha <= 0.5f))
				{
					m_fSizeX -= 0.02f;
					m_fSizeY -= 0.02f;
				}
				else if ((m_fAlpha > 0.5f) && (m_fAlpha <= 1.f))
				{
					m_fSizeX += 0.02f;
					m_fSizeY += 0.02f;
				}

				if (m_fAlpha < 0.f)
					m_eState = Engine::STATE_DESTROY;
			}
			break;
		case FADE_JUM:
			if (m_fAniTime >= 6.7f)
			{
				m_fAlpha -= 0.02f;

				if ((m_fAlpha > 0.f) && (m_fAlpha <= 0.5f))
				{
					m_fSizeX -= 0.02f;
					m_fSizeY -= 0.02f;
				}
				else if ((m_fAlpha > 0.5f) && (m_fAlpha <= 1.f))
				{
					m_fSizeX += 0.02f;
					m_fSizeY += 0.02f;
				}

				if (m_fAlpha < 0.f)
					m_eState = Engine::STATE_DESTROY;
			}
			break;
		case FADE_JI:
			if (m_fAniTime >= 6.5f)
			{
				m_fAlpha -= 0.02f;

				if ((m_fAlpha > 0.f) && (m_fAlpha <= 0.5f))
				{
					m_fSizeX -= 0.02f;
					m_fSizeY -= 0.02f;
				}
				else if ((m_fAlpha > 0.5f) && (m_fAlpha <= 1.f))
				{
					m_fSizeX += 0.02f;
					m_fSizeY += 0.02f;
				}

				if (m_fAlpha < 0.f)
					m_eState = Engine::STATE_DESTROY;
			}
			break;
		case FADE_GOO:
			if (m_fAniTime >= 6.3f)
			{
				m_fAlpha -= 0.02f;

				if ((m_fAlpha > 0.f) && (m_fAlpha <= 0.5f))
				{
					m_fSizeX -= 0.02f;
					m_fSizeY -= 0.02f;
				}
				else if ((m_fAlpha > 0.5f) && (m_fAlpha <= 1.f))
				{
					m_fSizeX += 0.02f;
					m_fSizeY += 0.02f;
				}

				if (m_fAlpha < 0.f)
					m_eState = Engine::STATE_DESTROY;
			}
			break;
		case FADE_HA:
			if (m_fAniTime >= 6.8f)
			{
				m_fAlpha -= 0.02f;

				if ((m_fAlpha > 0.f) && (m_fAlpha <= 0.5f))
				{
					m_fSizeX -= 0.02f;
					m_fSizeY -= 0.02f;
				}
				else if ((m_fAlpha > 0.5f) && (m_fAlpha <= 1.f))
				{
					m_fSizeX += 0.02f;
					m_fSizeY += 0.02f;
				}

				if (m_fAlpha < 0.f)
					m_eState = Engine::STATE_DESTROY;
			}
			break;
		case FADE_NOO:
			if (m_fAniTime >= 6.7f)
			{
				m_fAlpha -= 0.02f;

				if ((m_fAlpha > 0.f) && (m_fAlpha <= 0.5f))
				{
					m_fSizeX -= 0.02f;
					m_fSizeY -= 0.02f;
				}
				else if ((m_fAlpha > 0.5f) && (m_fAlpha <= 1.f))
				{
					m_fSizeX += 0.02f;
					m_fSizeY += 0.02f;
				}

				if (m_fAlpha < 0.f)
					m_eState = Engine::STATE_DESTROY;
			}
			break;
		case FADE_MAN:
			if (m_fAniTime >= 6.6f)
			{
				m_fAlpha -= 0.02f;

				if ((m_fAlpha > 0.f) && (m_fAlpha <= 0.5f))
				{
					m_fSizeX -= 0.02f;
					m_fSizeY -= 0.02f;
				}
				else if ((m_fAlpha > 0.5f) && (m_fAlpha <= 1.f))
				{
					m_fSizeX += 0.02f;
					m_fSizeY += 0.02f;
				}

				if (m_fAlpha < 0.f)
					m_eState = Engine::STATE_DESTROY;
			}
			break;
		case FADE_EU:
			if (m_fAniTime >= 6.5f)
			{
				m_fAlpha -= 0.02f;

				if ((m_fAlpha > 0.f) && (m_fAlpha <= 0.5f))
				{
					m_fSizeX -= 0.02f;
					m_fSizeY -= 0.02f;
				}
				else if ((m_fAlpha > 0.5f) && (m_fAlpha <= 1.f))
				{
					m_fSizeX += 0.02f;
					m_fSizeY += 0.02f;
				}

				if (m_fAlpha < 0.f)
					m_eState = Engine::STATE_DESTROY;
			}
			break;
		case FADE_DONG:
			if (m_fAniTime >= 6.4f)
			{
				m_fAlpha -= 0.02f;

				if ((m_fAlpha > 0.f) && (m_fAlpha <= 0.5f))
				{
					m_fSizeX -= 0.02f;
					m_fSizeY -= 0.02f;
				}
				else if ((m_fAlpha > 0.5f) && (m_fAlpha <= 1.f))
				{
					m_fSizeX += 0.02f;
					m_fSizeY += 0.02f;
				}

				if (m_fAlpha < 0.f)
					m_eState = Engine::STATE_DESTROY;
			}
			break;
		case FADE_GOOL:
			if (m_fAniTime >= 6.3f)
			{
				m_fAlpha -= 0.02f;

				if ((m_fAlpha > 0.f) && (m_fAlpha <= 0.5f))
				{
					m_fSizeX -= 0.02f;
					m_fSizeY -= 0.02f;
				}
				else if ((m_fAlpha > 0.5f) && (m_fAlpha <= 1.f))
				{
					m_fSizeX += 0.02f;
					m_fSizeY += 0.02f;
				}

				if (m_fAlpha < 0.f)
					m_eState = Engine::STATE_DESTROY;
			}
			break;
		case FADE_BA:
			if (m_fAniTime >= 6.9f)
			{
				m_fAlpha -= 0.02f;

				if ((m_fAlpha > 0.f) && (m_fAlpha <= 0.5f))
				{
					m_fSizeX -= 0.02f;
					m_fSizeY -= 0.02f;
				}
				else if ((m_fAlpha > 0.5f) && (m_fAlpha <= 1.f))
				{
					m_fSizeX += 0.02f;
					m_fSizeY += 0.02f;
				}

				if (m_fAlpha < 0.f)
					m_eState = Engine::STATE_DESTROY;
			}
			break;
		case FADE_R:
			if (m_fAniTime >= 6.8f)
			{
				m_fAlpha -= 0.02f;

				if ((m_fAlpha > 0.f) && (m_fAlpha <= 0.5f))
				{
					m_fSizeX -= 0.02f;
					m_fSizeY -= 0.02f;
				}
				else if ((m_fAlpha > 0.5f) && (m_fAlpha <= 1.f))
				{
					m_fSizeX += 0.02f;
					m_fSizeY += 0.02f;
				}

				if (m_fAlpha < 0.f)
					m_eState = Engine::STATE_DESTROY;
			}
			break;
		case FADE_DI:
			if (m_fAniTime >= 6.7f)
			{
				m_fAlpha -= 0.02f;

				if ((m_fAlpha > 0.f) && (m_fAlpha <= 0.5f))
				{
					m_fSizeX -= 0.02f;
					m_fSizeY -= 0.02f;
				}
				else if ((m_fAlpha > 0.5f) && (m_fAlpha <= 1.f))
				{
					m_fSizeX += 0.02f;
					m_fSizeY += 0.02f;
				}

				if (m_fAlpha < 0.f)
					m_eState = Engine::STATE_DESTROY;
			}
			break;
		case FADE_EL:
			if (m_fAniTime >= 6.6f)
			{
				m_fAlpha -= 0.02f;

				if ((m_fAlpha > 0.f) && (m_fAlpha <= 0.5f))
				{
					m_fSizeX -= 0.02f;
					m_fSizeY -= 0.02f;
				}
				else if ((m_fAlpha > 0.5f) && (m_fAlpha <= 1.f))
				{
					m_fSizeX += 0.02f;
					m_fSizeY += 0.02f;
				}

				if (m_fAlpha < 0.f)
					m_eState = Engine::STATE_DESTROY;
			}
			break;
		case FADE_JU:
			if (m_fAniTime >= 6.5f)
			{
				m_fAlpha -= 0.02f;

				if ((m_fAlpha > 0.f) && (m_fAlpha <= 0.5f))
				{
					m_fSizeX -= 0.02f;
					m_fSizeY -= 0.02f;
				}
				else if ((m_fAlpha > 0.5f) && (m_fAlpha <= 1.f))
				{
					m_fSizeX += 0.02f;
					m_fSizeY += 0.02f;
				}

				if (m_fAlpha < 0.f)
					m_eState = Engine::STATE_DESTROY;
			}
			break;
		case FADE_DOON:
			if (m_fAniTime >= 6.4f)
			{
				m_fAlpha -= 0.02f;

				if ((m_fAlpha > 0.f) && (m_fAlpha <= 0.5f))
				{
					m_fSizeX -= 0.02f;
					m_fSizeY -= 0.02f;
				}
				else if ((m_fAlpha > 0.5f) && (m_fAlpha <= 1.f))
				{
					m_fSizeX += 0.02f;
					m_fSizeY += 0.02f;
				}

				if (m_fAlpha < 0.f)
					m_eState = Engine::STATE_DESTROY;
			}
			break;
		case FADE_FIRE_JI:
			if (m_fAniTime >= 6.3f)
			{
				m_fAlpha -= 0.02f;

				if ((m_fAlpha > 0.f) && (m_fAlpha <= 0.5f))
				{
					m_fSizeX -= 0.02f;
					m_fSizeY -= 0.02f;
				}
				else if ((m_fAlpha > 0.5f) && (m_fAlpha <= 1.f))
				{
					m_fSizeX += 0.02f;
					m_fSizeY += 0.02f;
				}

				if (m_fAlpha < 0.f)
					m_eState = Engine::STATE_DESTROY;
			}
			break;
		}
	}
}



void CUI_Fade_Sub::Render_Object(void)
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

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture");

	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);

}


HRESULT CUI_Fade_Sub::Add_Component(void)
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
	case FADE_SANG:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Fade_Sang");
		pComponent = (Engine::CComponent*)m_pTextureCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
		break;
	case FADE_JUM:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Fade_Jum");
		pComponent = (Engine::CComponent*)m_pTextureCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
		break;
	case FADE_JI:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Fade_Ji");
		pComponent = (Engine::CComponent*)m_pTextureCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
		break;
	case FADE_GOO:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Fade_Goo");
		pComponent = (Engine::CComponent*)m_pTextureCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
		break;
		//// ICE

	case FADE_HA:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Fade_Ha");
		pComponent = (Engine::CComponent*)m_pTextureCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
		break;
	case FADE_NOO:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Fade_Noo");
		pComponent = (Engine::CComponent*)m_pTextureCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
		break;
	case FADE_MAN:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Fade_Man");
		pComponent = (Engine::CComponent*)m_pTextureCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
		break;
	case FADE_EU:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Fade_Eu");
		pComponent = (Engine::CComponent*)m_pTextureCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
		break;
	case FADE_DONG:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Fade_Dong");
		pComponent = (Engine::CComponent*)m_pTextureCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
		break;
	case FADE_GOOL:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Fade_Gool");
		pComponent = (Engine::CComponent*)m_pTextureCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
		break;
		//// Fire

	case FADE_BA:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Fade_Ba");
		pComponent = (Engine::CComponent*)m_pTextureCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
		break;
	case FADE_R:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Fade_R");
		pComponent = (Engine::CComponent*)m_pTextureCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
		break;
	case FADE_DI:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Fade_Di");
		pComponent = (Engine::CComponent*)m_pTextureCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
		break;
	case FADE_EL:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Fade_El");
		pComponent = (Engine::CComponent*)m_pTextureCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
		break;
	case FADE_JU:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Fade_Ju");
		pComponent = (Engine::CComponent*)m_pTextureCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
		break;
	case FADE_DOON:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Fade_Doon");
		pComponent = (Engine::CComponent*)m_pTextureCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
		break;
	case FADE_FIRE_JI:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Fade_Ji");
		pComponent = (Engine::CComponent*)m_pTextureCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
		break;
	}

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

	return S_OK;
}


CUI_Fade_Sub* CUI_Fade_Sub::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CUI_Fade_Sub*		pInstance = new CUI_Fade_Sub(pGraphicDev, wstrName, eState, bIsActive);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}



CUI_Fade_Sub* CUI_Fade_Sub::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, FADE_ID eType, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CUI_Fade_Sub*		pInstance = new CUI_Fade_Sub(pGraphicDev, wstrName, eState, bIsActive);

	pInstance->SetFadeType(eType);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Fade_Sub::Free(void)
{
	CUI::Free();
}
