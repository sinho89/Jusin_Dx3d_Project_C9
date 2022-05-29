#include "stdafx.h"
#include "Boss_Base.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "Boss_Logo.h"
#include "Boss_Name.h"
#include "Boss_SubName.h"
#include "Sound.h"

CBoss_Base::CBoss_Base(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
	:CUI(pGraphicDev, wstrName, eState, bIsActive)
	, m_pTransCom(NULL)
	, m_pBufferCom(NULL)
	, m_pTextureCom(NULL)
	, m_pRendererCom(NULL)
	, m_pShaderCom(NULL)
{

}

CBoss_Base::~CBoss_Base(void)
{

}

HRESULT CBoss_Base::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fX = (WINSIZEX >> 1);
	m_fY = (WINSIZEY >> 1);

	m_fSizeX = 1600.f;
	m_fSizeY = 900.f;
	m_fAlpha = 0.f;
	m_fAlphaTime = 0.f;
	m_bAlpha_Increase = false;

	m_pSound->MyPlaySound(L"Fade");

	Engine::CGameObject* pGameObject = CBoss_Logo::Create(m_pGraphicDev, L"Boss_Base");
	Engine::AddObject(Engine::LAYER_UI, L"UI_BossApear", pGameObject);

	if (Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
	{
		m_fFadeTime = 4.f;
		pGameObject = CBossName::Create(m_pGraphicDev, L"Boss_Name", 0, FADE_HA);
		Engine::AddObject(Engine::LAYER_UI, L"UI_BossApear", pGameObject);
		pGameObject = CBossName::Create(m_pGraphicDev, L"Boss_Name", 0, FADE_NOO);
		Engine::AddObject(Engine::LAYER_UI, L"UI_BossApear", pGameObject);
		pGameObject = CBossName::Create(m_pGraphicDev, L"Boss_Name", 0, FADE_MAN);
		Engine::AddObject(Engine::LAYER_UI, L"UI_BossApear", pGameObject);
		
		pGameObject = CBossSubName::Create(m_pGraphicDev, L"Boss_Name", 0);
		Engine::AddObject(Engine::LAYER_UI, L"UI_BossApear", pGameObject);
	}
	else if (Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE)
	{
		m_fFadeTime = 6.f;
		pGameObject = CBossName::Create(m_pGraphicDev, L"Boss_Name", 0, FADE_BA);
		Engine::AddObject(Engine::LAYER_UI, L"UI_BossApear", pGameObject);
		pGameObject = CBossName::Create(m_pGraphicDev, L"Boss_Name", 0, FADE_R);
		Engine::AddObject(Engine::LAYER_UI, L"UI_BossApear", pGameObject);
		pGameObject = CBossName::Create(m_pGraphicDev, L"Boss_Name", 0, FADE_DI);
		Engine::AddObject(Engine::LAYER_UI, L"UI_BossApear", pGameObject);
		pGameObject = CBossName::Create(m_pGraphicDev, L"Boss_Name", 0, FADE_EL);
		Engine::AddObject(Engine::LAYER_UI, L"UI_BossApear", pGameObject);


		pGameObject = CBossSubName::Create(m_pGraphicDev, L"Boss_Name", 1);
		Engine::AddObject(Engine::LAYER_UI, L"UI_BossApear", pGameObject);
	}
	

	return S_OK;
}

HRESULT CBoss_Base::Start_ForScene(void)
{
	return S_OK;
}

void CBoss_Base::Update_Object(const _float& fTimeDelta)
{
	m_fAlphaTime += fTimeDelta;

	if (!m_bAlpha_Increase)
	{
		m_fAlpha += 0.1f;
				
		if (m_fAlpha >= 1.f)
			m_fAlpha = 1.f;

		if (m_fAlphaTime >= m_fFadeTime)
		{
			m_bAlpha_Increase = true;
			m_fAlpha = 1.f;
		}
	}
	else
	{
		if (Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
			m_eState = Engine::STATE_DESTROY;
		else if (Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE)
		{
			m_fAlpha -= 0.05f;

			if(m_fAlpha <= 0.f)
				m_eState = Engine::STATE_DESTROY;
		}
	}


	CUI::Update_Object(fTimeDelta);

	m_pRendererCom->Add_WindowUI(21000, this);

	D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);

}

void CBoss_Base::Render_Object(void)
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


HRESULT CBoss_Base::Add_Component(void)
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
	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Action_Back");
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

	pComponent = m_pSound = CSound::Create(NULL);
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Sound", pComponent));
	m_pSound->Set_Sound(L"Fade", L"Fade.wav");

	return S_OK;
}


CBoss_Base* CBoss_Base::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CBoss_Base*		pInstance = new CBoss_Base(pGraphicDev, wstrName, eState, bIsActive);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}


void CBoss_Base::Free(void)
{
	CUI::Free();
}
