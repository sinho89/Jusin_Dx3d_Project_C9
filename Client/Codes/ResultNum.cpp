#include "stdafx.h"
#include "ResultNum.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"

CResultNum::CResultNum(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
	:CUI(pGraphicDev, wstrName, eState, bIsActive)
	, m_pTransCom(NULL)
	, m_pBufferCom(NULL)
	, m_pTextureCom(NULL)
	, m_pRendererCom(NULL)
	, m_pShaderCom(NULL)
{
}

CResultNum::~CResultNum(void)
{

}

HRESULT CResultNum::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fSizeX = 50.f;
	m_fSizeY = 55.f;

	m_fAlpha = 0.f;
	m_bAlpha_Increase = false;

	switch (m_iTypeIndex)
	{
	case 0:
		m_fFadeTime = 4.f;
		break;
	case 1:
		m_fFadeTime = 4.25f;
		break;
	case 2:
		m_fFadeTime = 4.5f;
		break;
	case 3:
		m_fFadeTime = 4.f;
		break;
	case 4:
		m_fFadeTime = 4.25f;
		break;
	case 5:
		m_fFadeTime = 4.5f;
		break;
	case 6:
		m_fFadeTime = 4.f;
		break;
	case 7:
		m_fFadeTime = 4.25f;
		break;
	case 8:
		m_fFadeTime = 4.5f;
		break;
	case 9:
		m_fFadeTime = 4.f;
		break;
	case 10:
		m_fFadeTime = 4.25f;
		break;
	case 11:
		m_fFadeTime = 4.5f;
		break;						///////////////// 첫라인 /////////////////

	case 12:
		m_fFadeTime = 5.f;
		break;
	case 13:
		m_fFadeTime = 5.25f;
		break;
	case 14:
		m_fFadeTime = 5.5f;
		break;
	case 15:
		m_fFadeTime = 5.f;
		break;
	case 16:
		m_fFadeTime = 5.25f;
		break;
	case 17:
		m_fFadeTime = 5.5f;
		break;						///////////////// 둘째라인 /////////////////
	case 18:
		m_fFadeTime = 6.f;
		break;
	case 19:
		m_fFadeTime = 6.25f;
		break;
	case 20:
		m_fFadeTime = 6.5f;
		break;
	case 21:
		m_fFadeTime = 6.f;
		break;
	case 22:
		m_fFadeTime = 6.25f;
		break;
	case 23:
		m_fFadeTime = 6.5f;
		break;
	case 24:
		m_fFadeTime = 6.f;
		break;
	case 25:
		m_fFadeTime = 6.25f;
		break;
	case 26:
		m_fFadeTime = 6.5f;
		break;
	case 27:
		m_fFadeTime = 6.f;
		break;
	case 28:
		m_fFadeTime = 6.25f;
		break;
	case 29:
		m_fFadeTime = 6.5f;
		break;
	case 30:
		m_fFadeTime = 3.75f;
		break;
	case 31:
		m_fFadeTime = 3.75f;
		break;
	case 32:
		m_fFadeTime = 5.75f;
		break;
	case 33: // Gold
		m_fFadeTime = 7.25f;
		break;
	case 34:
		m_fFadeTime = 7.5f;
		break;
	case 35:
		m_fFadeTime = 7.75f;
		break;
	case 36:
		m_fFadeTime = 8.f;
		break;
	}

	m_fTime = 0.f;
	m_bRender = false;

	return S_OK;
}

HRESULT CResultNum::Start_ForScene(void)
{

	return S_OK;
}

void CResultNum::Update_Object(const _float& fTimeDelta)
{
	m_fTime += fTimeDelta;

	if (m_fTime >= m_fFadeTime)
	{
		m_bRender = true;
		m_fTime = 0.f;
	}

	if (m_bRender)
	{
		if (!m_bAlpha_Increase)
		{
			m_fAlpha += 0.05f;

			if (m_fAlpha < 1.f)
			{
				m_fSizeX -= fTimeDelta * 100.f;
				m_fSizeY -= fTimeDelta * 100.f;

				if (m_fSizeX <= 30.f)
					m_fSizeX = 30.f;
				if (m_fSizeY <= 35.f)
					m_fSizeY = 35.f;
			}
		}
		if (m_fAlpha >= 1.f)
			m_bAlpha_Increase = true;

		CUI::Update_Object(fTimeDelta);

		m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);

		D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
	}
	
	
}


void CResultNum::Render_Object(void)
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

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture", m_iIndex);

	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);

}


HRESULT CResultNum::Add_Component(void)
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
	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"ResultNum");
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


CResultNum* CResultNum::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CResultNum*		pInstance = new CResultNum(pGraphicDev, wstrName, eState, bIsActive);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}


CResultNum* CResultNum::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, _uint iIndex, _uint iTypeIndex, _vec2 vPos, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CResultNum*		pInstance = new CResultNum(pGraphicDev, wstrName, eState, bIsActive);

	pInstance->SetIndex(iIndex);
	pInstance->SetWinPos(vPos);
	pInstance->SetTypeIndex(iTypeIndex);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CResultNum::Free(void)
{
	CUI::Free();
}
