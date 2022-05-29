#ifndef Grid_h__
#define Grid_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CGrid_Col;
	class CRenderer;
}

class CGrid
	: public Engine::CGameObject
{
private:
	explicit CGrid(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CGrid(void);
public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);
private:
	Engine::CTransform* m_pTransCom;
	Engine::CGrid_Col* m_pBufferCom;
	Engine::CRenderer* m_pRendererCom;
private:
	HRESULT Ready_Object(void);
	HRESULT Add_Component(void);
public:
	static CGrid* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
private:
	virtual void Free(void);
};

#endif // Grid_h__
