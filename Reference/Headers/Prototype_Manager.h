#ifndef Prototype_Manager_h__
#define Prototype_Manager_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CComponent;

class ENGINE_DLL CPrototype_Manager 
	: public CBase
{
public:
	DECLARE_SINGLETON(CPrototype_Manager)
private:
	explicit CPrototype_Manager(void);
	virtual ~CPrototype_Manager(void);
public:
	HRESULT Ready_Prototype(const _tchar* pProtoTag, CComponent* pInstance);
	CComponent* Clone_Prototype(const _tchar* pProtoTag);
private:
	map<const _tchar*, CComponent*>			m_mapPrototype;
	typedef map<const _tchar*, CComponent*>	MAPPROTOTYPE;
private:
	CComponent* Find_Prototype(const _tchar* pProtoTag);
public:
	virtual void Free(void);
};

END

#endif // Prototype_Manager_h__
