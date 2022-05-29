/*!
 * \file Skill_Avi.h
 * \date 2016/12/11 12:30
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


#ifndef Skill_Avi_h__
#define Skill_Avi_h__


#include "UI.h"

namespace Engine
{
	class CMediaObj;
	class CTransform;
	class CRect_Texture;
	class CRenderer;
	class CShader;
}

class CSkill_Avi
	: public CUI
{
private:
	explicit				CSkill_Avi(LPDIRECT3DDEVICE9 pGraphicDev,
		const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT,
		const _bool bIsActive = true);
	virtual					~CSkill_Avi(void);

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
	void					ReStart(void);
	void					ReSet(void);
	void					Play(void);
	_bool&					RenderCheck(void);
	_bool					EndCheck(void);

private:
	void					Loop(void);
	HRESULT					SetMedia(_bool _bLoop, _int _iVertexX, _int _iVertexY);
public:
	void					SetType(SKILL_ID eType);

private:
	HRESULT					Add_Component(void);

private:
	_bool					m_bLoop;
	SKILL_ID				m_eType;

	Engine::CMediaObj*		m_pMediaObj[SKILL_END];

public:
	static CSkill_Avi*	Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const wstring& wstrName,
		_bool _bLoop,
		_int _iVertexX = 0,
		_int _iVertexY = 0,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT,
		const _bool bIsActive = true);

public:
	virtual void			Free(void);
};


#endif // Skill_Avi_h__