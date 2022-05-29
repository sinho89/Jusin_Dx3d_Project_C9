#include "stdafx.h"
#include "PlayerDamParent.h"
#include "Monster.h"
#include "Player.h"
#include "PlayerDamNum.h"
CPlayerDamParent::CPlayerDamParent(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
	:CUI(pGraphicDev, wstrName, eState, bIsActive)
{

}

CPlayerDamParent::~CPlayerDamParent(void)
{

}

HRESULT CPlayerDamParent::Ready_Object(void)
{

	CUI::Start_ForScene();
	return S_OK;
}

HRESULT CPlayerDamParent::Start_ForScene(void)
{
	return S_OK;
}

void CPlayerDamParent::Update_Object(const _float& fTimeDelta)
{
	
}


void CPlayerDamParent::Render_Object(void)
{

}

void CPlayerDamParent::SetDamageNumber(_uint iDamageNumber, _bool bCritical)
{
	m_listDam.clear();
	
	if (iDamageNumber >= 10000)
		iDamageNumber = 9999;

	if (iDamageNumber < 100)
	{
		return;
	}

	Engine::CGameObject* pGameObject = CPlayerDamNum::Create(m_pGraphicDev, L"Dam_One", m_pParentObject, NUM_ONE, 0, false);
	Engine::AddObject(Engine::LAYER_UI, L"UI_Dam", pGameObject);
	m_listDam.push_back((CUI*)pGameObject);

	pGameObject = CPlayerDamNum::Create(m_pGraphicDev, L"Dam_Ten", m_pParentObject, NUM_TEN, 0, false);
	Engine::AddObject(Engine::LAYER_UI, L"UI_Dam", pGameObject);
	m_listDam.push_back((CUI*)pGameObject);

	pGameObject = CPlayerDamNum::Create(m_pGraphicDev, L"Dam_Hun", m_pParentObject, NUM_HUN, 0, false);
	Engine::AddObject(Engine::LAYER_UI, L"UI_Dam", pGameObject);
	m_listDam.push_back((CUI*)pGameObject);

	pGameObject = CPlayerDamNum::Create(m_pGraphicDev, L"Dam_Tou", m_pParentObject, NUM_TOU, 0, false);
	Engine::AddObject(Engine::LAYER_UI, L"UI_Dam", pGameObject);
	m_listDam.push_back((CUI*)pGameObject);
	
	if (iDamageNumber >= 1000)
	{
		LISTPLAYERDAM::iterator iter = m_listDam.begin();
		LISTPLAYERDAM::iterator iter_end = m_listDam.end();

		for (; iter != iter_end; ++iter)
		{
			_uint iNum = iDamageNumber % 10;

			(*iter)->SetIndex(iNum);
			((CPlayerDamNum*)(*iter))->SetFive(true);

			iDamageNumber /= 10;
		}
	}
	else if (iDamageNumber >= 100 && iDamageNumber < 1000)
	{
		LISTPLAYERDAM::iterator iter = m_listDam.begin();
		LISTPLAYERDAM::iterator iter_end = m_listDam.end();

		for (; iter != iter_end; ++iter)
		{
			_uint iNum = iDamageNumber % 10;

			(*iter)->SetIndex(iNum);
			((CPlayerDamNum*)(*iter))->SetFive(false);

			iDamageNumber /= 10;
		}
	}

}

CPlayerDamParent* CPlayerDamParent::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, Engine::CGameObject* pParentObject, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CPlayerDamParent*		pInstance = new CPlayerDamParent(pGraphicDev, wstrName, eState, bIsActive);

	pInstance->SetParent(pParentObject);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerDamParent::Free(void)
{
	CUI::Free();
}
