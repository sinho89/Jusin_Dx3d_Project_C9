#ifndef Layer_h__
#define Layer_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CGameObject;
class CComponent;

class ENGINE_DLL CLayer 
	: public CBase
{
private:
	explicit CLayer(void);
	virtual ~CLayer(void);
public:
	CGameObject* FindWithTag(const _tchar* pObjKey);
	CGameObject* Find(const _tchar* pObjKey);
	list<CGameObject*>* FindList(const _tchar* pObjKey);
	map<wstring, list<CGameObject*>>* Get_MapObject(void);
	_bool Get_Delete(void);
	PSRWLOCK Get_Lock(PSRWLOCK* pLock);
public:
	HRESULT AddObject(const _tchar* pObjectKey, CGameObject* pGameObject);
public:
	virtual HRESULT Start_ForScene(void); //스테이지 생성 전에 레이어에 접근 할 필요가 있을때 초기화 함수.
	virtual void Update_Layer(const _float& fTimeDelta);
	virtual void Render_Layer(void);

private:
	typedef list<CGameObject*>				OBJECTLIST;
	typedef map<wstring, OBJECTLIST>	MAPOBJLIST;
	MAPOBJLIST		m_mapObjlist;

private:
	_bool		m_bDelete;
	PSRWLOCK	m_psrwLock;

public:
	void SafeRelease(void);
public:
	static CLayer* Create(void);
	virtual void Free(void);
};

END

#endif // Layer_h__
