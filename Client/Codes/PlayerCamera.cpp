#include "stdafx.h"
#include "PlayerCamera.h"
#include "Export_Engine.h"

#include "WitchBlade.h"
#include "Transform.h"
#include "UI.h"
#include "Cursor.h"
#include "Main_Base.h"
#include "Main_Block.h"
#include "UI_Skill.h"
#include "UI_Item.h"
#include "Map_Ice.h"
#include "Map_Store.h"
#include "Mistic.h"
#include "Map_Fire.h"
#include "Hanuman.h"
#include "Fade_Manager.h"
#include "CoolTime.h"
#include "Sound.h"
#include "MouseEff.h"
#include "Boss_Base.h"
#include "Bardiel.h"


CPlayerCamera::CPlayerCamera(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
	: Engine::CCamera(pGraphicDev, wstrName)
	, m_fCamSpeed(0.0f)
	, m_vCamDir(0.0f, 0.0f, -1.0f)
	, m_vAngle(0.f, 0.f, 0.f)
	, m_fEpslnY(0.0f)
	, m_fFadeTime(0.f)
	//	, m_fGap(0.0f)
	, m_bIsMove(true)
	, m_pPlayerTrans(NULL)
	, m_pCursor(NULL)
	, m_pSkill(NULL)
	, m_pInven(NULL)
	, m_pState(NULL)
	, m_pOption(NULL)
	, m_pMain(NULL)
	, m_pPartition(NULL)
	, m_pEsc(NULL)
	, m_pSystem(NULL)
	, m_pDunGeon(NULL)
	, m_pWeaponShop(NULL)
	, m_pArmorShop(NULL)
	, m_pAccShop(NULL)
	, m_pPotionShop(NULL)
	, m_pBuy(NULL)
	, m_pActionTarget(NULL)
	, m_fOldGap(0.f)
	, m_bIsAction(false)
	, m_bActionEnd(false)
{

}

CPlayerCamera::~CPlayerCamera(void)
{

}

const _vec3* CPlayerCamera::Get_Pos(void)
{
	return &m_vEye;
}

_bool CPlayerCamera::Get_Move(void)
{
	return m_bIsMove;
}

void CPlayerCamera::MoveFoward(const _float& fTimeDelta)
{
	if (m_fOldGap == 0.f)
		m_fOldGap = m_fGap;

	if (m_fGap <= 3.f)
	{
		m_fGap = 3.f;
		return;
	}

	m_fGap += -30.f * fTimeDelta;
}

void CPlayerCamera::MoveReturn(const _float & fTimeDelta)
{
	if (m_fOldGap == 0.f)
		return;

	if (m_fGap > m_fOldGap)
	{
		m_fGap = m_fOldGap;
		m_fOldGap = 0.f;
		return;
	}
	m_fGap += 50.f * fTimeDelta;
}

void CPlayerCamera::Set_Move(void)
{
	if (!m_bIsMove && !m_pDunGeon->GetRender() /*&& !m_pWeaponShop->GetRender()
											   && !m_pArmorShop->GetRender()
											   && !m_pAccShop->GetRender()
											   && !m_pPotionShop->GetRender()*/)
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"UI_Option")->begin();
		list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"UI_Option")->end();

		for (; iter != iter_end; ++iter)
			((CUI*)*iter)->SetRender(true);

		m_pSkill->SetRender(false);
		m_pInven->SetRender(false);
		m_pState->SetRender(false);
		m_pPartition->SetRender(false);
		m_pEsc->SetRender(false);
		m_pSystem->SetRender(false);
		m_pWeaponShop->SetRender(false);
		m_pArmorShop->SetRender(false);
		m_pAccShop->SetRender(false);
		m_pPotionShop->SetRender(false);
		m_pBuy->SetRender(false);
		m_bIsMove = true;
	}
}

void CPlayerCamera::Set_Action(const _bool & bAction)
{
	if (bAction)
	{
		m_fOldGap = m_fGap;

		if (Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
		{
			m_fGap = 60.f;
			m_fEpslnY = 7.f;
			Engine::CGameObject* pGameObject = NULL;
			pGameObject = Engine::Find(L"Mesh_Hanuman");
			((CHanuman*)pGameObject)->Set_CameraStart(true);
			m_pActionTarget = (Engine::CTransform*)pGameObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

			Engine::Safe_Release(pGameObject);
			Engine::Safe_Release(m_pActionTarget);
		}
		if (Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE)
		{
			m_fGap = 100.f;
			m_fEpslnY = 15.f;
			Engine::CGameObject* pGameObject = NULL;
			pGameObject = Engine::Find(L"Mesh_Bardiel");
			((CBardiel*)pGameObject)->Set_CameraStart(true);
			m_pActionTarget = (Engine::CTransform*)pGameObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

			Engine::Safe_Release(pGameObject);
			Engine::Safe_Release(m_pActionTarget);
		}
	}
	else
	{
		m_fGap = m_fOldGap;
		m_fEpslnY = 5.f;
		m_pActionTarget = NULL;
	}

	m_bIsAction = bAction;
}

void CPlayerCamera::Set_ActionEnd(void)
{
	m_bActionEnd = true;
}

void CPlayerCamera::SetPlayBGM(const wstring & wstrKey)
{
	m_pSoundCom->MyStopSoundAll();
	m_pSoundCom->MyPlaySound(wstrKey, true);
}

void CPlayerCamera::Reset_CashingUI(void)
{
	m_pDunGeon = ((CUI*)Engine::Find(L"Dungeon_Base"));
	m_pWeaponShop = ((CUI*)Engine::Find(L"Shop_Base_Weapon"));
	m_pArmorShop = ((CUI*)Engine::Find(L"Shop_Base_Armor"));
	m_pAccShop = ((CUI*)Engine::Find(L"Shop_Base_Acc"));
	m_pPotionShop = ((CUI*)Engine::Find(L"Shop_Base_Potion"));
	::Safe_Release(m_pDunGeon);
	::Safe_Release(m_pWeaponShop);
	::Safe_Release(m_pArmorShop);
	::Safe_Release(m_pAccShop);
	::Safe_Release(m_pPotionShop);
}
void CPlayerCamera::Reset_Scene_Change(void)
{
	list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"UI_Option")->begin();
	list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"UI_Option")->end();

	for (; iter != iter_end; ++iter)
		((CUI*)*iter)->SetRender(true);

	m_pCursor->SetRender(true);
	m_pSkill->SetRender(false);
	m_pInven->SetRender(false);
	m_pState->SetRender(false);
	m_pPartition->SetRender(false);
	m_pEsc->SetRender(false);
	m_pSystem->SetRender(false);

	m_bIsMove = true;
}

