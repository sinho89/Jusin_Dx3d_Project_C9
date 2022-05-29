#include "stdafx.h"
#include "Scene_Stage_Ice.h"
#include "Export_Engine.h"

#include "WorldCamera.h"
#include "Map_Ice.h"
#include "WitchBlade.h"
#include "PlayerCamera.h"
#include "Environment.h"
#include "NPC.h"
#include "Cube.h"
#include "UICamera.h"
#include "LightCamera.h"

#include "Culling_Manager.h"

//Gate
#include  "Gate_A.h"
//Monster
#include "Babegazi_Warrior.h"
#include "Babegazi_Axe.h"
#include "Babegazi_Bow.h"
#include "Knole_Commander.h"
#include "Hanuman.h"

//Trap
#include "SnowBall.h"
#include "IceTwister.h"

#include "AstarManager.h"
#include "Cursor.h"
#include "Scene_LoadingForTown.h"
#include "NumberingMgr.h"

//Environment Effect
#include "Fire.h"
#include "UI_Fade_Line.h"
#include "UI_Fade_Title.h"
#include "UI_Fade_Sub.h"
#include "ComboNum.h"
#include "Snow.h"
#include "Portal.h"

//Monster Effect
#include "HanumaAttack01.h"
#include "HanumaAttack02.h"
#include "RoundPunch.h"
#include "CrazyFoot.h"
#include "HanumanCombo.h"
#include "Roar.h"
#include "HanumanBreath.h"

#include "ActionStartObj.h"

//Hanuman Effect
#include "HanumanFire.h"
#include "HanumanEye.h"
#include "Fade_Manager.h"
#include "Fade.h"
#include "ResultItemLogo.h"
#include "ResultItemChar.h"
#include "ResultItemBox.h"
#include "CoolTimeMgr.h"
#include "UI_CoolTime.h"
#include "UI_CoolEff.h"

CScene_Stage_Ice::CScene_Stage_Ice(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CScene(pGraphicDev)
, m_pHanuman(NULL)
, m_bFadeUI(false)
, m_fFadeNum(0.0f)
, m_fFadeTime(0.0f)
, m_iLightCount(0)
{
	Engine::SetSceneID(Engine::SCENE_STAGE_ICE);
	g_iPointLightIndex = 0;
}

CScene_Stage_Ice::~CScene_Stage_Ice(void)
{

}

HRESULT CScene_Stage_Ice::Ready_Scene(void)
{
	CCoolTime_Manager::GetInstance()->Ready_CoolTime();
	if (!g_bEnterToIce)
		m_fFadeNum = 3.f;
	else
		m_fFadeNum = 2.f;

	if(FAILED(Engine::CScene::Ready_Scene()))	return E_FAIL;

	if(FAILED(SetUp_DefaultSetting()))			return E_FAIL;

	if(FAILED(Ready_Resource()))				return E_FAIL;

	if(FAILED(Ready_Prototype()))				return E_FAIL;

	if(FAILED(Ready_Layer_Environment()))		return E_FAIL;

	if(FAILED(Ready_Layer_GameLogic()))			return E_FAIL;

	if (FAILED(Ready_Layer_Weapon()))			return E_FAIL;

	if (FAILED(Ready_Layer_UI()))				return E_FAIL;

	if(FAILED(Ready_Layer_Camera()))			return E_FAIL;

	if(FAILED(Ready_LightInfo()))				return E_FAIL;

	m_bExit = false;

	return S_OK;
}

HRESULT CScene_Stage_Ice::Start_ForScene(void)
{
	//Ready_GameObject();
	
	CPlayer* pPlayer = (CPlayer*)Engine::FindWithTag(L"Player");
	if (pPlayer == NULL)
		return E_FAIL;

	((Engine::CTransform*)pPlayer->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC))->Set_Position(&_vec3(-313.f, 3.f, 58.f));
	pPlayer->Set_SceneType(SCENE_DUNGEON);

	Engine::Safe_Release(pPlayer);
	
	if (FAILED(Ready_GameObject()))				return E_FAIL;
	if (FAILED(Ready_EffectLight()))			return E_FAIL;

	Engine::CCamera* pCamera = NULL;

	pCamera = Engine::GetCamera(Engine::CAMERA_PLAYER);
	((CPlayerCamera*)pCamera)->Reset_Scene_Change();

	((CPlayerCamera*)pCamera)->Start_ForScene();
	Engine::CScene::Start_ForScene();

	CCulling_Manager::GetInstance()->Ready_Culling(m_mapLayer[Engine::LAYER_GAMELOGIC]->Get_MapObject(), 50.f);
	CFade_Manager::GetInstance()->Ready_Fade();
	Engine::CGameObject* pGameObject = CFade::Create(m_pGraphicDev, L"Fade_Boss_Intro");
	Engine::AddObject(Engine::LAYER_UI, L"UI_Fade", pGameObject);
	CFade_Manager::GetInstance()->AddFade((CUI*)pGameObject, CFade_Manager::FADE_BOSS_INTRO);
	pGameObject = CFade::Create(m_pGraphicDev, L"Fade_Boss_Outro");
	Engine::AddObject(Engine::LAYER_UI, L"UI_Fade", pGameObject);
	CFade_Manager::GetInstance()->AddFade((CUI*)pGameObject, CFade_Manager::FADE_BOSS_OUTTRO);
	pGameObject = CFade::Create(m_pGraphicDev, L"Fade_Result_Item");
	Engine::AddObject(Engine::LAYER_UI, L"UI_Fade", pGameObject);
	CFade_Manager::GetInstance()->AddFade((CUI*)pGameObject, CFade_Manager::FADE_CLEAR_GETITEM);


	return S_OK;
}

