#ifndef Collision_Manager_h__
#define Collision_Manager_h__

#include "Base.h"
#include "Engine_Defines.h"

BEGIN(Engine)

class CCollider;
class CBoxCollider;
class CSphereCollider;
class CTransform;

class ENGINE_DLL CCollision_Manager
	: public CBase
{
public:
	DECLARE_SINGLETON(CCollision_Manager)
private:
	typedef struct tagOBB
	{
		_vec3			vPoint[8];
		_vec3			vCenter;
		_vec3			vProjAxis[3]; // 투영해야할 벡터
		_vec3			vAxis[3]; // 투영받는 벡터
	}OBB;
private:
	explicit CCollision_Manager(void);
	virtual ~CCollision_Manager(void);

public:	//Getter
	_bool IsCol_ForOBB(const _tchar* pTargetTag, const CCollider* pMyCollider, const _vec3* pNextPos);
	_bool IsCollision(const _tchar* pTargetTag, COLINFO* pColInfo, const CCollider* pMyCollider);
	_bool IsCapture(const _tchar* pTargetTag, CCollider* pCollider);
public: //Setter
	void Add_ColliderGroup(COL_TYPE eType, const _tchar* pTag, CCollider* pCollider);
	void Clear_ColliderGroup(void);
	void Clear_All(void);
	void Delete_Collider(const _tchar* pTag, const CCollider* pMyCollider);
public:
	CBoxCollider* Create_BoxCollider(LPDIRECT3DDEVICE9 pGraphicDev, CTransform* pParentTransform, const wstring& wstrTag, const _vec3& vPos, const _vec3& vScale, const _ulong& dwColor);
	CSphereCollider* Create_SphereCollider(LPDIRECT3DDEVICE9 pGraphicDev, CTransform* pParentTransform, const wstring& wstrTag, const _vec3& vPos, const _float& fRad);

private:
	map<wstring, list<CCollider*>> m_mapCollider[COL_END];
	typedef map<wstring, list<CCollider*>> MAPCOLLIDER;

private:
	_uint m_iColliderIndex;

private:
	_bool Collision_OBB(const _vec3* pDestMin, const _vec3* pDestMax, const _matrix* pDestWorld
		, const _vec3* pSourMin, const _vec3* pSourMax, const _matrix* pSourWorld);
	void Set_Point(OBB* pObb, const _vec3* pMin, const _vec3* pMax);
	void Set_Axis(OBB* pObb);

private:
	virtual void Free(void);
};

END

#endif // Collision_Manager_h__
