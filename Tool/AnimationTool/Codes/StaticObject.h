#ifndef StaticObject_h__
#define StaticObject_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CStaticMesh;
	class CRenderer;
	class CShader;
	class CCalculator;
	class CCollider;
}

class CStaticObject
	: public Engine::CGameObject
{
private:
	explicit CStaticObject(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CStaticObject(void);
public: //Getter
	_bool GetMouseCol(POINT ptMouse);
	_bool IsSelected(void);
	const _vec3* Get_Angle(_vec3* pAngle);
public: //Setter
	void SetSelected(_bool bIsSelect);
	void SetBone(Engine::CGameObject* pGameObject, const char* pBoneName);
	void SetPos(const _vec3& vPos);
	void SetScale(const _vec3& vScale);
	void SetRot(const _vec3& vRot);

	void Set_Collider(const _uint& iType);
	void Delete_Collider(void);
	void Set_ColliderPos(const _vec3& vPos);
	void Set_ColliderSize(const _vec3& vSize);
	void Set_ColliderSize(const _float& fRad);
public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);
private:
	Engine::CTransform* m_pTransCom;
	Engine::CRenderer* m_pRendererCom;
	Engine::CStaticMesh* m_pMeshCom;
	Engine::CShader* m_pShaderCom;
	Engine::CCalculator* m_pCalculatorCom;
	Engine::CCollider* m_pColliderCom;
private:
	const _matrix* m_pmatTarget;
	const _matrix* m_pmatWeapon;

private:
	_bool m_bIsSelect;
	_float m_fAlpha;
private:
	HRESULT Ready_Object(const wstring& wstrName);
	HRESULT Add_Component(const wstring& wstrName);
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	static CStaticObject* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
private:
	virtual void Free(void);	
};


#endif // StaticObject_h__
