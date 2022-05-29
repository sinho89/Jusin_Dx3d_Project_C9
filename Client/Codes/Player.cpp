#include "stdafx.h"
#include "Player.h"

#include "PlayerCamera.h"

#include "WitchBlade.h"
#include "Mistic.h"
#include "Layer.h"

#include "Map_Ice.h"
#include "Map_Store.h"
#include "Map_Fire.h"

#include "FootPrint.h"
#include "LevelUp.h"
#include "LevelUp_Back.h"
#include "PlayerDamParent.h"
#include "Sound.h"

//클라..

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
: Engine::CGameObject(pGraphicDev, wstrName)
, m_pTransCom(NULL)
, m_pMeshCom(NULL)
, m_pShaderCom(NULL)
, m_pRendererCom(NULL)
, m_pAnimatorCom(NULL)
, m_pCalculatorCom(NULL)
, m_pRigidbodyCom(NULL)
, m_pmapNextMotion(NULL)
, m_pColliderCom(NULL)
, m_pSphereColCom(NULL)
, m_pSoundCom(NULL)
, m_pLightCam(NULL)
, m_pPlayerCamera(NULL)
, m_ppMapMesh(NULL)
, m_bFootPrintLR(FALSE)
, m_iMapSize(0)
, m_iAniIdx(0)
, m_iPlayerAniState(0)
, m_iPlayerSkillState(0)
, m_fFootPrintTime(0.0f)
, m_fAlpha(1.0f)
, m_eSceneType(SCENE_END)
, m_bLevelUp(false)
//, m_iFootPrintCnt(0)
{
	D3DXMatrixIdentity(&m_matOldWorld);
	D3DXMatrixIdentity(&m_matOldView);
	D3DXMatrixIdentity(&m_matOldProj);
	m_bIsStatic = true;

	//for (int i = 0; i < 2; ++i)
	//	m_bOneTwoStep[i] = FALSE;
}

CPlayer::~CPlayer(void)
{

}

PLAYER_INFO* CPlayer::GetPlayerInfo(void)
{
	return &m_tPlayerInfo;
}

const vector<Engine::ANIINFO*>* CPlayer::Get_AniInfo(void) const
{
	return &m_vecAniInfo;
}

Engine::CAnimator * CPlayer::GetAnimatorInfo(void)
{
	return m_pAnimatorCom;
}

_uint CPlayer::GetStateNum(void)
{
	return m_pAnimatorCom->Get_State();
}

void CPlayer::SetPlayerInfo(PLAYER_INFO* pPlayerInfo)
{
	m_tPlayerInfo = *pPlayerInfo;
}

void CPlayer::Set_SceneType(const SCENE_TYPE & eType)
{
	switch (eType)
	{
	case SCENE_TOWN:
		m_pAnimatorCom->Set_State(PLAYER_TOWN);
		break;

	case SCENE_DUNGEON:
		m_pAnimatorCom->Set_State(PLAYER_FIGHT);
		break;
	}
}

void CPlayer::Camera_MoveFoward(const _float & fTimeDelta)
{
	m_pPlayerCamera->MoveFoward(fTimeDelta);
}

void CPlayer::Camera_MoveReturn(const _float & fTimeDelta)
{
	m_pPlayerCamera->MoveReturn(fTimeDelta);
}

void CPlayer::Set_CameraAction(const _bool & bAction)
{
	m_pPlayerCamera->Set_Action(bAction);
}

