#include "stdafx.h"
#include "NPC_Book.h"
#include "Export_Engine.h"
#include "ObjectName.h"
#include "NPC_Name.h"
#include "Npc_Col.h"
#include "Sound.h"

CNPC_Book::CNPC_Book(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
: CNPC(pGraphicDev, wstrName)
{
	//ZeroMemory(&m_tInfo, sizeof(OBJ_INFO));
}
CNPC_Book::~CNPC_Book(void)
{

}

void CNPC_Book::Update_Object(const _float& fTimeDelta)
{
	CNPC::Update_Object(fTimeDelta);

}

void CNPC_Book::Render_Object(void)
{
	//CNPC::Render_Object();
}

void CNPC_Book::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
{
	CNPC::Render_Object(pEffect, iPassIdx);
}

HRESULT CNPC_Book::Ready_Object(void)
{
	CNPC::Ready_Object();

	Engine::CGameObject* pGameObject = CNPC_Name::Create(m_pGraphicDev, L"Acc_Npc_Name", this, SHOP_ACC);
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"Name_UI", pGameObject);

	pGameObject = CNpc_Col::Create(m_pGraphicDev, L"Weapon_Eff", this);
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_Eff", pGameObject);

	m_pUIEff = pGameObject;

	m_bOutlineColor = _vec3(1.0f, 0.8549f, 0.7254f);

	Engine::CComponent* pComponent = NULL;

	pComponent = CSound::Create(m_pTransCom);
	if (pComponent == NULL)
		return E_FAIL;
	m_pSoundCom = (CSound*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"SoundCom", pComponent));

	m_pSoundCom->Set_Sound(L"Sound1", L"AccNPC1.wav");
	m_pSoundCom->Set_Sound(L"Sound2", L"AccNPC2.wav");
	m_pSoundCom->Set_Sound(L"Sound3", L"AccNPC3.wav");

	return S_OK;
}


void CNPC_Book::Free(void)
{
	Engine::CGameObject::Free();
	//	Safe_Delete(m_pMouseCol);

}

CNPC_Book* CNPC_Book::Create( LPDIRECT3DDEVICE9 pGraphicDev ,const wstring& wstrName )
{
	CNPC_Book*		pInstance = new CNPC_Book(pGraphicDev, wstrName);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CGrid Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}
