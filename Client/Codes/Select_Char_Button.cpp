#include "stdafx.h"
#include "Select_Char_Button.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "Select_Char_Model.h"
#include "Model_Floor.h"
#include "Sound.h"

CSelect_Char_Button::CSelect_Char_Button(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
:CUI(pGraphicDev, wstrName, eState, bIsActive)
,m_pTransCom(NULL)
,m_pBufferCom(NULL)
,m_pNCTextureCom(NULL)
,m_pCTextureCom(NULL)
,m_pClickTextureCom(NULL)
,m_pRendererCom(NULL)
,m_pShaderCom(NULL)
,m_bSlotActive(false)
,m_fClickTime(false)
{

}

CSelect_Char_Button::~CSelect_Char_Button(void)
{

}

HRESULT CSelect_Char_Button::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	m_fX = (WINSIZEX >> 1) + 655;
	//m_fY = (WINSIZEY >> 1) - 390;

	m_fSizeX = 220.0f;
	m_fSizeY = 100.0f;

	return S_OK;
}

HRESULT CSelect_Char_Button::Start_ForScene(void)
{
	return S_OK;
}

void CSelect_Char_Button::Update_Object(const _float& fTimeDelta)
{
	CUI::Update_Object(fTimeDelta);
	Alpha_Calculation(fTimeDelta);
	PtInRect_Collision(fTimeDelta);
	CunvertFontPos();

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);

	D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
}

void CSelect_Char_Button::Render_Object(void)
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

	if(m_bSelect)
	{
		m_pClickTextureCom->Set_Texture(pEffect, "g_BaseTexture");
	}
	else
	{
		if(!m_bCollision)
		{
			m_pNCTextureCom->Set_Texture(pEffect, "g_BaseTexture");
		}
		else
		{
			m_pCTextureCom->Set_Texture(pEffect, "g_BaseTexture");
		}
	}
	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();	

	pEffect->EndPass();
	pEffect->End();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView); 
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj); 

	if(!m_bSlotActive)
	{
		if(!m_bCollision)
			Engine::Render_Font(L"Font_Select_Slot", L"캐릭터 생성", &_vec2(m_fX - 55.f, m_fY - 10.f), D3DXCOLOR(0.4f, 0.4f, 0.4f, m_fAlpha));
		if(m_bCollision || m_bSelect)
			Engine::Render_Font(L"Font_Select_Slot", L"캐릭터 생성", &_vec2(m_fX - 55.f, m_fY - 10.f), D3DXCOLOR(0.8f, 0.8f, 0.8f, m_fAlpha));
	}

	if(m_bSlotActive)
	{
		_tchar		szInfo[128] = L"";
		wsprintf(szInfo, L"레벨 : %d", m_tPlayerInfo.m_iLevel);
		Engine::Render_Font(L"Font_Select_Level", szInfo, &_vec2(m_fX - 70.f, m_fY - 32.5f), D3DXCOLOR(1.f, 1.f, 0.f, m_fAlpha));
		Engine::Render_Font(L"Font_Select_Char", L"직업 : ", &_vec2(m_fX - 70.f, m_fY), D3DXCOLOR(0.f, 1.f, 0.f, m_fAlpha));
		Engine::Render_Font(L"Font_Select_Char", m_tPlayerInfo.m_szClassName , &_vec2(m_fX - 20.f, m_fY), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));
		Engine::Render_Font(L"Font_Select_Char", L"닉네임 : ", &_vec2(m_fX - 70.f, m_fY + 20.f), D3DXCOLOR(0.f, 1.f, 0.f, m_fAlpha));
		Engine::Render_Font(L"Font_Select_Char", m_tPlayerInfo.m_szID, &_vec2(m_fX - 10.f, m_fY + 20.f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));
	}
}