HRESULT CPlayer::Start_ForScene(void)
{
	Engine::CGameObject* pGameObject = Engine::FindWithTag(L"Map");
	if(pGameObject == NULL)
		return E_FAIL;

	m_pCalculatorCom->Clear_Map();

	if (m_ppMapMesh != NULL)
	{
		for (int i = 0; i < m_iMapSize; ++i)
			m_ppMapMesh[i] = NULL;
	}

	if(dynamic_cast<CMap_Store*>(pGameObject) != NULL)
	{
		//_int iSize = 0;
		//Engine::CStaticMesh** ppMesh = ((CMap_Store*)pGameObject)->GetMapMesh(&iSize);
		m_iMapSize = 0;
		m_ppMapMesh = ((CMap_Store*)pGameObject)->GetMapMesh(&m_iMapSize);
		
		for(_int i = 0; i < m_iMapSize; i++)
		{
			m_pCalculatorCom->Set_TargetMesh(m_ppMapMesh[i]->Get_Mesh());
		}
	}

	if(dynamic_cast<CMap_Ice*>(pGameObject) != NULL)
	{
		//_int iSize = 0;
		//Engine::CStaticMesh** ppMesh = ((CMap_Ice*)pGameObject)->GetMapMesh(&iSize);
		m_iMapSize = 0;
		m_ppMapMesh = ((CMap_Ice*)pGameObject)->GetMapMesh(&m_iMapSize);

		for(_int i = 0; i < m_iMapSize; i++)
		{
			m_pCalculatorCom->Set_TargetMesh(m_ppMapMesh[i]->Get_Mesh());
		}
	}

	if (dynamic_cast<CMap_Fire*>(pGameObject) != NULL)
	{
		//_int iSize = 0;
		//Engine::CStaticMesh** ppMesh = ((CMap_Fire*)pGameObject)->GetMapMesh(&iSize);
		m_iMapSize = 0;
		m_ppMapMesh = ((CMap_Fire*)pGameObject)->GetMapMesh(&m_iMapSize);
		
		for (_int i = 0; i < m_iMapSize; i++)
		{
			m_pCalculatorCom->Set_TargetMesh(m_ppMapMesh[i]->Get_Mesh());
		}
	}

	//m_pLogicLayer = Engine::FindLayer(Engine::LAYER_GAMELOGIC);
	m_bPlayerMoveState = true;
	Engine::Safe_Release(pGameObject);
	m_tPlayerInfo.m_bHit = false;
	m_tPlayerInfo.m_fHitAngle = 0.f;

	// UI Dam
	Engine::CGameObject* pDamParent = CPlayerDamParent::Create(m_pGraphicDev, L"Dam_Parent", this);
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_Dam", pDamParent);

	m_pDamParent = ((CUI*)pDamParent);
		
	return S_OK;
}

