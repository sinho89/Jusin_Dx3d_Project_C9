#ifndef SphereCollider_h__
#define SphereCollider_h__

#include "Collider.h"


BEGIN(Engine)

class ENGINE_DLL CSphereCollider
	: public CCollider
{
private:
	explicit CSphereCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSphereCollider(void);
public:
	virtual void Update_Component(const _float& fTimeDelta);
	virtual void Render_Collider(_bool bIsRender = true);
private:
	HRESULT Ready_SphereCol(CTransform* pParentTransform, const wstring& wstrTag, const _vec3& vPos, const _float& fRad, _uint iIndex = 0);
public:
	static CSphereCollider* Create(LPDIRECT3DDEVICE9 pGraphicDev, CTransform* pParentTransform,
		const wstring& wstrTag, const _vec3& vPos, const _float& fRad, _uint iIndex = 0);
private:
	virtual void Free(void);
};

END

#endif // SphereCollider_h__
