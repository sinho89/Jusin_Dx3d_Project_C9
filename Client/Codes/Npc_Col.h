#ifndef Npc_Col_h__
#define Npc_Col_h__


#include "Defines.h"
#include "Effect.h"

//UI Sprite�� ���� ���� Ŭ����
namespace Engine
{
	class CGameObject;
}

class CNpc_Col
	: public Engine::CEffect
{
private:
	explicit CNpc_Col(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CNpc_Col(void);

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
	_float		m_fDepth;

private:
	Engine::CGameObject*	m_pParent;

public:
	void SetParent(Engine::CGameObject* pParent) { m_pParent = pParent; }
	void SetRender(_bool bRender) { m_bRender = bRender; }

public:
	static CNpc_Col* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const wstring& wstrName);
	static CNpc_Col* Create(LPDIRECT3DDEVICE9 pGraphicDev
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


#endif // Npc_Col_h__
