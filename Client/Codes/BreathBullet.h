#ifndef HanumanBreathBullet_h__
#define HanumanBreathBullet_h__


#include "Defines.h"
#include "Effect.h"

namespace Engine
{
	class CCollider;
}

class CSprite;
class CWave;

class CBreathBullet
	: public Engine::CEffect
{
private:
	explicit CBreathBullet(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CBreathBullet(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);


private:
	HRESULT		 Ready_Object(void);

public:
	static CBreathBullet* Create(LPDIRECT3DDEVICE9 pGraphicDev
								, const wstring& wstrName);

private:
	//Bullet의 코어
	CWave*					m_pCore;
	CSprite*				m_pElectric;	      //코어 주변 스파크 스프라이트

	//Twist
	CWave*					m_pTwist;		    //코어를 따라다니면서 회전하는 트위스트
	CWave*					m_pTwistMiddle[2];  //코어를 따라다니며 회전하는 Middle 트위스트

	CWave*					m_pTwistGroup[10];  //트위스트 모음
	_vec3					m_vShootingDir;

	_double					m_dTimer;
	_bool					m_bDestroyCheck;

	Engine::CCollider*		m_pColliderCom;

private:
	virtual					void  Free(void);
	HRESULT					Add_Component(void);


public:
	void					SkillStart(void);
	void					EndAniCheck(void);

	void					TwistUpdate(void);


	//부모 행렬 곱셈 연산
	void					SettingRotate(void);

	void					SetShootingDir(_vec3 vDir);

	void					Shooting(void);

	void					SetRotate(_vec3 vRotate);

	void					DestroyCheck(void);
};

#endif