void CPlayer::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);
	PlayerInfoUpdate();
	LevelUp();
	Engine::Add_ColliderGroup(Engine::COL_BOX, L"Player", m_pColliderCom);

	if (m_tPlayerInfo.m_bHit)
		((CPlayerDamParent*)m_pDamParent)->SetDamageNumber(_uint(m_tPlayerInfo.m_fHitDam), false);
	
	//발자국
	/*if (Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
	{
		RunF
		if (m_pAnimatorCom->IsPlaying_ThisSection(m_vecAniInfo[CWitchBlade::WITCH_RUNF_IN]->tMotionInfo.dEndTime * 0.0,
			m_vecAniInfo[CWitchBlade::WITCH_RUNF_IN]->tMotionInfo.dEndTime * 0.1) && (!m_bOneTwoStep[0]))
		{
			m_bOneTwoStep[0] = TRUE;
			DrawPrintFoot(0);
		}
		else
		{
			m_bOneTwoStep[0] = FALSE;
		}

		if (m_pAnimatorCom->IsPlaying_ThisSection(m_vecAniInfo[CWitchBlade::WITCH_RUNF_IN]->tMotionInfo.dEndTime * 0.5,
			m_vecAniInfo[CWitchBlade::WITCH_RUNF_IN]->tMotionInfo.dEndTime * 0.6) && (!m_bOneTwoStep[1]))
		{
			m_bOneTwoStep[1] = TRUE;
			DrawPrintFoot(1);
		}
		else
		{
			m_bOneTwoStep[1] = FALSE;
		}

		RunB
		if (m_pAnimatorCom->IsPlaying_ThisSection(m_vecAniInfo[CWitchBlade::WITCH_RUNB_IN]->tMotionInfo.dEndTime * 0.0,
			m_vecAniInfo[CWitchBlade::WITCH_RUNB_IN]->tMotionInfo.dEndTime * 0.1) && (!m_bOneTwoStep[0]))
		{
			m_bOneTwoStep[0] = TRUE;
			DrawPrintFoot(0);
		}
		else
		{
			m_bOneTwoStep[0] = FALSE;
		}

		if (m_pAnimatorCom->IsPlaying_ThisSection(m_vecAniInfo[CWitchBlade::WITCH_RUNB_IN]->tMotionInfo.dEndTime * 0.5,
			m_vecAniInfo[CWitchBlade::WITCH_RUNB_IN]->tMotionInfo.dEndTime * 0.6) && (!m_bOneTwoStep[1]))
		{
			m_bOneTwoStep[1] = TRUE;
			DrawPrintFoot(1);
		}
		else
		{
			m_bOneTwoStep[1] = FALSE;
		}

		RunL
		if (m_pAnimatorCom->IsPlaying_ThisSection(m_vecAniInfo[CWitchBlade::WITCH_RUNL_IN]->tMotionInfo.dEndTime * 0.0,
			m_vecAniInfo[CWitchBlade::WITCH_RUNL_IN]->tMotionInfo.dEndTime * 0.1) && (!m_bOneTwoStep[0]))
		{
			m_bOneTwoStep[0] = TRUE;
			DrawPrintFoot(0);
		}
		else
		{
			m_bOneTwoStep[0] = FALSE;
		}

		if (m_pAnimatorCom->IsPlaying_ThisSection(m_vecAniInfo[CWitchBlade::WITCH_RUNL_IN]->tMotionInfo.dEndTime * 0.5,
			m_vecAniInfo[CWitchBlade::WITCH_RUNL_IN]->tMotionInfo.dEndTime * 0.6) && (!m_bOneTwoStep[1]))
		{
			m_bOneTwoStep[1] = TRUE;
			DrawPrintFoot(1);
		}
		else
		{
			m_bOneTwoStep[1] = FALSE;
		}

		RunR
		if (m_pAnimatorCom->IsPlaying_ThisSection(m_vecAniInfo[CWitchBlade::WITCH_RUNR_IN]->tMotionInfo.dEndTime * 0.0,
			m_vecAniInfo[CWitchBlade::WITCH_RUNR_IN]->tMotionInfo.dEndTime * 0.1) && (!m_bOneTwoStep[0]))
		{
			m_bOneTwoStep[0] = TRUE;
			DrawPrintFoot(0);
		}
		else
		{
			m_bOneTwoStep[0] = FALSE;
		}

		if (m_pAnimatorCom->IsPlaying_ThisSection(m_vecAniInfo[CWitchBlade::WITCH_RUNR_IN]->tMotionInfo.dEndTime * 0.5,
			m_vecAniInfo[CWitchBlade::WITCH_RUNR_IN]->tMotionInfo.dEndTime * 0.6) && (!m_bOneTwoStep[1]))
		{
			m_bOneTwoStep[1] = TRUE;
			DrawPrintFoot(1);
		}
		else
		{
			m_bOneTwoStep[1] = FALSE;
		}
	}*/
}

void CPlayer::Render_Object(void)
{
	
}

void CPlayer::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
{
}

