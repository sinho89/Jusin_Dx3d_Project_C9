/*!
 * \file UICalculater.h
 * \date 2016/11/14 18:31
 *
 * \author Shinho
 * Contact: user@company.com
 *
 * \brief Calculater for UI
 *
 * TODO: long description
 *
 * \note
*/

#ifndef UICalculater_h__
#define UICalculater_h__


#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CUICalculater
	: public CComponent
{
private:
	explicit CUICalculater(void);
	virtual ~CUICalculater(void);
public:
	HRESULT Ready_Transform(void);

public:
	_vec2	CunvertFontPos(const _vec3& vPos, _uint WINSIZEX, _uint WINSIZEY, _float fMoveX, _float fMoveY);
	bool	Collision_ByMouse(const _uint& WINSIZEX, const _uint& WINSIZEY
		, _float m_fX, _float m_fY, _float m_fSizeX, _float m_fSizeY, const POINT& ptMouse);
	bool	Move_Window_UI(const _uint& WINSIZEX, const _uint& WINSIZEY
		, _float m_fX, _float m_fY, _float m_fSizeX, _float m_fSizeY, const POINT& ptMouse);

	_vec3	WorldToScreenPos(const _matrix* pmatWorld, const _matrix* pmatView, const _matrix* pmatProj, _uint iWinCX, _uint iWINCY);

private:
	_vec2	m_vFontPos;
public:
	static CUICalculater* Create(void);
private:
	virtual void Free(void);
};

END
#endif // UICalculater_h__