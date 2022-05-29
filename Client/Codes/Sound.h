#ifndef Sound_h__
#define Sound_h__


#include "Component.h"
#include "Defines.h"

namespace Engine
{
	class CTransform;
}

typedef struct tagChannel
{
	FMOD_CHANNEL*	pChannel;
	_bool			bLoop;
	_uint			dwTime;
	_uint			iLength;
}CHANNEL;

class CSound_Manager;

class CSound
	: public Engine::CComponent
{
private:
	explicit CSound(void);
	virtual ~CSound(void);
public:
	_bool	IsPlaying(const wstring& wstrKey);

public:
	_bool	MyPlaySound(const wstring& wstrKey, _bool bLoop = false, _bool bOverWrite = false);
	void	MyStopSound(const wstring& wstrKey);
	void	MyStopSoundAll(void);
	void	Set_Sound(const wstring& wstrKey, const wstring& wstrFileName);

public:
	virtual void Update_Component(const _float& fTimeDelta);

private:
	Engine::CTransform* m_pTransCom;
	CSound_Manager*		m_pSoundMgr;
private:
	FMOD_SYSTEM* m_pSystem;
	map<wstring, CHANNEL>			m_mapChannel;
	typedef map<wstring, CHANNEL>	MAPCHANNEL;
	map<wstring, FMOD_SOUND*>				m_mapSound;
	typedef map<wstring, FMOD_SOUND*>		MAPSOUND;

private:
	HRESULT Ready_Sound(Engine::CTransform* pTrans);

private:
	void Check_SoundEnd(void);
	void Check_Distance(void);

public:
	static CSound* Create(Engine::CTransform* pTrans);

private:
	virtual void Free(void);
};


#endif // Sound_h__