HRESULT CPlayerCamera::InitCamera(void)
{
	m_pPlayer = static_cast<CWitchBlade*>(Engine::Find(L"Player"));
	m_pCursor = ((CCursor*)((CUI*)Engine::Find(L"Cursor")));
	m_pSkill = ((CUI*)Engine::Find(L"Skill_Base"));
	m_pInven = ((CUI*)Engine::Find(L"Inven_Base"));
	m_pState = ((CUI*)Engine::Find(L"Window_Info"));
	m_pOption = ((CUI*)Engine::Find(L"Option_Base"));
	m_pMain = ((CUI*)Engine::Find(L"Main_Base"));
	m_pPartition = ((CUI*)Engine::Find(L"Partition_Base"));
	m_pEsc = ((CUI*)Engine::Find(L"Esc_Base"));
	m_pSystem = ((CUI*)Engine::Find(L"System_Base"));
	m_pDunGeon = ((CUI*)Engine::Find(L"Dungeon_Base"));
	m_pWeaponShop = ((CUI*)Engine::Find(L"Shop_Base_Weapon"));
	m_pArmorShop = ((CUI*)Engine::Find(L"Shop_Base_Armor"));
	m_pAccShop = ((CUI*)Engine::Find(L"Shop_Base_Acc"));
	m_pPotionShop = ((CUI*)Engine::Find(L"Shop_Base_Potion"));
	m_pBuy = ((CUI*)Engine::Find(L"Buy_Base"));
	::Safe_Release(m_pPlayer);
	::Safe_Release(m_pPlayerTrans);
	::Safe_Release(m_pCursor);
	::Safe_Release(m_pSkill);
	::Safe_Release(m_pInven);
	::Safe_Release(m_pState);
	::Safe_Release(m_pOption);
	::Safe_Release(m_pMain);
	::Safe_Release(m_pPartition);
	::Safe_Release(m_pEsc);
	::Safe_Release(m_pSystem);
	::Safe_Release(m_pDunGeon);
	::Safe_Release(m_pWeaponShop);
	::Safe_Release(m_pArmorShop);
	::Safe_Release(m_pAccShop);
	::Safe_Release(m_pPotionShop);
	::Safe_Release(m_pBuy);

	m_fEpslnY = 5.0f;
	m_fGap = 19.0f;
	m_fActionAngleForHanuman = 20.f;
	m_fCamSpeed = 100.f;
	m_fFadeTime = 0.f;
	m_bFadeOut = false;
	m_bFadeIn = false;
	m_bUsingSkill = false;
	m_fSkillCoolTime = 0.f;
	m_fInvSpeed = 2.f;
	m_bActionUI = false;
	m_fFadeInTime = 0.f;

	_vec3	vPlayerPos = _vec3(0.0f, 0.0f, 0.0f);
	m_pPlayerTrans = static_cast<Engine::CTransform*>(m_pPlayer->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC));
	m_pPlayerTrans->Get_Infomation(Engine::INFO_POSITION, &vPlayerPos);

	m_vEye = vPlayerPos + (m_vCamDir * m_fGap) + _vec3(0.0f, m_fEpslnY, 0.0f);
	m_vAt = vPlayerPos + _vec3(0.0f, m_fEpslnY, 0.0f);

	m_fFovy = D3DXToRadian(60.0f);
	m_fAspect = _float(WINSIZEX) / _float(WINSIZEY);
	m_fNear = 0.1f;

	if (Engine::GetSceneID() == Engine::SCENE_TOWN)
	{
		m_fFar = 200.0f;
		if(!m_pSoundCom->IsPlaying(L"Town"))
			m_pSoundCom->MyStopSoundAll();
		m_pSoundCom->MyPlaySound(L"Town", true);
		m_pSoundCom->MyPlaySound(L"Fire", true);
		m_pSoundCom->MyPlaySound(L"Indoor", true);
		m_pSoundCom->Update_Component(0.f);
	}
	else if (Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
	{
		m_fFar = 450.0f;
		m_pSoundCom->MyStopSoundAll();
		m_pSoundCom->MyPlaySound(L"Stage1", true);
		m_pSoundCom->MyPlaySound(L"Water", true);
		m_pSoundCom->Update_Component(0.f);
	}
	else if (Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE)
	{
		m_fFar = 600.0f;
		m_pSoundCom->MyStopSoundAll();
		m_pSoundCom->MyPlaySound(L"Stage2", true);
		m_pSoundCom->MyPlaySound(L"Bet", true);
		m_pSoundCom->Update_Component(0.f);
	}
	
	D3DXQuaternionIdentity(&m_Quaternion);
	D3DXMatrixIdentity(&m_matWorld);

	for (int i = 0; i < UI_KEY_END; ++i)
		m_bPush[i] = false;
	for (int i = 0; i < 18; ++i)
		m_bSlotPush[i] = false;

	Engine::CCamera::Ready_Object();

	return S_OK;
}

void CPlayerCamera::Loading_Update(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);
}

