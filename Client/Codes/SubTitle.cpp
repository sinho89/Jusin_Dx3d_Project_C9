#include "stdafx.h"
#include "SubTitle.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"

CSubTitle::CSubTitle(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
	:CUI(pGraphicDev, wstrName, eState, bIsActive)
	, m_pTransCom(NULL)
	, m_pBufferCom(NULL)
	, m_pTextureCom(NULL)
	, m_pRendererCom(NULL)
	, m_pShaderCom(NULL)
	, m_fFadeStop(0.0f)
{

}

CSubTitle::~CSubTitle(void)
{

}

HRESULT CSubTitle::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;


	switch (m_iIndex)
	{
	case 0: //Å³
		m_fX = 1300.f;
		m_fY = 290.f;
		m_fSizeX = 75.f;
		m_fSizeY = 25.f;
		m_fFadeTime = 1.f;
		m_fFadeStop = 250.f;
		break;
	case 1: //½ºÅ³
		m_fX = 1300.f;
		m_fY = 340.f;
		m_fSizeX = 75.f;
		m_fSizeY = 25.f;
		m_fFadeTime = 1.f;
		m_fFadeStop = 250.f;
		break;
	case 2: //·©Å©
		m_fX = 1300.f;
		m_fY = 615.f;
		m_fSizeX = 65.f;
		m_fSizeY = 30.f;
		m_fFadeTime = 1.f;
		m_fFadeStop = 250.f;
		break;
	case 3: //¸àÅä¸µ
		m_fX = 1300.f;
		m_fY = 665.f;
		m_fSizeX = 90.f;
		m_fSizeY = 30.f;
		m_fFadeTime = 1.f;
		m_fFadeStop = 240.f;
		break;
	case 4: //³­ÀÔ
		m_fX = 1500.f;
		m_fY = 610.f;
		m_fSizeX = 65.f;
		m_fSizeY = 30.f;
		m_fFadeTime = 2.f;
		m_fFadeStop = 250.f;
		break;
	case 5: //º¸³Ê½º
		m_fX = 1500.f;
		m_fY = 660.f;
		m_fSizeX = 75.f;
		m_fSizeY = 35.f;
		m_fFadeTime = 2.f;
		m_fFadeStop = 250.f;
		break;
	case 6: //½ºÅ¸ÀÏ
		m_fX = WINSIZEX + 100.f;
		m_fY = 290.f;
		m_fSizeX = 75.f;
		m_fSizeY = 25.f;
		m_fFadeTime = 2.75f;
		m_fFadeStop = 340.f;
		break;
	case 7: //Å×Å©´Ð
		m_fX = WINSIZEX + 100.f;
		m_fY = 340.f;
		m_fSizeX = 75.f;
		m_fSizeY = 25.f;
		m_fFadeTime = 2.75f;
		m_fFadeStop = 340.f;
		break;
	case 8: //PC¹æ
		m_fX = WINSIZEX + 100.f;
		m_fY = 610.f;
		m_fSizeX = 75.f;
		m_fSizeY = 35.f;
		m_fFadeTime = 2.75f;
		m_fFadeStop = 250.f;
		break;
	case 9: //ÅäÅ»
		m_fX = WINSIZEX + 100.f;
		m_fY = 660.f;
		m_fSizeX = 75.f;
		m_fSizeY = 35.f;
		m_fFadeTime = 2.75f;
		m_fFadeStop = 250.f;
		break;
	case 10: //°ñµå
		m_fX = 1500.f;
		m_fY = 720.f;
		m_fSizeX = 75.f;
		m_fSizeY = 35.f;
		m_fFadeTime = 7.f;
		m_fFadeStop = 250.f;
		break;
	case 11: //·©Å©(±Â)
		break;
	case 12: //·©Å©(¾×¼¿·±Æ®)
		break;
	}

	m_fOrifX = m_fX;
	m_fAlpha = 0.f;
	m_fTime = 0.f;
	m_bRender = false;

	return S_OK;
}

HRESULT CSubTitle::Start_ForScene(void)
{
	CUI::Start_ForScene();

	return S_OK;
}

void CSubTitle::Update_Object(const _float& fTimeDelta)
{
	m_fTime += fTimeDelta;

	if (m_fTime >= m_fFadeTime)
	{
		if (!m_bRender)
		{
			m_fTime = 0.f;
			m_bRender = true;
		}
	}

	if (m_bRender)
	{
		m_fAlpha += 0.05f;
		if (m_fAlpha >= 1.f)
			m_fAlpha = 1.f;

		m_fX -= m_fTime * 30.f;

	
		if (m_fX <= m_fOrifX - m_fFadeStop)
			m_fX = m_fOrifX - m_fFadeStop;
		
		CUI::Update_Object(fTimeDelta);

		m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);

		D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
	}
	
}

void CSubTitle::Render_Object(void)
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


HRESULT CSubTitle::Add_Component(void)
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
	switch (m_iIndex)
	{
	case 0: //Å³
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Kill");
		break;
	case 1: //½ºÅ³
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Skill");
		break;
	case 2: //·©Å©
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Rank_S");
		break;
	case 3: //¸àÅä¸µ
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Mento");
		break;
	case 4: //³­ÀÔ
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Nanip");
		break;
	case 5: //º¸³Ê½º
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Bonus_S");
		break;
	case 6: //½ºÅ¸ÀÏ
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Style");
		break;
	case 7: //Å×Å©´Ð
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Technic");
		break;
	case 8: //PC¹æ
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"PC");
		break;
	case 9: //ÅäÅ»
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Total");
		break;
	case 10: //°ñµå
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Gold");
		break;
	case 11: //·©Å©(±Â)
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"ReturnStage");
		break;
	case 12: //·©Å©(¾×¼¿·±Æ®)
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Excellent");
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

	return S_OK;
}


CSubTitle* CSubTitle::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CSubTitle*		pInstance = new CSubTitle(pGraphicDev, wstrName, eState, bIsActive);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}


CSubTitle* CSubTitle::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, _uint iIndex, _uint iFadeType, _float fFadeTime, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CSubTitle*		pInstance = new CSubTitle(pGraphicDev, wstrName, eState, bIsActive);

	pInstance->SetFadeTime(fFadeTime);
	pInstance->SetFadeType(iFadeType);
	pInstance->SetIndex(iIndex);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubTitle::Free(void)
{
	CUI::Free();
}
