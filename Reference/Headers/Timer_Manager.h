#ifndef Timer_Manager_h__
#define Timer_Manager_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CTimer;
class ENGINE_DLL CTimer_Manager 
	: public CBase
{
public:
	DECLARE_SINGLETON(CTimer_Manager)
private:
	explicit CTimer_Manager(void);
	virtual ~CTimer_Manager(void);
public:
	_float Get_TimeDelta(const _tchar* pTimerTag) const;
public:
	void SetUp_TimeDelta(const _tchar* pTimerTag);
public:
	HRESULT Ready_Timer(const _tchar* pTimerTag);
public:
	map<const _tchar*, CTimer*>				m_mapTimers;
	typedef map<const _tchar*, CTimer*>		MAPTIMER;
private:
	CTimer* Find_Timer(const _tchar* pTimerTag) const;
public:
	virtual void Free(void);
};

END

#endif // Timer_Manager_h__
