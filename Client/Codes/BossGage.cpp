#include "stdafx.h"
#include "BossGage.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "Player.h"
#include "Monster.h"
#include "Fade_Manager.h"

CBossGage::CBossGage(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
	:CUI(pGraphicDev, wstrName, eState, bIsActive)
	, m_pTransCom(NULL)
	, m_pBufferCom(NULL)
	, m_pTextureCom(NULL)
	, m_pRendererCom(NULL)
	, m_pShaderCom(NULL)
{

}

CBossGage::~CBossGage(void)
{

}

HRESULT CBossGage::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fX = (WINSIZEX >> 1);
	m_fY = 95.f;

	m_fSizeX = 580.f;
	m_fSizeY = 35.f;

	m_fGage = 1.f;
	m_fOriGage = 1.f;
	m_iIndex = 1;
	m_fAlpha = 0.f;
	m_fEffectSpeed = 0.02f;
	m_fWhiteAlpha = 0.f;
	m_fGageTime = 0.f;
	m_fAngle = 0.4f;
	m_bEffect = false;
	m_bRender = false;
	m_bWhiteAlpha = false;
	m_bOnceStart = false;
	m_bAlpha_Increase = false;


	if (Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
		m_fHPLevel = 5;
	else if (Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE)
		m_fHPLevel = 10;
	CUI::Start_ForScene();

	return S_OK;
}

HRESULT CBossGage::Start_ForScene(void)
{
	return S_OK;
}

void CBossGage::Update_Object(const _float& fTimeDelta)
{
	if (CFade_Manager::GetInstance()->GetFading() == true)
		return;
	m_fTermY += fTimeDelta;

	
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


	m_iIndex = m_fHPLevel - (((CMonster*)m_pBossTarget)->GetInfo()->iHp / (((CMonster*)m_pBossTarget)->GetInfo()->iMaxHp / m_fHPLevel));

	if (((CMonster*)m_pBossTarget)->GetInfo()->iHp == ((CMonster*)m_pBossTarget)->GetInfo()->iMaxHp)
		m_iIndex = 1;
	
	m_fGage = float(((CMonster*)m_pBossTarget)->GetInfo()->iHp - (((((CMonster*)m_pBossTarget)->GetInfo()->iMaxHp) / m_fHPLevel)) * (m_fHPLevel - m_iIndex)) / float(float(((CMonster*)m_pBossTarget)->GetInfo()->iMaxHp) / m_fHPLevel);
	

	if (((CMonster*)m_pBossTarget)->GetInfo()->iHp <= 0.f)
	{
		m_iIndex = 0;
		m_fGage = 0.f;
		m_eState = Engine::STATE_DESTROY;
	}

	if (m_fOriGage != m_fGage)
	{
		m_fGageTime += fTimeDelta;

		m_fOriGage -= (m_fGageTime * 0.01f);

		if ((m_fOriGage <= m_fGage))
		{
			m_fGageTime = 0.f;
			m_fOriGage = m_fGage;
		}
		if (m_fOriGage <= 0.f)
			m_fOriGage = 1.f;

		/*if ((m_fOriGage <= m_fGage))
		{
			if ((abs(m_fGage - m_fOriGage) > 0.5f))
				return;
			
			m_fOriGage = m_fGage;
		}*/

	}
	m_fEffectSpeed = float(m_iIndex) * 0.02f;

	if (!m_bEffect)
		m_fAngle -= m_fEffectSpeed;
	else if (m_bEffect)
		m_fAngle += m_fEffectSpeed;

	if (m_fAngle <= 0.f)
		m_bEffect = true;
	if (m_fAngle >= 0.6f)
		m_bEffect = false;


	if (!m_bWhiteAlpha)
		m_fWhiteAlpha += 0.1f;
	else if (m_bWhiteAlpha)
		m_fWhiteAlpha -= 0.1f;

	if (m_fWhiteAlpha >= 1.f)
		m_bWhiteAlpha = true;
	else if (m_fWhiteAlpha <= 0.f)
		m_bWhiteAlpha = false;

}

void CBossGage::Render_Object(void)
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
	pEffect->SetFloat("g_fGage", m_fGage);
	pEffect->SetFloat("g_fOriGage", m_fOriGage);
	pEffect->SetFloat("g_fGageEffect", m_fAngle);
	pEffect->SetFloat("g_fWhiteAlpha", m_fWhiteAlpha);
	pEffect->SetInt("g_iBossGage", m_iIndex);

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture");

	pEffect->Begin(&iPassCnt, 0);
	if (Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
		pEffect->BeginPass(8);
	else if (Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE)
		pEffect->BeginPass(12);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);

	_tchar		szInfo[128] = L"";
	wsprintf(szInfo, L"x %d", ((m_fHPLevel + 1) - m_iIndex));	// 원천 능력
	Engine::Render_Font(L"Font_Select_DunGeun", szInfo, &_vec2(m_fX - 225.f, m_fY - 5.f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));

}


HRESULT CBossGage::Add_Component(void)
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
	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"BossGage");
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


CBossGage* CBossGage::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CBossGage*		pInstance = new CBossGage(pGraphicDev, wstrName, eState, bIsActive);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

CBossGage* CBossGage::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, Engine::CGameObject* pTarget, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CBossGage*		pInstance = new CBossGage(pGraphicDev, wstrName, eState, bIsActive);

	pInstance->SetTarget(pTarget);
	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CBossGage::Free(void)
{
	CUI::Free();
}
