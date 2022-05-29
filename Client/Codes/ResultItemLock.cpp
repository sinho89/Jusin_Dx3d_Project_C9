#include "stdafx.h"
#include "ResultItemLock.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "Fade_Manager.h"
#include "Sound.h"

CResultItemLock::CResultItemLock(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
	:CUI(pGraphicDev, wstrName, eState, bIsActive)
	, m_pTransCom(NULL)
	, m_pBufferCom(NULL)
	, m_pNCTextureCom(NULL)
	, m_pCTextureCom(NULL)
	, m_pRendererCom(NULL)
	, m_pShaderCom(NULL)
{

}

CResultItemLock::~CResultItemLock(void)
{

}

HRESULT CResultItemLock::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fX = *m_pParent->GetfX();
	m_fY = *m_pParent->GetfY();

	m_fSizeX = 50.f;
	m_fSizeY = 45.f;

	m_fAlpha = 0.f;
	m_fAngle = 0.f;

	return S_OK;
}

void CResultItemLock::PtInRect_Collision(const _float& fTimeDelta)
{
	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	m_bCollision = m_pCalCom->Collision_ByMouse(WINSIZEX
		, WINSIZEY
		, m_fX
		, m_fY
		, m_fSizeX
		, m_fSizeY
		, ptMouse);

	if (m_bCollision)
	{
		if (GetKeyState(VK_LBUTTON) & 0x8000)
		{
			if (m_bClick)
				return;
			m_bClick = true;
			m_bPick = true;
			m_pParent->SetPick(true);
			m_pSound->MyPlaySound(L"BoxPick");
			list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"UI_Result_Item")->begin();
			list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"UI_Result_Item")->end();

			for (; iter != iter_end; ++iter)
				((CUI*)(*iter))->SetFade(true);

		}
		else
			m_bClick = false;

		m_pParent->SetSize(170.f, 160.f);
	}
	else
		m_pParent->SetSize(150.f, 140.f);


}

HRESULT CResultItemLock::Start_ForScene(void)
{
	return S_OK;
}

void CResultItemLock::Update_Object(const _float& fTimeDelta)
{
	if (CFade_Manager::GetInstance()->GetResultFading() == false)
		return;

	if (!m_bFade)
		m_fAlpha = m_pParent->GetAlpha();

	else if (m_bFade)
	{
		if (!m_bPick)
		{
			m_fAlpha -= 0.05f;
			if (m_fAlpha <= 0.f)
				m_fAlpha = 0.f;
		}
		else
			m_fAlpha = m_pParent->GetAlpha();
	}

	if (!m_bPick && !m_bFade)
		PtInRect_Collision(fTimeDelta);
	else if (m_bPick)
	{
		m_bCollision = false;
		m_pParent->SetSize(170.f, 160.f);
	}

	CUI::Update_Object(fTimeDelta);
	m_pRendererCom->Add_WindowUI(30002, this);

	D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
}

void CResultItemLock::Render_Object(void)
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

	if (m_bCollision)
		m_pCTextureCom->Set_Texture(pEffect, "g_BaseTexture");
	else
		m_pNCTextureCom->Set_Texture(pEffect, "g_BaseTexture");

	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);
}



HRESULT CResultItemLock::Add_Component(void)
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

	m_pCTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"SelectItemLock_C");
	pComponent = (Engine::CComponent*)m_pCTextureCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));

	m_pNCTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"SelectItemLock_NC");
	pComponent = (Engine::CComponent*)m_pNCTextureCom;
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
	m_pSound->Set_Sound(L"BoxPick", L"BoxPick.wav");

	return S_OK;
}

CResultItemLock* CResultItemLock::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CResultItemLock*		pInstance = new CResultItemLock(pGraphicDev, wstrName, eState, bIsActive);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

CResultItemLock* CResultItemLock::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, CUI* pParent, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CResultItemLock*		pInstance = new CResultItemLock(pGraphicDev, wstrName, eState, bIsActive);

	pInstance->SetParent(pParent);
	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CResultItemLock::Free(void)
{
	CUI::Free();
}
