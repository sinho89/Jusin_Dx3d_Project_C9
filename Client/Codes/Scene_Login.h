/*!
 * \file Scene_Login.h
 * \date 2016/11/13 16:12
 *
 * \author Shinho
 * Contact: user@company.com
 *
 * \brief Scene for Login
 *
 * TODO: long description
 *
 * \note
*/

#ifndef Scene_Login_h__
#define Scene_Login_h__


#include "Defines.h"
#include "Scene.h"

class CScene_Login
	: public Engine::CScene
{
private:
	explicit			CScene_Login(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual				~CScene_Login(void);

public:
	virtual HRESULT		Ready_Scene(void);
	virtual void		Update_Scene(const _float& fTimeDelta);
	virtual void		Render_Scene(void);

private:
	HRESULT 			SetUp_DefaultSetting(void);
	HRESULT 			Ready_Layer_GameLogic(void);
	HRESULT 			Ready_Resource(void);

private:
	void				SkipScene();

private:
	_float				m_fEffTime;
	_float				m_fStartTime;
	_bool				m_bEffStart;
	_float				m_fStar1;
	_float				m_fStar2;
	_float				m_fStar3;
	_float				m_fStar4;
	_float				m_fStar5;
	_bool				m_bStar1;
	_bool				m_bStar2;
	_bool				m_bStar3;
	_bool				m_bStar4;
	_bool				m_bStar5;
	
public:
	static CScene_Login* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void		Free(void);
};



#endif // Scene_Login_h__