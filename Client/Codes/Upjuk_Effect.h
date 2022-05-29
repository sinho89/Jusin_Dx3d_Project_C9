#ifndef Upjuk_Effect_h__
#define Upjuk_Effect_h__


#include "Defines.h"
#include "Effect.h"

//UI Sprite�� ���� ���� Ŭ����
class CUI;
class CUpjuk_Effect
	: public Engine::CEffect
{
private:
	explicit CUpjuk_Effect(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CUpjuk_Effect(void);

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

	//===============��������Ʈ ���� ����==========================//
private:
	_int		m_iMaxFrame;  //��������Ʈ�� �Ѱ���
	_int		m_iRow; //��������Ʈ ���� ����
	_int		m_iColumn; //���� ����
	_float		m_fSpriteSpeed; //��������Ʈ �ӵ�


private:	// ���İ���
	_float		m_fAlpha;
	_float		m_fAlphaTime;
	_bool		m_bAlpha_Increase;
	_bool		m_bRender;

private:
	CUI*	m_pParent;

public:
	void SetParent(CUI* pParent) { m_pParent = pParent; }
	void SetRender(_bool bRender) { m_bRender = bRender; }

public:
	static CUpjuk_Effect* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const wstring& wstrName);
	static CUpjuk_Effect* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const wstring& wstrName, CUI* pParent);

private:
	virtual void	  Free(void);

	HRESULT			  Add_Component(void);
	HRESULT			  SetUp_ConstantTable(LPD3DXEFFECT pEffect);


public:
	void			  Update_RePart(void);
	void			  SpriteUpdate(const _float& fTimeDelta);
	void			  SetPart(void);


};

#endif // Upjuk_Effect_h__
