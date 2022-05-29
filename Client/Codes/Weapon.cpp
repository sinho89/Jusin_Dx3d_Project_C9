#include "stdafx.h"
#include "Weapon.h"


CWeapon::CWeapon(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
: Engine::CGameObject(pGraphicDev, wstrName)
, m_pTransCom(NULL)
, m_pRendererCom(NULL)
, m_pMeshCom(NULL)
, m_pShaderCom(NULL)
, m_pmatTarget(NULL)
, m_pmatWeapon(NULL)
, m_fAlpha(1.0f)
, m_bRender(false)
{
	m_bIsStatic = true;
}

CWeapon::~CWeapon(void)
{

}

void CWeapon::Set_Alpha(const _float& fAlpha)
{
	m_fAlpha = fAlpha;
}

HRESULT CWeapon::Start_ForScene(void)
{
	return S_OK;	
}

void CWeapon::Update_Object(const _float& fTimeDelta)
{
	if (m_bRender)
	{
		Engine::CGameObject::Update_Object(fTimeDelta);

		if (m_pRendererCom->GetShadowBuffer()	|| 
			m_pRendererCom->GetSoftShadow()		||
			m_pRendererCom->GetCACDShadow())
		{
			m_pRendererCom->Add_RenderGroup(Engine::RENDER_STATIC_SHADOW, this);
		}

		//m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_STATIC, this);
	}
}

void CWeapon::Render_Object(void)
{
	
}

void CWeapon::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
{
}

HRESULT CWeapon::LoadWeaponData(const _tchar* pTargetName, const _tchar* pFilePath)
{
	if(pTargetName == NULL)
		return E_FAIL;

	if(pFilePath == NULL)
		return E_FAIL;

	_tchar szFullPath[MAX_PATH] = L"";
	lstrcpy(szFullPath, L"../Bin/Data/WeaponData/");
	lstrcat(szFullPath, pFilePath);
	lstrcat(szFullPath, L".dat");

	DWORD dwByte = 0;
	HANDLE hFile = CreateFile(szFullPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile == NULL)
		return E_FAIL;

	while(true)
	{
		WEAPONINFO* tWeaponInfo = new WEAPONINFO;
		ReadFile(hFile, tWeaponInfo, sizeof(WEAPONINFO), &dwByte, NULL);
		if(dwByte == 0)
		{
			Engine::Safe_Delete(tWeaponInfo);
			break;
		}

		Engine::CGameObject* pGameObject = NULL;
		pGameObject = Engine::Find(pTargetName);
		
		if(pGameObject == NULL)
		{
			Engine::Safe_Delete(tWeaponInfo);
			return E_FAIL;
		}
		
		Engine::CTransform* pTransform = NULL;
		pTransform = (Engine::CTransform*)pGameObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

		if(pTransform == NULL)
		{
			Engine::Safe_Delete(tWeaponInfo);
			Engine::Safe_Release(pGameObject);
			return E_FAIL;
		}

		m_pmatTarget = pTransform->Get_WorldMatrixPointer();

		Engine::CDynamicMesh* pMesh = NULL;
		pMesh = (Engine::CDynamicMesh*)pGameObject->GetComponent<Engine::CDynamicMesh>(Engine::COMID_STATIC);

		if(pMesh == NULL)
		{
			Engine::Safe_Delete(tWeaponInfo);
			Engine::Safe_Release(pTransform);
			Engine::Safe_Release(pGameObject);
		}

		m_pmatWeapon = pMesh->Get_FrameByName(tWeaponInfo->szBoneName);

		m_pTransCom->Set_Position(&tWeaponInfo->vPos);
		m_pTransCom->Set_Angle(&tWeaponInfo->vRotate);
		m_pTransCom->Set_Scale(tWeaponInfo->vScale.x, tWeaponInfo->vScale.y, tWeaponInfo->vScale.z);

		Engine::Safe_Release(pMesh);
		Engine::Safe_Release(pTransform);
		Engine::Safe_Release(pGameObject);
		Engine::Safe_Delete(tWeaponInfo);
	}

	CloseHandle(hFile);
	return S_OK;
}

void CWeapon::Free(void)
{
	Engine::CGameObject::Free();
}