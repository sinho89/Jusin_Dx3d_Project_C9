/*!
 * \file Sprite_UI.h
 * \date 2016/12/20 10:02
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

#ifndef CoolTime_h__
#define CoolTime_h__

#include "Defines.h"
#include "Effect.h"

//UI Sprite를 위한 예시 클래스
class CUI_Skill;
class CCoolTime
	: public Engine::CEffect
{
private:
	explicit CCoolTime(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CCoolTime(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	HRESULT		 Ready_Object(void);

private:
	_matrix		m_matView, m_matProj;
	_float		m_fX, m_fY;
	_float		m_fSizeX, m_fSizeY;
	_float		m_fTermX, m_fTermY;
	_float		m_fAngle;

	//===============스프라이트 관련 변수==========================//
private:
	_int		m_iMaxFrame;  //스프라이트의 총개수
	_int		m_iRow; //스프라이트 가로 개수
	_int		m_iColumn; //세로 개수
	_float		m_fSpriteSpeed; //스프라이트 속도


private:	// 알파관련
	_float		m_fAlpha;
	_float		m_fAlphaTime;
	_bool		m_bAlpha_Increase;
	_bool		m_bRender;

private:
	CUI_Skill*	m_pParent;

public:
	void SetParent(CUI_Skill* pParent) { m_pParent = pParent; }
	void SetRender(_bool bRender) { m_bRender = bRender; }

public:
	static CCoolTime* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const wstring& wstrName);
	static CCoolTime* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const wstring& wstrName, CUI_Skill* pParent);

private:
	virtual void	  Free(void);

	HRESULT			  Add_Component(void);
	HRESULT			  SetUp_ConstantTable(LPD3DXEFFECT pEffect);


public:
	void			  Update_RePart(void);
	void			  SpriteUpdate(const _float& fTimeDelta);
	void			  SetPart(void);


};

#endif // Sprite_UI_h__