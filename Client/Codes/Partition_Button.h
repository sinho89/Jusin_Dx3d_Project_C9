/*!
 * \file Partition_Button.h
 * \date 2016/12/10 20:21
 *
 * \author ShinHo
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef Partition_Button_h__
#define Partition_Button_h__


#include "UI.h"

namespace Engine
{
	class CTransform;
	class CRect_Texture;
	class CTexture;
	class CRenderer;
	class CShader;
	class CScene;
}

class CPartition_Button : public CUI
{
private:
	explicit CPartition_Button(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CPartition_Button(void);

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
	Engine::CTexture*					m_pClickTextureCom;
	Engine::CRenderer*					m_pRendererCom;
	Engine::CShader*					m_pShaderCom;
	Engine::CScene*						m_pScene;

private:
	CUI*								m_pParent;
	bool								m_bType;
public:
	void PtInRect_Collision(const _float& fTimeDelta);
	void SetParent(CUI* pParent);
	void SetType(_bool bType) { m_bType = bType; }
	void SetScene(Engine::CScene* pScene) { m_pScene = pScene; }

private:
	HRESULT Add_Component(void);

public:
	static CPartition_Button* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CPartition_Button* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		CUI* pParent, _bool bType, Engine::CScene* pScene, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};

#endif // Partition_Button_h__