#ifndef Weapon_h__
#define Weapon_h__

#include "GameObject.h"
#include "Defines.h"

namespace Engine
{
	class CTransform;
	class CRenderer;
	class CStaticMesh;
	class CShader;
}

class CWeapon
	: public Engine::CGameObject
{
protected:
	explicit CWeapon(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CWeapon(void);
public:
	void Set_Alpha(const _float& fAlpha);
public:
	virtual HRESULT Start_ForScene(void);
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);
	virtual	void Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);
protected:
	Engine::CTransform* m_pTransCom;
	Engine::CRenderer* m_pRendererCom;
	Engine::CStaticMesh* m_pMeshCom;
	Engine::CShader* m_pShaderCom;
protected:
	const _matrix* m_pmatTarget;
	const _matrix* m_pmatWeapon;
protected:
	_float	m_fAlpha;
	_bool	m_bRender;
	wstring 	m_strParentTag;
public:
	void SetRender(_bool bRender) { m_bRender = bRender; }
protected:
	HRESULT LoadWeaponData(const _tchar* pTargetName, const _tchar* pFilePath);

protected:
	virtual void Free(void);
};

#endif // Weapon_h__
