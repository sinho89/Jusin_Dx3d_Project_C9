#include "stdafx.h"
#include "Slide_BackSound.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "Partition_Base.h"
#include "UI_Item.h"
#include "..\Engine\Utility\Codes\Scene.h"
#include "UI_Potion.h"
#include "Cursor.h"

CSlide_BackSound::CSlide_BackSound(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
	:CUI(pGraphicDev, wstrName, eState, bIsActive)
	, m_pTransCom(NULL)
	, m_pBufferCom(NULL)
	, m_pNCTextureCom(NULL)
	, m_pCTextureCom(NULL)
	, m_pClickTextureCom(NULL)
	, m_pRendererCom(NULL)
	, m_pShaderCom(NULL)
	, m_pScene(NULL)
	, m_fBackSoundData(0.f)
	, m_fOriX(0.f)
	, m_bDragOff(false)
	, m_bDragOn(false)
{

}

CSlide_BackSound::~CSlide_BackSound(void)
{

}

HRESULT CSlide_BackSound::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;
	m_fX = m_pParent->GetInfo().x + 180.f;
	m_fY = m_pParent->GetInfo().y + 110.f;

	m_fOriX = m_fX;

	m_fSizeX = 25.f;
	m_fSizeY = 25.f;

	m_fAlpha = 1.f;

	m_bIsStatic = true;
	return S_OK;
}

HRESULT CSlide_BackSound::Start_ForScene(void)
{
	if (Engine::GetSceneID() != Engine::SCENE_TOWN && Engine::GetSceneID() != Engine::SCENE_STAGE_ICE && Engine::GetSceneID() != Engine::SCENE_STAGE_FIRE)
	{
		m_bIsActive = false;
		return S_OK;
	}
	if (Engine::GetSceneID() == Engine::SCENE_TOWN || Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE
		|| Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
		m_bIsActive = true;

	return S_OK;
}

void CSlide_BackSound::Update_Object(const _float& fTimeDelta)
{
	m_bRender = m_pParent->GetRender();
	g_fBGMValue = ((-m_fBackSoundData / 60.f) + 1.f) * 0.5f;

	if (m_bRender)
	{
		if (m_bCollision && m_bKeyBoard)
		{
			Drag();
		}
		else
		{
			m_fX = m_pParent->GetInfo().x - 160.f - m_fBackSoundData;
			m_fY = m_pParent->GetInfo().y + 157.5f;
		}

		CUI::Update_Object(fTimeDelta);
		PtInRect_Collision(fTimeDelta);
		Drag();
		m_wSortNum = m_pParent->GetSortNum();
		m_pRendererCom->Add_WindowUI(m_wSortNum, this);
		D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
	}
	else
	{
		m_bBarCollision = false;
		m_bCollision = false;
		m_bClick = false;
		m_bWindowProcess = false;
		m_bSelect = false;
		m_bMove = false;
	}

}

void CSlide_BackSound::Render_Object(void)
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

	if (m_bKeyBoard)
		m_pClickTextureCom->Set_Texture(pEffect, "g_BaseTexture");
	else
	{
		if (m_bCollision)
			m_pCTextureCom->Set_Texture(pEffect, "g_BaseTexture");

		else
			m_pNCTextureCom->Set_Texture(pEffect, "g_BaseTexture");
	}

	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(2);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);

}


HRESULT CSlide_BackSound::Add_Component(void)
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
	m_pNCTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Slide_Button_NC");
	pComponent = (Engine::CComponent*)m_pNCTextureCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_NC", pComponent));

	m_pCTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Slide_Button_C");
	pComponent = (Engine::CComponent*)m_pCTextureCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_C", pComponent));

	m_pClickTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Slide_Button_Click");
	pComponent = (Engine::CComponent*)m_pClickTextureCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_Click", pComponent));

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
void CSlide_BackSound::PtInRect_Collision(const _float& fTimeDelta)
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
		if (Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB) & 0x80)
		{
			if (m_bKeyBoard)
				return;
			m_bKeyBoard = true;
			m_bDragOn = true;
		}
		else
		{
			if (m_bKeyBoard)
				m_bDragOff = true;
			m_bKeyBoard = false;
		}
	}
	else
	{
		if (m_bKeyBoard)
			m_bDragOff = true;
		m_bKeyBoard = false;
	}

	if (m_bDragOff)
	{
		m_bDragOff = false;
		m_fBackSoundData = m_fOriX - m_fX;

		
	}

}

void CSlide_BackSound::SetParent(CUI* pParent)
{
	m_pParent = pParent;
}

void CSlide_BackSound::Drag(void)
{
	if (m_bKeyBoard && m_bCollision)
	{
		POINT		ptMouse;
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);
		if (m_bDragOn)
		{
			m_fOriX = m_fX + m_fBackSoundData;
			m_bDragOn = false;
		}

		if (float(ptMouse.x) < m_pParent->GetInfo().x - 220
			|| float(ptMouse.x) > m_pParent->GetInfo().x - 100)
		{
			m_bDragOff = true;
			m_bKeyBoard = false;
			return;
		}
		m_fX = float(ptMouse.x);
	}
}

CSlide_BackSound* CSlide_BackSound::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CSlide_BackSound*		pInstance = new CSlide_BackSound(pGraphicDev, wstrName, eState, bIsActive);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

CSlide_BackSound* CSlide_BackSound::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, CUI* pParent, Engine::CScene* pScene, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CSlide_BackSound*		pInstance = new CSlide_BackSound(pGraphicDev, wstrName, eState, bIsActive);

	pInstance->SetParent(pParent);
	pInstance->SetScene(pScene);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CSlide_BackSound::Free(void)
{
	CUI::Free();
}
