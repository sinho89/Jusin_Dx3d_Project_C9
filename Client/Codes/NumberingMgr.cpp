#include "stdafx.h"
#include "NumberingMgr.h"
#include "UI.h"
#include "DamNum.h"

IMPLEMENT_SINGLETON(CNumbering_Manager)

CNumbering_Manager::CNumbering_Manager(void)
	:m_pCombo_Base(NULL)
	, m_pCombo_Effect(NULL)
{	

}

CNumbering_Manager::~CNumbering_Manager(void)
{

}

HRESULT CNumbering_Manager::Ready_Numbering(CUI* pCombo_Base, CUI* pCombo_Effect)
{
	m_pCombo_Base = pCombo_Base;
	m_pCombo_Effect = pCombo_Effect;

	m_iComboCnt = 0;
	m_iBeforeComboCnt = 0;
	
	m_fComboResetTime = 0.f;
	m_fProtecteForTickTime = 0.f;

	m_sUnitDigit = 0;
	m_sDoubleDigit = 0;
	m_sTrippleDigit = 0;

	m_bHit = false;
	m_bUpdate = false;

	m_vecDigit.clear();

	m_fSkill = 0.f;
	m_fKill = 0.f;
	m_fStyle = float(rand() % 10) + 80.f;
	m_fTechnic = 0.f;

	m_fRank = 0.f;
	m_fTotal = 0.f;
	m_fGold = 0.f;
	m_fMaxKill = 25.f;
	m_fMaxCombo = 0.f;

	return S_OK;
}

void CNumbering_Manager::AddVecDigit(CUI* pDigit)
{
	pDigit->SetIndex(0);
	pDigit->SetPick(false);
	pDigit->SetAngle(1.f);
	pDigit->SetAlpha(0.f);
	pDigit->SetSize(80.f, 80.f);
	pDigit->GetAlphaIncrease() = false;

	m_vecDigit.push_back(pDigit);
}


void CNumbering_Manager::Increase_ComboCount(void)
{
	m_bHit = true;
	m_iBeforeComboCnt = m_iComboCnt;
	++m_iComboCnt;
}

void CNumbering_Manager::Update_Combo(const _float& fTimeDelta)
{
	m_fProtecteForTickTime += fTimeDelta;

	if (m_fProtecteForTickTime >= 1.f)
	{
		if (m_iComboCnt < 3)
		{
			m_pCombo_Base->SetRender(false);
			return;
		}
		if (!m_bUpdate)
			return;
		
		m_bUpdate = false;
		
		if (m_iComboCnt % 10 == 0)
		{
			for (size_t i = 0; i < m_vecDigit.size(); ++i)
				m_vecDigit[i]->SetAngle(m_vecDigit[i]->GetAngle() - 0.2f);
		}

		m_fProtecteForTickTime = 0.f;

		m_sUnitDigit = m_iComboCnt % 10;

		m_sDoubleDigit = m_iComboCnt / 10;
		if (m_sDoubleDigit >= 10)
			m_sDoubleDigit -= 10 * (m_sDoubleDigit / 10);
		
		m_sTrippleDigit = m_iComboCnt / 100;
		if (m_sTrippleDigit >= 10)
			m_sTrippleDigit -= 10 * (m_sTrippleDigit / 10);

		m_vecDigit[0]->SetIndex((_uint)m_sUnitDigit);
		m_vecDigit[1]->SetIndex((_uint)m_sDoubleDigit);
		m_vecDigit[2]->SetIndex((_uint)m_sTrippleDigit);

		if (m_sTrippleDigit >= 1)
			m_vecDigit[1]->SetPick(true);

		m_pCombo_Base->SetAngle(10.f);
		m_pCombo_Base->SetWinPos(_vec2((WINSIZEX >> 1) + 750, (WINSIZEY >> 1) - 150));
		m_pCombo_Base->SetSize(150.f, 50.f);

		for (size_t i = 0; i < m_vecDigit.size(); ++i)
		{
			m_vecDigit[i]->SetAlpha(0.f);
			m_vecDigit[i]->SetSize(80.f, 80.f);
			m_vecDigit[i]->GetAlphaIncrease() = false;
		}
		m_pCombo_Effect->SetAlpha(0.f);
		m_pCombo_Effect->SetWinPos(_vec2((WINSIZEX >> 1) + 600, m_pCombo_Base->GetInfo().y + 25.f));
		m_pCombo_Effect->GetAlphaIncrease() = false;
		m_pCombo_Base->SetRender(true);
	}
}

void CNumbering_Manager::ResetCombo(const _float& fTimeDelta)
{
	m_fComboResetTime += fTimeDelta;

	if (m_bHit)
	{
		m_fComboResetTime = 0.f;
		m_bHit = false;
		m_bUpdate = true;
	}

	if (m_fComboResetTime >= 5.f)
	{
		m_pCombo_Base->SetRender(false);
		m_pCombo_Base->SetAngle(10.f);
		m_pCombo_Base->SetWinPos(_vec2((WINSIZEX >> 1) + 750, (WINSIZEY >> 1) - 150));
		m_pCombo_Base->SetSize(150.f, 50.f);

		m_pCombo_Effect->SetAlpha(0.f);
		m_pCombo_Effect->SetWinPos(_vec2((WINSIZEX >> 1) + 600, m_pCombo_Base->GetInfo().y + 25.f));
		m_pCombo_Effect->GetAlphaIncrease() = false;

		_uint iCompareCombo = max(_uint(m_fMaxCombo), m_iComboCnt);	// SetMaxCombo
		if(iCompareCombo == m_iComboCnt)
			m_fMaxCombo = float(m_iComboCnt);

		m_iComboCnt = 0;
		m_fComboResetTime = 0.f;
		m_fProtecteForTickTime = 0.f;

		for (size_t i = 0; i < m_vecDigit.size(); ++i)
		{
			m_vecDigit[i]->SetIndex(0);
			m_vecDigit[i]->SetPick(false);
			m_vecDigit[i]->SetAngle(1.f);
			m_vecDigit[i]->SetAlpha(0.f);
			m_vecDigit[i]->SetSize(80.f, 80.f);
			m_vecDigit[i]->GetAlphaIncrease() = false;
		}

	}
}

void CNumbering_Manager::SetKillCount()
{
	m_fKill += 1.f;
}

void CNumbering_Manager::SetSkillCount()
{
	m_fSkill += 1.f;
}



void CNumbering_Manager::Render_ResultNum()
{

}

void CNumbering_Manager::Free(void)
{

}