HRESULT CPlayerCamera::Start_ForScene(void)
{
	InitCamera();

	Engine::CGameObject* pGameObject = Engine::FindWithTag(L"Map");
	if (pGameObject == NULL)
		return E_FAIL;

	m_mapMesh.clear();

	if (dynamic_cast<CMap_Store*>(pGameObject) != NULL)
	{
		_int iSize = 0;
		Engine::CStaticMesh** ppMesh = ((CMap_Store*)pGameObject)->GetMapMesh(&iSize);

		for (_int i = 0; i < iSize; i++)
		{
			m_mapMesh.insert(MAPMESH::value_type(i, ppMesh[i]->Get_Mesh()));
		}

		m_iMeshSize = iSize;
	}

	if (dynamic_cast<CMap_Ice*>(pGameObject) != NULL)
	{
		_int iSize = 0;
		Engine::CStaticMesh** ppMesh = ((CMap_Ice*)pGameObject)->GetMapMesh(&iSize);

		for (_int i = 0; i < iSize; i++)
		{
			m_mapMesh.insert(MAPMESH::value_type(i, ppMesh[i]->Get_Mesh()));
		}

		m_iMeshSize = iSize;
	}

	if (dynamic_cast<CMap_Fire*>(pGameObject) != NULL)
	{
		_int iSize = 0;
		Engine::CStaticMesh** ppMesh = ((CMap_Fire*)pGameObject)->GetMapMesh(&iSize);

		for (_int i = 0; i < iSize; i++)
		{
			m_mapMesh.insert(MAPMESH::value_type(i, ppMesh[i]->Get_Mesh()));
		}

		m_iMeshSize = iSize;
	}

	Engine::Safe_Release(pGameObject);

	D3DXQuaternionIdentity(&m_Quaternion);
	D3DXMatrixIdentity(&m_matWorld);
	m_vAngle = _vec3(0.f, 0.f, 0.f);
	m_vCamDir = _vec3(0.f, 0.f, -1.f);

	return S_OK;
}

void CPlayerCamera::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);
	if (m_bUsingSkill)
	{
		m_fSkillCoolTime += fTimeDelta;

		if (m_fSkillCoolTime >= 5.f)
		{
			m_bUsingSkill = false;
			m_fSkillCoolTime = 0.f;
		}
	}
	if (m_bIsAction)
	{
		/*m_fFadeInTime += fTimeDelta;

		if (m_fFadeInTime >= 1.f)
			CFade_Manager::GetInstance()->SetFading(true);

		if (!m_bFadeIn)
		{
			CFade_Manager::GetInstance()->FadeImmidy(0.75f, 0.075f);
			m_bFadeIn = true;
		}*/
		if(Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
			CameraAction(fTimeDelta);
		if (Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE)
			FireAction(fTimeDelta);
	}

	if (!m_bIsAction)
	{
		if (m_bIsMove)
		{
			MouseMove(fTimeDelta);
			FixMouse();
		}

		//임시 예외
		if (Engine::SCENE_STAGE_ICE == Engine::GetSceneID()
			|| Engine::SCENE_STAGE_FIRE == Engine::GetSceneID()
			|| Engine::SCENE_TOWN == Engine::GetSceneID())
		{
			Set_CtrlKey_Check();
			Set_SkillKey_Check();
			Set_InvenKey_Check();
			Set_StateKey_Check();
			Set_ESCKey_Check();
			Set_MainSlot_KeyCheck(fTimeDelta);

			Set_Mouse_Active_Check();
		}

		_vec3	vPlayerPos = _vec3(0.0f, 0.0f, 0.0f);
		m_pPlayerTrans->Get_Infomation(Engine::INFO_POSITION, &vPlayerPos);

		m_vEye = vPlayerPos + (m_vCamDir * m_fGap) + _vec3(0.0f, m_fEpslnY, 0.0f);
		m_vAt = vPlayerPos + _vec3(0.0f, m_fEpslnY, 0.0f);
		
	}

	CheckCollision();
	CameraShake();

	_matrix matTrans, matRot;
	D3DXMatrixTranslation(&matTrans, m_vEye.x, m_vEye.y, m_vEye.z);
	D3DXMatrixRotationQuaternion(&matRot, &m_Quaternion);
	m_matWorld = matRot * matTrans;

	if (CFade_Manager::GetInstance()->GetResultFading() == true)
	{
		m_pCursor->SetRender(false);
		m_bIsMove = false;
	}
	Engine::CCamera::Update_Object(fTimeDelta);
}

