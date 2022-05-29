/*!
 * \file Select_BackGround.h
 * \date 2016/11/14 9:53
 *
 * \author HyunGi
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef PickingRect_h__
#define PickingRect_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CRect_Texture;
	class CTexture;
	class CRenderer;
	class CShader;
}

class CPickingRect 
	: public Engine::CGameObject
{
private:
	explicit CPickingRect(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CPickingRect(void);

public:
	virtual HRESULT Start_ForScene(void);
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	Engine::CTransform*					m_pTransCom;
	Engine::CRect_Texture*				m_pBufferCom;
	Engine::CTexture*					m_pTextureCom;
	Engine::CRenderer*					m_pRendererCom;
	Engine::CShader*					m_pShaderCom;

public:
	_float								m_fDist;
	_bool								m_bRenderCheck;

public:
	HRESULT					Ready_Object(void);
	void					Set_Positon(_vec3* vPos);
	Engine::CRect_Texture*	Get_BufferCom(void);
	Engine::CTransform*		Get_TransCom(void);
	void					InitDist(void);



private:
	HRESULT Add_Component(void);

public:
	static CPickingRect* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);

public:
	virtual void Free(void);
};

#endif // Select_BackGround_h__