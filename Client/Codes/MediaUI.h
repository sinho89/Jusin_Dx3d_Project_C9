/*!
 * \file MediaUI.h
 * \date 2016/11/18 21:51
 *
 * \author WINTER
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef MediaUI_h__
#define MediaUI_h__

#include "UI.h"

namespace Engine
{
	class CMediaObj;
	class CTransform;
	class CRect_Texture;
	class CRenderer;
	class CShader;
}

class COpening_Banner;
class CMediaUI
	: public CUI
{
private:
	explicit			CMediaUI(LPDIRECT3DDEVICE9 pGraphicDev, 
								const wstring& wstrName, 								
								const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, 
								const _bool bIsActive = true);
	virtual				~CMediaUI(void);

public:
	virtual HRESULT 	Start_ForScene(void);
	virtual void		Update_Object(const _float& fTimeDelta);
	virtual void		Render_Object(void);

public:
	HRESULT				Ready_Object(void);

private:
	Engine::CTransform*					m_pTransCom;
	Engine::CRect_Texture*				m_pBufferCom;
	Engine::CRenderer*					m_pRendererCom;
	Engine::CShader*					m_pShaderCom;

public:
	void				Alpha_Calculation(const _float& fTimeDelta);
	_bool				EndCheck(void);

private:
	void				Loop(void);
	HRESULT				SetMedia(_tchar* _filePath, _bool _bLoop, _int _iVertexX, _int _iVertexY);	

private:
	HRESULT				Add_Component(void);

private:
	_bool				m_bPlayButton;
	_bool				m_bLoop;
	
	COpening_Banner*	m_pOpenningBanner;

	Engine::CMediaObj*	m_pMediaObj;

public:
	static CMediaUI*	Create(LPDIRECT3DDEVICE9 pGraphicDev, 
								const wstring& wstrName, 
								_tchar* _filePath,
								_bool _bLoop,
								_int _iVertexX = 0,
								_int _iVertexY = 0,
								const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, 
								const _bool bIsActive = true);

public:
	virtual void		Free(void);
};

#endif // MediaUI_h__