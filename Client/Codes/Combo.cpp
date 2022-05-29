#include "stdafx.h"
#include "Combo.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"

CCombo::CCombo(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
	:CUI(pGraphicDev, wstrName, eState, bIsActive)
	, m_pTransCom(NULL)
	, m_pBufferCom(NULL)
	, m_pTextureCom(NULL)
	, m_pRendererCom(NULL)
	, m_pShaderCom(NULL)
	, m_pNum(NULL)
{

}

CCombo::~CCombo(void)
{

}

HRESULT CCombo::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fX = (WINSIZEX >> 1) + 400;
	m_fY = (WINSIZEY >> 1) - 150;

	m_fSizeX = 150.f;
	m_fSizeY = 50.f;

	m_fAlpha = 0.f;
	m_fAngle = 10.f;
	m_bIsStatic = true;
	m_bRender = false;

	m_iIndex = 0;
	return S_OK;
}

HRESULT CCombo::Start_ForScene(void)
{
	if (Engine::GetSceneID() == Engine::SCENE_LOADING)
		m_bIsActive = false;
	if (Engine::GetSceneID() == Engine::SCENE_TOWN || Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE
		|| Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
		m_bIsActive = true;

	m_pNum = ((CUI*)Engine::Find(L"ComboNum_One"));
	m_pNumTen = ((CUI*)Engine::Find(L"ComboNum_Ten"));
	m_pNumHun = ((CUI*)Engine::Find(L"ComboNum_Hun"));
	m_pEffect = ((CUI*)Engine::Find(L"ComboEffect"));

	::Safe_Release(m_pNum);
	::Safe_Release(m_pNumTen);
	::Safe_Release(m_pNumHun);
	::Safe_Release(m_pEffect);

	return S_OK;
}

void CCombo::Update_Object(const _float& fTimeDelta)
{
	if (m_bRender)
	{
		CUI::Update_Object(fTimeDelta);

		m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);

		D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);

		Combo_Animation(fTimeDelta);
	}
	else
	{
		m_fX = (WINSIZEX >> 1) + 750;
		m_fY = (WINSIZEY >> 1) - 150;

		m_fSizeX = 150.f;
		m_fSizeY = 50.f;

		m_fAlpha = 0.f;
	}
	
}
void CCombo::Combo_Calculater(const _float& fTimeDelta)
{

}


void CCombo::Combo_Animation(const _float& fTimeDelta)
{
	m_fAngle -= fTimeDelta;

	if (m_fAngle <= 8.f)
	{
		m_fAngle = 10.f;
		m_bRender = false;
	}
	else
	{
		if (m_fX <= ((WINSIZEX >> 1) + 700))
		{
			m_fX -= (m_fAngle - 8.f) * 0.2f;
			m_fAlpha += 0.2f;
			return;
		}
		m_fAlpha += 0.2f;
		m_fSizeY -= 5.f;
		m_fX -= m_fAngle * 5.f;
	}
}

void CCombo::Reset(void)
{
	m_fAngle = 10.f;
	m_fX = (WINSIZEX >> 1) + 750;
	m_fY = (WINSIZEY >> 1) - 150;

	m_fSizeX = 150.f;
	m_fSizeY = 50.f;

	m_fAlpha = 0.f;
	m_pNum->SetAlpha(0.f);
	m_pNum->SetSize(80.f, 80.f);
	m_pNum->GetAlphaIncrease() = false;
	m_pNumTen->SetAlpha(0.f);
	m_pNumTen->SetSize(80.f, 80.f);
	m_pNumTen->GetAlphaIncrease() = false;
	m_pNumHun->SetAlpha(0.f);
	m_pNumHun->SetSize(80.f, 80.f);
	m_pNumHun->GetAlphaIncrease() = false;
	m_pEffect->SetAlpha(0.f);
	m_pEffect->SetWinPos(_vec2((WINSIZEX >> 1) + 600, m_fY + 25.f));
	m_pEffect->GetAlphaIncrease() = false;
}

void CCombo::Render_Object(void)
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


HRESULT CCombo::Add_Component(void)
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
	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"COMBO_FONT");
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


CCombo* CCombo::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CCombo*		pInstance = new CCombo(pGraphicDev, wstrName, eState, bIsActive);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CCombo::Free(void)
{
	CUI::Free();
}
