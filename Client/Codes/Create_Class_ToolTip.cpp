#include "stdafx.h"
#include "Create_Class_ToolTip.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"

CCreate_Class_ToolTip::CCreate_Class_ToolTip(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
:CUI(pGraphicDev, wstrName, eState, bIsActive)
,m_pTransCom(NULL)
,m_pBufferCom(NULL)
,m_pRendererCom(NULL)
,m_pShaderCom(NULL)
{
	ZeroMemory(&m_szName, NULL);
}

CCreate_Class_ToolTip::~CCreate_Class_ToolTip(void)
{

}

HRESULT CCreate_Class_ToolTip::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	m_fX = (WINSIZEX >> 1) - 500;
	m_fY = (WINSIZEY >> 1) + 400;

	m_fSizeX = 500.f;
	m_fSizeY = 50.f;

	return S_OK;
}

HRESULT CCreate_Class_ToolTip::Start_ForScene(void)
{
	return S_OK;
}

void CCreate_Class_ToolTip::Update_Object(const _float& fTimeDelta)
{
	CUI::Update_Object(fTimeDelta);
	Alpha_Calculation(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_PRIORITY, this);

	D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
}

void CCreate_Class_ToolTip::Render_Object(void)
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if(NULL == pEffect)
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


	Engine::Render_Font(L"Font_Create_State", m_szName, &_vec2(m_fX - 150.f, m_fY - 10.f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));
}

void CCreate_Class_ToolTip::Alpha_Calculation(const _float& fTimeDelta)
{
	m_fAlphaTime += fTimeDelta;

	if(m_fAlpha <= 0.f)
		m_bAlpha_Increase = true;
	if(m_fAlpha >= 1.f)
		m_bAlpha_Increase = false;

	if(m_fAlphaTime >= 0.005f)
	{
		m_fAlphaTime = 0.f;

		if(m_bAlpha_Increase)
			m_fAlpha += 0.01f;

	}

	switch(m_iIndex)
	{
	case 0:
		m_szName = L"검과 방패, 두꺼운 갑옷을 입고 전장을 해집고 다니는 파이터.";
			break;
	case 1:
		m_szName = L"날카로운 눈썰미, 그리고 정확한 활 솜씨로 적을 유린하는 헌터.";
			break;
	case 2:
		m_szName = L"정령과 영적 교류를 통해 강력한 힘을 빌려 적을 멸하는 샤먼.";
			break;
	case 3:
		m_szName = L"신비로운 마력과 매정하고 날카로운 검으로 적을 멸하는 마검사.";
			break;
	case 4:
		m_szName = L"여신의 가호로 무기의 잠재된 힘을 깨우는 신비한 존재 미스틱.";
			break;
	}
}

HRESULT CCreate_Class_ToolTip::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create(); 
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.Buffer Component
	m_pBufferCom = (Engine::CRect_Texture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_RcTex");
	pComponent = (Engine::CComponent*)m_pBufferCom;
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));

	// For.Texture Component 
	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Class_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom;
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));

	// For.Renderer Component
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	if(NULL == pComponent)
		return E_FAIL;
	pComponent->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));

	pComponent = m_pShaderCom = (Engine::CShader*)Engine::Clone_Prototype(L"Shader_UI");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	return S_OK;
}

CCreate_Class_ToolTip* CCreate_Class_ToolTip::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CCreate_Class_ToolTip*		pInstance = new CCreate_Class_ToolTip(pGraphicDev, wstrName, eState, bIsActive);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CCreate_Class_ToolTip::Free(void)
{
	CUI::Free();
}