void CScene_Stage_Ice::Update_Scene(const _float& fTimeDelta)
{
	//잴위에 있어야됨
	CCulling_Manager::GetInstance()->Start_Culling();

	CNumbering_Manager::GetInstance()->ResetCombo(fTimeDelta);
	CNumbering_Manager::GetInstance()->Update_Combo(fTimeDelta);
	CCoolTime_Manager::GetInstance()->UpdateCoolTime();

	m_fFadeTime += fTimeDelta;

	if (m_fFadeTime >= m_fFadeNum)
	{
		m_fFadeTime = 0.f;
		if (!m_bFadeUI)
		{
			m_bFadeUI = true;

			Engine::CGameObject* pGameObject = CUI_Fade_Line::Create(m_pGraphicDev, L"Fade_Line");
			Engine::AddObject(Engine::LAYER_UI, L"UI", pGameObject);
			pGameObject = CUI_Fade_Title::Create(m_pGraphicDev, L"Fade_So", FADE_SO);
			Engine::AddObject(Engine::LAYER_UI, L"UI", pGameObject);
			pGameObject = CUI_Fade_Title::Create(m_pGraphicDev, L"Fade_Ul", FADE_UL);
			Engine::AddObject(Engine::LAYER_UI, L"UI", pGameObject);
			pGameObject = CUI_Fade_Title::Create(m_pGraphicDev, L"Fade_Vell", FADE_VELL);
			Engine::AddObject(Engine::LAYER_UI, L"UI", pGameObject);
			pGameObject = CUI_Fade_Title::Create(m_pGraphicDev, L"Fade_Lee", FADE_ICE_LEE);
			Engine::AddObject(Engine::LAYER_UI, L"UI", pGameObject);
			pGameObject = CUI_Fade_Sub::Create(m_pGraphicDev, L"Fade_Sang", FADE_HA);
			Engine::AddObject(Engine::LAYER_UI, L"UI", pGameObject);
			pGameObject = CUI_Fade_Sub::Create(m_pGraphicDev, L"Fade_Jum", FADE_NOO);
			Engine::AddObject(Engine::LAYER_UI, L"UI", pGameObject);
			pGameObject = CUI_Fade_Sub::Create(m_pGraphicDev, L"Fade_Ji", FADE_MAN);
			Engine::AddObject(Engine::LAYER_UI, L"UI", pGameObject);
			pGameObject = CUI_Fade_Sub::Create(m_pGraphicDev, L"Fade_Goo", FADE_EU);
			Engine::AddObject(Engine::LAYER_UI, L"UI", pGameObject);
			pGameObject = CUI_Fade_Sub::Create(m_pGraphicDev, L"Fade_Goo", FADE_DONG);
			Engine::AddObject(Engine::LAYER_UI, L"UI", pGameObject);
			pGameObject = CUI_Fade_Sub::Create(m_pGraphicDev, L"Fade_Goo", FADE_GOOL);
			Engine::AddObject(Engine::LAYER_UI, L"UI", pGameObject);
		}
	}

	KeyCheck(fTimeDelta);

	static_cast<CPlayerCamera*>(Engine::GetCurrentCamera())->Update_Object(fTimeDelta);

	Engine::CScene::Update_Scene(fTimeDelta);


	// 커맨드액션 쿨타임

	//switch(m_eCameraID)
	//{
	//case Engine::CAMERA_WORLD:
	//	static_cast<CWorldCamera*>(Engine::GetCurrentCamera())->Update_Object(fTimeDelta);
	//	break;

	//case Engine::CAMERA_PLAYER:
	//	static_cast<CPlayerCamera*>(Engine::GetCurrentCamera())->Update_Object(fTimeDelta);
	//	break;

	//case Engine::CAMERA_MINIMAP:
	//	//static_cast<CMapCamera*>(Engine::GetCurrentCamera())->Update_Object(fTimeDelta);
	//	break;
	//}	

	if (m_bExit)
	{
		Engine::CScene*	pScene = NULL;

		pScene = CScene_LoadingForTown::Create(m_pGraphicDev);

		if (NULL == pScene)
			return;
		if (FAILED(Engine::SetUp_CurrentScene(pScene)))
			return;
	}
}

void CScene_Stage_Ice::Render_Scene(void)
{

}

void CScene_Stage_Ice::LoadPlayerInfo(void)
{
	m_iSlotIndex = 2;
	_tchar		szPath[128] = L"";
	wsprintf(szPath, L"../Bin/Data/PlayerInfo/Player%d.dat", m_iSlotIndex);

	DWORD	dwByte = 0;

	HANDLE hFile = CreateFile(szPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	int iIndex = 0;

	PLAYER_INFO MyPlayerInfo;
	memset(&MyPlayerInfo, NULL, sizeof(MyPlayerInfo));

	while (true)
	{
		PLAYER_INFO* pPlayerInfo = new PLAYER_INFO;

		if (!ReadFile(hFile, pPlayerInfo, sizeof(PLAYER_INFO), &dwByte, NULL))
		{
			delete pPlayerInfo;
			pPlayerInfo = NULL;

			CloseHandle(hFile);
			return;
		}

		if (!iIndex)
		{
			memcpy(&MyPlayerInfo, pPlayerInfo, sizeof(PLAYER_INFO));

			delete pPlayerInfo;
			pPlayerInfo = NULL;
		}
		if (dwByte == 0)
		{
			delete pPlayerInfo;
			pPlayerInfo = NULL;
			break;
		}

		++iIndex;
	}

	CloseHandle(hFile);

	CPlayer* pPlayer = static_cast<CWitchBlade*>(Engine::Find(L"Player"));

	pPlayer->SetPlayerInfo(&MyPlayerInfo);
	Engine::Safe_Release(pPlayer);
}

void CScene_Stage_Ice::SaveSlotIndex(_uint iSaveIndex)
{
	m_iSlotIndex = iSaveIndex;
}

HRESULT CScene_Stage_Ice::SetUp_DefaultSetting(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	return S_OK;
}

HRESULT CScene_Stage_Ice::Ready_LightInfo(void)
{
	Engine::LightMap_Clear();

	D3DLIGHT9	LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	if (NULL == Engine::Find_Light(0, D3DLIGHT_DIRECTIONAL))
	{
		LightInfo.Type = D3DLIGHT_DIRECTIONAL;
		LightInfo.Diffuse = D3DXCOLOR(0.6784f, 0.8470f, 0.9019f, 1.0f);
		LightInfo.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		LightInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
		LightInfo.Direction = _vec3(1.0f, -1.0f, 1.0f);

		if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, 0)))	return E_FAIL;

		for (int i = 0; i < 100; i++)
		{
			LightInfo.Type = D3DLIGHT_POINT;
			LightInfo.Range = 0.f;

			LightInfo.Position = _vec3(11111.f, 11111.f, 11111.f);
			LightInfo.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			LightInfo.Specular = D3DXCOLOR(0.001f, 0.001f, 0.001f, 1.0f);
			LightInfo.Ambient = D3DXCOLOR(0.001f, 0.001f, 0.001f, 1.0f);

			LightInfo.Attenuation0 = 0.0f;
			LightInfo.Attenuation1 = 1.0f;
			LightInfo.Attenuation2 = 0.0f;

			if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
			++m_iLightCount;
		}

		return S_OK;
	}

	Engine::Find_LightInfo(0, D3DLIGHT_DIRECTIONAL)->Type = D3DLIGHT_DIRECTIONAL;
	Engine::Find_LightInfo(0, D3DLIGHT_DIRECTIONAL)->Diffuse = D3DXCOLOR(0.6784f, 0.8470f, 0.9019f, 1.0f);
	Engine::Find_LightInfo(0, D3DLIGHT_DIRECTIONAL)->Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Engine::Find_LightInfo(0, D3DLIGHT_DIRECTIONAL)->Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	Engine::Find_LightInfo(0, D3DLIGHT_DIRECTIONAL)->Direction = _vec3(1.0f, -1.0f, -1.0f);

	return S_OK;
}

