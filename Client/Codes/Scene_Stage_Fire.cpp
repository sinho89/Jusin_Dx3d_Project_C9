#include "stdafx.h"
#include "Scene_Stage_Fire.h"
#include "Export_Engine.h"

#include "WorldCamera.h"
#include "Map_Fire.h"
#include "WitchBlade.h"
#include "PlayerCamera.h"
#include "Environment.h"
#include "NPC.h"
#include "Cube.h"
#include "UICamera.h"
#include "LightCamera.h"

#include "Culling_Manager.h"

//Monster
#include "Bardiel.h"

#include "AstarManager.h"
#include "Cursor.h"
#include "Scene_LoadingForTown.h"
#include "NumberingMgr.h"
#include "Map_Ice.h"
#include "UI_Fade_Line.h"
#include "UI_Fade_Title.h"
#include "UI_Fade_Sub.h"
#include "ComboNum.h"
#include "Fade_Manager.h"
#include "Fade.h"
#include "ResultItemBox.h"
#include "ResultItemChar.h"
#include "ResultItemLogo.h"

//Effect
#include "Lava.h"
#include "FireBall.h"
#include "Summon.h"
#include "Pillar.h"
#include "Penalty.h"
#include "WhirlWind.h"
#include "Volcano.h"
#include "ShortHammer.h"
#include "LFoot.h"
#include "Hammer.h"
#include "FireWall.h"
#include "Divide.h"
#include "Clean.h"
#include "FireFrame.h"
#include "ChangePhase2.h"
#include "Portal.h"
#include "Fire.h"

#include "ActionStartObj.h"
#include "CoolTimeMgr.h"
#include "UI_CoolEff.h"
#include "UI_CoolTime.h"


