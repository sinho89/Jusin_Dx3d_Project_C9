#include "stdafx.h"
#include "BossOutBar.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "Player.h"
#include "Monster.h"
#include "Fade_Manager.h"

CBossOutBar::CBossOutBar(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
	:CUI(pGraphicDev, wstrName, eState, bIsActive)
	, m_pTransCom(NULL)
	, m_pBufferCom(NULL)
	, m_pTextureCom(NULL)
	, m_pRendererCom(NULL)
	, m_pShaderCom(NULL)
{

}

CBossOutBar::~CBossOutBar(void)
{

}

HRESULT CBossOutBar::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fX = (WINSIZEX >> 1);
	m_fY = 95.f;
	m_fTermY = 0.f;

	m_fSizeX = 600.f;
	m_fSizeY = 50.f;
	m_iHpLineCnt = 0;
	m_fAlpha = 0.f;
	m_bRender = false;
	m_bOnceStart = false;
	m_bAlpha_Increase = false;
	CUI::Start_ForScene();

	return S_OK;
}

HRESULT CBossOutBar::Start_ForScene(void)
{
	return S_OK;
}

void CBossOutBar::Update_Object(const _float& fTimeDelta)
{
	if (CFade_Manager::GetInstance()->GetFading() == true)
		return;
	if (m_bRender)
	{
		CUI::Update_Object(fTimeDelta);

		m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);

		D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
	}
	else
	{
		_vec3 vPlayerPos, vBossPos;
		((Engine::CTransform*)m_pPlayer->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC))->Get_Infomation(Engine::INFO_POSITION, &vPlayerPos);
		((Engine::CTransform*)m_pBossTarget->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC))->Get_Infomation(Engine::INFO_POSITION, &vBossPos);

		float fLength = D3DXVec3Length(&(vPlayerPos - vBossPos));

		if (Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
		{
			if (fLength <= 150.f)
				m_bAlpha_Increase = true;
		}
		else if (Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE)
		{
			if (fLength <= 244.f)
				m_bAlpha_Increase = true;
		}

		if (m_bAlpha_Increase)
			m_fAlpha += 0.1f;

		if (m_fAlpha >= 1.f)
			m_bRender = true;
	}

	if (m_bFade)
	{
		m_fTermY += fTimeDelta;
		m_fAlpha -= 0.05f;
		m_fY -= m_fTermY;

		if (m_fAlpha <= 0.f)
			m_eState = Engine::STATE_DESTROY;
	}
	if (((CMonster*)m_pBossTarget)->GetInfo()->iHp <= 0)
		m_bFade = true;
}

void CBossOutBar::Render_Object(void)
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


HRESULT CBossOutBar::Add_Component(void)
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
	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"BossBar2");
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


CBossOutBar* CBossOutBar::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CBossOutBar*		pInstance = new CBossOutBar(pGraphicDev, wstrName, eState, bIsActive);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

CBossOutBar* CBossOutBar::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, Engine::CGameObject* pTarget, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CBossOutBar*		pInstance = new CBossOutBar(pGraphicDev, wstrName, eState, bIsActive);

	pInstance->SetTarget(pTarget);
	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CBossOutBar::Free(void)
{
	CUI::Free();
}
