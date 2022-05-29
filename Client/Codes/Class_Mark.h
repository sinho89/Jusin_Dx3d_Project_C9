/*!
 * \file Class_Mark.h
 * \date 2016/11/24 15:42
 *
 * \author Shinho
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef Class_Mark_h__
#define Class_Mark_h__


#include "UI.h"

namespace Engine
{
	class CTransform;
	class CRect_Texture;
	class CTexture;
	class CRenderer;
	class CShader;
}

class CClass_Mark : public CUI
{
private:
	explicit CClass_Mark(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CClass_Mark(void);

public:
	virtual HRESULT Start_ForScene(void);
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

public:
	HRESULT Ready_Object(void);

private:
	Engine::CTransform*					m_pTransCom;
	Engine::CRect_Texture*				m_pBufferCom;
	Engine::CTexture*					m_pTextureCom;
	Engine::CRenderer*					m_pRendererCom;
	Engine::CShader*					m_pShaderCom;
public:
	void SetID(CLASSID eID) { m_eType = eID; }
private:
	CLASSID		m_eType;
private:
	HRESULT Add_Component(void);

public:
	static CClass_Mark* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CClass_Mark* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		CLASSID m_eType, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};

#endif // Class_Mark_h__