HRESULT CPlayerCamera::Ready_Camera(void)
{
	Engine::CComponent* pComponent1 = NULL;

	pComponent1 = Engine::CTransform::Create();
	if (pComponent1 == NULL)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Trans", pComponent1));

	Engine::CComponent* pComponent = NULL;

	pComponent = CSound::Create((Engine::CTransform*)pComponent1);
	if (pComponent == NULL)
		return E_FAIL;
	m_pSoundCom = (CSound*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Sound", pComponent));
	m_pSoundCom->Set_Sound(L"Select", L"SelectBGM.mp3");
	m_pSoundCom->Set_Sound(L"Town", L"TownBGM.mp3");
	m_pSoundCom->Set_Sound(L"Stage1", L"SoulValleyBGM.ogg");
	m_pSoundCom->Set_Sound(L"Stage1Boss", L"HanumanBGM.ogg");
	m_pSoundCom->Set_Sound(L"Stage2", L"VolcanoBGM.ogg");
	m_pSoundCom->Set_Sound(L"Stage2Boss", L"BardielBGM.ogg");
	m_pSoundCom->Set_Sound(L"IntoDungeon", L"IntoDungeonBGM.ogg");
	m_pSoundCom->Set_Sound(L"Fire", L"TownFireBGM.wav");
	m_pSoundCom->Set_Sound(L"Water", L"Stage1BGM.wav");
	m_pSoundCom->Set_Sound(L"Bet", L"Stage2BGM.wav");
	m_pSoundCom->Set_Sound(L"Indoor", L"TownBGM2.wav");
	m_pSoundCom->Set_Sound(L"Inven_Open", L"Inven_Open.wav");
	m_pSoundCom->Set_Sound(L"Inven_Close", L"Inven_Close.wav");
	m_pSoundCom->Set_Sound(L"FadeOption", L"FadeOption.wav");
	m_pSoundCom->Set_Sound(L"Potion", L"PotionDrink.wav");
	m_pSoundCom->MyPlaySound(L"Select", true);


	Engine::CCamera::Ready_Object();
	return S_OK;
}

void CPlayerCamera::CheckCollision(void)
{
	_float fU, fV, fDist;
	BOOL bHit;
	_float fResultDist = 9999.f;

	_vec3	vPlayerPos = _vec3(0.0f, 0.0f, 0.0f);
	m_pPlayerTrans->Get_Infomation(Engine::INFO_POSITION, &vPlayerPos);
	vPlayerPos.y += 10.f;
	_vec3 vRayDir = vPlayerPos - m_vEye;
	D3DXVec3Normalize(&vRayDir, &vRayDir);

	for (_uint i = 0; i < m_iMeshSize; i++)
	{
		D3DXIntersect(*(m_mapMesh[i]), &m_vEye, &vRayDir, &bHit, NULL, &fU, &fV, &fDist, NULL, NULL);

		if (bHit)
		{
			if ((fDist < m_fGap) && (fResultDist > fDist))
				fResultDist = fDist * 1.3f;
		}
	}

	if (fResultDist != 9999.f)
	{
		m_vEye = m_vEye + (vRayDir * fResultDist);
	}
}

void CPlayerCamera::MouseMove(const _float& fTimeDelta)
{
	_int iDistance = 0;


	if (iDistance = Engine::Get_DIMouseMove(Engine::CInput_Device::DIMS_X))
	{
		D3DXQUATERNION qRot;
		D3DXQuaternionRotationAxis(&qRot, &m_vUp, D3DXToRadian(iDistance / 10.0f));
		RotateSlerp(&m_Quaternion, &qRot, 1.f);

		m_vAngle.y += iDistance / 10.0f;		
	}

	if (iDistance = Engine::Get_DIMouseMove(Engine::CInput_Device::DIMS_Y))
	{
		_bool bCanMove = false;
		if (m_vAngle.x + iDistance / 10.0f <= 50.f && m_vAngle.x + iDistance / 10.0f >= -30.f)
		{
			bCanMove = true;
		}
		
		if (bCanMove)
		{
			D3DXQUATERNION qRot;
			D3DXQuaternionRotationAxis(&qRot, &(_vec3)(&m_matWorld.m[0][0]), D3DXToRadian(iDistance / 10.0f));
			RotateSlerp(&m_Quaternion, &qRot, 1.f);

			m_vAngle.x += iDistance / 10.0f;
		}
	}

	if (iDistance = Engine::Get_DIMouseMove(Engine::CInput_Device::DIMS_Z))
	{
		if (iDistance < 0)
		{
			m_fGap += 60.f * fTimeDelta;
			if (m_fGap >= 27.0f)
				m_fGap = 27.0f;

		}
		else
		{
			m_fGap -= 60.f  * fTimeDelta;
			if (m_fGap < 3.0f)
				m_fGap = 3.0f;
		}
	}
}

void CPlayerCamera::FixMouse(void)
{
	POINT	ptMouse = { WINSIZEX >> 1, WINSIZEY >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

void CPlayerCamera::Set_CtrlKey_Check(void)
{
	if (Engine::Get_DIKeyState(DIK_LCONTROL) & 0x80)
	{
		if (m_bPush[UI_KEY_Ctrl])
			return;

		m_bPush[UI_KEY_Ctrl] = true;
		Set_CtrlKey_UIRender();
	}
	else
		m_bPush[UI_KEY_Ctrl] = false;
}
void CPlayerCamera::Set_SkillKey_Check(void)
{
	if (Engine::Get_DIKeyState(DIK_K) & 0x80)
	{
		if (m_bPush[UI_KEY_SKILL])
			return;

		m_bPush[UI_KEY_SKILL] = true;
		Set_SkillKey_UIRender();
	}
	else
		m_bPush[UI_KEY_SKILL] = false;
}
void CPlayerCamera::Set_InvenKey_Check(void)
{
	if (Engine::Get_DIKeyState(DIK_I) & 0x80)
	{
		if (m_bPush[UI_KEY_INVEN])
			return;

		m_bPush[UI_KEY_INVEN] = true;
		Set_InvenKey_UIRender();
	}
	else
		m_bPush[UI_KEY_INVEN] = false;
}

void CPlayerCamera::Set_StateKey_Check(void)
{
	if (Engine::Get_DIKeyState(DIK_P) & 0x80)
	{
		if (m_bPush[UI_KEY_INFO])
			return;
		m_bPush[UI_KEY_INFO] = true;

		Set_StateKey_UIRender();
	}
	else
		m_bPush[UI_KEY_INFO] = false;
}

void CPlayerCamera::Set_ESCKey_Check(void)
{

	if (Engine::Get_DIKeyState(DIK_ESCAPE) & 0x80)
	{
		if (m_bPush[UI_KEY_ESC])
			return;
		m_bPush[UI_KEY_ESC] = true;

		if (m_pWeaponShop->GetRender())
		{
			m_pWeaponShop->SetRender(false);
			if (m_pInven->GetRender())
				m_pInven->SetRender(false);
			if (m_pBuy->GetRender())
				m_pBuy->SetRender(false);

			return;
		}
		else if (m_pArmorShop->GetRender())
		{
			m_pArmorShop->SetRender(false);
			if (m_pInven->GetRender())
				m_pInven->SetRender(false);
			if (m_pBuy->GetRender())
				m_pBuy->SetRender(false);
			return;
		}
		else if (m_pAccShop->GetRender())
		{
			m_pAccShop->SetRender(false);
			if (m_pInven->GetRender())
				m_pInven->SetRender(false);
			if (m_pBuy->GetRender())
				m_pBuy->SetRender(false);
			return;
		}
		else if (m_pPotionShop->GetRender())
		{
			m_pPotionShop->SetRender(false);
			if (m_pInven->GetRender())
				m_pInven->SetRender(false);
			if (m_pBuy->GetRender())
				m_pBuy->SetRender(false);
			return;
		}
		if (m_pDunGeon->GetRender())
		{
			m_pDunGeon->SetRender(false);
			return;
		}
		if (!m_pEsc->GetRender())
		{
			if (m_pInven->GetRender() || m_pSkill->GetRender() || m_pState->GetRender() || !m_pOption->GetRender() || m_pBuy->GetRender())
			{
				m_pInven->SetRender(false);
				m_pSkill->SetRender(false);
				m_pState->SetRender(false);
				m_pBuy->SetRender(false);

				list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"UI_Option")->begin();
				list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"UI_Option")->end();

				for (; iter != iter_end; ++iter)
					((CUI*)*iter)->SetRender(true);

				return;
			}

		}
		if (m_pSystem->GetRender())
		{
			m_pSystem->SetRender(false);
			m_pEsc->SetRender(false);
			return;
		}

		Set_ESCKey_UIRender();
	}
	else
		m_bPush[UI_KEY_ESC] = false;
}
void CPlayerCamera::Set_MainSlot_KeyCheck(const _float& fTimeDelta)
{
	if (m_pEsc->GetRender())
		return;
	if (m_pDunGeon->GetRender())
		return;
	if (m_pWeaponShop->GetRender() || m_pAccShop->GetRender() || m_pArmorShop->GetRender() || m_pPotionShop->GetRender())
		return;

	_uint			iKeyCount = 0;
	_uint			iIndex = -1;
	_bool			bSlotType = false;
	_bool			bKeyCheck = false;

	vector<CUI*>*	pVecMainSlot = ((CMain_Base*)m_pMain)->GetVecBlock();

	if (Engine::Get_DIKeyState(DIK_1) & 0x80)
	{
		if (m_bSlotPush[0])
			return;
		m_bSlotPush[0] = true;
		bKeyCheck = true;
		iKeyCount = 0;
	}
	else if (Engine::Get_DIKeyState(DIK_2) & 0x80)
	{
		if (m_bSlotPush[1])
			return;
		m_bSlotPush[1] = true;
		bKeyCheck = true;
		iKeyCount = 1;
	}
	else if (Engine::Get_DIKeyState(DIK_3) & 0x80)
	{
		if (m_bSlotPush[2])
			return;
		m_bSlotPush[2] = true;
		bKeyCheck = true;
		iKeyCount = 2;
	}
	else if (Engine::Get_DIKeyState(DIK_4) & 0x80)
	{
		if (m_bSlotPush[3])
			return;
		m_bSlotPush[3] = true;
		bKeyCheck = true;
		iKeyCount = 3;
	}
	else if (Engine::Get_DIKeyState(DIK_5) & 0x80)
	{
		if (m_bSlotPush[4])
			return;
		m_bSlotPush[4] = true;
		bKeyCheck = true;
		iKeyCount = 4;
	}
	else if (Engine::Get_DIKeyState(DIK_6) & 0x80)
	{
		if (m_bSlotPush[5])
			return;
		m_bSlotPush[5] = true;
		bKeyCheck = true;
		iKeyCount = 5;
	}
	else if (Engine::Get_DIKeyState(DIK_7) & 0x80)
	{
		if (m_bSlotPush[6])
			return;
		m_bSlotPush[6] = true;
		bKeyCheck = true;
		iKeyCount = 6;
	}
	else if (Engine::Get_DIKeyState(DIK_8) & 0x80)
	{
		if (m_bSlotPush[7])
			return;
		m_bSlotPush[7] = true;
		bKeyCheck = true;
		iKeyCount = 7;
	}
	else if (Engine::Get_DIKeyState(DIK_9) & 0x80)
	{
		if (m_bSlotPush[8])
			return;
		m_bSlotPush[8] = true;
		bKeyCheck = true;
		iKeyCount = 8;
	}
	else if (Engine::Get_DIKeyState(DIK_0) & 0x80)
	{
		if (m_bSlotPush[9])
			return;
		m_bSlotPush[9] = true;
		bKeyCheck = true;
		iKeyCount = 9;
	}

	else if (Engine::Get_DIKeyState(DIK_Z) & 0x80)
	{
		if (m_bSlotPush[10])
			return;
		m_bSlotPush[10] = true;
		bKeyCheck = true;
		bSlotType = true;
		iKeyCount = 30;
	}
	else if (Engine::Get_DIKeyState(DIK_X) & 0x80)
	{
		if (m_bSlotPush[11])
			return;
		m_bSlotPush[11] = true;
		bKeyCheck = true;
		bSlotType = true;
		iKeyCount = 31;
	}
	else if (Engine::Get_DIKeyState(DIK_C) & 0x80)
	{
		if (m_bSlotPush[12])
			return;
		m_bSlotPush[12] = true;
		bKeyCheck = true;
		bSlotType = true;
		iKeyCount = 32;
	}
	else if (Engine::Get_DIKeyState(DIK_V) & 0x80)
	{
		if (m_bSlotPush[13])
			return;
		m_bSlotPush[13] = true;
		bKeyCheck = true;
		bSlotType = true;
		iKeyCount = 33;
	}
	else if (Engine::Get_DIKeyState(DIK_R) & 0x80)
	{
		if (m_bSlotPush[14])
			return;
		m_bSlotPush[14] = true;
		bKeyCheck = true;
		bSlotType = true;
		iKeyCount = 34;
	}
	else if (Engine::Get_DIKeyState(DIK_G) & 0x80)
	{
		if (m_bSlotPush[15])
			return;
		m_bSlotPush[15] = true;
		bKeyCheck = true;
		bSlotType = true;
		iKeyCount = 35;
	}
	else if (Engine::Get_DIKeyState(DIK_T) & 0x80)
	{
		if (m_bSlotPush[16])
			return;
		m_bSlotPush[16] = true;
		bKeyCheck = true;
		bSlotType = true;
		iKeyCount = 36;
	}
	else if (Engine::Get_DIKeyState(DIK_N) & 0x80)
	{
		if (m_bSlotPush[17])
			return;
		m_bSlotPush[17] = true;
		bKeyCheck = true;
		bSlotType = true;
		iKeyCount = 37;
	}
	else
	{
		for (int i = 0; i < 18; ++i)
			m_bSlotPush[i] = false;
	}

	if (!bKeyCheck)
		return;

	if (!bSlotType)
		iIndex = iKeyCount + (10 * (m_pMain->GetSlotLevel() - 1));
	else
		iIndex = iKeyCount;

	if (((CMain_Block*)(*pVecMainSlot)[iIndex])->GetSkill() == NULL && ((CMain_Block*)(*pVecMainSlot)[iIndex])->GetItem() == NULL)
		return;
	if (((CMain_Block*)(*pVecMainSlot)[iIndex])->GetSkill() != NULL && ((CMain_Block*)(*pVecMainSlot)[iIndex])->GetSkill()->Get_Skill_Info()->m_bActive)
		return;
	
	if ((m_pPlayer->GetStateNum() == 2) && m_pPlayer->GetPlayerMoveState() == true)
	{
		if (((CMain_Block*)(*pVecMainSlot)[iIndex])->GetSkill() != NULL && ((CMain_Block*)(*pVecMainSlot)[iIndex])->GetItem() == NULL)
		{
			((CMain_Block*)(*pVecMainSlot)[iIndex])->GetSkill()->Get_Skill_Info()->m_bActive = true;
			Set_MainSlot_Skill(((CMain_Block*)(*pVecMainSlot)[iIndex])->GetSkill(), iIndex);

			Engine::CGameObject* pGameObject = CCoolTime::Create(m_pGraphicDev, L"CoolTimeEff", ((CMain_Block*)(*pVecMainSlot)[iIndex])->GetSkill());
			((CUI*)pGameObject)->SetWinPos(_vec2(*((CMain_Block*)(*pVecMainSlot)[iIndex])->GetSkill()->GetfX(), *((CMain_Block*)(*pVecMainSlot)[iIndex])->GetSkill()->GetfY()));
			Engine::AddObject(Engine::LAYER_UI, L"UI_Effect", pGameObject);
		}
	}
	
	if (((CMain_Block*)(*pVecMainSlot)[iIndex])->GetSkill() == NULL && ((CMain_Block*)(*pVecMainSlot)[iIndex])->GetItem() != NULL)
		Set_MainSlot_Item(((CMain_Block*)(*pVecMainSlot)[iIndex])->GetItem(), iIndex, pVecMainSlot);
}

void CPlayerCamera::Set_MainSlot_Skill(CUI_Skill* pSkill, _uint iIndex)
{
	if(m_pPlayer->GetPlayerInfo()->m_eClassID == CLASS_WITHBLADE)
		((CWitchBlade*)m_pPlayer)->Set_Skill(pSkill);	// 스킬 사용
	else
		((CMistic*)m_pPlayer)->Set_Skill(pSkill->Get_Skill_Info()->m_eSkillId);	// 스킬 사용

	m_pPlayer->GetPlayerInfo()->m_iMp -= 125;
}
void CPlayerCamera::Set_MainSlot_Item(CUI_Item* pItem, _uint iIndex, vector<CUI*>* pVecMainSlot)
{
	switch (pItem->Get_Item_Info()->m_eItemType)
	{
	case ITEM_HPPOTION:
		if (m_pPlayer->GetPlayerInfo()->m_iHp < m_pPlayer->GetPlayerInfo()->m_iMaxHp)
		{
			m_pPlayer->GetPlayerInfo()->m_iHp += pItem->Get_Item_Info()->m_iHp;

			if (m_pPlayer->GetPlayerInfo()->m_iHp > m_pPlayer->GetPlayerInfo()->m_iMaxHp)
				m_pPlayer->GetPlayerInfo()->m_iHp = m_pPlayer->GetPlayerInfo()->m_iMaxHp;

			--pItem->Get_Item_Info()->m_iCount;

			if (pItem->Get_Item_Info()->m_iCount <= 0)
				((CMain_Block*)(*pVecMainSlot)[iIndex])->SetItem(NULL);
		}
		break;
	case ITEM_MPPOTION:
		if (m_pPlayer->GetPlayerInfo()->m_iMp < m_pPlayer->GetPlayerInfo()->m_iMaxMp)
		{
			m_pPlayer->GetPlayerInfo()->m_iMp += pItem->Get_Item_Info()->m_iMp;

			if (m_pPlayer->GetPlayerInfo()->m_iMp > m_pPlayer->GetPlayerInfo()->m_iMaxMp)
				m_pPlayer->GetPlayerInfo()->m_iMp = m_pPlayer->GetPlayerInfo()->m_iMaxMp;

			--pItem->Get_Item_Info()->m_iCount;

			if (pItem->Get_Item_Info()->m_iCount <= 0)
				((CMain_Block*)(*pVecMainSlot)[iIndex])->SetItem(NULL);
		}
		break;
	case ITEM_LEVELPOTION:
		m_pPlayer->GetPlayerInfo()->m_fExp += (_float(m_pPlayer->GetPlayerInfo()->m_iLevel) * 200.f);
		
		--pItem->Get_Item_Info()->m_iCount;

		if (pItem->Get_Item_Info()->m_iCount <= 0)
			((CMain_Block*)(*pVecMainSlot)[iIndex])->SetItem(NULL);
		break;
	}

	m_pSoundCom->MyPlaySound(L"Potion");
	// 아이템 사용
}
void CPlayerCamera::Set_CtrlKey_UIRender(void)
{
	if (m_pEsc->GetRender())
		return;
	if (m_pDunGeon->GetRender())
		return;
	if (m_pWeaponShop->GetRender() || m_pAccShop->GetRender() || m_pArmorShop->GetRender() || m_pPotionShop->GetRender())
		return;

	m_pSoundCom->MyPlaySound(L"FadeOption");
	list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"UI_Option")->begin();
	list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"UI_Option")->end();

	for (; iter != iter_end; ++iter)
	{
		if (((CUI*)*iter)->GetRender())
			((CUI*)*iter)->SetRender(false);
		else
			((CUI*)*iter)->SetRender(true);
	}
}

