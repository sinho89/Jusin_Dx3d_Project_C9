#include "stdafx.h"
#include "LevelUp.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"

CLevelUp::CLevelUp(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
	:CUI(pGraphicDev, wstrName, eState, bIsActive)
	, m_pTransCom(NULL)
	, m_pBufferCom(NULL)
	, m_pTextureCom(NULL)
	, m_pRendererCom(NULL)
	, m_pShaderCom(NULL)
{
}

CLevelUp::~CLevelUp(void)
{

}

HRESULT CLevelUp::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fX = WINSIZEX >> 1;
	m_fY = 150.f;

	m_fSizeX = 600.f;
	m_fSizeY = 300.f;

	m_fAlpha = 0.f;
	m_bAlpha_Increase = false;

	m_fTime = 10.f;
	m_bRender = true;

	m_fStartUV = -0.05f;
	m_fEndUV = 0.0f;

	return S_OK;
}

HRESULT CLevelUp::Start_ForScene(void)
{

	return S_OK;
}

void CLevelUp::Update_Object(const _float& fTimeDelta)
{
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

				if (m_fSizeX <= 300.f)
					m_fSizeX = 300.f;
				if (m_fSizeY <= 75.f)
					m_fSizeY = 75.f;
			}
			if (m_fAlpha >= 2.f)
			{
				m_bAlpha_Increase = true;
				m_fAlpha = 2.f;
			}
		}
		else
		{
			m_fAlpha -= 0.05f;

			if (m_fAlpha <= -1.f)
				m_eState = Engine::STATE_DESTROY;
		}

		CUI::Update_Object(fTimeDelta);

		m_pRendererCom->Add_WindowUI(1, this);

		D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);

		if (m_bAlpha_Increase)
		{
			if (m_fStartUV >= 1.f)
				return;

			m_fStartUV += 0.05f;
			m_fEndUV += 0.05f;
		}
	}


}


void CLevelUp::Render_Object(void)
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
	pEffect->BeginPass(10);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);

}


HRESULT CLevelUp::Add_Component(void)
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
	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"LevelUp");
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


CLevelUp* CLevelUp::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CLevelUp*		pInstance = new CLevelUp(pGraphicDev, wstrName, eState, bIsActive);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}


CLevelUp* CLevelUp::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, _uint iIndex, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CLevelUp*		pInstance = new CLevelUp(pGraphicDev, wstrName, eState, bIsActive);

	pInstance->SetIndex(iIndex);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevelUp::Free(void)
{
	CUI::Free();
}
