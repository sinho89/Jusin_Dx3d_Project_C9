/*!
 * \file Create_Movie.h
 * \date 2016/11/19 14:38
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
#ifndef Create_Movie_h__
#define Create_Movie_h__

#include "UI.h"

namespace Engine
{
	class CMediaObj;
	class CTransform;
	class CRect_Texture;
	class CRenderer;
	class CShader;
}

class CCreate_Movie
	: public CUI
{
private:
	explicit				CCreate_Movie(LPDIRECT3DDEVICE9 pGraphicDev, 
									const wstring& wstrName, 								
									const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, 
									const _bool bIsActive = true);
	virtual					~CCreate_Movie(void);

public:
	virtual HRESULT 		Start_ForScene(void);
	virtual void			Update_Object(const _float& fTimeDelta);
	virtual void			Render_Object(void);

public:
	HRESULT					Ready_Object(void);

private:
	Engine::CTransform*		m_pTransCom;
	Engine::CRect_Texture*	m_pBufferCom;
	Engine::CRenderer*		m_pRendererCom;
	Engine::CShader*		m_pShaderCom;

public:
	void					Alpha_Calculation(const _float& fTimeDelta);
	void					ReStart(void);
	void					Play(void);
	_bool&					RenderCheck(void);
	_bool					EndCheck(void);

private:
	void					Loop(void);
	HRESULT					SetMedia(_tchar* _filePath, _bool _bLoop, _int _iVertexX, _int _iVertexY);	

private:
	HRESULT					Add_Component(void);

private:
	_bool					m_bRender;
	_bool					m_bLoop;

	Engine::CMediaObj*		m_pMediaObj;

public:
	static CCreate_Movie*	Create(LPDIRECT3DDEVICE9 pGraphicDev, 
									const wstring& wstrName, 
									_tchar* _filePath,
									_bool _bLoop,
									_int _iVertexX = 0,
									_int _iVertexY = 0,
									const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, 
									const _bool bIsActive = true);

public:
	virtual void			Free(void);
};

#endif // Create_Movie_h__