void CPlayer::LevelUp(void)
{
	while (true)
	{
		if (_int(m_tPlayerInfo.m_fExp) < _int(_int(200.f) * m_tPlayerInfo.m_iLevel))
			break;
		m_tPlayerInfo.m_fExp -= (_int(200.f) * m_tPlayerInfo.m_iLevel);
		m_tPlayerInfo.m_iLevel += 1;
		m_bLevelUp = true;
	}
	if (m_bLevelUp)
	{
		m_tPlayerInfo.m_iStr += 8;
		m_tPlayerInfo.m_iInt += 7;
		m_tPlayerInfo.m_iWis += 7;
		m_tPlayerInfo.m_iCon += 5;

		m_tPlayerInfo.m_iLeftAtt += _int(_float(8 * (rand() % 10) * 3));
		m_tPlayerInfo.m_iRightAtt += _int(_float(8 * (rand() % 10) * 3));
		m_tPlayerInfo.m_iPhysicalDefense += _int(_float(5 * (rand() % 10)) * 0.3f);
		m_tPlayerInfo.m_iMegicalDefense += _int(_float(5 * (rand() % 10)) * 0.3f);
		m_tPlayerInfo.m_fCritical += (_float)((rand() % 5) + 2) * 0.1f;
		m_tPlayerInfo.m_fConcentration += (_float)((rand() % 5) + 2) * 0.5f;
		m_tPlayerInfo.m_fCriticalResist += (_float)((rand() % 5) + 2) * 0.5f;
		m_tPlayerInfo.m_fGlancingArmor += (_float)((rand() % 2) + 1) * 0.1f;
		m_tPlayerInfo.m_fHitAaccuracy += (_float)((rand() % 5) + 5) * 0.5f;
		m_tPlayerInfo.m_fHitResist += (_float)((rand() % 5) + 2) * 0.4f;
		m_tPlayerInfo.m_fFireDef += (_float)((rand() % 2) + 1) * 0.1f;
		m_tPlayerInfo.m_fSkyDef += (_float)((rand() % 2) + 1) * 0.1f;
		m_tPlayerInfo.m_fWaterDef += (_float)((rand() % 2) + 1) * 0.1f;
		m_tPlayerInfo.m_fLandDef += (_float)((rand() % 2) + 1) * 0.1f;
		m_tPlayerInfo.m_fFireAtt += (_float)((rand() % 2) + 1) * 0.1f;
		m_tPlayerInfo.m_fSkyAtt += (_float)((rand() % 2) + 1) * 0.1f;
		m_tPlayerInfo.m_fWaterAtt += (_float)((rand() % 2) + 1) * 0.1f;
		m_tPlayerInfo.m_fLandDef += (_float)((rand() % 2) + 1) * 0.1f;
		
		m_tPlayerInfo.m_iMaxHp += 5 * ((rand() % 5) * 10);
		m_tPlayerInfo.m_iMaxMp += 3 * ((rand() % 5) * 10);
		m_tPlayerInfo.m_iHp = m_tPlayerInfo.m_iMaxHp;
		m_tPlayerInfo.m_iMp = m_tPlayerInfo.m_iMaxMp;

		Engine::CGameObject*pGameObject = CLevelUp_Back::Create(m_pGraphicDev, L"Level_Eff_Back");
		Engine::AddObject(Engine::LAYER_UI, L"UI_Eff", pGameObject);

		pGameObject = CLevelUp::Create(m_pGraphicDev, L"Level_Eff");
		Engine::AddObject(Engine::LAYER_UI, L"UI_Eff", pGameObject);

		// Sound
		m_pSoundCom->MyPlaySound(L"LevelUp");
		m_bLevelUp = false;
	}
}

HRESULT CPlayer::LoadAniInfo(const _tchar* pFileName)
{
	if (pFileName == NULL)
		return E_FAIL;

	_tchar szFullPath[MAX_PATH] = L"";
	lstrcpy(szFullPath, L"../Bin/Data/PlayerAnimationData/");
	lstrcat(szFullPath, pFileName);
	lstrcat(szFullPath, L".dat");


	DWORD dwByte = 0;
	HANDLE hFile = CreateFile(szFullPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == NULL)
		return E_FAIL;

	_uint iMaxAniCnt = 0;
	_uint iContainerSize = 0;

	ReadFile(hFile, &iMaxAniCnt, sizeof(_uint), &dwByte, NULL);
	ReadFile(hFile, &iContainerSize, sizeof(_uint), &dwByte, NULL);

	if (iMaxAniCnt == 0)
		return E_FAIL;

	m_vecAniInfo.reserve(iMaxAniCnt);
	m_pmapNextMotion = new	map<_uint, list<Engine::ANIMATORINFO*>>[iContainerSize];

	for (_uint i = 0; i < iMaxAniCnt; i++)
	{
		Engine::ANIINFO* pAniInfo = new Engine::ANIINFO;
		ReadFile(hFile, &pAniInfo->tMotionInfo, sizeof(Engine::MOTIONINFO), &dwByte, NULL);
		_uint iEventSize = 0;
		ReadFile(hFile, &iEventSize, sizeof(_uint), &dwByte, NULL);
		pAniInfo->pMotionEventInfo = new Engine::MOTIONEVENTINFO[iEventSize];
		for (_uint j = 0; j < iEventSize; j++)
		{
			ReadFile(hFile, &pAniInfo->pMotionEventInfo[j], sizeof(Engine::MOTIONEVENTINFO), &dwByte, NULL);
		}
		for (_uint j = 0; j < iContainerSize; j++)
		{
			list<Engine::ANIMATORINFO*> listAnimator;
			_uint iNextMotionSize = 0;
			ReadFile(hFile, &iNextMotionSize, sizeof(_uint), &dwByte, NULL);
			for (_uint k = 0; k < iNextMotionSize; k++)
			{
				Engine::ANIMATORINFO* pAnimatorInfo = new Engine::ANIMATORINFO;
				ReadFile(hFile, pAnimatorInfo, sizeof(Engine::ANIMATORINFO), &dwByte, NULL);
				listAnimator.push_back(pAnimatorInfo);
			}
			m_pmapNextMotion[j].insert(map<_uint, list<Engine::ANIMATORINFO*>>::value_type(i, listAnimator));
		}
		pAniInfo->pUse = new _bool[iContainerSize];
		for (_uint j = 0; j < iContainerSize; j++)
		{
			ReadFile(hFile, &pAniInfo->pUse[j], sizeof(_bool), &dwByte, NULL);
		}

		m_vecAniInfo.push_back(pAniInfo);
	}
	
	CloseHandle(hFile);

	return S_OK;
}

