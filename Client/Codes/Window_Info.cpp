#include "stdafx.h"
#include "Window_Info.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "Player.h"

CWindow_Info::CWindow_Info(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
:CUI(pGraphicDev, wstrName, eState, bIsActive)
,m_pTransCom(NULL)
,m_pBufferCom(NULL)
,m_pTextureCom(NULL)
,m_pRendererCom(NULL)
,m_pShaderCom(NULL)
{

}

CWindow_Info::~CWindow_Info(void)
{
}

HRESULT CWindow_Info::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	memset(&m_tOriInfo, NULL, sizeof(PLAYER_INFO));
	m_fX = (WINSIZEX >> 1) - 350.f;
	m_fY = (WINSIZEY >> 1) - 50.f;

	m_fSizeX = 350.f;
	m_fSizeY = 600.f;

	m_fAlpha = 1.f;
	m_bRender = false;
	m_eWinType = WINDOW_INFO;

	m_bIsStatic = true;
	return S_OK;
}


HRESULT CWindow_Info::Start_ForScene(void)
{
	if (Engine::GetSceneID() != Engine::SCENE_TOWN && Engine::GetSceneID() != Engine::SCENE_STAGE_ICE && Engine::GetSceneID() != Engine::SCENE_STAGE_FIRE)
	{
		m_bIsActive = false;
		return S_OK;
	}
	if (Engine::GetSceneID() == Engine::SCENE_TOWN || Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE
		|| Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
		m_bIsActive = true;

	CUI::Start_ForScene();
	memcpy(&m_tOriInfo, m_pPlayer->GetPlayerInfo(), sizeof(PLAYER_INFO));
	return S_OK;
}

void CWindow_Info::Update_Object(const _float& fTimeDelta)
{
	if(m_bRender)
	{		
		CUI::Update_Object(fTimeDelta);
		Window_MoveToMouse();
		m_pRendererCom->Add_WindowUI(m_wSortNum, this);
		D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
	}
	else
	{
		m_fX = (WINSIZEX >> 1) - 350.f;
		m_fY = (WINSIZEY >> 1) - 50.f;

		m_bBarCollision = false;
		m_bCollision = false;
		m_bClick = false;
		m_bWindowProcess = false;
		m_bSelect = false;
		m_bMove = false;
		m_bFoward = false;
	}
}

void CWindow_Info::Render_Object(void)
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


	Engine::Render_Font(L"Font_Info_Data", m_pPlayer->GetPlayerInfo()->m_szID, &_vec2(m_fX - 35.f, m_fY - 260.f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));
	Engine::Render_Font(L"Font_Info_Data", m_pPlayer->GetPlayerInfo()->m_szClassName, &_vec2(m_fX - 35.f, m_fY - 240.f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));
	Engine::Render_Font(L"Font_Info_Data", L"쥬신게임아카데미 63기", &_vec2(m_fX - 35.f, m_fY - 220.f), D3DXCOLOR(0.f, 0.5f, 1.f, m_fAlpha));

	_tchar		szInfo[128] = L"";
	wsprintf(szInfo, L"%d", m_pPlayer->GetPlayerInfo()->m_iStr);	// 원천 능력
	if (m_tOriInfo.m_iStr < m_pPlayer->GetPlayerInfo()->m_iStr)
		m_vColor = _vec4(1.f, 0.f, 1.f, 1.f);
	else
		m_vColor = _vec4(1.f, 1.f, 1.f, 1.f);
	Engine::Render_Font(L"Font_Info_Data",  szInfo, &_vec2(m_fX - 42.5f - ((lstrlen(szInfo) - 2) * 5), m_fY - 87.5f), (D3DXCOLOR)m_vColor);
	wsprintf(szInfo, L"%d", m_pPlayer->GetPlayerInfo()->m_iWis);
	if (m_tOriInfo.m_iWis < m_pPlayer->GetPlayerInfo()->m_iWis)
		m_vColor = _vec4(1.f, 0.f, 1.f, 1.f);
	else
		m_vColor = _vec4(1.f, 1.f, 1.f, 1.f);
	Engine::Render_Font(L"Font_Info_Data",  szInfo, &_vec2(m_fX - 42.5f - ((lstrlen(szInfo) - 2) * 5), m_fY - 67.5f), (D3DXCOLOR)m_vColor);
	wsprintf(szInfo, L"%d", m_pPlayer->GetPlayerInfo()->m_iCon);
	if (m_tOriInfo.m_iCon < m_pPlayer->GetPlayerInfo()->m_iCon)
		m_vColor = _vec4(1.f, 0.f, 1.f, 1.f);
	else
		m_vColor = _vec4(1.f, 1.f, 1.f, 1.f);
	Engine::Render_Font(L"Font_Info_Data",  szInfo, &_vec2(m_fX + 117.5f - ((lstrlen(szInfo) - 2) * 5), m_fY - 87.5f), (D3DXCOLOR)m_vColor);
	wsprintf(szInfo, L"%d", m_pPlayer->GetPlayerInfo()->m_iInt);
	if (m_tOriInfo.m_iInt < m_pPlayer->GetPlayerInfo()->m_iInt)
		m_vColor = _vec4(1.f, 0.f, 1.f, 1.f);
	else
		m_vColor = _vec4(1.f, 1.f, 1.f, 1.f);
	Engine::Render_Font(L"Font_Info_Data",  szInfo, &_vec2(m_fX + 117.5f - ((lstrlen(szInfo) - 2) * 5), m_fY - 67.5f), (D3DXCOLOR)m_vColor);

	wsprintf(szInfo, L"%d - %d", m_pPlayer->GetPlayerInfo()->m_iRightMinAtt, m_pPlayer->GetPlayerInfo()->m_iRightMaxAtt); // 기본 능력
	if (m_tOriInfo.m_iRightMinAtt < m_pPlayer->GetPlayerInfo()->m_iRightMinAtt)
		m_vColor = _vec4(0.f, 1.f, 0.f, 1.f);
	else
		m_vColor = _vec4(1.f, 1.f, 1.f, 1.f);
	Engine::Render_Font(L"Font_Info_Data",  szInfo, &_vec2(m_fX + 30.f -((lstrlen(szInfo) - 4) * 2), m_fY - 22.5f), (D3DXCOLOR)m_vColor);
	wsprintf(szInfo, L"%d - %d", m_pPlayer->GetPlayerInfo()->m_iLeftMinAtt, m_pPlayer->GetPlayerInfo()->m_iLeftMaxAtt);
	if (m_tOriInfo.m_iLeftMinAtt < m_pPlayer->GetPlayerInfo()->m_iLeftMinAtt)
		m_vColor = _vec4(0.f, 1.f, 0.f, 1.f);
	else
		m_vColor = _vec4(1.f, 1.f, 1.f, 1.f);
	Engine::Render_Font(L"Font_Info_Data",  szInfo, &_vec2(m_fX + 30.f - ((lstrlen(szInfo) - 4) * 2), m_fY - 2.5f), (D3DXCOLOR)m_vColor);
	wsprintf(szInfo, L"%d - %d", m_pPlayer->GetPlayerInfo()->m_iTotalMinAtt, m_pPlayer->GetPlayerInfo()->m_iTotalMaxAtt);
	if (m_tOriInfo.m_iTotalMinAtt < m_pPlayer->GetPlayerInfo()->m_iTotalMinAtt)
		m_vColor = _vec4(0.f, 1.f, 0.f, 1.f);
	else
		m_vColor = _vec4(1.f, 1.f, 1.f, 1.f);
	Engine::Render_Font(L"Font_Info_Data",  szInfo, &_vec2(m_fX + 30.f - ((lstrlen(szInfo) - 4) * 2), m_fY + 17.5f), (D3DXCOLOR)m_vColor);
	wsprintf(szInfo, L"%d", m_pPlayer->GetPlayerInfo()->m_iPhysicalDefense);
	if (m_tOriInfo.m_iPhysicalDefense < m_pPlayer->GetPlayerInfo()->m_iPhysicalDefense)
		m_vColor = _vec4(0.f, 0.5f, 1.f, 1.f);
	else
		m_vColor = _vec4(1.f, 1.f, 1.f, 1.f);
	Engine::Render_Font(L"Font_Info_Data",  szInfo, &_vec2(m_fX - 45.f - ((lstrlen(szInfo) - 2) * 5), m_fY + 40.f), (D3DXCOLOR)m_vColor);
	wsprintf(szInfo, L"%d", m_pPlayer->GetPlayerInfo()->m_iMegicalDefense);
	if (m_tOriInfo.m_iMegicalDefense < m_pPlayer->GetPlayerInfo()->m_iMegicalDefense)
		m_vColor = _vec4(0.f, 0.5f, 1.f, 1.f);
	else
		m_vColor = _vec4(1.f, 1.f, 1.f, 1.f);
	Engine::Render_Font(L"Font_Info_Data",  szInfo, &_vec2(m_fX + 112.5f - ((lstrlen(szInfo) - 2) * 5), m_fY + 40.f), (D3DXCOLOR)m_vColor);

	wsprintf(szInfo, L"%d", int(m_pPlayer->GetPlayerInfo()->m_fHitAaccuracy));
	_stprintf_p(szInfo, lstrlen(szInfo) + 3, L"%f", m_pPlayer->GetPlayerInfo()->m_fHitAaccuracy);
	if (m_tOriInfo.m_fHitAaccuracy < m_pPlayer->GetPlayerInfo()->m_fHitAaccuracy)
		m_vColor = _vec4(0.5f, 0.8f, 0.f, 1.f);
	else
		m_vColor = _vec4(1.f, 1.f, 1.f, 1.f);
	Engine::Render_Font(L"Font_Info_Data",  szInfo, &_vec2(m_fX - 45.f - ((lstrlen(szInfo) - 2) * 5), m_fY + 82.5f), (D3DXCOLOR)m_vColor);
	wsprintf(szInfo, L"%d", int(m_pPlayer->GetPlayerInfo()->m_fConcentration));
	if (m_tOriInfo.m_fConcentration < m_pPlayer->GetPlayerInfo()->m_fConcentration)
		m_vColor = _vec4(0.5f, 0.8f, 0.f, 1.f);
	else
		m_vColor = _vec4(1.f, 1.f, 1.f, 1.f);
	_stprintf_p(szInfo, lstrlen(szInfo) + 3,L"%f", m_pPlayer->GetPlayerInfo()->m_fConcentration);
	Engine::Render_Font(L"Font_Info_Data",  szInfo, &_vec2(m_fX - 45.f - ((lstrlen(szInfo) - 2) * 5), m_fY + 102.5f), (D3DXCOLOR)m_vColor);
	wsprintf(szInfo, L"%d", int(m_pPlayer->GetPlayerInfo()->m_fCritical));
	if (m_tOriInfo.m_fCritical < m_pPlayer->GetPlayerInfo()->m_fCritical)
		m_vColor = _vec4(1.f, 0.25f, 0.f, 1.f);
	else
		m_vColor = _vec4(1.f, 1.f, 1.f, 1.f);
	_stprintf_p(szInfo, lstrlen(szInfo) + 3,L"%f", m_pPlayer->GetPlayerInfo()->m_fCritical);
	Engine::Render_Font(L"Font_Info_Data",  szInfo, &_vec2(m_fX - 45.f - ((lstrlen(szInfo) - 2) * 5), m_fY + 122.5f), (D3DXCOLOR)m_vColor);
	wsprintf(szInfo, L"%d", int(m_pPlayer->GetPlayerInfo()->m_fAttSpeed));
	if (m_tOriInfo.m_fAttSpeed < m_pPlayer->GetPlayerInfo()->m_fAttSpeed)
		m_vColor = _vec4(0.5f, 0.f, 0.7f, 1.f);
	else
		m_vColor = _vec4(1.f, 1.f, 1.f, 1.f);
	_stprintf_p(szInfo, lstrlen(szInfo) + 3,L"%f", m_pPlayer->GetPlayerInfo()->m_fAttSpeed);
	Engine::Render_Font(L"Font_Info_Data",  szInfo, &_vec2(m_fX - 45.f - ((lstrlen(szInfo) - 2) * 5) , m_fY + 142.5f), (D3DXCOLOR)m_vColor);
	wsprintf(szInfo, L"%d", int(m_pPlayer->GetPlayerInfo()->m_fCastSpeed));
	_stprintf_p(szInfo, lstrlen(szInfo) + 3,L"%f", m_pPlayer->GetPlayerInfo()->m_fCastSpeed);
	if (m_tOriInfo.m_fCastSpeed < m_pPlayer->GetPlayerInfo()->m_fCastSpeed)
		m_vColor = _vec4(0.5f, 0.8f, 0.f, 1.f);
	else
		m_vColor = _vec4(1.f, 1.f, 1.f, 1.f);
	Engine::Render_Font(L"Font_Info_Data",  szInfo, &_vec2(m_fX - 45.f - ((lstrlen(szInfo) - 2) * 5), m_fY + 162.5f), (D3DXCOLOR)m_vColor);

	wsprintf(szInfo, L"%d", int(m_pPlayer->GetPlayerInfo()->m_fHitResist));
	_stprintf_p(szInfo, lstrlen(szInfo) + 3,L"%f", m_pPlayer->GetPlayerInfo()->m_fHitResist);
	if (m_tOriInfo.m_fHitResist < m_pPlayer->GetPlayerInfo()->m_fHitResist)
		m_vColor = _vec4(0.5f, 0.8f, 0.f, 1.f);
	else
		m_vColor = _vec4(1.f, 1.f, 1.f, 1.f);
	Engine::Render_Font(L"Font_Info_Data",  szInfo, &_vec2(m_fX + 112.5f - ((lstrlen(szInfo) - 2) * 5), m_fY + 82.5f), (D3DXCOLOR)m_vColor);
	wsprintf(szInfo, L"%d", int(m_pPlayer->GetPlayerInfo()->m_fGlancingArmor));
	_stprintf_p(szInfo, lstrlen(szInfo) + 3,L"%f", m_pPlayer->GetPlayerInfo()->m_fGlancingArmor);
	if (m_tOriInfo.m_fGlancingArmor < m_pPlayer->GetPlayerInfo()->m_fGlancingArmor)
		m_vColor = _vec4(0.5f, 0.8f, 0.f, 1.f);
	else
		m_vColor = _vec4(1.f, 1.f, 1.f, 1.f);
	Engine::Render_Font(L"Font_Info_Data",  szInfo, &_vec2(m_fX + 112.5f - ((lstrlen(szInfo) - 2) * 5), m_fY + 102.5f), (D3DXCOLOR)m_vColor);
	wsprintf(szInfo, L"%d", int(m_pPlayer->GetPlayerInfo()->m_fCriticalResist));
	_stprintf_p(szInfo, lstrlen(szInfo) + 3, L"%f", m_pPlayer->GetPlayerInfo()->m_fCriticalResist);
	if (m_tOriInfo.m_fCriticalResist < m_pPlayer->GetPlayerInfo()->m_fCriticalResist)
		m_vColor = _vec4(0.5f, 0.8f, 0.f, 1.f);
	else
		m_vColor = _vec4(1.f, 1.f, 1.f, 1.f);
	Engine::Render_Font(L"Font_Info_Data",  szInfo, &_vec2(m_fX + 112.5f - ((lstrlen(szInfo) - 2) * 5), m_fY + 122.5f), (D3DXCOLOR)m_vColor);
	wsprintf(szInfo, L"%d", int(m_pPlayer->GetPlayerInfo()->m_fMoveSpeed));
	_stprintf_p(szInfo, lstrlen(szInfo) + 3, L"%f", m_pPlayer->GetPlayerInfo()->m_fMoveSpeed);
	if (m_tOriInfo.m_fMoveSpeed < m_pPlayer->GetPlayerInfo()->m_fMoveSpeed)
		m_vColor = _vec4(0.5f, 0.8f, 0.f, 1.f);
	else
		m_vColor = _vec4(1.f, 1.f, 1.f, 1.f);
	Engine::Render_Font(L"Font_Info_Data",  szInfo, &_vec2(m_fX + 112.5f - ((lstrlen(szInfo) - 2) * 5), m_fY + 142.5f), (D3DXCOLOR)m_vColor);
	wsprintf(szInfo, L"%d", int(m_pPlayer->GetPlayerInfo()->m_fLimitDefence));
	_stprintf_p(szInfo, lstrlen(szInfo) + 3, L"%f", m_pPlayer->GetPlayerInfo()->m_fLimitDefence);
	if (m_tOriInfo.m_fLimitDefence < m_pPlayer->GetPlayerInfo()->m_fLimitDefence)
		m_vColor = _vec4(0.5f, 0.8f, 0.f, 1.f);
	else
		m_vColor = _vec4(1.f, 1.f, 1.f, 1.f);
	Engine::Render_Font(L"Font_Info_Data",  szInfo, &_vec2(m_fX + 112.5f - ((lstrlen(szInfo) - 2) * 5), m_fY + 162.5f), (D3DXCOLOR)m_vColor);

	wsprintf(szInfo, L"%d", int(m_pPlayer->GetPlayerInfo()->m_fFireAtt));
	_stprintf_p(szInfo, lstrlen(szInfo) + 3, L"%f", m_pPlayer->GetPlayerInfo()->m_fFireAtt);
	if (m_tOriInfo.m_fFireAtt < m_pPlayer->GetPlayerInfo()->m_fFireAtt)
		m_vColor = _vec4(1.f, 0.25f, 0.f, 1.f);
	else
		m_vColor = _vec4(1.f, 1.f, 1.f, 1.f);
	_tcscat_s(szInfo, L"%");
	Engine::Render_Font(L"Font_Info_Data",  szInfo, &_vec2(m_fX - 45.f - ((lstrlen(szInfo) - 2) * 5), m_fY + 207.5f), (D3DXCOLOR)m_vColor);
	wsprintf(szInfo, L"%d", int(m_pPlayer->GetPlayerInfo()->m_fSkyAtt));
	_stprintf_p(szInfo, lstrlen(szInfo) + 3, L"%f", m_pPlayer->GetPlayerInfo()->m_fSkyAtt);
	if (m_tOriInfo.m_fSkyAtt < m_pPlayer->GetPlayerInfo()->m_fSkyAtt)
		m_vColor = _vec4(0.f, 1.f, 1.f, 1.f);
	else
		m_vColor = _vec4(1.f, 1.f, 1.f, 1.f);
	_tcscat_s(szInfo, L"%"); 
	Engine::Render_Font(L"Font_Info_Data",  szInfo, &_vec2(m_fX - 45.f - ((lstrlen(szInfo) - 2) * 5), m_fY + 227.5f), (D3DXCOLOR)m_vColor);
	wsprintf(szInfo, L"%d", int(m_pPlayer->GetPlayerInfo()->m_fLandAtt));
	_stprintf_p(szInfo, lstrlen(szInfo) + 3, L"%f", m_pPlayer->GetPlayerInfo()->m_fLandAtt);
	if (m_tOriInfo.m_fLandAtt < m_pPlayer->GetPlayerInfo()->m_fLandAtt)
		m_vColor = _vec4(1.f, 1.f, 0.f, 1.f);
	else
		m_vColor = _vec4(1.f, 1.f, 1.f, 1.f);
	_tcscat_s(szInfo, L"%"); 
	Engine::Render_Font(L"Font_Info_Data",  szInfo, &_vec2(m_fX - 45.f - ((lstrlen(szInfo) - 2) * 5), m_fY + 247.5f), (D3DXCOLOR)m_vColor);
	wsprintf(szInfo, L"%d", int(m_pPlayer->GetPlayerInfo()->m_fWaterAtt));
	_stprintf_p(szInfo, lstrlen(szInfo) + 3, L"%f", m_pPlayer->GetPlayerInfo()->m_fWaterAtt);
	if (m_tOriInfo.m_fWaterAtt < m_pPlayer->GetPlayerInfo()->m_fWaterAtt)
		m_vColor = _vec4(0.f, 0.25f, 1.f, 1.f);
	else
		m_vColor = _vec4(1.f, 1.f, 1.f, 1.f);
	_tcscat_s(szInfo, L"%"); 
	Engine::Render_Font(L"Font_Info_Data",  szInfo, &_vec2(m_fX - 45.f - ((lstrlen(szInfo) - 2) * 5), m_fY + 267.5f), (D3DXCOLOR)m_vColor);
	wsprintf(szInfo, L"%d", int(m_pPlayer->GetPlayerInfo()->m_fFireDef));
	_stprintf_p(szInfo, lstrlen(szInfo) + 3, L"%f", m_pPlayer->GetPlayerInfo()->m_fFireDef);
	if (m_tOriInfo.m_fFireDef < m_pPlayer->GetPlayerInfo()->m_fFireDef)
		m_vColor = _vec4(1.f, 0.25f, 0.f, 1.f);
	else
		m_vColor = _vec4(1.f, 1.f, 1.f, 1.f);
	_tcscat_s(szInfo, L"%"); 
	Engine::Render_Font(L"Font_Info_Data",  szInfo, &_vec2(m_fX + 112.5f - ((lstrlen(szInfo) - 2) * 5), m_fY + 207.5f), (D3DXCOLOR)m_vColor);
	wsprintf(szInfo, L"%d", int(m_pPlayer->GetPlayerInfo()->m_fSkyDef));
	_stprintf_p(szInfo, lstrlen(szInfo) + 3, L"%f", m_pPlayer->GetPlayerInfo()->m_fSkyDef);
	if (m_tOriInfo.m_fSkyDef < m_pPlayer->GetPlayerInfo()->m_fSkyDef)
		m_vColor = _vec4(0.f, 1.f, 1.f, 1.f);
	else
		m_vColor = _vec4(1.f, 1.f, 1.f, 1.f);
	_tcscat_s(szInfo, L"%"); 
	Engine::Render_Font(L"Font_Info_Data",  szInfo, &_vec2(m_fX + 112.5f - ((lstrlen(szInfo) - 2) * 5), m_fY + 227.5f), (D3DXCOLOR)m_vColor);
	wsprintf(szInfo, L"%d", int(m_pPlayer->GetPlayerInfo()->m_fLandDef));
	_stprintf_p(szInfo, lstrlen(szInfo) + 3, L"%f", m_pPlayer->GetPlayerInfo()->m_fLandDef);
	if (m_tOriInfo.m_fLandDef < m_pPlayer->GetPlayerInfo()->m_fLandDef)
		m_vColor = _vec4(1.f, 1.f, 0.f, 1.f);
	else
		m_vColor = _vec4(1.f, 1.f, 1.f, 1.f);
	_tcscat_s(szInfo, L"%"); 
	Engine::Render_Font(L"Font_Info_Data",  szInfo, &_vec2(m_fX + 112.5f - ((lstrlen(szInfo) - 2) * 5), m_fY + 247.5f), (D3DXCOLOR)m_vColor);
	wsprintf(szInfo, L"%d", int(m_pPlayer->GetPlayerInfo()->m_fWaterDef));
	_stprintf_p(szInfo, lstrlen(szInfo) + 3, L"%f", m_pPlayer->GetPlayerInfo()->m_fWaterDef);
	if (m_tOriInfo.m_fWaterDef < m_pPlayer->GetPlayerInfo()->m_fWaterDef)
		m_vColor = _vec4(0.f, 0.25f, 1.f, 1.f);
	else
		m_vColor = _vec4(1.f, 1.f, 1.f, 1.f);
	_tcscat_s(szInfo, L"%"); 
	Engine::Render_Font(L"Font_Info_Data",  szInfo, &_vec2(m_fX + 112.5f - ((lstrlen(szInfo) - 2) * 5), m_fY + 267.5f), (D3DXCOLOR)m_vColor);

}


