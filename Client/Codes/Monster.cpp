#include "stdafx.h"

#include "Monster.h"

#include "Export_Engine.h"
#include "VIBuffer.h"

#include "Map_Ice.h"
#include "Map_Fire.h"


#include "Environment.h"
#include "Player.h"
#include "Cube.h"
#include "NavigationMesh.h"
#include "NumberingMgr.h"
#include "Camera.h"
#include "DamParent.h"
#include "Light.h"

//Å¬¶ó Monster

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
	: Engine::CGameObject(pGraphicDev, wstrName)
	, m_pTransCom(NULL)
	, m_pMeshCom(NULL)
	, m_pRendererCom(NULL)
	, m_pShaderCom(NULL)
	, m_pNaviCom(NULL)
	, m_pAnimatorCom(NULL)
	, m_pCalculatorCom(NULL)
	, m_pRigidbodyCom(NULL)
	, m_pAICom(NULL)
	, m_ppMapMesh(NULL)
	, m_pTargetTransCom(NULL)
	, m_pColliderCom(NULL)
	, m_pSphereColCom(NULL)
	,m_pSoundCom(NULL)
	, m_pPointLight(NULL)
	, m_iAniIdx(0)
	, m_iMonsterAniState(0)
	, m_fAlpha(1.0f)
	, m_fClearTime(0.f)
	, m_bDamaged(false)
	, m_bDamageEffectCheck(false)
	, m_pDamParent(NULL)
	, m_bDeath(false)
	, m_bDown(false)
	, m_bDefence(false)
	, m_bCriRadialBlur(FALSE)
	, m_bCriRadialTime(0.0f)	
	, m_fLightRange(0.f)
{
	ZeroMemory(&m_tColInfo, sizeof(Engine::COL_INFO));
}
CMonster::~CMonster(void)
{

}

HRESULT CMonster::Start_ForScene(void)
{
	Engine::CGameObject* pGameObject = Engine::FindWithTag(L"Map");
	if (pGameObject == NULL)
		return E_FAIL;

	if (dynamic_cast<CMap_Ice*>(pGameObject) != NULL)
	{
		_int iSize = 0;
		Engine::CStaticMesh** ppMesh = ((CMap_Ice*)pGameObject)->GetMapMesh(&iSize);
		m_ppMapMesh = ((CMap_Ice*)pGameObject)->GetMapMesh(&iSize);

		for (_int i = 0; i < iSize; i++)
		{
			m_pCalculatorCom->Set_TargetMesh(ppMesh[i]->Get_Mesh());
		}
	}

	if (dynamic_cast<CMap_Fire*>(pGameObject) != NULL)
	{
		_int iSize = 0;
		Engine::CStaticMesh** ppMesh = ((CMap_Fire*)pGameObject)->GetMapMesh(&iSize);
		m_ppMapMesh = ((CMap_Fire*)pGameObject)->GetMapMesh(&iSize);

		for (_int i = 0; i < iSize; i++)
		{
			m_pCalculatorCom->Set_TargetMesh(ppMesh[i]->Get_Mesh());
		}
	}

	Engine::Safe_Release(pGameObject);

	pGameObject = Engine::FindWithTag(L"Player");
	if (pGameObject == NULL)
		return E_FAIL;

	m_pTargetTransCom = (Engine::CTransform*)pGameObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);
	if (m_pTargetTransCom == NULL)
		return E_FAIL;


	// UI Dam

	Engine::CGameObject* pDamParent = CDamParent::Create(m_pGraphicDev, L"Dam_Parent", this);
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_Dam", pDamParent);

	m_pDamParent = ((CUI*)pDamParent);

	Engine::Safe_Release(m_pTargetTransCom);
	Engine::Safe_Release(pGameObject);

	m_pPointLight = Engine::Get_PointLight(g_iPointLightIndex);
	g_iPointLightIndex++;

	return S_OK;
}

