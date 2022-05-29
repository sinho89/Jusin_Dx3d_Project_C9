/*!
 * \file Inven_Sheet.h
 * \date 2016/11/27 13:08
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

#ifndef Inven_Sheet_h__
#define Inven_Sheet_h__


#include "UI.h"

namespace Engine
{
	class CTransform;
	class CRect_Texture;
	class CTexture;
	class CRenderer;
	class CShader;
}
class CUI_Item;
class CToolTip;
class CCompare_ToolTip;
class CInven_Sheet : public CUI
{
private:
	explicit CInven_Sheet(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CInven_Sheet(void);

public:
	virtual HRESULT Start_ForScene(void);
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

public:
	HRESULT Ready_Object(void);

private:
	Engine::CTransform*					m_pTransCom;
	Engine::CRect_Texture*				m_pBufferCom;
	Engine::CTexture*					m_pNCTextureCom;
	Engine::CTexture*					m_pCTextureCom;
	Engine::CTexture*					m_pSTextureCom;
	Engine::CRenderer*					m_pRendererCom;
	Engine::CShader*					m_pShaderCom;

private:
	CUI*					m_pParentWindow;
	CUI*					m_pArrayItem;
	CToolTip*				m_pToolTip;
	CCompare_ToolTip*		m_pCompare_ToolTip;
	_vec2					m_vParentBasePos;
	vector<CUI*>			m_vecBlock;
	multimap<ITEM_ID, CUI_Item*>	m_ArrayMap;
	typedef multimap<ITEM_ID, CUI_Item*>	ARRAYMAP;

public:
	void PtInRect_Collision(const _float& fTimeDelta);
	void SetParent(CUI* pParentWindow);
	vector<CUI*>* GetVecBlock(void);
	void AddVecBlock(_uint iIndex, CUI* pUI);
	void ItemPick(void);
	void ItemPartition(void);
	void RenderToolTip(void);
	void FadeCheck(void);
	_uint EmptySpaceCheck(SLOT_ID Id);
	void ArrayItem(void);

public:
	void AddInven(CUI_Item* pItem);
	void AddResultInven(CUI_Item* pItem);

private:
	HRESULT Add_Component(void);

public:
	static CInven_Sheet* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CInven_Sheet* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		CUI* pParentWindow, _uint iIndex, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};



#endif // Inven_Sheet_h__