HRESULT CPlayer::LoadCollider(const _tchar* pFileName)
{
	if(pFileName == NULL)
		return E_FAIL;

	_tchar szFullPath[MAX_PATH] = L"";
	lstrcpy(szFullPath, L"../Bin/Data/PlayerColliderData/");
	lstrcat(szFullPath, pFileName);
	lstrcat(szFullPath, L".dat");

	DWORD dwByte = 0;
	HANDLE hFile = CreateFile(szFullPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile == NULL)
		return E_FAIL;

	Engine::COL_TYPE eType;
	_vec3 vPos = _vec3(0.f, 0.f, 0.f);
	_vec3 vScale = _vec3(1.f, 1.f, 1.f);
	_float fRad = 1.f;

	ReadFile(hFile, &eType, sizeof(Engine::COL_TYPE), &dwByte, NULL);
	ReadFile(hFile, &vPos, sizeof(_vec3), &dwByte, NULL);

	Engine::CComponent* pComponent = NULL;
	switch(eType)
	{
	case Engine::COL_SPHERE:
		ReadFile(hFile, &fRad, sizeof(_float), &dwByte, NULL);
		pComponent = Engine::Create_SphereCollider(m_pGraphicDev, m_pTransCom, L"Player", vPos, fRad);
		if(pComponent == NULL)
			return E_FAIL;
		m_pColliderCom = (Engine::CSphereCollider*)pComponent;
		m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Collider", pComponent));
		break;

	case Engine::COL_BOX:
		ReadFile(hFile, &vScale, sizeof(_vec3), &dwByte, NULL);
		pComponent = Engine::Create_BoxCollider(m_pGraphicDev, m_pTransCom, L"Player", vPos, vScale, D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
		if(pComponent == NULL)
			return E_FAIL;
		m_pColliderCom = (Engine::CBoxCollider*)pComponent;
		m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Collider", pComponent));
		break;
	}

	CloseHandle(hFile);
	return S_OK;
}

void CPlayer::Compute_CameraLook(const _vec3* pTargetPos)
{
	_vec3 vPos;
	m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &vPos);

	_vec3 vDir = vPos - (*pTargetPos);
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);

	_vec3			vDirection;
	_matrix			matWorld;
	m_pTransCom->Get_WorldMatrix(&matWorld);
	memcpy(&vDirection, &matWorld.m[2][0], sizeof(_vec3));
	D3DXVec3Normalize(&vDirection, &vDirection);

	_vec3 vUp = _vec3(0.f, 1.f, 0.f);
	_vec3 vRight;

	D3DXVec3Cross(&vRight, &vUp, &vDirection);
	D3DXVec3Normalize(&vRight, &vRight);
	float fCos = D3DXVec3Dot(&vRight, &vDir);
	float fRealCos = D3DXVec3Dot(&vDirection, &vDir);
	float fAngle = acosf(fRealCos);

	fAngle = D3DXToDegree(fAngle);

	if(fAngle > 180.f)
		fAngle = 360.f - fAngle;

	if(fCos >= 0.001f)
		m_pTransCom->Rotation(Engine::ROT_Y, fAngle);
	else if(fCos < -0.001f)
		m_pTransCom->Rotation(Engine::ROT_Y, -fAngle);
}

