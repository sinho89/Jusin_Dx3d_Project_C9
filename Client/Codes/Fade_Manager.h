#ifndef Fade_Manager_h__
#define Fade_Manager_h__


#include "Defines.h"
#include "Base.h"

namespace Engine
{
}
class CUI;
class CFade_Manager
	: public CBase
{
public:
	enum FADE_TYPE {FADE_BOSS_INTRO, FADE_BOSS_OUTTRO, FADE_CLEAR_GETITEM, FADE_END};
public:
	DECLARE_SINGLETON(CFade_Manager)

private:
	explicit CFade_Manager(void);
	virtual ~CFade_Manager(void);

public:
	_bool GetFading(void) { return m_bFading; }
	_bool GetFadeIn(void) { return m_bFadeIn; }
	_bool GetFadeOut(void) { return m_bFadeOut; }
	_bool GetResultFading(void) { return m_bResultFading; }

public:
	void SetFading(_bool bFading) { m_bFading = bFading; }
	void SetFadeIn(_bool bFadeIn) { m_bFadeIn = bFadeIn; }
	void SetFadeOut(_bool bFadeOut) { m_bFadeOut = bFadeOut; }
	void SetResultFading(_bool bResultFading) {	m_bResultFading = bResultFading;}

private:
	map<FADE_TYPE, CUI*>			m_mapFade;
	typedef map<FADE_TYPE, CUI*>	MAPFADE;
	_bool					m_bFadeOut;
	_bool					m_bFadeIn;
	_bool					m_bFading;
	_bool					m_bResultFading;
	_bool					m_bBardielFade;

public:
	void SetBardielFade(_bool bBardielFade) { m_bBardielFade = bBardielFade; }
	_bool GetBardielFade(void) { return m_bBardielFade; }

public:
	HRESULT Ready_Fade(void);
	void AddFade(CUI* pFade, FADE_TYPE eType);
	void FadeStart(_float fAlphaTime, _float fFadeSpeed, FADE_TYPE eType);
	void FadeEnd(_float fAlphaTime, _float fFadeSpeed, FADE_TYPE eType);
	void FadeImmidy(_float fAlphaTime, _float fFadeSpeed, FADE_TYPE eType);
	void CreateSelectItem(FADE_TYPE eType);
	void FadeReset(void);

private:
	virtual void Free(void);
};


#endif // Fade_Manager_h__
