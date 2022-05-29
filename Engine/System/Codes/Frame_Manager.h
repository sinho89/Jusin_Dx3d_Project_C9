#ifndef Frame_Manager_h__
#define Frame_Manager_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CFrame;
class ENGINE_DLL CFrame_Manager 
	: public CBase
{
public:
	DECLARE_SINGLETON(CFrame_Manager)
private:
	explicit CFrame_Manager(void);
	virtual ~CFrame_Manager(void);
public:
	_bool isPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta);
public:
	HRESULT Ready_Frame(const _tchar* pFrameTag, const _float& fCallLimit);
private:
	map<const _tchar*, CFrame*>				m_mapFrames;
	typedef map<const _tchar*, CFrame*>		MAPFRAMES;
private:
	CFrame* Find_Frame(const _tchar* pFrameTag);
public:
	virtual void Free(void);
};


END

#endif // Frame_Manager_h__
