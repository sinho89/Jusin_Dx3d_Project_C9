#ifndef Collider_h__
#define Collider_h__

#include "Component.h"

BEGIN(Engine)

class CVIBuffer;
class CTransform;

class ENGINE_DLL CCollider
	: public CComponent
{
protected:
	explicit CCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCollider(void);
public:
	void SetScale(const _vec3& vScale);
	void SetScale(const _float& fRad);
	void SetPos(const _vec3& vPos);
	void Change_BoxColor(const _ulong& dwColor);
	void Set_ColInfo(const COLINFO* pColInfo);
	void Clear_Map(void);
	void Add_IsCol(const _uint& iIndex);
	void Set_Render(_bool bRender);
public:
	void GetScale(_vec3* pScale) const;
	void GetScale(_float* pRad) const;
	void GetPos(_vec3* pPos) const;
	void GetWorldMatrix(_matrix* pMatrix) const;
	_uint Get_Index(void);
	const COLINFO* Get_ColInfo(void) const;
	_bool Get_IsCol(const _uint& iIndex);
	CTransform* Get_ParentTrans(void);
	_bool IsHit(void);
public:
	virtual void Update_Component(const _float& fTimeDelta)PURE;
	virtual void Render_Collider(_bool bIsRender = true)PURE;
protected:
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	CTransform* m_pParentTransform;
	CVIBuffer* m_pBuffer;
protected:
	map<_uint, _bool> m_mapColCheck;
	_matrix m_matWorld;
	_vec3 m_vPos;
	_vec3 m_vScale;
	wstring m_wstrTag;
	COL_TYPE m_eType;
	COLINFO m_tColInfo;
protected:
	_uint m_iIndex; //식별번호? 뭐이런거
	_bool m_bIsRender;

protected:
	virtual void Free(void);
};

END

#endif // Collider_h__
