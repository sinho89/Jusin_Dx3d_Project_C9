/*!
 * \file Inven_Base.h
 * \date 2016/11/24 21:43
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

#ifndef Inven_Base_h__
#define Inven_Base_h__


#include "UI.h"
#include "Defines.h"

namespace Engine
{
	class CTransform;
	class CRect_Texture;
	class CTexture;
	class CRenderer;
	class CShader;
}
class CInven_Space;
class CWear_ToolTip;
class CPlayer;
class CInven_Base : public CUI
{
private:
	explicit CInven_Base(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CInven_Base(void);

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
	CPlayer*							m_pPlayer;
	CWear_ToolTip*						m_pToolTip;

public:
	void Window_MoveToMouse();
	void RenderToolTip();

private:
	vector<CInven_Space*>		m_vecSpace;

public:
	void AddVecSpace(CInven_Space* pSpace) { m_vecSpace.push_back(pSpace); }

private:
	HRESULT Add_Component(void);

public:
	static CInven_Base* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};

#endif // Inven_Base_h__