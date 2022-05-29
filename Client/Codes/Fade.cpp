#include "stdafx.h"
#include "Fade.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "ResultItemLogo.h"
#include "ResultItemChar.h"
#include "ResultItemBox.h"

CFade::CFade(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
	:CUI(pGraphicDev, wstrName, eState, bIsActive)
	, m_pTransCom(NULL)
	, m_pBufferCom(NULL)
	, m_pTextureCom(NULL)
	, m_pRendererCom(NULL)
	, m_pShaderCom(NULL)
{

}

CFade::~CFade(void)
{

}

HRESULT CFade::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fX = (WINSIZEX >> 1);
	m_fY = (WINSIZEY >> 1);

	m_fSizeX = WINSIZEX;
	m_fSizeY = WINSIZEY;

	m_fAlpha = 0.f;
	m_bAlpha_Increase = false;
	m_bFade = false;
	m_fAlphaTime = 0.f;
	m_fTime = 0.f;
	m_fAngle = 0.f;
	m_bPick = false;
	return S_OK;
}

HRESULT CFade::Start_ForScene(void)
{
	return S_OK;
}

void CFade::Update_Object(const _float& fTimeDelta)
{
	if (!m_bPick)
	{
		if (m_bFade)
		{
			m_fTime += fTimeDelta;

			if (m_fTime >= m_fAlphaTime)
			{
				m_fAlpha += m_fAngle;

				if (m_fAlpha >= 1.f)
					m_fAlpha = 1.f;
			}
		}
		else
		{
			m_fAlpha -= m_fAngle;

			if (m_fAlpha <= 0.f)
			{
				m_fAlpha = 0.f;
				m_fTime = 0.f;
			}
		}
	}

	else
	{
		if (m_bFade)
		{
			m_fAlpha += m_fAngle;

			if (m_fAlpha >= 1.f)
			{
				m_fTime += fTimeDelta;

				m_fAlpha = 1.f;

				if (m_fTime >= m_fAlphaTime)
					m_bFade = false;
			}

		}
		else
		{
			m_fAlpha -= m_fAngle;

			if (m_fAlpha <= 0.f)
			{
				m_fAlpha = 0.f;
				m_fTime = 0.f;
				m_bPick = false;
			}
		}
	}

	CUI::Update_Object(fTimeDelta);
	
	m_pRendererCom->Add_WindowUI(30000, this);

	D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
}

void CFade::Render_Object(void)
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



HRESULT CFade::Add_Component(void)
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

	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Fade");
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


	return S_OK;
}

void CFade::CreateSelectItem(void)
{
	//ResultGetItemUI
	Engine::CGameObject* pGameObject = CResultItemLogo::Create(m_pGraphicDev, L"ItemLogo", this);
	Engine::AddObject(Engine::LAYER_UI, L"UI_Fade", pGameObject);
	pGameObject = CResultItemChar::Create(m_pGraphicDev, L"ItemChar", this);
	Engine::AddObject(Engine::LAYER_UI, L"UI_Fade", pGameObject);
	//Box
	for (int i = 0; i < 4; ++i)
	{
		pGameObject = CResultItemBox::Create(m_pGraphicDev, L"ItemBox", this, i);
		Engine::AddObject(Engine::LAYER_UI, L"UI_Result_Item", pGameObject);
	}
}

CFade* CFade::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CFade*		pInstance = new CFade(pGraphicDev, wstrName, eState, bIsActive);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CFade::Free(void)
{
	CUI::Free();
}