void CMonster::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	if (m_fLightRange > 0.f)
	{
		switch (m_eMonsterID)
		{
		case MON_B_War:
			m_fLightRange -= 5.f * fTimeDelta;
			break;

		case MON_B_Axe:
			m_fLightRange -= 5.f * fTimeDelta;
			break;

		case MON_B_Bow:
			m_fLightRange -= 5.f * fTimeDelta;
			break;

		case MON_HANUMAN:
			m_fLightRange -= 12.f * fTimeDelta;
			break;

		case MON_BARDIEL:
			m_fLightRange -= 12.f * fTimeDelta;
			break;

		case MON_KNOLE_COMMANDER:
			m_fLightRange -= 5.f * fTimeDelta;
			break;
		}
		
		if(m_fLightRange > 0.f)
			m_pPointLight->Set_LightRange(m_fLightRange);
	}
	else
		m_pPointLight->Set_LightRange(0.f);

	
	_vec3 vTargetPos;
	vTargetPos = m_pTargetTransCom->Get_Now_Position();
	_vec3 vMyPos;
	vMyPos = m_pTransCom->Get_Now_Position();

	_vec3 vDis;
	vDis = vTargetPos - vMyPos;

	m_pMeshCom->Play_AnimationSet(fTimeDelta);

	if (m_pCalculatorCom->Get_IsCol())
	{
		m_pRigidbodyCom->Set_Gravity(false);
	}
	else
	{
		m_pRigidbodyCom->Set_Gravity(true);
	}

	/////////////////////////////////////////////
	if (Engine::IsCollision(L"Player", &m_tColInfo, m_pColliderCom))
	{
		int iRand = rand() % 3000;
		if (!m_bDown)
		{
			if (!m_bDefence)
			{
				m_tMonInfo.iHp -= m_tColInfo.iDamage + iRand;
				Collision_CameraWork();
				Collision_Light();
				m_bDamaged = true;
				if (!m_bDeath)
				{
					if (m_tMonInfo.iHp >= 0)
					{
						CNumbering_Manager::GetInstance()->Increase_ComboCount();
						((CDamParent*)m_pDamParent)->SetDamageNumber(m_tColInfo.iDamage + iRand, m_tColInfo.bCritical);

						if (m_tColInfo.bCritical)
							m_bCriRadialBlur = TRUE;
					}
				}
			}
			else  //m_bDefence
			{
				m_tMonInfo.iHp -= 1000;
				Collision_CameraWork();
				Collision_Light();
				m_bDamaged = true;
				if (!m_bDeath)
				{
					if (m_tMonInfo.iHp >= 0)
					{
						CNumbering_Manager::GetInstance()->Increase_ComboCount();
						((CDamParent*)m_pDamParent)->SetDamageNumber(1000, m_tColInfo.bCritical);

						if (m_tColInfo.bCritical)
							m_bCriRadialBlur = TRUE;
					}
				}
			}

		}
		else if (m_bDown && m_tColInfo.eColInfo == Engine::COL_STAMP)
		{
			if (!m_bDeath)
			{
				if (m_tMonInfo.iHp >= 0)
				{
					m_tMonInfo.iHp -= m_tColInfo.iDamage + iRand;
					Collision_CameraWork();
					Collision_Light();
					m_bDamaged = true;
					CNumbering_Manager::GetInstance()->Increase_ComboCount();
					((CDamParent*)m_pDamParent)->SetDamageNumber(m_tColInfo.iDamage + iRand, m_tColInfo.bCritical);

					if (m_tColInfo.bCritical)
						m_bCriRadialBlur = TRUE;
				}
			}
		}
	}
	else
	{
		m_bDamaged = false;
	}
	///////////////////////////////////
	if (m_pRendererCom->GetShadowBuffer() ||
		m_pRendererCom->GetSoftShadow() ||
		m_pRendererCom->GetCACDShadow())
	{
		m_pRendererCom->Add_RenderGroup(Engine::RENEDER_DYANAMIC_SHADOW, this);
	}

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_DYNAMIC, this);

	if (!m_bDown && !m_bDeath)
	{
		Engine::Add_ColliderGroup(Engine::COL_BOX, L"Monster", m_pColliderCom);
	}
	

	if (m_bCriRadialBlur)
	{
		Engine::SetRadialBlur(TRUE);
		Engine::SetRadialBlurPower(-0.25f);

		m_bCriRadialTime += fTimeDelta;
		if (m_bCriRadialTime >= 0.25f)
		{
			m_bCriRadialTime = 0.0f;
			Engine::SetRadialBlur(FALSE);
			m_bCriRadialBlur = FALSE;
		}
	}

	if (FALSE == m_bCriRadialBlur)
	{		
		if (m_tMonInfo.iHp < 0)
		{
			if (m_wstrName == L"Mesh_Hanuman")
				g_bHanumanDead = TRUE;
			else if (m_wstrName == L"Mesh_Bardiel")
				g_bBardielDead = TRUE;

			CNumbering_Manager::GetInstance()->SetKillCount();
		}
	}	
}