CScene_Stage_Fire::CScene_Stage_Fire(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CScene(pGraphicDev)
, m_bFadeUI(false)
, m_fFadeNum(0.0f)
, m_fFadeTime(0.0f)
, m_iLightCount(0)
{
	Engine::SetSceneID(Engine::SCENE_STAGE_FIRE);
	g_iPointLightIndex = 0;
}

CScene_Stage_Fire::~CScene_Stage_Fire(void)
{

}

HRESULT CScene_Stage_Fire::Ready_Scene(void)
{
	m_fFadeNum = 3.f;
	CCoolTime_Manager::GetInstance()->Ready_CoolTime();

	if(FAILED(Engine::CScene::Ready_Scene()))	return E_FAIL;

	if(FAILED(SetUp_DefaultSetting()))			return E_FAIL;

	if(FAILED(Ready_Resource()))				return E_FAIL;

	if(FAILED(Ready_Prototype()))				return E_FAIL;

	if(FAILED(Ready_Layer_Environment()))		return E_FAIL;

	if(FAILED(Ready_Layer_GameLogic()))			return E_FAIL;

	if (FAILED(Ready_Layer_Weapon()))			return E_FAIL;

	if (FAILED(Ready_Layer_UI()))				return E_FAIL;

	if (FAILED(Ready_LightInfo()))				return E_FAIL;

	if(FAILED(Ready_Layer_Camera()))			return E_FAIL;

	m_bExit = false;

	return S_OK;
}

HRESULT CScene_Stage_Fire::Start_ForScene(void)
{

	//Ready_GameObject();

	CPlayer* pPlayer = (CPlayer*)Engine::FindWithTag(L"Player");
	if (pPlayer == NULL)
		return E_FAIL;

	((Engine::CTransform*)pPlayer->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC))->Set_Position(&_vec3(35.f, -114.f, -407.f));
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

void CScene_Stage_Fire::Update_Scene(const _float& fTimeDelta)
{
	m_fFadeTime += fTimeDelta;

	if (m_fFadeTime >= m_fFadeNum)
	{
		m_fFadeTime = 0.f;
		if (!m_bFadeUI)
		{
			m_bFadeUI = true;

			Engine::CGameObject* pGameObject = CUI_Fade_Line::Create(m_pGraphicDev, L"Fade_Line");
			Engine::AddObject(Engine::LAYER_UI, L"UI", pGameObject);
			pGameObject = CUI_Fade_Title::Create(m_pGraphicDev, L"Fade_So", FADE_VOL);
			Engine::AddObject(Engine::LAYER_UI, L"UI", pGameObject);
			pGameObject = CUI_Fade_Title::Create(m_pGraphicDev, L"Fade_Ul", FADE_S);
			Engine::AddObject(Engine::LAYER_UI, L"UI", pGameObject);
			pGameObject = CUI_Fade_Title::Create(m_pGraphicDev, L"Fade_Vell", FADE_LAN);
			Engine::AddObject(Engine::LAYER_UI, L"UI", pGameObject);
			pGameObject = CUI_Fade_Title::Create(m_pGraphicDev, L"Fade_Lee", FADE_D);
			Engine::AddObject(Engine::LAYER_UI, L"UI", pGameObject);
			pGameObject = CUI_Fade_Sub::Create(m_pGraphicDev, L"Fade_Sang", FADE_BA);
			Engine::AddObject(Engine::LAYER_UI, L"UI", pGameObject);
			pGameObject = CUI_Fade_Sub::Create(m_pGraphicDev, L"Fade_Jum", FADE_R);
			Engine::AddObject(Engine::LAYER_UI, L"UI", pGameObject);
			pGameObject = CUI_Fade_Sub::Create(m_pGraphicDev, L"Fade_Ji", FADE_DI);
			Engine::AddObject(Engine::LAYER_UI, L"UI", pGameObject);
			pGameObject = CUI_Fade_Sub::Create(m_pGraphicDev, L"Fade_Goo", FADE_EL);
			Engine::AddObject(Engine::LAYER_UI, L"UI", pGameObject);
			pGameObject = CUI_Fade_Sub::Create(m_pGraphicDev, L"Fade_Goo", FADE_JU);
			Engine::AddObject(Engine::LAYER_UI, L"UI", pGameObject);
			pGameObject = CUI_Fade_Sub::Create(m_pGraphicDev, L"Fade_Goo", FADE_DOON);
			Engine::AddObject(Engine::LAYER_UI, L"UI", pGameObject);
			pGameObject = CUI_Fade_Sub::Create(m_pGraphicDev, L"Fade_Goo", FADE_FIRE_JI);
			Engine::AddObject(Engine::LAYER_UI, L"UI", pGameObject);
		}
	}

	KeyCheck(fTimeDelta);

	Engine::CScene::Update_Scene(fTimeDelta);

	CNumbering_Manager::GetInstance()->Update_Combo(fTimeDelta);
	CNumbering_Manager::GetInstance()->ResetCombo(fTimeDelta);

	// Ä¿¸Çµå¾×¼Ç ÄðÅ¸ÀÓ
	CCoolTime_Manager::GetInstance()->UpdateCoolTime();

	static_cast<CPlayerCamera*>(Engine::GetCurrentCamera())->Update_Object(fTimeDelta);

	/*switch(m_eCameraID)
	{
	case Engine::CAMERA_WORLD:
		static_cast<CWorldCamera*>(Engine::GetCurrentCamera())->Update_Object(fTimeDelta);
		break;

	case Engine::CAMERA_PLAYER:
		static_cast<CPlayerCamera*>(Engine::GetCurrentCamera())->Update_Object(fTimeDelta);
		break;

	case Engine::CAMERA_MINIMAP:
		static_cast<CMapCamera*>(Engine::GetCurrentCamera())->Update_Object(fTimeDelta);
		break;
	}*/	

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

void CScene_Stage_Fire::Render_Scene(void)
{

}

void CScene_Stage_Fire::LoadPlayerInfo(void)
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

void CScene_Stage_Fire::SaveSlotIndex(_uint iSaveIndex)
{
	m_iSlotIndex = iSaveIndex;
}

HRESULT CScene_Stage_Fire::SetUp_DefaultSetting(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	return S_OK;
}

HRESULT CScene_Stage_Fire::Ready_LightInfo(void)
{
	Engine::LightMap_Clear();

	D3DLIGHT9	LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	if (NULL == Engine::Find_Light(0, D3DLIGHT_DIRECTIONAL))
	{
		LightInfo.Type = D3DLIGHT_DIRECTIONAL;
		LightInfo.Diffuse = D3DXCOLOR(0.08039f, 0.03607f, 0.03607f, 1.0f);
		LightInfo.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		LightInfo.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		LightInfo.Direction = _vec3(1.0f, -1.0f, 1.0f);

		if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, 0)))	return E_FAIL;

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

		return S_OK;
	}

	Engine::Find_LightInfo(0, D3DLIGHT_DIRECTIONAL)->Type = D3DLIGHT_DIRECTIONAL;
	Engine::Find_LightInfo(0, D3DLIGHT_DIRECTIONAL)->Diffuse = D3DXCOLOR(0.08039f, 0.03607f, 0.03607f, 1.0f);
	Engine::Find_LightInfo(0, D3DLIGHT_DIRECTIONAL)->Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Engine::Find_LightInfo(0, D3DLIGHT_DIRECTIONAL)->Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
	Engine::Find_LightInfo(0, D3DLIGHT_DIRECTIONAL)->Direction = _vec3(1.0f, -1.0f, -1.0f);

	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Range = 12.0f;

	LightInfo.Position = _vec3(0.f, 0.f, 0.f);
	LightInfo.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	LightInfo.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	LightInfo.Ambient = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);

	LightInfo.Attenuation0 = 1.0f;
	LightInfo.Attenuation1 = 1.0f;
	LightInfo.Attenuation2 = 0.0f;

	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;
	return S_OK;
}

