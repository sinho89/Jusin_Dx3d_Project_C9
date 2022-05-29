#include "stdafx.h"
#include "CoolTimeMgr.h"
#include "UI_CoolTime.h"

IMPLEMENT_SINGLETON(CCoolTime_Manager)

CCoolTime_Manager::CCoolTime_Manager(void)
{

}

CCoolTime_Manager::~CCoolTime_Manager(void)
{

}

void CCoolTime_Manager::AddCoolTime(COOLTYPE eType, CUI_Skill* pSkill)
{
	m_SkillMap.insert(SKILLMAP::value_type(eType, pSkill));
}

void CCoolTime_Manager::StartCoolTime(COOLTYPE eType)
{
	SKILLMAP::iterator iter = m_SkillMap.find(eType);

	if (iter == m_SkillMap.end())
		return;

	iter->second->SetRender(true);
}

CUI_Skill* CCoolTime_Manager::FindCoolTime(COOLTYPE eType)
{
	SKILLMAP::iterator iter = m_SkillMap.find(eType);

	if (iter == m_SkillMap.end())
		return NULL;

	return iter->second;
}


bool CCoolTime_Manager::CheckCoolTime(COOLTYPE eType)
{
	SKILLMAP::iterator iter = m_SkillMap.find(eType);

	if (iter == m_SkillMap.end())
		return true;

	if (iter->second->GetRender() == true)
		return false;
	
	return true;
}

void CCoolTime_Manager::Ready_CoolTime(void)
{
	m_SkillMap.clear();
}

void CCoolTime_Manager::UpdateCoolTime(void)
{
	SKILLMAP::iterator iter = m_SkillMap.begin();
	SKILLMAP::iterator iter_end = m_SkillMap.end();

	if (iter == iter_end)
		return;

	_uint iSkillSize = 0;

	iter = m_SkillMap.begin();

	multimap<_float, CUI_Skill*>	m_SortRenderMap;

	for (; iter != iter_end; ++iter)
		m_SortRenderMap.insert(multimap<_float, CUI_Skill*>::value_type(iter->second->GetAlphaTime(), iter->second));

	multimap<_float, CUI_Skill*>::iterator Sort_iter = m_SortRenderMap.begin();
	multimap<_float, CUI_Skill*>::iterator Sort_iter_end = m_SortRenderMap.end();

	for (; Sort_iter != Sort_iter_end; ++Sort_iter)
	{
		if (Sort_iter->second->GetRender())
		{
			Sort_iter->second->SetWinPos(_vec2((WINSIZEX >> 1) - (iSkillSize  * 25.f), _float(WINSIZEY - 150)));
			++iSkillSize;
		}
	}

	Sort_iter = m_SortRenderMap.begin();

	for (; Sort_iter != Sort_iter_end; ++Sort_iter)
	{
		if (Sort_iter->second->GetRender())
			(*Sort_iter->second->GetfX()) += ((iSkillSize - 1) * 10.f);
	}

	m_SortRenderMap.clear();

}

void CCoolTime_Manager::Free(void)
{

}