void CMonster::Collision_CameraWork(void)
{
	switch (m_tColInfo.dwSkillID)
	{
	case SKILL_RUSH:
		switch (m_tColInfo.eColInfo)
		{
		case Engine::COL_BASE:	
			break;
		}

		break;
	case SKILL_FLYDANCE:
		switch (m_tColInfo.eColInfo)
		{
		case Engine::COL_FLY:
			Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 0.3f, 1);
			break;
		}

		break;
	case SKILL_FLEDGE_BLADE:
		switch (m_tColInfo.eColInfo)
		{
		case Engine::COL_PUSH:
			Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 0.3f, 1);
			break;
		case Engine::COL_SPINA:
			_matrix matView = Engine::GetCurrentCamera()->GetMatView();
			D3DXMatrixInverse(&matView, NULL, &matView);
			_vec3 vRight;
			memcpy(&vRight, &matView.m[0][0], sizeof(_vec3));
			D3DXVec3Normalize(&vRight, &vRight);

			Engine::Set_Shake(Engine::CAMERA_PLAYER, vRight, 1.0f, 3);
			break;
		}

		break;
	case SKILL_STUNPIN:
		switch (m_tColInfo.eColInfo)
		{
		case Engine::COL_PUSH:
			Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 0.3f, 2);
			break;
		case Engine::COL_SPINA:
			_matrix matView = Engine::GetCurrentCamera()->GetMatView();
			D3DXMatrixInverse(&matView, NULL, &matView);
			_vec3 vRight;
			memcpy(&vRight, &matView.m[0][0], sizeof(_vec3));
			D3DXVec3Normalize(&vRight, &vRight);

			Engine::Set_Shake(Engine::CAMERA_PLAYER, vRight, 1.0f, 2);
			break;
		}

		break;
	case SKILL_VIOLET:
		switch (m_tColInfo.eColInfo)
		{
		case Engine::COL_SMASH:
			Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 1.0f, 4);
			break;
		}

		break;
	case SKILL_CROSS:

		break;
	case SKILL_SPIRALDANCE:
		switch (m_tColInfo.eColInfo)
		{
		case Engine::COL_FLY:
			Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 0.5f, 5);
			break;
		}

		break;
	case SKILL_DARKPOLLING:

		break;
	case SKILL_DRAGONDANCE:
		switch (m_tColInfo.eColInfo)
		{
		case Engine::COL_SPINA:
			_matrix matView = Engine::GetCurrentCamera()->GetMatView();
			D3DXMatrixInverse(&matView, NULL, &matView);
			_vec3 vRight;
			memcpy(&vRight, &matView.m[0][0], sizeof(_vec3));
			D3DXVec3Normalize(&vRight, &vRight);

			Engine::Set_Shake(Engine::CAMERA_PLAYER, vRight, 1.0f, 2);
			break;
		}

		break;
	case SKILL_DASH:

		break;
	case SKILL_LIDDLEKICK:
		switch (m_tColInfo.eColInfo)
		{
		case Engine::COL_STAMP:
			Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 0.5f, 1);
			break;
		}

		break;
	case SKILL_EVADEATT:

		break;
	case SKILL_TECHDOWN:

		break;
	case SKILL_BLARE:

		break;
	case SKILL_UPATT:
		switch (m_tColInfo.eColInfo)
		{
		case Engine::COL_PUSH:
			_matrix matView = Engine::GetCurrentCamera()->GetMatView();
			D3DXMatrixInverse(&matView, NULL, &matView);
			_vec3 vRight;
			memcpy(&vRight, &matView.m[0][0], sizeof(_vec3));
			D3DXVec3Normalize(&vRight, &vRight);

			Engine::Set_Shake(Engine::CAMERA_PLAYER, vRight, 1.0f, 2);
			break;
		}
		break;
	case SKILL_VIFERKICK:
		switch (m_tColInfo.eColInfo)
		{
		case Engine::COL_FLY:
			Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 1.0f, 1);
			break;
		}
		break;
	case SKILL_SPININGHEART:

		break;
	case SKILL_BLACKROSE:
		switch (m_tColInfo.eColInfo)
		{
		case Engine::COL_PUSH:
			Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 0.3f, 1);
			break;
		case Engine::COL_FLY:
			Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 0.5f, 1);
			break;
		case Engine::COL_SPINB:
			Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 1.0f, 1);
			break;
		}

		break;
	case SKILL_SKINNING:

		break;
	case SKILL_TRICKEY:
		switch (m_tColInfo.eColInfo)
		{
		case Engine::COL_BASE:
		{
			_matrix matView = Engine::GetCurrentCamera()->GetMatView();
			D3DXMatrixInverse(&matView, NULL, &matView);
			_vec3 vRight;
			memcpy(&vRight, &matView.m[0][0], sizeof(_vec3));
			D3DXVec3Normalize(&vRight, &vRight);

			Engine::Set_Shake(Engine::CAMERA_PLAYER, vRight, 0.2f, 1);
		}
		break;
		case Engine::COL_PUSH:
			Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 0.3f, 1);
			break;
		case Engine::COL_FLY:
			Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 0.5f, 1);
			break;
		case Engine::COL_SPINB:
			Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 1.0f, 1);
			break;
		}
		break;
	case SKILL_FLENITDANCE:
		switch (m_tColInfo.eColInfo)
		{
		case Engine::COL_BASE:
		{
			_matrix matView = Engine::GetCurrentCamera()->GetMatView();
			D3DXMatrixInverse(&matView, NULL, &matView);
			_vec3 vRight;
			memcpy(&vRight, &matView.m[0][0], sizeof(_vec3));
			D3DXVec3Normalize(&vRight, &vRight);

			Engine::Set_Shake(Engine::CAMERA_PLAYER, vRight, 0.2f, 1);
		}
		break;
		case Engine::COL_FLY:
			Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 0.5f, 1);
			break;
		}

		break;
	case SKILL_PLORA:

		break;
	case SKILL_SOFTLANDING:

		break;
	case SKILL_SILUETDANCE:
		switch (m_tColInfo.eColInfo)
		{
		case Engine::COL_FLY:
			Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 0.5f, 1);
			break;
		}
		break;
	case SKILL_HILLTURN:

		break;
	case SKILL_URBANBREAT:

		break;
	}


}