HRESULT CScene_Stage_Fire::Ready_Layer_Environment( void )
{
	Engine::CLayer*	pLayer = Engine::CLayer::Create();
	if(NULL == pLayer) return E_FAIL;

	Engine::CGameObject*	pGameObject = NULL;

	//Map ¸Ê¹Ù²Ù´Â°÷..
	pGameObject = CMap_Fire::Create(m_pGraphicDev, L"Map_Fire");
	if(NULL == pGameObject) return E_FAIL;
	pLayer->AddObject(L"Map", pGameObject);

	pGameObject = CActionStartObj::Create(m_pGraphicDev, L"ActionStart", _vec3(314.f, -25.f, -339.f));
	if (NULL == pGameObject) return E_FAIL;
	pLayer->AddObject(L"ActionStart", pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(Engine::LAYER_ENVIRONMENT, pLayer));

	return S_OK;
}

HRESULT CScene_Stage_Fire::Ready_Layer_GameLogic(void)
{
	Engine::CLayer*	pLayer = Engine::CLayer::Create();
	if(NULL == pLayer) return E_FAIL;

	Engine::CGameObject*	pGameObject = NULL;


	//Portal
	pGameObject = CPortal::Create(m_pGraphicDev, L"Portal");
	if (NULL == pGameObject) return E_FAIL;
	_vec3 vPos = _vec3(25.f, -109.f, -405.f);
	((CPortal*)pGameObject)->SetPortalPosition(&vPos);
	_vec3 vAngle = _vec3(0.f, 100.f, 0.f);
	((CPortal*)pGameObject)->SetAngle(&vAngle);
	((CPortal*)pGameObject)->SkillStart();
	pLayer->AddObject(L"SoftEffect", pGameObject);



	m_mapLayer.insert(MAPLAYER::value_type(Engine::LAYER_GAMELOGIC, pLayer));
	
	return S_OK;
}

HRESULT CScene_Stage_Fire::Ready_Layer_Weapon(void)
{
	Engine::CLayer*	pLayer = Engine::CLayer::Create();
	if (NULL == pLayer) return E_FAIL;

	m_mapLayer.insert(MAPLAYER::value_type(Engine::LAYER_WEAPON, pLayer));
	return S_OK;
}

