/*!
 * \file State_Gage.h
 * \date 2016/11/24 13:45
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

#ifndef State_Gage_h__
#define State_Gage_h__


#include "UI.h"

namespace Engine
{
	class CTransform;
	class CRect_Texture;
	class CTexture;
	class CRenderer;
	class CShader;
}

class CPlayer;
class CState_Gage : public CUI
{
private:
	explicit CState_Gage(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CState_Gage(void);

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

private:
	HRESULT Add_Component(void);
	void SetGageSize(const _float& fTimeDelta);

public:
	void SetPlayerInfo(PLAYER_INFO* pPlayerInfo);
private:
	_float								m_fMagicScale;
public:
	static CState_Gage* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CState_Gage* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		_uint iIndex, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};



#endif // State_Gage_h__