/*!
 * \file ObjectName.h
 * \date 2016/12/25 17:38
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

#ifndef ObjectName_h__
#define ObjectName_h__


#include "Defines.h"
#include "Effect.h"


namespace Engine
{
	class CGameObject;
}
class CObjectName
	: public Engine::CEffect
{
private:
	explicit CObjectName(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CObjectName(void);

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
	static CObjectName* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const wstring& wstrName);
	static CObjectName* Create(LPDIRECT3DDEVICE9 pGraphicDev
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

#endif // ObjectName_h__