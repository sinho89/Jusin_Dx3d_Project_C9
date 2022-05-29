#include "stdafx.h"
#include "Scene_Create.h"
#include "Export_Engine.h"
#include "Scene_Select.h"

#include "UICamera.h"
#include "Cursor.h"
#include "Create_InfoPanel.h"
#include "Create_Class_Button.h"
#include "Create_Class_BackGround.h"
#include "Create_Class_Texture.h"
#include "Create_Class_ToolTip.h"
#include "Create_Input_ID.h"
#include "Create_OK_Button.h"
#include "Create_Pentagon.h"

#include "Layer.h"
#include "MediaUI.h"
#include "Create_Movie.h"

CScene_Create::CScene_Create(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CScene(pGraphicDev)
{
	Engine::SetSceneID(Engine::SCENE_CREATE);
}

CScene_Create::~CScene_Create(void)
{

}

HRESULT CScene_Create::Ready_Scene(void)
{
	if(FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;

	if(FAILED(SetUp_DefaultSetting()))
		return E_FAIL;

	if(FAILED(Ready_Resource()))
		return E_FAIL;

	if(FAILED(Ready_Layer_GameLogic()))
		return E_FAIL;
	
	return S_OK;
}

void CScene_Create::Update_Scene(const _float& fTimeDelta)
{
	Engine::CScene::Update_Scene(fTimeDelta);
	SkipScene();
	//Button_Progress();
}

void CScene_Create::Render_Scene(void)
{
}

HRESULT CScene_Create::SetUp_DefaultSetting(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

HRESULT CScene_Create::Ready_Layer_GameLogic(void)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	if(NULL == pLayer)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = NULL;

	pGameObject = CCreate_Class_BackGround::Create(m_pGraphicDev, L"ClassBackGround");
	if(NULL == pGameObject)
		goto except;

	if(FAILED(pLayer->AddObject(L"UI",pGameObject)))
		goto except;

	pGameObject = CCreate_Class_Texture::Create(m_pGraphicDev, L"ClassTexture");
	if(NULL == pGameObject)
		goto except;

	if(FAILED(pLayer->AddObject(L"UI",pGameObject)))
		goto except;

	pGameObject = CCreate_Class_ToolTip::Create(m_pGraphicDev, L"ClassToolTip");
	if(NULL == pGameObject)
		goto except;

	if(FAILED(pLayer->AddObject(L"UI",pGameObject)))
		goto except;

	pGameObject = CCreate_Input_ID::Create(m_pGraphicDev, L"ClassNick");
	if(NULL == pGameObject)
		goto except;

	if(FAILED(pLayer->AddObject(L"UI",pGameObject)))
		goto except;

	pGameObject = CCreate_OK_Button::Create(m_pGraphicDev, L"OK", 700, 400, 0);
	if(NULL == pGameObject)
		goto except;

	if(FAILED(pLayer->AddObject(L"UI",pGameObject)))
		goto except;

	pGameObject = CCreate_OK_Button::Create(m_pGraphicDev, L"BEFORE", 500, 400, 1);
	if(NULL == pGameObject)
		goto except;

	if(FAILED(pLayer->AddObject(L"UI",pGameObject)))
		goto except;


	pGameObject = CCreate_InfoPanel::Create(m_pGraphicDev, L"InfoPanel");
	if(NULL == pGameObject)
		goto except;

	if(FAILED(pLayer->AddObject(L"UI",pGameObject)))
		goto except;

	pGameObject = CCreate_Pentagon::Create(m_pGraphicDev, L"Pentagon");
	if(NULL == pGameObject)
		goto except;

	if(FAILED(pLayer->AddObject(L"UI",pGameObject)))
		goto except;

	// Button

	pGameObject = CCreate_Class_Button::Create(m_pGraphicDev, L"ClassButton", -220.f, 200.f, 0);
	if(NULL == pGameObject)
		goto except;

	if(FAILED(pLayer->AddObject(L"UI_BUTTON",pGameObject)))
		goto except;

	pGameObject = CCreate_Class_Button::Create(m_pGraphicDev, L"ClassButton2", -185.f, 135.f, 1);
	if(NULL == pGameObject)
		goto except;

	if(FAILED(pLayer->AddObject(L"UI_BUTTON",pGameObject)))
		goto except;

	pGameObject = CCreate_Class_Button::Create(m_pGraphicDev, L"ClassButton3", -160.f, 70.f, 2);
	if(NULL == pGameObject)
		goto except;

	if(FAILED(pLayer->AddObject(L"UI_BUTTON",pGameObject)))
		goto except;

	pGameObject = CCreate_Class_Button::Create(m_pGraphicDev, L"ClassButton4", -185.f, 5.f, 3);
	if(NULL == pGameObject)
		goto except;

	if(FAILED(pLayer->AddObject(L"UI_BUTTON",pGameObject)))
		goto except;

	pGameObject = CCreate_Class_Button::Create(m_pGraphicDev, L"ClassButton5", -220.f, -60.f, 4);
	if(NULL == pGameObject)
		goto except;

	if(FAILED(pLayer->AddObject(L"UI_BUTTON",pGameObject)))
		goto except;

	//Intro Vedio
	pGameObject = CCreate_Movie::Create(m_pGraphicDev, L"Intro_Fighter", L"../Bin/Resource/Vedio/Fighter_Intro.avi", TRUE);
	if(NULL == pGameObject)
		goto except;

	static_cast<CCreate_Movie*>(pGameObject)->Play();
	static_cast<CCreate_Movie*>(pGameObject)->RenderCheck() = TRUE;

	if(FAILED(pLayer->AddObject(L"Intro_Fighter",pGameObject)))
		goto except;

	pGameObject = CCreate_Movie::Create(m_pGraphicDev, L"Intro_Hunter", L"../Bin/Resource/Vedio/Hunter_Intro.avi", TRUE);
	if(NULL == pGameObject)
		goto except;

	if(FAILED(pLayer->AddObject(L"Intro_Hunter",pGameObject)))
		goto except;

	pGameObject = CCreate_Movie::Create(m_pGraphicDev, L"Intro_Shaman", L"../Bin/Resource/Vedio/Shaman_Intro.avi", TRUE);
	if(NULL == pGameObject)
		goto except;

	if(FAILED(pLayer->AddObject(L"Intro_Shaman",pGameObject)))
		goto except;

	pGameObject = CCreate_Movie::Create(m_pGraphicDev, L"Intro_WitchBlade", L"../Bin/Resource/Vedio/WitchBlade_Intro.avi", TRUE);
	if(NULL == pGameObject)
		goto except;

	if(FAILED(pLayer->AddObject(L"Intro_WitchBlade",pGameObject)))
		goto except;

	pGameObject = CCreate_Movie::Create(m_pGraphicDev, L"Intro_Mistic", L"../Bin/Resource/Vedio/Mistic_Intro.avi", TRUE);
	if(NULL == pGameObject)
		goto except;

	if(FAILED(pLayer->AddObject(L"Intro_Mistic",pGameObject)))
		goto except;

	m_mapLayer.insert(MAPLAYER::value_type(Engine::LAYER_UI, pLayer));

	Engine::DefineCurrentCamera(Engine::CAMERA_UI);

	return S_OK;

except:
	::Safe_Release(pLayer);
	return E_FAIL;	
}

HRESULT CScene_Create::Ready_Resource(void)
{
	return S_OK;
}

void CScene_Create::Button_Progress()
{
	
}
void CScene_Create::SaveSlotIndex(_uint iSaveIndex)
{
	m_iSlotIndex = iSaveIndex;
}
void CScene_Create::SavePlayerDat(_uint iIndex)
{
	// iIndex 1 : 파이터 2 : 헌터 3 : 샤먼 4 : 위치블레이드 5 : 미스틱
	// m_iSlotIndex : 슬롯창 순서

	_tchar		szPath[128];

	wsprintf(szPath, L"../Bin/Data/PlayerInfo/Player%d.dat", m_iSlotIndex);
	
	DWORD	dwByte = 0;
	HANDLE	hFile = CreateFile(szPath, GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	PLAYER_INFO		m_tPlayerInfo;
	memset(&m_tPlayerInfo, NULL, sizeof(PLAYER_INFO));

	switch(iIndex)
	{
	case 1:
		m_tPlayerInfo.m_eClassID = CLASS_FIGHTER;
		lstrcpy(m_tPlayerInfo.m_szClassName, L"파이터");
		lstrcpy(m_tPlayerInfo.m_szID, g_wstrNick.c_str());
		break;
	case 2:
		m_tPlayerInfo.m_eClassID = CLASS_HUNTER;
		lstrcpy(m_tPlayerInfo.m_szClassName, L"헌터");
		lstrcpy(m_tPlayerInfo.m_szID, g_wstrNick.c_str());
		break;
	case 3:
		m_tPlayerInfo.m_eClassID = CLASS_SHAMAN;
		lstrcpy(m_tPlayerInfo.m_szClassName, L"샤먼");
		lstrcpy(m_tPlayerInfo.m_szID, g_wstrNick.c_str());
		break;
	case 4:
		m_tPlayerInfo.m_eClassID = CLASS_WITHBLADE;
		lstrcpy(m_tPlayerInfo.m_szClassName, L"위치블레이드");
		lstrcpy(m_tPlayerInfo.m_szID, g_wstrNick.c_str());
		break;
	case 5:
		m_tPlayerInfo.m_eClassID = CLASS_MISTIC;
		lstrcpy(m_tPlayerInfo.m_szClassName, L"미스틱");
		lstrcpy(m_tPlayerInfo.m_szID, g_wstrNick.c_str());
		break;
	}

	m_tPlayerInfo.m_iSlotIndex = m_iSlotIndex;
	m_tPlayerInfo.m_iStr = m_tPlayerInfo.m_iWis = 13;
	m_tPlayerInfo.m_iCon = 8;
	m_tPlayerInfo.m_iInt = 6;
	m_tPlayerInfo.m_iLevel = 1;
	m_tPlayerInfo.m_iRightAtt = m_tPlayerInfo.m_iLeftAtt = m_tPlayerInfo.m_iTotalAtt = 26;
	m_tPlayerInfo.m_iPhysicalDefense = 16;
	m_tPlayerInfo.m_iMegicalDefense = 12;
	m_tPlayerInfo.m_fCritical = 1.f;
	m_tPlayerInfo.m_fAttSpeed = 100.f;
	m_tPlayerInfo.m_fMoveSpeed = 20.f;
	m_tPlayerInfo.m_iTiredCount = 150;
	m_tPlayerInfo.m_iMoney = 999999;

	m_tPlayerInfo.m_iHp = (m_tPlayerInfo.m_iCon * 20) + 4000;
	m_tPlayerInfo.m_iMp = (m_tPlayerInfo.m_iWis * 5) + 1000;

	m_tPlayerInfo.m_iMaxHp = m_tPlayerInfo.m_iHp;
	m_tPlayerInfo.m_iMaxMp = m_tPlayerInfo.m_iMp;
	m_tPlayerInfo.m_fExp = 0.f;

	WriteFile(hFile, &m_tPlayerInfo, sizeof(PLAYER_INFO), &dwByte, NULL);
	
	CloseHandle(hFile);
}
void CScene_Create::SkipScene()
{
	CUI*	pUi = ((CUI*)Engine::Find(L"OK"));

	if(pUi->GetSkipScene())		// 캐릭터 생성 완료 (저장 로드 구현)
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"UI_BUTTON")->begin();
		list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"UI_BUTTON")->end();

		for(; iter != iter_end; ++iter)
		{
			if(((CUI*)(*iter))->GetSelect())
			{
				SavePlayerDat(((CUI*)(*iter))->GetIndex() + 1);
				Engine::CScene*	pScene = NULL;

				pScene = CScene_Select::Create(m_pGraphicDev, ((CUI*)(*iter))->GetIndex() + 1);

				if(NULL == pScene)								
					return;
				if(FAILED(Engine::Return_SelectScene(pScene)))	
					return;

				g_wstrNick.clear();
				::Safe_Release(pUi);
				return;
			}
		}
		pUi->SetSkipScene(false);
	}
	::Safe_Release(pUi);

	pUi = ((CUI*)Engine::Find(L"BEFORE"));

	if(pUi->GetSkipScene())
	{
		Engine::CScene*	pScene = NULL;

		pScene = CScene_Select::Create(m_pGraphicDev);

		if(NULL == pScene)								
			return;
		if(FAILED(Engine::Return_SelectScene(pScene)))
			return;

		g_wstrNick.clear();
	}		
	::Safe_Release(pUi);

}
CScene_Create* CScene_Create::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene_Create*			pInstance = new CScene_Create(pGraphicDev);

	if(FAILED(pInstance->Ready_Scene()))
	{
		MSG_BOX("CScene_Create Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;	
}

void CScene_Create::Free(void)
{
	//::Safe_Delete(m_pLine);
	Engine::CScene::Free();
}



