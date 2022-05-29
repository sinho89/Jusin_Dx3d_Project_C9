/*!
 * \file Babegazi_Name.h
 * \date 2016/12/26 10:17
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

#ifndef Babegazi_Name_h__
#define Babegazi_Name_h__



#include "Defines.h"
#include "Effect.h"


namespace Engine
{
	class CGameObject;
}
class CBabegazi_Name
	: public Engine::CEffect
{
private:
	explicit CBabegazi_Name(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CBabegazi_Name(void);

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

private:
	_bool		m_bBillBoard;
private:	// ���İ���
	_float		m_fAlpha;
	_float		m_fAlphaTime;
	_bool		m_bAlpha_Increase;

private:
	Engine::CGameObject* m_pParentObject;

public:
	void SetParent(Engine::CGameObject* pParent) { m_pParentObject = pParent; }
public:
	static CBabegazi_Name* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const wstring& wstrName);
	static CBabegazi_Name* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const wstring& wstrName, Engine::CGameObject* pParentObject);

private:
	virtual void	  Free(void);

	HRESULT			  Add_Component(void);
	HRESULT			  SetUp_ConstantTable(LPD3DXEFFECT pEffect);


public:
	void			  Update_RePart(void);
	void			  SpriteUpdate(const _float& fTimeDelta);
	void			  SetPart(void);


};


#endif // Babegazi_Name_h__