void CPlayerCamera::Set_SkillKey_UIRender(void)
{
	if (m_pEsc->GetRender())
		return;
	if (m_pDunGeon->GetRender())
		return;
	if (m_pWeaponShop->GetRender() || m_pAccShop->GetRender() || m_pArmorShop->GetRender() || m_pPotionShop->GetRender())
		return;

	if (!m_pSkill->GetRender())
	{
		m_pSoundCom->MyPlaySound(L"Inven_Open");
		m_pSkill->SetRender(true);
	}
	else
	{
		m_pSoundCom->MyPlaySound(L"Inven_Close");
		m_pSkill->SetRender(false);
	}

	if (m_pInven->GetRender())
		m_pInven->SetRender(false);
}
void CPlayerCamera::Set_InvenKey_UIRender(void)
{
	if (m_pEsc->GetRender())
		return;
	if (m_pDunGeon->GetRender())
		return;
	if (m_pWeaponShop->GetRender() || m_pAccShop->GetRender() || m_pArmorShop->GetRender() || m_pPotionShop->GetRender())
		return;

	if (!m_pInven->GetRender())
	{
		m_pSoundCom->MyPlaySound(L"Inven_Open");
		m_pInven->SetRender(true);
	}
	else
	{
		m_pSoundCom->MyPlaySound(L"Inven_Close");
		m_pInven->SetRender(false);
	}

	if (m_pSkill->GetRender())
		m_pSkill->SetRender(false);
}

