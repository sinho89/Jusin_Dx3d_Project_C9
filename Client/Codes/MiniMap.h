/*!
 * \file MiniMap.h
 * \date 2016/11/21 15:18
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
#ifndef MiniMap_h__
#define MiniMap_h__

#include "UI.h"

namespace Engine
{
	class CTransform;
	class CRect_Texture;
	class CTexture;
	class CRenderer;
	class CShader;
	class CCamera;
	class CLayer;
}

//struct McRndSf
//{
//	LPDIRECT3DTEXTURE9	pTx;
//	LPDIRECT3DSURFACE9	pSf;
//
//	McRndSf(void)
//	{
//		pTx = NULL;
//		pSf = NULL;
//	}
//
//	void Invalidate(void)
//	{
//		::Safe_Release(pTx);
//		::Safe_Release(pSf);
//	}
//};

class CMiniPlayer;
class CMiniMap
	: public CUI
{
private:
	explicit				CMiniMap(LPDIRECT3DDEVICE9 pGraphicDev, 
									const wstring& wstrName,
									const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT,
									const _bool bIsActive = TRUE);
	virtual					~CMiniMap(void);

public:
	virtual HRESULT 		Start_ForScene(void);
	virtual void			Update_Object(const _float& fTimeDelta);
	virtual void			Render_Object(void);
	virtual	void			Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);

public:
	HRESULT					Ready_Object(void);

private:
	Engine::CTransform*		m_pTransCom;
	Engine::CRect_Texture*	m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;
	Engine::CShader*		m_pShaderCom;
	Engine::CRenderer*		m_pRendererCom;

	Engine::CTexture*		m_pMinimapTexCom;
	Engine::CTexture*		m_pMinimapPlayerTexCom;

	Engine::CTexture*		m_pMinimapNPCTexCom;
	Engine::CTexture*		m_pMinimapMonsterTexCom;
	Engine::CTexture*		m_pMinimapBossTexCom;

	Engine::CTransform*		m_pPlayerTrans;

	Engine::CLayer*				m_pLogicLayer;
	list<Engine::CGameObject*>	m_listMiniUnit;

	Engine::CGameObject*		m_pStageBoss;

private:
	Engine::CCamera*		m_pMapCamera;	
	CMiniPlayer*			m_pMiniPlayer;

	_vec3					m_vOriginalCenterPos;
	_float					m_fMiniMapMove_X;
	_float					m_fMiniMapMove_Y;

private:
	//LPDIRECT3DSURFACE9		m_pCurBck;	//백 버퍼
	//LPDIRECT3DSURFACE9		m_pCurDpt;	//서피스(깊이버퍼)

	//McRndSf					m_RndSf;	//그려줄 곳.

	_int					m_iTxW;		//텍스쳐 너비

private:
	HRESULT					Add_Component(void);

private:
	void					FindNPC(void);
	void					DrawTownUnit(LPD3DXEFFECT pEffect);

	void					StageIceFindMonster(void);
	void					DrawIceUnit(LPD3DXEFFECT pEffect);

	void					StageFireFindMonster(void);
	void					DrawFireUnit(LPD3DXEFFECT pEffect);

public:
	static CMiniMap*		Create(LPDIRECT3DDEVICE9 pGraphicDev, 
									const wstring& wstrName, 
									const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, 
									const _bool bIsActive = TRUE);

public:
	virtual void			Free(void);
};

#endif // MiniMap_h__