HRESULT CMonster::LoadCollider(const _tchar * pFileName)
{
	if (pFileName == NULL)
		return E_FAIL;

	_tchar szFullPath[MAX_PATH] = L"";
	lstrcpy(szFullPath, L"../Bin/Data/MonsterColliderData/");
	lstrcat(szFullPath, pFileName);
	lstrcat(szFullPath, L".dat");

	DWORD dwByte = 0;
	HANDLE hFile = CreateFile(szFullPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == NULL)
		return E_FAIL;

	Engine::COL_TYPE eType;
	_vec3 vPos = _vec3(0.f, 0.f, 0.f);
	_vec3 vScale = _vec3(1.f, 1.f, 1.f);
	_float fRad = 1.f;

	ReadFile(hFile, &eType, sizeof(Engine::COL_TYPE), &dwByte, NULL);
	ReadFile(hFile, &vPos, sizeof(_vec3), &dwByte, NULL);

	Engine::CComponent* pComponent = NULL;
	switch (eType)
	{
	case Engine::COL_SPHERE:
		ReadFile(hFile, &fRad, sizeof(_float), &dwByte, NULL);
		pComponent = Engine::Create_SphereCollider(m_pGraphicDev, m_pTransCom, L"Monster", vPos, fRad);
		if (pComponent == NULL)
			return E_FAIL;
		m_pColliderCom = (Engine::CSphereCollider*)pComponent;
		m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Collider", pComponent));
		break;

	case Engine::COL_BOX:
		ReadFile(hFile, &vScale, sizeof(_vec3), &dwByte, NULL);
		pComponent = Engine::Create_BoxCollider(m_pGraphicDev, m_pTransCom, L"Monster", vPos, vScale, D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
		if (pComponent == NULL)
			return E_FAIL;
		m_pColliderCom = (Engine::CBoxCollider*)pComponent;
		m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Collider", pComponent));
		break;
	}

	CloseHandle(hFile);
	return S_OK;
}

void CMonster::Render_Ray(void)
{
	//_vec3			vPoint[2];

	//vPoint[0] = m_vPivotPos;
	//vPoint[1] = m_vTargetPos;

	//_matrix			matView, matProj;

	//m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	//m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	//for (_ulong i = 0; i < 2; ++i)
	//{
	//	D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matView);
	//	if (vPoint[i].z <= 0.1f)
	//		vPoint[i].z = 0.1f;

	//	D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matProj);
	//}

	//m_pGraphicDev->EndScene();

	//m_pGraphicDev->BeginScene();

	//_matrix				matTmp;
}

Engine::CTransform* CMonster::GetTransCom(void)
{
	return m_pTransCom;
}


void CMonster::SetObjectInfo(_vec3* vPos, _vec3* vScale, _vec3* vAngle)
{
	m_pTransCom->Set_Position(vPos);
	m_pTransCom->Set_Scale(vScale->x, vScale->y, vScale->z);
	m_pTransCom->Rotation(Engine::ROT_Y, vAngle->y);
}

void CMonster::Free(void)
{
	m_pPointLight->Set_LightRange(0.f);
	Engine::CGameObject::Free();
	m_BestList.clear();
}

vector<Engine::ANIINFO*>* CMonster::GetAniInfo(void)
{
	return &m_vecAniInfo;
}

void CMonster::Collision_Light(void)
{
	D3DXCOLOR tDiffuse;
	
	_vec3 vPos;
	m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &vPos);

	_vec3 vLook;
	m_pTransCom->Get_Infomation(Engine::INFO_LOOK, &vLook);

	vLook.y = 0.f;
	D3DXVec3Normalize(&vLook, &vLook);

	switch (m_eMonsterID)
	{
	case MON_B_War:
		m_fLightRange = 4.f;
		vPos.y += 4.f;
		vPos += vLook * 2.f;
		break;

	case MON_B_Axe:
		m_fLightRange = 4.f;
		vPos.y += 4.f;
		vPos += vLook * 2.f;
		break;

	case MON_B_Bow:
		m_fLightRange = 4.f;
		vPos.y += 4.f;
		vPos += vLook * 2.f;
		break;

	case MON_HANUMAN:
		m_fLightRange = 10.f;
		vPos.y += 10.f;
		vPos += vLook * 4.f;
		break;

	case MON_BARDIEL:
		m_fLightRange = 10.f;
		vPos.y += 10.f;
		vPos += vLook * 4.f;
		break;

	case MON_KNOLE_COMMANDER:
		m_fLightRange = 4.f;
		vPos.y += 4.f;
		vPos += vLook * 2.f;
		break;
	}

	switch (m_tColInfo.dwSkillID)
	{
	case SKILL_RUSH:
		tDiffuse = D3DXCOLOR(250.f / 255.f, 244.f / 255.f, 192.f / 255.f, 1.f) * 4.f;
		break;
	case SKILL_FLYDANCE:
		tDiffuse = D3DXCOLOR(4.f, 0.f, 0.f, 1.f);
		break;
	case SKILL_FLEDGE_BLADE:
		tDiffuse = D3DXCOLOR(250.f / 255.f, 244.f / 255.f, 192.f / 255.f, 1.f) * 4.f;
		break;
	case SKILL_STUNPIN:
		tDiffuse = D3DXCOLOR(4.f, 0.f, 4.f, 1.f);
		break;
	case SKILL_VIOLET:
		tDiffuse = D3DXCOLOR(4.f, 0.f, 4.f, 1.f);
		break;
	case SKILL_CROSS:
		break;
	case SKILL_SPIRALDANCE:
		tDiffuse = D3DXCOLOR(250.f / 255.f, 244.f / 255.f, 192.f / 255.f, 1.f) * 4.f;
		break;
	case SKILL_DARKPOLLING:
		tDiffuse = D3DXCOLOR(4.f, 0.f, 127.f / 255.f * 4.f, 1.f);
		break;
	case SKILL_DRAGONDANCE:
		tDiffuse = D3DXCOLOR(4.f, 0.f, 4.f, 1.f);
		break;
	case SKILL_DASH:
		break;
	case SKILL_LIDDLEKICK:
		tDiffuse = D3DXCOLOR(250.f / 255.f, 244.f / 255.f, 192.f / 255.f, 1.f) * 4.f;
		break;
	case SKILL_EVADEATT:
		break;
	case SKILL_TECHDOWN:
		break;
	case SKILL_BLARE:
		tDiffuse = D3DXCOLOR(250.f / 255.f, 244.f / 255.f, 192.f / 255.f, 1.f) * 4.f;
		break;
	case SKILL_UPATT:
		tDiffuse = D3DXCOLOR(250.f / 255.f, 244.f / 255.f, 192.f / 255.f, 1.f) * 4.f;
		break;
	case SKILL_VIFERKICK:
		tDiffuse = D3DXCOLOR(4.f, 0.f, 0.f, 1.f);
		break;
	case SKILL_SPININGHEART:
		break;
	case SKILL_BLACKROSE:
		tDiffuse = D3DXCOLOR(4.f, 0.f, 0.f, 1.f);
		break;
	case SKILL_SKINNING:
		break;
	case SKILL_TRICKEY:
		tDiffuse = D3DXCOLOR(250.f / 255.f, 244.f / 255.f, 192.f / 255.f, 1.f) * 4.f;
		break;
	case SKILL_FLENITDANCE:
		tDiffuse = D3DXCOLOR(250.f / 255.f, 244.f / 255.f, 192.f / 255.f, 1.f) * 4.f;
		break;
	case SKILL_PLORA:
		break;
	case SKILL_SOFTLANDING:
		break;
	case SKILL_SILUETDANCE:
		tDiffuse = D3DXCOLOR(250.f / 255.f, 244.f / 255.f, 192.f / 255.f, 1.f) * 4.f;
		break;
	case SKILL_HILLTURN:
		tDiffuse = D3DXCOLOR(250.f / 255.f, 244.f / 255.f, 192.f / 255.f, 1.f) * 4.f;
		break;
	case SKILL_URBANBREAT:
		break;
	case SKILL_BLOODAGIL:
		m_fLightRange = 0.f;
		break;
	}
	m_pPointLight->Set_LightPos(&vPos);
	m_pPointLight->Set_LightDiffuse(tDiffuse);
	m_pPointLight->Set_LightRange(m_fLightRange);
}

Engine::CAnimator * CMonster::GetAnimatorInfo(void)
{
	return m_pAnimatorCom;
}