HRESULT		CScene_Stage_Fire::Ready_GameObject(void)
{
	//Monster..
	Engine::CGameObject*	pGameObject = NULL;

	DWORD	dwByte = 0;
	HANDLE hFile = CreateFile(L"../Bin/Data/Stage_Fire/Fire_Obj.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

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

			if (szName == L"Mesh_Bardiel")
			{
				pGameObject = CBardiel::Create(m_pGraphicDev, pInfo->m_szName);

				((CBardiel*)pGameObject)->SetObjectInfo(&pInfo->m_vPos,
					&pInfo->m_vScale,
					&pInfo->m_vAngle);

				Engine::AddObject(pInfo->m_eLayerType, L"Monster", pGameObject);
			}
			//else if (szName == L"Mesh_Babegazi_Axe")
			//{
			//	pGameObject = CBabegazi_Axe::Create(m_pGraphicDev, pInfo->m_szName);

			//	((CBabegazi_Axe*)pGameObject)->SetObjectInfo(&pInfo->m_vPos,
			//		&pInfo->m_vScale,
			//		&pInfo->m_vAngle);
			//	pGameObject->Start_ForScene();

			//	Engine::AddObject(pInfo->m_eLayerType, L"Monster", pGameObject);
			//}
		}

		else if (szTemp == L"Environment")
		{
			pGameObject = CEnvironment::Create(m_pGraphicDev, pInfo->m_szName);

			((CEnvironment*)pGameObject)->SetObjectInfo(&pInfo->m_vPos, &pInfo->m_vScale, &pInfo->m_vAngle);

			Engine::AddObject(pInfo->m_eLayerType, L"Environment", pGameObject);
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

	//===================================
	//È¶ºÒ ¼³Ä¡ÇÏ´Â ÆÄÀÏ...
	pGameObject = NULL;

	dwByte = 0;
	hFile = CreateFile(L"../Bin/Data/Stage_Fire/Fire_Torch.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//Ice Fire (È¶ºÒ ¼³Ä¡)=================================================================
	//ºÒÀÌÆåÆ®
	Engine::CGameObject*	pEffectGameObject = NULL;
	pEffectGameObject = CFire::LoadCreate(m_pGraphicDev, L"RedFire");

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

			//Á¡±¤¿øÀ» ´Þ±â À§ÇÑ À§Ä¡¸¦ ´ã´Â ÄÁÅ×ÀÌ³Ê
			m_listTochs.push_back(pInfo->m_vPos);

			Engine::AddObject(pInfo->m_eLayerType, L"Environment", pGameObject);

		}
		delete pInfo;
		pInfo = NULL;
	}

	((CFire*)pEffectGameObject)->SetInitAnimation();
	Engine::AddObject(Engine::LAYER_ENVIRONMENT, L"Fire_Fire", pEffectGameObject);

	CloseHandle(hFile);

	//¹Ù¸£µð¿¤ Summonµ¿ÀÛ
	pGameObject = CSummon::Create(m_pGraphicDev, L"BardielCummon");
	if (NULL == pGameObject) return E_FAIL;
	Engine::AddObject(Engine::LAYER_GAMELOGIC, L"SoftEffect", pGameObject);



	//Bardiel Effect=====================
	pGameObject = CLava::Create(m_pGraphicDev, L"Bardiel_Lave");
	if (NULL == pGameObject) return E_FAIL;
	Engine::AddObject(Engine::LAYER_GAMELOGIC, L"SoftEffect", pGameObject);

	pGameObject = CFireBall::Create(m_pGraphicDev, L"Bardiel_FireBall");
	if (NULL == pGameObject) return E_FAIL;
	Engine::AddObject(Engine::LAYER_GAMELOGIC, L"SoftEffect", pGameObject);


	pGameObject = CPillar::Create(m_pGraphicDev, L"Bardiel_Pillar");
	if (NULL == pGameObject) return E_FAIL;
	Engine::AddObject(Engine::LAYER_GAMELOGIC, L"SoftEffect", pGameObject);

	pGameObject = CPenalty::Create(m_pGraphicDev, L"Bardiel_Penalty");
	if (NULL == pGameObject) return E_FAIL;
	Engine::AddObject(Engine::LAYER_GAMELOGIC, L"SoftEffect", pGameObject);

	pGameObject = CWhirlWind::Create(m_pGraphicDev, L"Bardiel_WhirlWind");
	if (NULL == pGameObject) return E_FAIL;
	Engine::AddObject(Engine::LAYER_GAMELOGIC, L"SoftEffect", pGameObject);

	pGameObject = CVolcano::Create(m_pGraphicDev, L"Bardiel_Volcano");
	if (NULL == pGameObject) return E_FAIL;
	Engine::AddObject(Engine::LAYER_GAMELOGIC, L"SoftEffect", pGameObject);

	pGameObject = CShortHammer::Create(m_pGraphicDev, L"Bardiel_ShortHammer");
	if (NULL == pGameObject) return E_FAIL;
	Engine::AddObject(Engine::LAYER_GAMELOGIC, L"SoftEffect", pGameObject);

	pGameObject = CLFoot::Create(m_pGraphicDev, L"Bardiel_LFoot");
	if (NULL == pGameObject) return E_FAIL;
	Engine::AddObject(Engine::LAYER_GAMELOGIC, L"SoftEffect", pGameObject);

	pGameObject = CHammer::Create(m_pGraphicDev, L"Bardiel_Hammer");
	if (NULL == pGameObject) return E_FAIL;
	Engine::AddObject(Engine::LAYER_GAMELOGIC, L"SoftEffect", pGameObject);

	pGameObject = CFireWall::Create(m_pGraphicDev, L"Bardiel_FireWall");
	if (NULL == pGameObject) return E_FAIL;
	Engine::AddObject(Engine::LAYER_GAMELOGIC, L"SoftEffect", pGameObject);

	pGameObject = CDivide::Create(m_pGraphicDev, L"Bardiel_Divide");
	if (NULL == pGameObject) return E_FAIL;
	Engine::AddObject(Engine::LAYER_GAMELOGIC, L"SoftEffect", pGameObject);

	pGameObject = CClean::Create(m_pGraphicDev, L"Bardiel_Clean");
	if (NULL == pGameObject) return E_FAIL;
	Engine::AddObject(Engine::LAYER_GAMELOGIC, L"SoftEffect", pGameObject);

	pGameObject = CFireFrame::Create(m_pGraphicDev, L"Bardiel_Frame");
	if (NULL == pGameObject) return E_FAIL;
	Engine::AddObject(Engine::LAYER_GAMELOGIC, L"SoftEffect", pGameObject);

	pGameObject = CChangePhase2::Create(m_pGraphicDev, L"Bardiel_ChangePahse2");
	if (NULL == pGameObject) return E_FAIL;
	Engine::AddObject(Engine::LAYER_GAMELOGIC, L"SoftEffect", pGameObject);
	//===================================

	return S_OK;
}
HRESULT CScene_Stage_Fire::Ready_EffectLight(void)
{		
	D3DLIGHT9	LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	list<_vec3>::iterator iter = m_listTochs.begin();
	list<_vec3>::iterator iter_end = m_listTochs.end();

	for (; iter != iter_end; ++iter)
	{
		LightInfo.Type = D3DLIGHT_POINT;
		LightInfo.Range = 40.0f;

		LightInfo.Position = (*iter) + _vec3(0.0f, 3.0f, 0.0f);

		LightInfo.Diffuse = D3DXCOLOR(1.0f * 3.0f, 0.2705f * 3.0f, 0.0f * 3.0f, 1.0f);
		LightInfo.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		LightInfo.Ambient = D3DXCOLOR(0.001f, 0.001f, 0.001f, 1.0f);

		if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
		++m_iLightCount;
	}

	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Range = 50.0f;
	LightInfo.Position = _vec3(72.0f, -104.0f, 465.0f);
	LightInfo.Diffuse = D3DXCOLOR(0.5019f * 2.0f, 0.0f, 0.0f, 1.0f);
	LightInfo.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	LightInfo.Ambient = D3DXCOLOR(0.001f, 0.001f, 0.001f, 1.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(122.0f, -108.0f, -426.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(72.0f, -115.0f, -344.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(169.0f, -97.0f, -337.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(203.0f, -86.0f, -377.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(218.0f, -85.0f, -466.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(139.0f, -85.0f, -526.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(96.0f, -89.0f, -550.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(221.0f, -27.0f, -314.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	//ÀÔ±¸ ´« 2°³
	LightInfo.Range = 15.0f;
	LightInfo.Diffuse = D3DXCOLOR(1.0f * 2.0f, 0.5490f * 2.0f, 0.0f, 1.0f);
	LightInfo.Position = _vec3(258.0f, -4.0f, -399.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(242.0f, -5.0f, -378.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	//»ï·æÀÌ ´«±ò 6°³
	LightInfo.Range = 10.0f;
	LightInfo.Diffuse = D3DXCOLOR(1.0f * 2.0f, -0.3f * 2.0f, -0.9f * 2.0f, 1.0f);
	LightInfo.Position = _vec3(425.0f, 43.0f, -236.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(440.0f, 43.0f, -212.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(559.0f, 43.0f, -211.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(573.0f, 43.0f, -235.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(503.0f, 43.0f, -370.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(475.0f, 43.0f, -367.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	//»ï·æÀÌ ¾Æ°¡¸® 3°³
	LightInfo.Range = 40.0f;
	LightInfo.Diffuse = D3DXCOLOR(1.0f * 2.0f, 0.5019f * 2.0f, 0.4470f * 2.0f, 1.0f);
	LightInfo.Position = _vec3(569.0f, 27.0f, -222.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(490.0f, 25.0f, -371.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	LightInfo.Position = _vec3(432.0f, 26.0f, -223.0f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	return S_OK;
	
}
HRESULT CScene_Stage_Fire::Ready_Layer_UI(void)
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

HRESULT CScene_Stage_Fire::Ready_Layer_Camera(void)
{
	Engine::DefineCurrentCamera(Engine::CAMERA_PLAYER);
	m_eCameraID = Engine::CAMERA_PLAYER;

	return S_OK;
}

HRESULT CScene_Stage_Fire::Ready_Resource(void)
{

	return S_OK;
}

HRESULT CScene_Stage_Fire::Ready_Prototype(void)
{
	return S_OK;
}

void CScene_Stage_Fire::KeyCheck(const _float& fTimeDelta)
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


CScene_Stage_Fire* CScene_Stage_Fire::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene_Stage_Fire*			pInstance = new CScene_Stage_Fire(pGraphicDev);

	if(FAILED(pInstance->Ready_Scene()))
	{
		MSG_BOX("CScene_Fire Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;	
}

void CScene_Stage_Fire::Free(void)
{
	m_listTochs.clear();
	CAStarManager::GetInstance()->StopThread();
	CAStarManager::GetInstance()->DestroyInstance();
	CCulling_Manager::GetInstance()->StopThread();
	Engine::Clear_All();
	Engine::CScene::Free();
}




