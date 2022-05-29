#include "stdafx.h"
#include "UI_CoolTime.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "UI_CoolEff.h"

CUI_CoolTime::CUI_CoolTime(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
	:CUI_Skill(pGraphicDev, wstrName, eState, bIsActive)
	, m_pEffect(NULL)
{

}

CUI_CoolTime::~CUI_CoolTime(void)
{

}

HRESULT CUI_CoolTime::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;
		
	m_fX = WINSIZEX >> 1;
	m_fY = WINSIZEY - 150.f;
	m_fSizeX = 25.f;
	m_fSizeY = 25.f;


	switch (m_eSkillID)
	{
	case SKILL_DASH:
		m_fFadeTime = 5.f;
		break;
	case SKILL_LIDDLEKICK:
		m_fFadeTime = 5.f;
		break;
	case SKILL_BLARE:
		m_fFadeTime = 5.f;
		break;
	case SKILL_UPATT:
		m_fFadeTime = 5.f;
		break;
	case SKILL_TRICKEY:
		m_fFadeTime = 5.f;
		break;
	case SKILL_FLENITDANCE:
		m_fFadeTime = 5.f;
		break;
	case SKILL_VIFERKICK:
		m_fFadeTime = 5.f;
		break;
	case SKILL_HILLTURN:
		m_fFadeTime = 5.f;
		break;
	case SKILL_SILUETDANCE:
		m_fFadeTime = 5.f;
		break;
	case SKILL_SPININGHEART:
		m_fFadeTime = 5.f;
		break;
	case SKILL_SOFTLANDING:
		m_fFadeTime = 5.f;
		break;
	}

	m_fAlpha = 1.f;
	m_fAlphaTime = 0.f;
	m_bRender = false;
	return S_OK;
}

HRESULT CUI_CoolTime::Start_ForScene(void)
{
	return S_OK;
}

void CUI_CoolTime::Update_Object(const _float& fTimeDelta)
{
	if (m_bRender)
	{
		m_fAlphaTime += fTimeDelta;

		CUI::Update_Object(fTimeDelta);
		m_pRendererCom->Add_WindowUI(0, this);
		D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);

		if (m_fAlphaTime >= m_fFadeTime)
		{
			m_bRender = false;
			m_fAlphaTime = 0.f;

		}
	}
}

void CUI_CoolTime::Render_Object(void)
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
	pEffect->BeginPass(2);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);
}

void CUI_CoolTime::SetId(SKILL_ID Id)
{
	m_eSkillID = Id;
}


HRESULT CUI_CoolTime::Add_Component(void)
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

	switch (m_eSkillID)
	{
	case SKILL_DASH:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Dash");
		break;
	case SKILL_LIDDLEKICK:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"LiddleKick");
		break;
	case SKILL_BLARE:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"BlareSiluet");
		break;
	case SKILL_UPATT:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"UpAtt");
		break;
	case SKILL_TRICKEY:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"TrickyRabbit");
		break;
	case SKILL_FLENITDANCE:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"PlanitDance");
		break;
	case SKILL_VIFERKICK:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"ViferKick");
		break;
	case SKILL_HILLTURN:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"HillTurn");
		break;
	case SKILL_SILUETDANCE:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"SiluetDance");
		break;
	case SKILL_SPININGHEART:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"SpiningHeart");
		break;
	case SKILL_SOFTLANDING:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Softlanding");
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

	// For.Calculater Component
	pComponent = m_pCalCom = Engine::CUICalculater::Create();
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_UICal", pComponent));

	return S_OK;
}

CUI_CoolTime* CUI_CoolTime::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, SKILL_ID Id, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CUI_CoolTime*		pInstance = new CUI_CoolTime(pGraphicDev, wstrName, eState, bIsActive);

	pInstance->SetId(Id);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CUI_Rush Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}


void CUI_CoolTime::Free(void)
{
	CUI_Skill::Free();
}

