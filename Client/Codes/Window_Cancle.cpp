#include "stdafx.h"
#include "Window_Cancle.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "Cursor.h"
#include "Sound.h"

CWindow_Cancle::CWindow_Cancle(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
:CUI(pGraphicDev, wstrName, eState, bIsActive)
,m_pTransCom(NULL)
,m_pBufferCom(NULL)
,m_pNCTextureCom(NULL)
,m_pCTextureCom(NULL)
,m_pClickTextureCom(NULL)
,m_pRendererCom(NULL)
,m_pShaderCom(NULL)
{

}

CWindow_Cancle::~CWindow_Cancle(void)
{

}

HRESULT CWindow_Cancle::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	_vec4 vInfo = m_pParentWindow->GetInfo();

	m_fX = vInfo.x + 155.f;
	m_fY = vInfo.y - 287.5f;

	m_fSizeX = 20.f;
	m_fSizeY = 20.f;

	m_fAlpha = 1.f;

	m_bIsStatic = true;
	return S_OK;
}

HRESULT CWindow_Cancle::Start_ForScene(void)
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

void CWindow_Cancle::Update_Object(const _float& fTimeDelta)
{
	CUI::Update_Object(fTimeDelta);

	m_wSortNum = m_pParentWindow->GetSortNum();

	if(m_pParentWindow->GetRender())
	{		
		PtInRect_Collision(fTimeDelta);
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

void CWindow_Cancle::Render_Object(void)
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

	if(m_bPick)
	{
		m_pClickTextureCom->Set_Texture(pEffect, "g_BaseTexture");
	}
	else
	{
		if(m_bBarCollision)
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


HRESULT CWindow_Cancle::Add_Component(void)
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
	m_pNCTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Cancle_NC");
	pComponent = (Engine::CComponent*)m_pNCTextureCom;
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_NC", pComponent));

	m_pCTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Cancle_C");
	pComponent = (Engine::CComponent*)m_pCTextureCom;
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_C", pComponent));

	m_pClickTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Cancle_Click");
	pComponent = (Engine::CComponent*)m_pClickTextureCom;
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_Click", pComponent));

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

	// For.Calculater Component
	pComponent = m_pCalCom = Engine::CUICalculater::Create(); 
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_UICal", pComponent));

	// Sound
	pComponent = m_pSound = CSound::Create(NULL);
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Sound", pComponent));
	m_pSound->Set_Sound(L"ButtonPick", L"Pick.wav");

	return S_OK;
}
void CWindow_Cancle::PtInRect_Collision(const _float& fTimeDelta)
{
	_vec4 vInfo = m_pParentWindow->GetInfo();

	if(m_pParentWindow->GetWinType() == WINDOW_SKILL)
	{
		m_fX = vInfo.x + 195.f;
		m_fY = vInfo.y - 260.f;

	}
	else if (m_pParentWindow->GetWinType() == WINDOW_INFO || m_pParentWindow->GetWinType() == WINDOW_INVEN)
	{
		m_fX = vInfo.x + 155.f;
		m_fY = vInfo.y - 287.5f;

	}

	else if (m_pParentWindow->GetWinType() == WINDOW_PARTITION)
	{
		m_fX = vInfo.x + 130.f;
		m_fY = vInfo.y - 85.f;
	}

	else if (m_pParentWindow->GetWinType() == WINDOW_ESC)
	{
		if (m_pParentWindow->GetAlpha() != 1.f)
			m_fAlpha = 0.f;
		else
			m_fAlpha = 0.f;
		m_fX = vInfo.x + 125.f;
		m_fY = vInfo.y - 125.f;
	}
	else if (m_pParentWindow->GetWinType() == WINDOW_SYSTEM)
	{
		m_fX = vInfo.x + 200.f;
		m_fY = vInfo.y - 200.f;
	}

	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	m_bBarCollision = m_pCalCom->Collision_ByMouse(WINSIZEX
		, WINSIZEY
		, m_fX
		, m_fY
		, m_fSizeX
		, m_fSizeY
		, ptMouse);

	
	if(m_bBarCollision)
	{
		m_pCursor = ((CUI*)Engine::Find(L"Cursor"));
		if(((CCursor*)m_pCursor)->GetItem() != NULL)
		{	
			::Safe_Release(m_pCursor);
			return;
		}
		::Safe_Release(m_pCursor);

		/*if (m_pParentWindow->GetWinType() == WINDOW_INVEN)
		{
			CUI*	pPartitionBase = ((CUI*)Engine::Find(L"Partition_Base"));
			if (pPartitionBase->GetRender())
			{
				::Safe_Release(pPartitionBase);
				return;
			}
			::Safe_Release(pPartitionBase);
		}*/
		

		if(Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB) & 0x80)
		{
			if(m_bPick)
				return;
			m_bPick = true;
		}
		else
		{
			if(m_bPick)
			{
				m_pSound->MyPlaySound(L"ButtonPick");
				if (m_pParentWindow->GetWinType() == WINDOW_INVEN)
				{
					CUI*	pPartitionBase = ((CUI*)Engine::Find(L"Partition_Base"));
					m_pParentWindow->SetRender(false);
					pPartitionBase->SetRender(false);
					::Safe_Release(pPartitionBase);
				}
				else
					m_pParentWindow->SetRender(false);
			}
			m_bPick = false;
		}

	}
	else
		m_bPick = false;

}

void CWindow_Cancle::SetParent(CUI* pParentWindow)
{
	m_pParentWindow = pParentWindow;
}

CWindow_Cancle* CWindow_Cancle::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CWindow_Cancle*		pInstance = new CWindow_Cancle(pGraphicDev, wstrName, eState, bIsActive);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

CWindow_Cancle* CWindow_Cancle::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, CUI* pParentWindow, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CWindow_Cancle*		pInstance = new CWindow_Cancle(pGraphicDev, wstrName, eState, bIsActive);

	pInstance->SetParent(pParentWindow);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CWindow_Cancle::Free(void)
{
	//::Safe_Release(m_pParentWindow);
	CUI::Free();
}
