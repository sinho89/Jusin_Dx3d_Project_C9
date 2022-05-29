#ifndef BoxCollider_h__
#define BoxCollider_h__

#include "Collider.h"


BEGIN(Engine)

class ENGINE_DLL CBoxCollider
	: public CCollider
{
private:
	explicit CBoxCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoxCollider(void);
public:
	virtual void Update_Component(const _float& fTimeDelta);
	virtual void Render_Collider(_bool bIsRender = true);
private:
	HRESULT Ready_BoxCol(CTransform* pParentTransform, const wstring& wstrTag, const _vec3& vPos, const _vec3& vScale, const _ulong& dwColor, _uint iIndex = 0);
public:
	static CBoxCollider* Create(LPDIRECT3DDEVICE9 pGraphicDev, CTransform* pParentTransform,
		const wstring& wstrTag, const _vec3& vPos, const _vec3& vScale, const _ulong& dwColor, _uint iIndex = 0);
private:
	virtual void Free(void);
};

END

#endif // BoxCollider_h__
