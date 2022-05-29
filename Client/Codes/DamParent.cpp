#include "stdafx.h"
#include "DamParent.h"
#include "DamNum.h"
#include "Critical.h"
#include "Monster.h"
#include "Player.h"
CDamParent::CDamParent(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
	:CUI(pGraphicDev, wstrName, eState, bIsActive)
{

}

CDamParent::~CDamParent(void)
{

}

HRESULT CDamParent::Ready_Object(void)
{

	CUI::Start_ForScene();
	return S_OK;
}

HRESULT CDamParent::Start_ForScene(void)
{
	return S_OK;
}

void CDamParent::Update_Object(const _float& fTimeDelta)
{
	if (((CMonster*)m_pParentObject)->GetInfo()->iHp <= 0.f)
	{
		switch (((CMonster*)m_pParentObject)->GetInfo()->m_eMonID)
		{
		case MON_B_War:
			m_pPlayer->GetPlayerInfo()->m_fExp += 250.f;
			break;
		case MON_B_Axe:
			m_pPlayer->GetPlayerInfo()->m_fExp += 300.f;
			break;
		case MON_B_Bow:
			m_pPlayer->GetPlayerInfo()->m_fExp += 250.f;
			break;
		case MON_KNOLE_COMMANDER:
			m_pPlayer->GetPlayerInfo()->m_fExp += 550.f;
			break;
		case MON_HANUMAN:
			m_pPlayer->GetPlayerInfo()->m_fExp += 8000.f;
			break;
		case MON_BARDIEL:
			m_pPlayer->GetPlayerInfo()->m_fExp += 20000.f;
			break;
		}

		m_eState = Engine::STATE_DESTROY;

	}
}


void CDamParent::Render_Object(void)
{

}

void CDamParent::SetDamageNumber(_uint iDamageNumber, _bool bCritical)
{
	m_listDam.clear();

	//bCritical = true;

	if (iDamageNumber < 1000)
	{
		return;
	}
	Engine::CGameObject* pGameObject = CDamNum::Create(m_pGraphicDev, L"Dam_One", m_pParentObject, NUM_ONE, 0, false);
	Engine::AddObject(Engine::LAYER_UI, L"UI_Dam", pGameObject);
	m_listDam.push_back((CUI*)pGameObject);

	pGameObject = CDamNum::Create(m_pGraphicDev, L"Dam_Ten", m_pParentObject, NUM_TEN, 0, false);
	Engine::AddObject(Engine::LAYER_UI, L"UI_Dam", pGameObject);
	m_listDam.push_back((CUI*)pGameObject);

	pGameObject = CDamNum::Create(m_pGraphicDev, L"Dam_Hun", m_pParentObject, NUM_HUN, 0, false);
	Engine::AddObject(Engine::LAYER_UI, L"UI_Dam", pGameObject);
	m_listDam.push_back((CUI*)pGameObject);

	pGameObject = CDamNum::Create(m_pGraphicDev, L"Dam_Tou", m_pParentObject, NUM_TOU, 0, false);
	Engine::AddObject(Engine::LAYER_UI, L"UI_Dam", pGameObject);
	m_listDam.push_back((CUI*)pGameObject);

	pGameObject = CDamNum::Create(m_pGraphicDev, L"Dam_Mil", m_pParentObject, NUM_MIL, 0, false);
	Engine::AddObject(Engine::LAYER_UI, L"UI_Dam", pGameObject);
	m_listDam.push_back((CUI*)pGameObject);


	if (iDamageNumber >= 10000)
	{
		LISTDAM::iterator iter = m_listDam.begin();
		LISTDAM::iterator iter_end = m_listDam.end();

		int iIndex = 0;
		for (; iter != iter_end; ++iter)
		{
			_uint iNum = iDamageNumber % 10;

			Engine::CGameObject* pGameObject = CDamNum::Create(m_pGraphicDev, L"Dam_BackBlur", m_pParentObject, (NUM_ID)iIndex, iNum, true);
			Engine::AddObject(Engine::LAYER_UI, L"UI_Dam", pGameObject);

			(*iter)->SetIndex(iNum);
			((CDamNum*)(*iter))->SetFive(true);
			((CDamNum*)pGameObject)->SetFive(true);
			iDamageNumber /= 10;
			++iIndex;

			if (bCritical)
			{
				((CDamNum*)pGameObject)->SetCritical(true);

				(*iter)->SetSize(40.f, 45.f);
				((CDamNum*)(*iter))->SetCritical(true);
			}
			else
			{
				(*iter)->SetSize(30.f, 35.f);
				((CDamNum*)(*iter))->SetCritical(false);
			}
		}
	}
	else if(iDamageNumber >= 1000 && iDamageNumber < 10000)
	{
		LISTDAM::iterator iter = m_listDam.begin();
		LISTDAM::iterator iter_end = m_listDam.end();

		int iIndex = 0;
		for (; iter != iter_end; ++iter)
		{
			_uint iNum = iDamageNumber % 10;

			Engine::CGameObject* pGameObject = CDamNum::Create(m_pGraphicDev, L"Dam_BackBlur", m_pParentObject, (NUM_ID)iIndex, iNum, true);
			Engine::AddObject(Engine::LAYER_UI, L"UI_Dam", pGameObject);

			(*iter)->SetIndex(iNum);
			((CDamNum*)(*iter))->SetFive(false);
			((CDamNum*)pGameObject)->SetFive(false);
			iDamageNumber /= 10;
			++iIndex;
			if (bCritical)
			{
				((CDamNum*)pGameObject)->SetCritical(true);

				(*iter)->SetSize(40.f, 45.f);
				((CDamNum*)(*iter))->SetCritical(true);
			}
			else
			{
				(*iter)->SetSize(30.f, 35.f);
				((CDamNum*)(*iter))->SetCritical(false);
			}
		}
	}
	if (bCritical)
	{
		Engine::CGameObject* pGameObject = CCritical::Create(m_pGraphicDev, L"Critical", m_pParentObject);
		Engine::AddObject(Engine::LAYER_UI, L"UI_Dam", pGameObject);
		bCritical = false;
	}

}

CDamParent* CDamParent::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, Engine::CGameObject* pParentObject, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CDamParent*		pInstance = new CDamParent(pGraphicDev, wstrName, eState, bIsActive);

	pInstance->SetParent(pParentObject);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CDamParent::Free(void)
{
	CUI::Free();
}