void CPlayerCamera::Set_StateKey_UIRender(void)
{
	if (m_pEsc->GetRender())
		return;
	if (m_pDunGeon->GetRender())
		return;
	if (m_pWeaponShop->GetRender() || m_pAccShop->GetRender() || m_pArmorShop->GetRender() || m_pPotionShop->GetRender())
		return;

	if (!m_pState->GetRender())
	{
		m_pSoundCom->MyPlaySound(L"Inven_Open");
		m_pState->SetRender(true);
	}
	else
	{
		m_pSoundCom->MyPlaySound(L"Inven_Close");
		m_pState->SetRender(false);

	}
}
void CPlayerCamera::Set_Mouse_Active_Check(void)
{
	if (!m_pOption->GetRender() || m_pSkill->GetRender() || m_pInven->GetRender() || m_pState->GetRender() || m_pEsc->GetRender() || m_pDunGeon->GetRender() || m_pWeaponShop->GetRender() || m_pArmorShop->GetRender() || m_pAccShop->GetRender() || m_pPotionShop->GetRender())
	{
		if (m_pCursor->GetRender())
		{
			m_pCursor->SetRender(false);
			m_bIsMove = false;
		}
	}
	else if (m_pOption->GetRender() && !m_pSkill->GetRender() && !m_pInven->GetRender() && !m_pState->GetRender() && !m_pEsc->GetRender() && !m_pDunGeon->GetRender() && !m_pWeaponShop->GetRender() && !m_pArmorShop->GetRender() && !m_pAccShop->GetRender() && !m_pPotionShop->GetRender())
	{
		if (!m_pCursor->GetRender())
		{
			m_pCursor->SetRender(true);
			m_bIsMove = true;
		}
	}
}

