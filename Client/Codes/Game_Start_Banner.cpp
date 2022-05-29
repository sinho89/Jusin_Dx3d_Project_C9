#include "stdafx.h"
#include "Game_Start_Banner.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "Select_Char_Button.h"
#include "Sound.h"

CGame_Start_Banner::CGame_Start_Banner(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
:CUI(pGraphicDev, wstrName, eState, bIsActive)
,m_pTransCom(NULL)
,m_pBufferCom(NULL)
,m_pNCTextureCom(NULL)
,m_pCTextureCom(NULL)
,m_pRendererCom(NULL)
,m_pShaderCom(NULL)
{

}

CGame_Start_Banner::~CGame_Start_Banner(void)
{

}

HRESULT CGame_Start_Banner::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	m_fX = (WINSIZEX >> 1) + 670;
	m_fY = (WINSIZEY >> 1) + 410;

	m_fSizeX = 250.0f;
	m_fSizeY = 60.0f;

	return S_OK;
}

HRESULT CGame_Start_Banner::Start_ForScene(void)
{
	return S_OK;
}

void CGame_Start_Banner::Update_Object(const _float& fTimeDelta)
{
	CUI::Update_Object(fTimeDelta);
	Alpha_Calculation(fTimeDelta);
	PtInRect_Collision();
	CunvertFontPos();

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);

	D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
}

void CGame_Start_Banner::Render_Object(void)
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

	if(!m_bCollision)
	{
		m_pNCTextureCom->Set_Texture(pEffect, "g_BaseTexture");
	}
	else
	{
		m_pCTextureCom->Set_Texture(pEffect, "g_BaseTexture");
	}
	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();	

	pEffect->EndPass();
	pEffect->End();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView); 
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj); 

	if(!m_bCollision)
		Engine::Render_Font(L"Font_Select_Slot", L"게임 시작", &_vec2(m_fX - 40.f, m_fY - 15.f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));
	else
		Engine::Render_Font(L"Font_Select_Slot", L"게임 시작", &_vec2(m_fX - 40.f, m_fY - 15.f), D3DXCOLOR(0.f, 1.f, 0.f, m_fAlpha));
	


}

void CGame_Start_Banner::PtInRect_Collision()
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

	if(m_bCollision)
	{
		if(GetKeyState(VK_LBUTTON) & 0x8000)
		{
			if(m_bClick)
				return;
			m_bClick = true;
			m_pSound->MyPlaySound(L"ButtonPick"); 

			list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"UI_Char_Button")->begin();
			list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"UI_Char_Button")->end();

			for(; iter != iter_end; ++iter)
			{
				if(((CUI*)(*iter))->GetSelect())
				{
					if(((CSelect_Char_Button*)((CUI*)(*iter)))->GetSlotActive())
					{
						m_iIndex = ((CUI*)(*iter))->GetIndex();
						m_bSkipScene = true;
						return;
					}
				}
			}
		}
		else
			m_bClick = false;
	}
}

void CGame_Start_Banner::CunvertFontPos(void)
{
	//m_vFontPos = m_pCalCom->CunvertFontPos(m_pTransCom->m_vInfo[Engine::INFO_POSITION], WINSIZEX, WINSIZEY, 0, 0);
}

void CGame_Start_Banner::Alpha_Calculation(const _float& fTimeDelta)
{
	m_fAlphaTime += fTimeDelta;

	if(m_fAlpha <= 0.f)
		m_bAlpha_Increase = true;
	if(m_fAlpha >= 1.f)
		m_bAlpha_Increase = false;

	if(m_fAlphaTime >= 0.05f)
	{
		m_fAlphaTime = 0.f;

		if(m_bAlpha_Increase)
			m_fAlpha += 0.01f;

	}
}

void CGame_Start_Banner::Set_Info(_float fY, _uint iIndex)
{
	m_fY = (WINSIZEY >> 1) - fY;
	m_iIndex = iIndex;
}

HRESULT CGame_Start_Banner::Add_Component(void)
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
	m_pNCTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Texture_Game_Start_NC");
	pComponent = (Engine::CComponent*)m_pNCTextureCom;
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_NC", pComponent));

	m_pCTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Texture_Game_Start_C");
	pComponent = (Engine::CComponent*)m_pCTextureCom;
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_C", pComponent));

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
	pComponent = m_pSound = CSound::Create(m_pTransCom);
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Sound", pComponent));
	m_pSound->Set_Sound(L"ButtonPick", L"Pick.wav");
	m_pSound->Set_Sound(L"SceneSkip", L"ActionStart.wav");

	return S_OK;
}

CGame_Start_Banner* CGame_Start_Banner::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CGame_Start_Banner*		pInstance = new CGame_Start_Banner(pGraphicDev, wstrName, eState, bIsActive);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

CGame_Start_Banner* CGame_Start_Banner::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, float fY, _uint iIndex, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CGame_Start_Banner*		pInstance = new CGame_Start_Banner(pGraphicDev, wstrName, eState, bIsActive);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	pInstance->Set_Info(fY, iIndex);

	return pInstance;
}

void CGame_Start_Banner::Free(void)
{
	CUI::Free();
}