HRESULT CScene_Stage_Ice::Ready_Layer_Environment( void )
{
	Engine::CLayer*	pLayer = Engine::CLayer::Create();
	if(NULL == pLayer) return E_FAIL;

	Engine::CGameObject*	pGameObject = NULL;

	//Map 맵바꾸는곳..
	pGameObject = CMap_Ice::Create(m_pGraphicDev, L"Map_Ice");
	if(NULL == pGameObject) return E_FAIL;
	pLayer->AddObject(L"Map", pGameObject);



	pGameObject = CSnow::LoadCreate(m_pGraphicDev, L"NewSnow");
	if (NULL == pGameObject) return E_FAIL;
	pLayer->AddObject(L"SoftEffect", pGameObject);

	pGameObject = CActionStartObj::Create(m_pGraphicDev, L"ActionStart", _vec3(454.f, 117.f, -246.f));
	if (NULL == pGameObject) return E_FAIL;
	pLayer->AddObject(L"ActionStart", pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(Engine::LAYER_ENVIRONMENT, pLayer));


	return S_OK;
}

HRESULT CScene_Stage_Ice::Ready_Layer_GameLogic(void)
{
	Engine::CLayer*	pLayer = Engine::CLayer::Create();
	if(NULL == pLayer) return E_FAIL;

	if (g_bEnterToIce == true)
	{
	
		Engine::CGameObject*	pGameObject = NULL;
		////Portal
		pGameObject = CPortal::Create(m_pGraphicDev, L"Portal");
		if (NULL == pGameObject) return E_FAIL;

		_vec3 vPos = _vec3(-323.f, 10.f, 51.f);
		((CPortal*)pGameObject)->SetPortalPosition(&vPos);
		_vec3 vAngle = _vec3(0.f, -128.f, 0.f);
		((CPortal*)pGameObject)->SetAngle(&vAngle);
		((CPortal*)pGameObject)->SkillStart();
		pLayer->AddObject(L"SoftEffect", pGameObject);

		m_mapLayer.insert(MAPLAYER::value_type(Engine::LAYER_GAMELOGIC, pLayer));

		return S_OK;
	}

	Engine::CGameObject*	pGameObject = NULL;
	//Portal
	pGameObject = CPortal::Create(m_pGraphicDev, L"Portal");
	if (NULL == pGameObject) return E_FAIL;
	_vec3 vPos = _vec3(-323.f, 10.f, 51.f);
	((CPortal*)pGameObject)->SetPortalPosition(&vPos);
	_vec3 vAngle = _vec3(0.f, -128.f, 0.f);
	((CPortal*)pGameObject)->SetAngle(&vAngle);
	((CPortal*)pGameObject)->SkillStart();
	pLayer->AddObject(L"SoftEffect", pGameObject);


	m_mapLayer.insert(MAPLAYER::value_type(Engine::LAYER_GAMELOGIC, pLayer));
	
	return S_OK;
}

HRESULT CScene_Stage_Ice::Ready_Layer_Weapon(void)
{
	Engine::CLayer*	pLayer = Engine::CLayer::Create();
	if (NULL == pLayer) return E_FAIL;

	m_mapLayer.insert(MAPLAYER::value_type(Engine::LAYER_WEAPON, pLayer));
	return S_OK;
}