void CPlayer::PlayerInfoUpdate()
{
	m_tPlayerInfo.m_iLeftMinAtt = m_tPlayerInfo.m_iLeftAtt - (m_tPlayerInfo.m_iLeftAtt / 10 * 3);
	m_tPlayerInfo.m_iLeftMaxAtt = m_tPlayerInfo.m_iLeftAtt + (m_tPlayerInfo.m_iLeftAtt / 10 * 5);

	m_tPlayerInfo.m_iRightMinAtt = m_tPlayerInfo.m_iRightAtt - (m_tPlayerInfo.m_iRightAtt / 10 * 3);
	m_tPlayerInfo.m_iRightMaxAtt = m_tPlayerInfo.m_iRightAtt + (m_tPlayerInfo.m_iRightAtt / 10 * 5);

	m_tPlayerInfo.m_iTotalAtt = (m_tPlayerInfo.m_iLeftAtt + m_tPlayerInfo.m_iRightAtt) / 2;
	m_tPlayerInfo.m_iTotalMinAtt = m_tPlayerInfo.m_iTotalAtt - (m_tPlayerInfo.m_iTotalAtt / 10 * 3);
	m_tPlayerInfo.m_iTotalMaxAtt = m_tPlayerInfo.m_iTotalAtt + (m_tPlayerInfo.m_iTotalAtt / 10 * 5);

	if (m_tPlayerInfo.m_iHp <= 0)
		m_tPlayerInfo.m_iHp = m_tPlayerInfo.m_iMaxHp;
	if (m_tPlayerInfo.m_iMp <= 0)
		m_tPlayerInfo.m_iMp = m_tPlayerInfo.m_iMaxMp;

}

void CPlayer::Free(void)
{
	_uint iSize = m_vecAniInfo.size();
	for (_uint i = 0; i < iSize; i++)
	{
		Engine::Safe_Delete_Array(m_vecAniInfo[i]->pMotionEventInfo);
		Engine::Safe_Delete_Array(m_vecAniInfo[i]->pUse);
		Engine::Safe_Delete(m_vecAniInfo[i]);
	}
	m_vecAniInfo.clear();
	for (_uint i = 0; i < PLAYER_END; i++)
	{		
		map<_uint, list<Engine::ANIMATORINFO*>>::iterator iter = m_pmapNextMotion[i].begin();
		map<_uint, list<Engine::ANIMATORINFO*>>::iterator iter_end = m_pmapNextMotion[i].end();

		for (iter; iter != iter_end; iter++)
		{
			list<Engine::ANIMATORINFO*>::iterator iter_list = iter->second.begin();
			list<Engine::ANIMATORINFO*>::iterator iter_list_end = iter->second.end();
			for (iter_list; iter_list != iter_list_end; iter_list++)
			{
				Engine::Safe_Delete((*iter_list));
			}
		}
		m_pmapNextMotion[i].clear();
	}

	Engine::Safe_Delete_Array(m_pmapNextMotion);

	/*if (m_ppMapMesh != NULL)
	{
		for (int i = 0; i<m_iMapSize; ++i)
			::Safe_Release(m_ppMapMesh[i]);
	}*/

	Engine::CGameObject::Free();
}

//void CPlayer::DrawPrintFoot(_int iLeftRight)
//{
//	++m_iFootPrintCnt;
//
//	_vec3 vAngle = _vec3(90.0f, m_pTransCom->Get_Now_Angle().y, 0.0f);//_vec3(-90.0f, 0.0f, 0.0f);
//	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);
//
//	if (0 == iLeftRight)
//	{
//		vPos = m_pTransCom->Get_Now_Position() + _vec3(-0.5f, 0.0f, 0.0f);
//	}
//	else if (1 == iLeftRight)
//	{
//		vPos = m_pTransCom->Get_Now_Position() + _vec3(0.5f, 0.0f, 0.0f);
//	}
//
//	Engine::CGameObject*	pGameObject = CFootPrint::Create(m_pGraphicDev, L"FootPrintDecal", vPos, vAngle, 0);
//	if (NULL == pGameObject) return;
//
//	Engine::AddObject(Engine::LAYER_GAMELOGIC, L"FootPrintDecal", pGameObject);
//	//m_pLogicLayer->AddObject(L"FootPrintDecal", pGameObject);	
//}