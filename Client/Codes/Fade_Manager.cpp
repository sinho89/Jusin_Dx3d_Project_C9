#include "stdafx.h"
#include "Fade_Manager.h"
#include "UI.h"
#include "Fade.h"
IMPLEMENT_SINGLETON(CFade_Manager)

CFade_Manager::CFade_Manager(void)
{

}

CFade_Manager::~CFade_Manager(void)
{

}

HRESULT CFade_Manager::Ready_Fade(void)
{
	m_mapFade.clear();
	m_bFadeOut = false;
	m_bFadeIn = false;
	m_bFading = false;
	m_bResultFading = false;
	m_bBardielFade = false;

	return S_OK;
}

void CFade_Manager::AddFade(CUI* pFade, FADE_TYPE eType)
{
	m_mapFade.insert(MAPFADE::value_type(eType, pFade));
}

void CFade_Manager::FadeStart(_float fAlphaTime, _float fFadeSpeed, FADE_TYPE eType)
{
	m_mapFade[eType]->SetFade(true);
	m_mapFade[eType]->SetAlphaTime(fAlphaTime);
	m_mapFade[eType]->SetAngle(fFadeSpeed);

	m_bFadeOut = true;
}

void CFade_Manager::FadeEnd(_float fAlphaTime, _float fFadeSpeed, FADE_TYPE eType)
{
	m_mapFade[eType]->SetFade(false);
	m_mapFade[eType]->SetAlphaTime(fAlphaTime);
	m_mapFade[eType]->SetAngle(fFadeSpeed);

	m_bFadeOut = false;
}

void CFade_Manager::FadeImmidy(_float fAlphaTime, _float fFadeSpeed, FADE_TYPE eType)
{
	m_mapFade[eType]->SetPick(true);
	m_mapFade[eType]->SetFade(true);
	m_mapFade[eType]->SetAlphaTime(fAlphaTime);
	m_mapFade[eType]->SetAngle(fFadeSpeed);

}

void CFade_Manager::CreateSelectItem(FADE_TYPE eType)
{
	((CFade*)(m_mapFade[eType]))->CreateSelectItem();
}

void CFade_Manager::FadeReset(void)
{
	MAPFADE::iterator iter = m_mapFade.begin();
	MAPFADE::iterator iter_end = m_mapFade.end();

	for (; iter != iter_end; ++iter)
	{
		iter->second->SetPick(false);
		iter->second->SetFade(false);
		iter->second->SetAlphaTime(0.f);
		iter->second->SetAngle(0.f);
	}
	
}

void CFade_Manager::Free(void)
{

}