HRESULT		CScene_Stage_Ice::Ready_GameObject(void)
{
	//Monster..
	Engine::CGameObject*	pGameObject = NULL;

	DWORD	dwByte = 0;
	HANDLE hFile = CreateFile(L"../Bin/Data/Stage_Ice/Ice_Obj.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	while (true)
	{
		OBJ_INFO*	pInfo = new OBJ_INFO;

		ReadFile(hFile, pInfo, sizeof(OBJ_INFO), &dwByte, NULL);

		if (dwByte == 0)
		{
			delete pInfo;
			break;
		}

		wstring szTemp;

		szTemp = (wchar_t*)pInfo->m_szTag;

		if (szTemp == L"Monster")
		{
			wstring szName;

			szName = (wchar_t*)pInfo->m_szName;

			if (szName == L"Mesh_Babegazi_Warrior")
			{
				pGameObject = CBabegazi_Warrior::Create(m_pGraphicDev, pInfo->m_szName);

				((CBabegazi_Warrior*)pGameObject)->SetObjectInfo(&pInfo->m_vPos,
					&pInfo->m_vScale,
					&pInfo->m_vAngle);

				Engine::AddObject(pInfo->m_eLayerType, L"Monster", pGameObject);
			}
			else if (szName == L"Mesh_Babegazi_Axe")
			{
				pGameObject = CBabegazi_Axe::Create(m_pGraphicDev, pInfo->m_szName);

				((CBabegazi_Axe*)pGameObject)->SetObjectInfo(&pInfo->m_vPos,
					&pInfo->m_vScale,
					&pInfo->m_vAngle);

				Engine::AddObject(pInfo->m_eLayerType, L"Monster", pGameObject);
			}
			else if (szName == L"Mesh_Babegazi_Bow")
			{
				pGameObject = CBabegazi_Bow::Create(m_pGraphicDev, pInfo->m_szName);

				((CBabegazi_Bow*)pGameObject)->SetObjectInfo(&pInfo->m_vPos,
					&pInfo->m_vScale,
					&pInfo->m_vAngle);

				//Engine::AddObject(pInfo->m_eLayerType, L"Monster", pGameObject);
			}

			else if (szName == L"Mesh_Knole_Commander")
			{
				pGameObject = CKnole_Commander::Create(m_pGraphicDev, pInfo->m_szName);

				((CKnole_Commander*)pGameObject)->SetObjectInfo(&pInfo->m_vPos,
					&pInfo->m_vScale,
					&pInfo->m_vAngle);

				Engine::AddObject(pInfo->m_eLayerType, L"Monster", pGameObject);
			}
		
			else if (szName == L"Mesh_Hanuman")
			{
				pGameObject = CHanuman::Create(m_pGraphicDev, pInfo->m_szName);

				((CHanuman*)pGameObject)->SetObjectInfo(&pInfo->m_vPos,
					&pInfo->m_vScale,
					&pInfo->m_vAngle);

				Engine::AddObject(pInfo->m_eLayerType, L"Monster", pGameObject);

				//m_pHanuman = static_cast<CMonster*>(pGameObject);
			}
		}

		if (szTemp == L"Trap")
		{
			wstring szName;

			szName = (wchar_t*)pInfo->m_szName;

			if (szName == L"Mesh_SnowBall")
			{
				pGameObject = CSnowBall::Create(m_pGraphicDev, pInfo->m_szName);

				((CSnowBall*)pGameObject)->SetObjectInfo(&pInfo->m_vPos,
					&pInfo->m_vScale,
					&pInfo->m_vAngle);

				Engine::AddObject(pInfo->m_eLayerType, L"Trap", pGameObject);
			}
			else if (szName == L"Mesh_IceTwister")
			{
				pGameObject = CIceTwister::Create(m_pGraphicDev, pInfo->m_szName);

				((CIceTwister*)pGameObject)->SetObjectInfo(&pInfo->m_vPos,
					&pInfo->m_vScale,
					&pInfo->m_vAngle);

				Engine::AddObject(pInfo->m_eLayerType, L"Trap", pGameObject);
			}
		}

		else if (szTemp == L"Environment")
		{
			wstring szName;

			szName = (wchar_t*)pInfo->m_szName;

			if (szName == L"Mesh_Gate_A")
			{
				pGameObject = CGate_A::Create(m_pGraphicDev, pInfo->m_szName);

				((CGate_A*)pGameObject)->SetObjectInfo(&pInfo->m_vPos, &pInfo->m_vScale, &pInfo->m_vAngle);

				Engine::AddObject(Engine::LAYER_GAMELOGIC, L"Environment", pGameObject);
			}
			else
			{
				pGameObject = CEnvironment::Create(m_pGraphicDev, pInfo->m_szName);

				((CEnvironment*)pGameObject)->SetObjectInfo(&pInfo->m_vPos, &pInfo->m_vScale, &pInfo->m_vAngle);

				Engine::AddObject(Engine::LAYER_GAMELOGIC, L"Environment", pGameObject);
			}
	
		}

		else if (szTemp == L"Cube")
		{
			pGameObject = CCube::Create(m_pGraphicDev, pInfo->m_szName);
			((CCube*)pGameObject)->SetObjectInfo(&pInfo->m_vPos, &pInfo->m_vScale, &pInfo->m_vAngle);
			//Engine::AddObject(pInfo->m_eLayerType, pInfo->m_szTag, pGameObject);
			Engine::AddObject(pInfo->m_eLayerType, L"Cube", pGameObject);
		}
		
		delete pInfo;
		pInfo = NULL;
	}
	
	CloseHandle(hFile);
	
	//Hanuman Effect=====================
	pGameObject = CHanumaAttack01::Create(m_pGraphicDev, L"HanumaAttack01");
	if (NULL == pGameObject) return E_FAIL;
	Engine::AddObject(Engine::LAYER_GAMELOGIC, L"SoftEffect", pGameObject);

	pGameObject = CHanumanAttack02::Create(m_pGraphicDev, L"HanumaAttack02");
	if (NULL == pGameObject) return E_FAIL;
	Engine::AddObject(Engine::LAYER_GAMELOGIC, L"SoftEffect", pGameObject);

	//하누만 불꽃
	for (int i = 0; i < 4; ++i)
	{
		Engine::CGameObject*	pFireGameObject = NULL;
		pFireGameObject = CHanumanFire::LoadCreate(m_pGraphicDev, L"HanumanFire");
		if (NULL == pGameObject)
			return E_FAIL;
		((CHanumanFire*)pFireGameObject)->SetPattern(i);
		Engine::AddObject(Engine::LAYER_GAMELOGIC, L"SoftEffect", pFireGameObject);
	}

	//하누만 눈
	/*pGameObject = CHanumanEye::LoadCreate(m_pGraphicDev, L"HanumanEye");
	if (NULL == pGameObject) return E_FAIL;
	Engine::AddObject(Engine::LAYER_GAMELOGIC, L"SoftEffect", pGameObject);*/
	//=====================================================================

	pGameObject = CHanumanCombo::Create(m_pGraphicDev, L"HanumanCombo");
	if (NULL == pGameObject) return E_FAIL;
	Engine::AddObject(Engine::LAYER_GAMELOGIC, L"SoftEffect", pGameObject);

	
	pGameObject = CRoundPunch::Create(m_pGraphicDev, L"RoundPunch");
	if (NULL == pGameObject) return E_FAIL;
	pGameObject->Start_ForScene();
	Engine::AddObject(Engine::LAYER_GAMELOGIC, L"SoftEffect", pGameObject);

	pGameObject = CCrazyFoot::Create(m_pGraphicDev, L"CrazyFoot");
	if (NULL == pGameObject) return E_FAIL;
	pGameObject->Start_ForScene();
	Engine::AddObject(Engine::LAYER_GAMELOGIC, L"SoftEffect", pGameObject);

	pGameObject = CRoar::Create(m_pGraphicDev, L"Roar");
	if (NULL == pGameObject) return E_FAIL;
	pGameObject->Start_ForScene();
	Engine::AddObject(Engine::LAYER_GAMELOGIC, L"SoftEffect", pGameObject);


	pGameObject = CHanumanBreath::Create(m_pGraphicDev, L"Breath");
	if (NULL == pGameObject) return E_FAIL;
	pGameObject->Start_ForScene();
	Engine::AddObject(Engine::LAYER_GAMELOGIC, L"SoftEffect", pGameObject);

	//===================================

	//횃불 설치하는 파일...
	pGameObject = NULL;

	dwByte = 0;
	hFile = CreateFile(L"../Bin/Data/Stage_Ice/Ice_Torch.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	//Ice Fire (횃불 설치)=================================================================
	//불이펙트
	Engine::CGameObject*	pEffectGameObject = NULL;
	pEffectGameObject = CFire::LoadCreate(m_pGraphicDev, L"BlueFire");



	while (true)
	{
		OBJ_INFO*	pInfo = new OBJ_INFO;

		ReadFile(hFile, pInfo, sizeof(OBJ_INFO), &dwByte, NULL);

		if (dwByte == 0)
		{
			delete pInfo;
			break;
		}

		wstring szTemp;

		szTemp = (wchar_t*)pInfo->m_szTag;

		if (szTemp == L"Environment")
		{
			_matrix matWorld;
			_matrix matScale;
			_matrix matTrans;
			_matrix matRotate[3];

			//======================================================================
			D3DXMatrixIdentity(&matWorld);
			D3DXMatrixScaling(&matScale, pInfo->m_vScale.x, pInfo->m_vScale.y, pInfo->m_vScale.z);
			D3DXMatrixRotationX(&matRotate[0], D3DXToRadian(pInfo->m_vAngle.x));
			D3DXMatrixRotationY(&matRotate[1], D3DXToRadian(pInfo->m_vAngle.y));
			D3DXMatrixRotationZ(&matRotate[2], D3DXToRadian(pInfo->m_vAngle.z));
			D3DXMatrixTranslation(&matTrans, pInfo->m_vPos.x, pInfo->m_vPos.y, pInfo->m_vPos.z);
			matWorld = matScale * matRotate[0] * matRotate[1] * matRotate[2] * matTrans;


			_vec3 vStart = _vec3(-0.1f, 1.2f, 0.0f);
			D3DXVec3TransformCoord(&vStart, &vStart, &matWorld);
			((CFire*)pEffectGameObject)->AddInitSetPart(&vStart);
			//========================================================================


			pGameObject = CEnvironment::Create(m_pGraphicDev, pInfo->m_szName);

			((CEnvironment*)pGameObject)->SetObjectInfo(&pInfo->m_vPos, &pInfo->m_vScale, &pInfo->m_vAngle);

			Engine::AddObject(pInfo->m_eLayerType, L"Environment", pGameObject);

		}
		delete pInfo;
		pInfo = NULL;
	}
		
	((CFire*)pEffectGameObject)->SetInitAnimation();
	Engine::AddObject(Engine::LAYER_ENVIRONMENT, L"Ice_Fire", pEffectGameObject);

	CloseHandle(hFile);

	//LAMP_BONE_A 설치하는 파일...
	pGameObject = NULL;

	dwByte = 0;
	hFile = CreateFile(L"../Bin/Data/Stage_Ice/Ice_Lamp_Bone_A.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	//Ice Fire (횃불 설치)=================================================================
	//불이펙트
	//Engine::CGameObject*	pEffectGameObject = NULL;
	//pEffectGameObject = CFire::LoadCreate(m_pGraphicDev, L"BlueFire");



	while (true)
	{
		OBJ_INFO*	pInfo = new OBJ_INFO;

		ReadFile(hFile, pInfo, sizeof(OBJ_INFO), &dwByte, NULL);

		if (dwByte == 0)
		{
			delete pInfo;
			break;
		}

		wstring szTemp;

		szTemp = (wchar_t*)pInfo->m_szTag;

		if (szTemp == L"Environment")
		{
			_matrix matWorld;
			_matrix matScale;
			_matrix matTrans;
			_matrix matRotate[3];

			//======================================================================
			//D3DXMatrixIdentity(&matWorld);
			//D3DXMatrixScaling(&matScale, pInfo->m_vScale.x, pInfo->m_vScale.y, pInfo->m_vScale.z);
			//D3DXMatrixRotationX(&matRotate[0], D3DXToRadian(pInfo->m_vAngle.x));
			//D3DXMatrixRotationY(&matRotate[1], D3DXToRadian(pInfo->m_vAngle.y));
			//D3DXMatrixRotationZ(&matRotate[2], D3DXToRadian(pInfo->m_vAngle.z));
			//D3DXMatrixTranslation(&matTrans, pInfo->m_vPos.x, pInfo->m_vPos.y, pInfo->m_vPos.z);
			//matWorld = matScale * matRotate[0] * matRotate[1] * matRotate[2] * matTrans;


			//_vec3 vStart = _vec3(-0.1f, 1.2f, 0.0f);
			//D3DXVec3TransformCoord(&vStart, &vStart, &matWorld);
			//((CFire*)pEffectGameObject)->AddInitSetPart(&vStart);
			//========================================================================


			pGameObject = CEnvironment::Create(m_pGraphicDev, pInfo->m_szName);

			((CEnvironment*)pGameObject)->SetObjectInfo(&pInfo->m_vPos, &pInfo->m_vScale, &pInfo->m_vAngle);

			//위치를 저장키 위한 컨테이너
			m_listBoneA.push_back(pInfo->m_vPos);

			Engine::AddObject(pInfo->m_eLayerType, L"Environment", pGameObject);

		}
		delete pInfo;
		pInfo = NULL;
	}


	/*((CFire*)pEffectGameObject)->SetInitAnimation();
	Engine::AddObject(Engine::LAYER_ENVIRONMENT, L"Ice_Fire", pEffectGameObject);*/


	CloseHandle(hFile);

	//LAMP_BONE_B 설치하는 파일...
	pGameObject = NULL;

	dwByte = 0;
	hFile = CreateFile(L"../Bin/Data/Stage_Ice/Ice_Lamp_Bone_B.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	//Ice Fire (횃불 설치)=================================================================
	//불이펙트

	Engine::CGameObject*	pCandleGameObject = NULL;
	pCandleGameObject = CFire::LoadCreate(m_pGraphicDev, L"BlueCandle");

	while (true)
	{
		OBJ_INFO*	pInfo = new OBJ_INFO;

		ReadFile(hFile, pInfo, sizeof(OBJ_INFO), &dwByte, NULL);

		if (dwByte == 0)
		{
			delete pInfo;
			break;
		}

		wstring szTemp;

		szTemp = (wchar_t*)pInfo->m_szTag;

		if (szTemp == L"Environment")
		{
			_matrix matWorld;
			_matrix matScale;
			_matrix matTrans;
			_matrix matRotate[3];

			//양초 설치======================================================================
			D3DXMatrixIdentity(&matWorld);
			D3DXMatrixScaling(&matScale, pInfo->m_vScale.x, pInfo->m_vScale.y, pInfo->m_vScale.z);
			D3DXMatrixRotationX(&matRotate[0], D3DXToRadian(pInfo->m_vAngle.x));
			D3DXMatrixRotationY(&matRotate[1], D3DXToRadian(pInfo->m_vAngle.y));
			D3DXMatrixRotationZ(&matRotate[2], D3DXToRadian(pInfo->m_vAngle.z));
			D3DXMatrixTranslation(&matTrans, pInfo->m_vPos.x, pInfo->m_vPos.y, pInfo->m_vPos.z);
			matWorld = matScale * matRotate[0] * matRotate[1] * matRotate[2] * matTrans;


			_vec3 vStart = _vec3(0.7f, 6.5f, -3.1f);
			D3DXVec3TransformCoord(&vStart, &vStart, &matWorld);
			((CFire*)pCandleGameObject)->AddInitSetPart(&vStart);
			//===============================================================================
			vStart = _vec3(1.4f, 6.8f, 2.3f);
			D3DXVec3TransformCoord(&vStart, &vStart, &matWorld);
			((CFire*)pCandleGameObject)->AddInitSetPart(&vStart);
			//=============================================================================
			vStart = _vec3(-2.6f, 8.5f, 1.2f);
			D3DXVec3TransformCoord(&vStart, &vStart, &matWorld);
			((CFire*)pCandleGameObject)->AddInitSetPart(&vStart);
			//============================================================================
			vStart = _vec3(-1.3f, 12.5f, 0.5f);
			D3DXVec3TransformCoord(&vStart, &vStart, &matWorld);
			((CFire*)pCandleGameObject)->AddInitSetPart(&vStart);
			//============================================================================
			vStart = _vec3(0.85f, 16.3f, 0.15f);
			D3DXVec3TransformCoord(&vStart, &vStart, &matWorld);
			((CFire*)pCandleGameObject)->AddInitSetPart(&vStart);
			//===========================================================================

			pGameObject = CEnvironment::Create(m_pGraphicDev, pInfo->m_szName);

			((CEnvironment*)pGameObject)->SetObjectInfo(&pInfo->m_vPos, &pInfo->m_vScale, &pInfo->m_vAngle);
				

			Engine::AddObject(pInfo->m_eLayerType, L"Environment", pGameObject);

		}
		delete pInfo;
		pInfo = NULL;
	}


	((CFire*)pCandleGameObject)->SetInitAnimation();
	Engine::AddObject(Engine::LAYER_ENVIRONMENT, L"Candle", pCandleGameObject);
	
	CloseHandle(hFile);

	return S_OK;
}

HRESULT CScene_Stage_Ice::Ready_EffectLight(void)
{
	D3DLIGHT9	LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	//if (Ready_Light_Toch()) return E_FAIL;
	if (Ready_Light_Cristal_0()) return E_FAIL;
	if (Ready_Light_Cristal_1()) return E_FAIL;
	if (Ready_Light_Wall()) return E_FAIL;
	if (Ready_Light_Health()) return E_FAIL;
	CFire* pIceFire = static_cast<CFire*>(Engine::FindWithTag(L"Ice_Fire"));
	vector<_vec3>* IceFireVec = pIceFire->GetVecPos();
	for (size_t i = 0; i < IceFireVec->size(); ++i)
	{
		LightInfo.Type = D3DLIGHT_POINT;
		LightInfo.Range = 30.0f + (rand() % 20);

		LightInfo.Position = (*IceFireVec)[i] + _vec3(0.0f, 2.0f, 0.0f);

		LightInfo.Diffuse = D3DXCOLOR(0.4156f * 3.0f, 0.3529f * 3.0f, 0.8039f * 3.0f, 1.0f);
		LightInfo.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		LightInfo.Ambient = D3DXCOLOR(0.001f, 0.001f, 0.001f, 1.0f);

		//LightInfo.Diffuse = D3DXCOLOR(0.4156f, 0.3529f, 0.8039f, 1.0f);
		//LightInfo.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//LightInfo.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

		LightInfo.Attenuation0 = 0.0f;
		LightInfo.Attenuation1 = 1.0f;
		LightInfo.Attenuation2 = 0.0f;

		if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
		++m_iLightCount;
	}
	Engine::Safe_Release(pIceFire);

	list<_vec3>::iterator iter = m_listBoneA.begin();
	list<_vec3>::iterator iter_end = m_listBoneA.end();

	for (; iter != iter_end; ++iter)
	{
		LightInfo.Type = D3DLIGHT_POINT;
		LightInfo.Range = 40.0f;

		LightInfo.Position = (*iter) + _vec3(0.0f, 10.0f, 0.0f);

		LightInfo.Diffuse = D3DXCOLOR(0.0f * 3.0f, 0.7490f * 3.0f, 1.0f * 3.0f, 1.0f);
		LightInfo.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		LightInfo.Ambient = D3DXCOLOR(0.001f, 0.001f, 0.001f, 1.0f);

		if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
		++m_iLightCount;
	}

	/*list<Engine::CGameObject*>* pCristalVec = Engine::FindList(L"Environment");
	
	list<Engine::CGameObject*>::iterator iter		= pCristalVec->begin();
	list<Engine::CGameObject*>::iterator iter_end	= pCristalVec->end();

	for (; iter != iter_end; ++iter)
	{
		if (L"Mesh_Cristal_1" == (*iter)->GetName())
		{
			LightInfo.Type = D3DLIGHT_POINT;
			LightInfo.Range = 30.0f;

			LightInfo.Position = static_cast<CEnvironment*>(*iter)->GetTransCom()->Get_Now_Position() + _vec3(0.0f, 8.0f, 0.0f);

			LightInfo.Diffuse = D3DXCOLOR(0.54117f, 0.1686f, 0.8862f, 1.0f);
			LightInfo.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			LightInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

			LightInfo.Attenuation0 = 0.0f;
			LightInfo.Attenuation1 = 1.0f;
			LightInfo.Attenuation2 = 0.0f;

			if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
			++m_iLightCount;
		}
		else if (L"Mesh_Cristal_2" == (*iter)->GetName())
		{
			LightInfo.Type = D3DLIGHT_POINT;
			LightInfo.Range = 30.0f;

			LightInfo.Position = static_cast<CEnvironment*>(*iter)->GetTransCom()->Get_Now_Position() + _vec3(0.0f, 6.0f, 0.0f);

			LightInfo.Diffuse = D3DXCOLOR(0.0980f, 0.0980f, 0.4392f, 1.0f);
			LightInfo.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			LightInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

			LightInfo.Attenuation0 = 0.0f;
			LightInfo.Attenuation1 = 1.0f;
			LightInfo.Attenuation2 = 0.0f;

			if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
			++m_iLightCount;
		}
	}*/

	CFire* pCandleFire = static_cast<CFire*>(Engine::FindWithTag(L"Candle"));
	vector<_vec3>* CandleFireVec = pCandleFire->GetVecPos();

	for (size_t i = 0; i < CandleFireVec->size(); ++i)
	{
		LightInfo.Type = D3DLIGHT_POINT;
		LightInfo.Range = 3.0f;

		LightInfo.Position = (*CandleFireVec)[i] + _vec3(0.0f, 3.0f, 0.0f);

		LightInfo.Diffuse = D3DXCOLOR(0.0980f * 2.0f, 0.0980f * 2.0f, 0.4392f * 2.0f, 1.0f);
		LightInfo.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		LightInfo.Ambient = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);

		LightInfo.Attenuation0 = 0.0f;
		LightInfo.Attenuation1 = 1.0f;
		LightInfo.Attenuation2 = 0.0f;

		if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
		++m_iLightCount;
	}

	Engine::Safe_Release(pCandleFire);

	return S_OK;
}

HRESULT CScene_Stage_Ice::Ready_Layer_UI(void)
{
	Engine::CLayer*	pLayer = Engine::CLayer::Create();
	if (NULL == pLayer) return E_FAIL;

	Engine::CGameObject*	pGameObject = NULL;

	CUI* pCombo = ((CUI*)Engine::Find(L"Combo"));
	::Safe_Release(pCombo);
	CUI* pComboEffect = ((CUI*)Engine::Find(L"ComboEffect"));
	::Safe_Release(pComboEffect);

	CNumbering_Manager::GetInstance()->Ready_Numbering((CUI*)pCombo, (CUI*)pComboEffect);

	pGameObject = CComboNum::Create(m_pGraphicDev, L"ComboNum_One", pCombo, NUM_ONE);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;


	CNumbering_Manager::GetInstance()->AddVecDigit((CUI*)pGameObject);

	pGameObject = CComboNum::Create(m_pGraphicDev, L"ComboNum_Ten", pCombo, NUM_TEN);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;
	CNumbering_Manager::GetInstance()->AddVecDigit((CUI*)pGameObject);

	pGameObject = CComboNum::Create(m_pGraphicDev, L"ComboNum_Hun", pCombo, NUM_HUN);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;
	CNumbering_Manager::GetInstance()->AddVecDigit((CUI*)pGameObject);


	pGameObject = CUI_CoolTime::Create(m_pGraphicDev, L"CoolTime", SKILL_LIDDLEKICK);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;
	CCoolTime_Manager::GetInstance()->AddCoolTime(CCoolTime_Manager::COOL_RIDDLE, (CUI_Skill*)pGameObject);

	pGameObject = CUI_CoolEff::Create(m_pGraphicDev, L"CoolTime", ((CUI_Skill*)pGameObject), 5.f, SKILL_LIDDLEKICK);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	pGameObject = CUI_CoolTime::Create(m_pGraphicDev, L"CoolTime", SKILL_FLENITDANCE);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;
	CCoolTime_Manager::GetInstance()->AddCoolTime(CCoolTime_Manager::COOL_PLANIT, (CUI_Skill*)pGameObject);

	pGameObject = CUI_CoolEff::Create(m_pGraphicDev, L"CoolTime", ((CUI_Skill*)pGameObject), 6.f, SKILL_FLENITDANCE);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	pGameObject = CUI_CoolTime::Create(m_pGraphicDev, L"CoolTime", SKILL_VIFERKICK);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;
	CCoolTime_Manager::GetInstance()->AddCoolTime(CCoolTime_Manager::COOL_VIPER, (CUI_Skill*)pGameObject);

	pGameObject = CUI_CoolEff::Create(m_pGraphicDev, L"CoolTime", ((CUI_Skill*)pGameObject), 3.f, SKILL_VIFERKICK);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	pGameObject = CUI_CoolTime::Create(m_pGraphicDev, L"CoolTime", SKILL_BLARE);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;
	CCoolTime_Manager::GetInstance()->AddCoolTime(CCoolTime_Manager::COOL_BLARE, (CUI_Skill*)pGameObject);

	pGameObject = CUI_CoolEff::Create(m_pGraphicDev, L"CoolTime", ((CUI_Skill*)pGameObject), 3.f, SKILL_BLARE);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	pGameObject = CUI_CoolTime::Create(m_pGraphicDev, L"CoolTime", SKILL_HILLTURN);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;
	CCoolTime_Manager::GetInstance()->AddCoolTime(CCoolTime_Manager::COOL_HILLTURN, (CUI_Skill*)pGameObject);

	pGameObject = CUI_CoolEff::Create(m_pGraphicDev, L"CoolTime", ((CUI_Skill*)pGameObject), 6.f, SKILL_HILLTURN);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	pGameObject = CUI_CoolTime::Create(m_pGraphicDev, L"CoolTime", SKILL_SILUETDANCE);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;
	CCoolTime_Manager::GetInstance()->AddCoolTime(CCoolTime_Manager::COOL_SILUET, (CUI_Skill*)pGameObject);

	pGameObject = CUI_CoolEff::Create(m_pGraphicDev, L"CoolTime", ((CUI_Skill*)pGameObject), 5.f, SKILL_SILUETDANCE);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	pGameObject = CUI_CoolTime::Create(m_pGraphicDev, L"CoolTime", SKILL_SOFTLANDING);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;
	CCoolTime_Manager::GetInstance()->AddCoolTime(CCoolTime_Manager::COOL_SOFTRANDING, (CUI_Skill*)pGameObject);

	pGameObject = CUI_CoolEff::Create(m_pGraphicDev, L"CoolTime", ((CUI_Skill*)pGameObject), 3.f, SKILL_SOFTLANDING);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	pGameObject = CUI_CoolTime::Create(m_pGraphicDev, L"CoolTime", SKILL_SPININGHEART);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;
	CCoolTime_Manager::GetInstance()->AddCoolTime(CCoolTime_Manager::COOL_SPINING, (CUI_Skill*)pGameObject);

	pGameObject = CUI_CoolEff::Create(m_pGraphicDev, L"CoolTime", ((CUI_Skill*)pGameObject), 10.f, SKILL_SPININGHEART);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	pGameObject = CUI_CoolTime::Create(m_pGraphicDev, L"CoolTime", SKILL_TRICKEY);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;
	CCoolTime_Manager::GetInstance()->AddCoolTime(CCoolTime_Manager::COOL_TRICK, (CUI_Skill*)pGameObject);

	pGameObject = CUI_CoolEff::Create(m_pGraphicDev, L"CoolTime", ((CUI_Skill*)pGameObject), 10.f, SKILL_TRICKEY);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	pGameObject = CUI_CoolTime::Create(m_pGraphicDev, L"CoolTime", SKILL_UPATT);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;
	CCoolTime_Manager::GetInstance()->AddCoolTime(CCoolTime_Manager::COOL_UPATT, (CUI_Skill*)pGameObject);

	pGameObject = CUI_CoolEff::Create(m_pGraphicDev, L"CoolTime", ((CUI_Skill*)pGameObject), 8.f, SKILL_UPATT);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	m_mapLayer.insert(MAPLAYER::value_type(Engine::LAYER_UI, pLayer));

	return S_OK;

except:
	::Safe_Release(pLayer);
	return E_FAIL;
}

HRESULT CScene_Stage_Ice::Ready_Layer_Camera(void)
{
	Engine::DefineCurrentCamera(Engine::CAMERA_PLAYER);
	m_eCameraID = Engine::CAMERA_PLAYER;

	return S_OK;
}

HRESULT CScene_Stage_Ice::Ready_Resource(void)
{

	return S_OK;
}

HRESULT CScene_Stage_Ice::Ready_Prototype(void)
{
	return S_OK;
}

void CScene_Stage_Ice::KeyCheck(const _float& fTimeDelta)
{
	/*if(Engine::Get_DIKeyState(DIK_NUMPAD1)	& 0x80)
	{
		m_eCameraID = Engine::CAMERA_WORLD;
		Engine::DefineCurrentCamera(m_eCameraID);
	}*/

	if(Engine::Get_DIKeyState(DIK_NUMPAD2)	& 0x80)
	{
		m_eCameraID = Engine::CAMERA_PLAYER;
		Engine::DefineCurrentCamera(m_eCameraID);
	}

	if(Engine::Get_DIKeyState(DIK_NUMPAD3)	& 0x80)
	{
		m_eCameraID = Engine::CAMERA_MINIMAP;
		Engine::DefineCurrentCamera(m_eCameraID);
	}

	if(Engine::Get_DIKeyState(DIK_NUMPADPLUS) 	& 0x80)
	{
		CPlayer* pPlayer = static_cast<CWitchBlade*>(Engine::Find(L"Player"));

		PLAYER_INFO* pPlayerInfo = pPlayer->GetPlayerInfo();
		pPlayerInfo->m_iLevel += 1;

		Engine::Safe_Release(pPlayer);
	}
}


CScene_Stage_Ice* CScene_Stage_Ice::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene_Stage_Ice*			pInstance = new CScene_Stage_Ice(pGraphicDev);

	if(FAILED(pInstance->Ready_Scene()))
	{
		MSG_BOX("CScene_Town Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;	
}

void CScene_Stage_Ice::Free(void)
{
	m_listBoneA.clear();
	CCulling_Manager::GetInstance()->StopThread();
	CNumbering_Manager::GetInstance()->DestroyInstance();
	Engine::Clear_All();
	Engine::CScene::Free();
	CAStarManager::GetInstance()->StopThread();
	CAStarManager::GetInstance()->DestroyInstance();
}

//더러운 곳===========================================================
//===================================================================
HRESULT CScene_Stage_Ice::Ready_Light_Toch(void)
{
	D3DLIGHT9	LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	//==============================================================================================
	//2인분짜리
	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Range = 50.0f;
	LightInfo.Position = _vec3(-285.0f, 18.0f, 85.0f);
	LightInfo.Diffuse = D3DXCOLOR(0.4156f * 1.5f, 0.3529f * 1.5f, 0.8039f * 1.5f, 1.0f);
	LightInfo.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	LightInfo.Ambient = D3DXCOLOR(0.001f, 0.001f, 0.001f, 1.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(-299.0f, 21.0f, 236.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(-171.0f, 13.0f, 184.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(-87.0f, 62.0f, 408.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(-72.0f, 68.0f, 453.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(23.0f, 19.0f, 173.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(-288.0f, 81.0f, 29.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(361.0f, 83.0f, 28.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(326.0f, 103.0f, 22.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(324.0f, 93.0f, -31.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(322.0f, 91.0f, -109.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(474.0f, 141.0f, -248.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	//==============================================================================================
	//==============================================================================================
	//1인분짜리
	LightInfo.Range = 30.0f;
	LightInfo.Position = _vec3(-303.0f, 23.0f, 121.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(-306.0f, 21.0f, 164.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(-346.0f, 19.0f, 208.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(-225.0f, 12.0f, 264.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(-256.0f, 31.0f, 310.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(-267.0f, 8.0f, 433.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(-298.0f, 19.0f, 480.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(-112.0f, 16.0f, 254.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(-53.0f, 46.0f, 331.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(-49.0f, 67.0f, 153.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(-143.0f, 66.0f, 228.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(12.0f, 16.0f, 250.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(28.0f, 24.0f, 61.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(-30.0f, 30.0f, -4.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(61.0f, 23.0f, 0.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(125.0f, 34.0f, -50.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(151.0f, 35.0f, 45.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(143.0f, 48.0f, 113.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(164.0f, 44.0f, 163.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(205.0f, 39.0f, 207.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(215.0f, 38.0f, 291.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(285.0f, 39.0f, 245.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(350.0f, 38.0f, 252.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(368.0f, 66.0f, 144.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(295.0f, 66.0f, 138.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(406.0f, 128.0f, -124.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(460.0f, 132.0f, -164.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(413.0f, 150.0f, -257.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(384.0f, 180.0f, -294.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(450.0f, 191.0f, -239.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(391.0f, 234.0f, -255.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(385.0f, 177.0f, -289.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(374.0f, 286.0f, -317.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(433.0f, 291.0f, -264.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(358.0f, 160.0f, -374.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(304.0f, 160.0f, -368.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(270.0f, 199.0f, -327.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(240.0f, 197.0f, -287.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;
	
	//중앙================================================================
	LightInfo.Range = 100.0f;
	LightInfo.Position = _vec3(333.0f, 193.0f, -237.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	//석상 눈.================================================================
	LightInfo.Range = 15.0f;
	LightInfo.Diffuse = D3DXCOLOR(0.5019f * 20.0f, 0.0f, 0.0f, 1.0f);
	LightInfo.Position = _vec3(320.0f, 224.0f, -317.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(340.0f, 227.0f, -314.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	return S_OK;
}

HRESULT CScene_Stage_Ice::Ready_Light_Cristal_0(void)
{
	D3DLIGHT9	LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	//==============================================================================================
	//1인분짜리
	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Range = 15.0f;
	LightInfo.Position = _vec3(-194.0f, -1.0f, 389.0f);
	LightInfo.Diffuse = D3DXCOLOR(0.54117f * 2.0f, 0.1686f * 2.0f, 0.8862f * 2.0f, 1.0f);
	LightInfo.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	LightInfo.Ambient = D3DXCOLOR(0.001f, 0.001f, 0.001f, 1.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(-315.0f, 0.0f, 429.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(-107.0f, -1.0f, 358.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(27.0f, -1.0f, 265.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(-58.0f, 2.0f, 304.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(-143.0f, 2.0f, 254.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(433.0f, 143.0f, -274.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	//==============================================================================================
	//2인분짜리
	LightInfo.Range = 30.0f;
	LightInfo.Position = _vec3(406.0f, 146.0f, -302.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	//==============================================================================================
	//좀 많은
	LightInfo.Range = 50.0f;
	LightInfo.Position = _vec3(259.0f, 158.0f, -349.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(221.0f, 155.0f, -343.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	//==============================================================================================
	//다리 쪽.
	LightInfo.Range = 60.0f;
	LightInfo.Position = _vec3(263.0f, 120.0f, 77.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(383.0f, 94.0f, 45.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(375.0f, 14.0f, 55.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(266.0f, 6.0f, 69.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	return S_OK;
}

HRESULT CScene_Stage_Ice::Ready_Light_Cristal_1(void)
{
	D3DLIGHT9	LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	//==============================================================================================
	//1인분짜리
	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Range = 20.0f;
	LightInfo.Position = _vec3(-333.0f, 1.0f, 524.0f);
	LightInfo.Diffuse = D3DXCOLOR(0.0980f * 2.0f, 0.0980f * 2.0f, 0.4392f * 2.0f, 1.0f);
	LightInfo.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	LightInfo.Ambient = D3DXCOLOR(0.001f, 0.001f, 0.001f, 1.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(-57.0f, 47.0f, 511.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(391.0f, 121.0f, -289.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(435.0f, 120.0f, -196.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(374.0f, 120.0f, -154.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(311.0f, 120.0f, -133.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(234.0f, 119.0f, -213.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(207.0f, 120.0f, -302.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(270.0f, 121.0f, -347.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	return S_OK;
}

HRESULT CScene_Stage_Ice::Ready_Light_Wall(void)
{
	D3DLIGHT9	LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	//==============================================================================================
	//1인분짜리
	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Range = 30.0f;
	LightInfo.Position = _vec3(58.0f, 37.0f, 296.0f);
	LightInfo.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.5019f * 1.5f, 1.0f);
	LightInfo.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	LightInfo.Ambient = D3DXCOLOR(0.001f, 0.001f, 0.001f, 1.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(22.0f, 61.0f, -33.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(141.0f, 71.0f, -19.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(145.0f, 80.0f, 138.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(202.0f, 73.0f, 205.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(346.0f, 77.0f, 254.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(385.0f, 149.0f, 65.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(263.0f, 120.0f, -99.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(356.0f, 168.0f, -66.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(512.0f, 189.0f, -185.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(231.0f, 216.0f, -272.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	return S_OK;
}

HRESULT CScene_Stage_Ice::Ready_Light_Health(void)
{
	D3DLIGHT9	LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	//==============================================================================================
	//1인분짜리
	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Range = 40.0f;
	LightInfo.Position = _vec3(260.0f, 71.0f, -69.0f);
	LightInfo.Diffuse = D3DXCOLOR(0.5647f * 3.0f, 0.9333f * 3.0f, 0.5647f * 3.0f, 1.0f);
	LightInfo.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	LightInfo.Ambient = D3DXCOLOR(0.001f, 0.001f, 0.001f, 1.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	return S_OK;
}


