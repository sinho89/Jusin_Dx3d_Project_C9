#ifndef Sound_Manager_h__
#define Sound_Manager_h__

#include "Defines.h"
#include "Base.h"

class CSound_Manager
	: public CBase
{
public:
	DECLARE_SINGLETON(CSound_Manager)
private:
	explicit CSound_Manager(void);
	virtual ~CSound_Manager(void);

public:
	FMOD_SYSTEM*	GetSystem(void);
	FMOD_SOUND*		GetSound(const wstring& wstrKey);
	_bool			ErrorCheck(FMOD_RESULT Result);

public:
	void	Ready_SoundManager(void);

private:
	FMOD_SYSTEM*									m_pSystem;
	map<wstring, FMOD_SOUND*>				m_mapSound;
	typedef map<wstring, FMOD_SOUND*>		MAPSOUND;
	FMOD_RESULT										m_Result;
	_uint											m_iVersion;

private:
	void	LoadSoundFile(void);

private:
	virtual void Free(void);
};

#endif // Sound_Manager_h__