void CWindow_Info::Window_MoveToMouse()
{
	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	m_bBarCollision = m_pCalCom->Move_Window_UI(WINSIZEX, WINSIZEY, m_fX, m_fY, m_fSizeX, m_fSizeY, ptMouse);
	m_bCollision = m_pCalCom->Collision_ByMouse(WINSIZEX, WINSIZEY, m_fX, m_fY, m_fSizeX, m_fSizeY, ptMouse);

	list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"UI_Window")->begin();
	list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"UI_Window")->end();

	for(; iter != iter_end; ++iter)
	{
		if(this == (*iter))
			continue;
		if(((CUI*)(*iter))->GetWindowProcess())
		{
			//m_bClick = false;
			m_bBarCollision = false;
			m_bCollision = false;
		}
	}

	if(m_bMove)
	{
		m_bMove = false;
		m_fTermX = m_fX - ptMouse.x;
		m_fTermY = m_fY - ptMouse.y;
	}
	if(m_bClick)
	{
		m_bWindowProcess = true;
		m_fX = ptMouse.x + m_fTermX;
		m_fY = ptMouse.y + m_fTermY;
	}

	if(m_bWindowProcess || m_bSelect)
	{
		iter = Engine::FindList(L"UI_Window")->begin();

		for(; iter != iter_end; ++iter)
		{
			if(this == (*iter))
				m_wSortNum = 2;
			else if(((CUI*)(*iter))->GetSelect())
			{
				((CUI*)(*iter))->SetSortNum(this->GetSortNum() - 1);
				((CUI*)(*iter))->SetSelect(false);
			}
			else
				((CUI*)(*iter))->SetSortNum(0);
		}
	}

	if(m_bBarCollision)
	{
		if(Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB) & 0x80)
		{
			if(m_bClick)
				return;
			m_bClick = true;
			m_bMove = true;
		}
		else
		{
			if(m_bClick)
			{
				m_bWindowProcess = false;
			}
			m_bClick = false;
		}
	}

	if(m_bCollision)
	{
		if(Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB) & 0x80)
		{
			iter = Engine::FindList(L"UI_Window")->begin();

			for(; iter != iter_end; ++iter)
				((CUI*)(*iter))->SetSelect(false);

			m_bSelect = true;

			iter = Engine::FindList(L"UI_Window")->begin();

			for (; iter != iter_end; ++iter)
				((CUI*)(*iter))->SetFoward(false);

			m_bFoward = true;
		}
	}

	Overlap_Window();
	
}

HRESULT CWindow_Info::Add_Component(void)
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
	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Window_Char_Info");
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

	// For.Calculater Component
	pComponent = m_pCalCom = Engine::CUICalculater::Create(); 
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_UICal", pComponent));

	return S_OK;
}

CWindow_Info* CWindow_Info::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CWindow_Info*		pInstance = new CWindow_Info(pGraphicDev, wstrName, eState, bIsActive);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CWindow_Info::Free(void)
{
	CUI::Free();
}
