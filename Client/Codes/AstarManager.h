#ifndef AstarManager_h__
#define AstarManager_h__

#include "Defines.h"
#include "Base.h"

namespace Engine
{
	class CCell;
}
class CAStarManager
	:public CBase
{
public:
	DECLARE_SINGLETON(CAStarManager)
private:
	explicit CAStarManager(void);
	virtual ~CAStarManager(void);

public:
	static unsigned int APIENTRY Thread_Main(void* pArg);
	void Complete_Thread(void);
public:
	CRITICAL_SECTION* Get_CriticalSection(void) {
		return &m_Crt;
	}
public:
	void  Ready_AStarMgr(vector<Engine::CCell*>* pVecCell,
		list<_vec3>* pBestList,
		_vec3* pStartPos,
		_vec3* pEndPos,
		_ulong dwStartIndex,
		_ulong dwEndIndex);

public:			//길찾기하는 함수들..
	_ulong Get_CurrentIndex(_vec3* pPos);
	void   FindPath(void);
	void   Release_Path(void);
	void   Add_Node(Engine::NODE* pParent, _float fGCost, _float fHCost,
		_ulong dwIndex, _vec3* pPos);
	_bool  CheckList(_ulong dwIndex);

	void   StopThread(void);

private:						//멀티쓰레드용 변수..
	HANDLE						m_hThread;
	CRITICAL_SECTION			m_Crt;
	_bool						m_bWait;

private:
	multimap<float, Engine::NODE*>	m_OpenMap;
	vector<Engine::CCell*>*			m_pvecCell;
	list<_vec3>*					m_pBestList;

private:
	_vec3					m_vStartPos;
	_vec3					m_vEndPos;
	_ulong					m_dwStartIndex;
	_ulong					m_dwEndIndex;
	_ulong					m_dwIndex;		//A스타용 현재 인덱스...
	_uint					m_iCount;
	_uint					m_iLoop;
private:
	virtual void			Free(void);




};
#endif // AstarManager_h__