void CSelect_Char_Button::PtInRect_Collision(const _float& fTimeDelta)
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

			if(m_bDoubleClick && !m_bSlotActive)
				m_bSkipScene = true;

			list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"UI_Char_Button")->begin();
			list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"UI_Char_Button")->end();

			for(; iter != iter_end; ++iter)
				((CUI*)(*iter))->SetSelect(false);

			m_bSelect = true;
			m_bDoubleClick = true;

			if(m_bSlotActive)
			{
				CUI* pUi = ((CUI*)Engine::Find(L"SelectBackGround"));
				pUi->SetIndex(m_tPlayerInfo.m_eClassID);
				pUi->SetAlpha(0.f);
				::Safe_Release(pUi);

				pUi = ((CUI*)Engine::Find(L"SelectModel"));
				pUi->SetIndex(m_tPlayerInfo.m_eClassID);
				((CSelect_Char_Model*)pUi)->SetMotionStart();
				::Safe_Release(pUi);

				pUi = ((CUI*)Engine::Find(L"ModelFloor"));
				pUi->SetIndex(m_tPlayerInfo.m_eClassID);
				((CModel_Floor*)pUi)->SetInit();
				::Safe_Release(pUi);
			}
			else
			{
				CUI* pUi = ((CUI*)Engine::Find(L"SelectBackGround"));
				pUi->SetIndex(-1);
				pUi->SetAlpha(0.f);
				::Safe_Release(pUi);

				pUi = ((CUI*)Engine::Find(L"SelectModel"));
				pUi->SetIndex(-1);
				::Safe_Release(pUi);

				pUi = ((CUI*)Engine::Find(L"ModelFloor"));
				pUi->SetIndex(-1);
				::Safe_Release(pUi);
			}
		}
		else
			m_bClick = false;
	}

	if(m_bDoubleClick)
	{
		m_fClickTime += fTimeDelta;

		if(m_fClickTime >= 0.1f)
		{
			m_bDoubleClick = false;
			m_fClickTime = 0.f;
		}
	}
}

void CSelect_Char_Button::CunvertFontPos(void)
{
	//m_vFontPos = m_pCalCom->CunvertFontPos(m_pTransCom->m_vInfo[Engine::INFO_POSITION], WINSIZEX, WINSIZEY, 0, 0);
}

_bool CSelect_Char_Button::GetSlotActive()
{
	return m_bSlotActive;
}

void CSelect_Char_Button::Alpha_Calculation(const _float& fTimeDelta)
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

void CSelect_Char_Button::Set_Info(_float fY, _uint iIndex)
{
	m_fY = (WINSIZEY >> 1) - fY;
	m_iIndex = iIndex;

	/*if(m_iIndex == 1)
		m_bSelect = true;*/
}

_bool CSelect_Char_Button::Load_Player_Info()
{
	_tchar		szPath[128] = L"";

	wsprintf(szPath, L"../Bin/Data/PlayerInfo/Player%d.dat", m_iIndex);

	DWORD	dwByte = 0;

	HANDLE hFile = CreateFile(szPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	int iIndex = 0;

	while(true)
	{
		PLAYER_INFO* pPlayerInfo = new PLAYER_INFO;

		if(!ReadFile(hFile, pPlayerInfo, sizeof(PLAYER_INFO), &dwByte, NULL))
		{
			delete pPlayerInfo;
			pPlayerInfo = NULL;

			CloseHandle(hFile);
			return false;
		}

		if(!iIndex)
		{
			memcpy(&m_tPlayerInfo, pPlayerInfo, sizeof(PLAYER_INFO));

			delete pPlayerInfo;
			pPlayerInfo = NULL;
		}
		if(dwByte == 0)
		{
			delete pPlayerInfo;
			pPlayerInfo = NULL;
			break;
		}

		++iIndex;
	}

	CloseHandle(hFile);
	return true;
}

PLAYER_INFO* CSelect_Char_Button::GetPlayerInfo()
{
	return &m_tPlayerInfo;
}

void CSelect_Char_Button::SetSlotActive(_bool bSlotActive)
{
	m_bSlotActive = bSlotActive;
}

HRESULT CSelect_Char_Button::Add_Component(void)
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
	m_pNCTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Texture_Select_Char_Button_NC");
	pComponent = (Engine::CComponent*)m_pNCTextureCom;
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_NC", pComponent));

	m_pCTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Texture_Select_Char_Button_C");
	pComponent = (Engine::CComponent*)m_pCTextureCom;
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_C", pComponent));

	m_pClickTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Texture_Select_Char_Button_Click");
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
	pComponent = m_pSound = CSound::Create(m_pTransCom);
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Sound", pComponent));
	m_pSound->Set_Sound(L"ButtonPick", L"Pick.wav");

	return S_OK;
}

CSelect_Char_Button* CSelect_Char_Button::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CSelect_Char_Button*		pInstance = new CSelect_Char_Button(pGraphicDev, wstrName, eState, bIsActive);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

CSelect_Char_Button* CSelect_Char_Button::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, float fY, _uint iIndex, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CSelect_Char_Button*		pInstance = new CSelect_Char_Button(pGraphicDev, wstrName, eState, bIsActive);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}
	pInstance->Set_Info(fY, iIndex);
	pInstance->SetSlotActive(pInstance->Load_Player_Info());
	return pInstance;
}

void CSelect_Char_Button::Free(void)
{
	CUI::Free();
}
