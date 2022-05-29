#ifndef SnowBall_h__
#define SnowBall_h__

#include "Defines.h"
#include "Trap.h"

class CSnowBall
	: public CTrap
{
private:
	enum BALL_TYPE{LEFT, RIGHT};
private:
	explicit CSnowBall(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CSnowBall(void);
public:
	virtual HRESULT				Start_ForScene(void);
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);
	virtual	void Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);

private:
	HRESULT Ready_Object(void);

private:
	BALL_TYPE m_eType;
	_vec3	  m_vTargetPos;
	_vec3	  m_vDir;
	_float	  m_fJumpForce;
	_float	  m_fSpeed;

public:
	static CSnowBall* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const wstring& wstrName);
private:
	virtual void Free(void);

};
#endif // SnowBall_h__