void CPlayerCamera::Set_ESCKey_UIRender(void)
{
	if (m_pDunGeon->GetRender())
		return;
	if (m_pWeaponShop->GetRender() || m_pAccShop->GetRender() || m_pArmorShop->GetRender() || m_pPotionShop->GetRender())
		return;

	if (!m_pEsc->GetRender())
	{
		m_pSoundCom->MyPlaySound(L"Inven_Open");
		m_pEsc->SetRender(true);
	}

	else
	{
		m_pSoundCom->MyPlaySound(L"Inven_Close");
		m_pEsc->SetRender(false);
	}
}
CPlayerCamera* CPlayerCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
{
	CPlayerCamera*		pInstance = new CPlayerCamera(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Camera()))
	{
		MSG_BOX("CPlayerCamera Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerCamera::RotateYaw(void)
{
	/*_vec3 vUp;
	if (m_vUp.y > 0.f)
		vUp = _vec3(0.f, 1.f, 0.f);
	else
		vUp = _vec3(0.f, -1.f, 0.f);*/

	D3DXQUATERNION qRot;
	D3DXQuaternionRotationAxis(&qRot, &m_vUp, D3DXToRadian(m_vAngle.y));
	RotateSlerp(&m_Quaternion, &qRot, 1.f);
}

void CPlayerCamera::RotatePitch(void)
{
	D3DXQUATERNION qRot;
	D3DXQuaternionRotationAxis(&qRot, &(_vec3)(&m_matWorld.m[0][0]), D3DXToRadian(m_vAngle.x));
	RotateSlerp(&m_Quaternion, &qRot, 1.f);
}

void CPlayerCamera::RotateRoll(void)
{
	D3DXQUATERNION qRot;
	D3DXQuaternionRotationAxis(&qRot, &(_vec3)(&m_matWorld.m[2][0]), D3DXToRadian(m_vAngle.z));
	//RotateSlerp(&m_Quaternion, &qRot, 1.f);
}

void CPlayerCamera::RotateSlerp(const D3DXQUATERNION* pOriQuat, const D3DXQUATERNION* pDestQuat, const _float& fSpeed)
{
	D3DXQUATERNION qQuat;
	D3DXQuaternionSlerp(&qQuat, pOriQuat, pDestQuat, fSpeed);
	m_Quaternion *= qQuat;
	D3DXQuaternionNormalize(&m_Quaternion, &m_Quaternion);

	_matrix matAxis;
	D3DXMatrixRotationQuaternion(&matAxis, &m_Quaternion);
	D3DXVec3TransformNormal(&m_vCamDir, &_vec3(0.f, 0.f, -1.f), &matAxis);
}

void CPlayerCamera::CameraAction(const _float & fTimeDelta)
{
	if (m_bActionEnd)
	{
		if (m_fFadeTime >= 0.5f && 0.8f > m_fFadeTime)
		{
			m_fInvSpeed -= fTimeDelta * 0.5f;

			if (m_fInvSpeed <= 0.f)
				m_fInvSpeed = 0.f;

			m_fGap -= m_fInvSpeed * 2.f;
			if (m_fGap < 30.0f)
				m_fGap = 30.0f;

			if (m_fFadeTime >= 0.7f)
			{
				if (!m_bActionUI)
				{
					Engine::CGameObject* pGameObject = CBoss_Base::Create(m_pGraphicDev, L"Boss_Base");
					Engine::AddObject(Engine::LAYER_UI, L"UI_BossApear", pGameObject);
					m_bActionUI = true;
				}
			}
		}

		//페이드아웃대기시간
		if (m_fFadeTime > 4.f)
		{
			if (!m_bFadeOut)
			{
				m_bFadeOut = true;
				CFade_Manager::GetInstance()->FadeReset();
				CFade_Manager::GetInstance()->FadeImmidy(2.f, 0.1f, CFade_Manager::FADE_BOSS_OUTTRO);
				CFade_Manager::GetInstance()->SetBardielFade(true);

			}
		}
		if (m_fFadeTime > 4.5f)
		{
			Set_Action(false);
			m_pPlayer->Set_ActionEnd();
			CFade_Manager::GetInstance()->SetFading(false);
			m_bActionEnd = false;
			return;
		}

		m_fFadeTime += fTimeDelta;
	}

	_vec3	vTargetPos = _vec3(0.0f, 0.0f, 0.0f);
	m_pActionTarget->Get_Infomation(Engine::INFO_POSITION, &vTargetPos);
	
	_vec3 vLook;
	m_pActionTarget->Get_Infomation(Engine::INFO_LOOK, &vLook);
	vLook.y = 0.f;
	D3DXVec3Normalize(&vLook, &vLook);

	_vec3 vUp = _vec3(0.f, 1.f, 0.f);
	_vec3 vRight;
	D3DXVec3Cross(&vRight, &vUp, &vLook);

	_vec3 vPos;
	m_pActionTarget->Get_Infomation(Engine::INFO_POSITION, &vPos);

	if (vPos.y <= 220.f && vPos.y > 150.f)
	{
		m_fGap += 10.f * fTimeDelta;
		m_fActionAngleForHanuman -= 30.f * fTimeDelta;
	}

	_matrix matAxis;
	D3DXMatrixRotationAxis(&matAxis, &vRight, D3DXToRadian(m_fActionAngleForHanuman));

	D3DXVec3TransformNormal(&vLook, &vLook, &matAxis);

	m_vEye = vTargetPos + (vLook * m_fGap) + _vec3(0.0f, m_fEpslnY, 0.0f);
	m_vAt = vTargetPos + _vec3(0.0f, m_fEpslnY, 0.0f);
}

void CPlayerCamera::FireAction(const _float & fTimeDelta)
{
	m_fFadeInTime += fTimeDelta;

	if (m_fFadeInTime >= 27.6f)
	{
		if (!m_bActionUI)
		{
			Engine::CGameObject* pGameObject = CBoss_Base::Create(m_pGraphicDev, L"Boss_Base");
			Engine::AddObject(Engine::LAYER_UI, L"UI_BossApear", pGameObject);
			m_bActionUI = true;
		}
	}
	if (m_bActionEnd)
	{
		if (!m_bFadeOut)
		{
			m_bFadeOut = true;
			CFade_Manager::GetInstance()->FadeReset();
			CFade_Manager::GetInstance()->FadeImmidy(2.f, 0.1f, CFade_Manager::FADE_BOSS_OUTTRO);

		}
		
		//스테이지2 페이드아웃대기시간
		if (m_fFadeTime > 0.5f)
		{
			Set_Action(false);
			m_pPlayer->Set_ActionEnd();
			CFade_Manager::GetInstance()->SetFading(false);
			m_bActionEnd = false;
			return;
		}

		m_fFadeTime += fTimeDelta;
	}

	_vec3 vTargetPos;
	m_pActionTarget->Get_Infomation(Engine::INFO_POSITION, &vTargetPos);

	_vec3 vLook;
	m_pActionTarget->Get_Infomation(Engine::INFO_LOOK, &vLook);
	vLook.y = 0.f;
	D3DXVec3Normalize(&vLook, &vLook);


	m_vEye = vTargetPos + (vLook * m_fGap) + _vec3(0.0f, m_fEpslnY, 0.0f);
	m_vAt = vTargetPos + _vec3(0.0f, m_fEpslnY, 0.0f);
}

void CPlayerCamera::Free(void)
{
	Engine::CCamera::Free();
}




