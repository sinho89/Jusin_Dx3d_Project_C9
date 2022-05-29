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
	//Bullet�� �ھ�
	CWave*					m_pCore;
	CSprite*				m_pElectric;	      //�ھ� �ֺ� ����ũ ��������Ʈ

	//Twist
	CWave*					m_pTwist;		    //�ھ ����ٴϸ鼭 ȸ���ϴ� Ʈ����Ʈ
	CWave*					m_pTwistMiddle[2];  //�ھ ����ٴϸ� ȸ���ϴ� Middle Ʈ����Ʈ

	CWave*					m_pTwistGroup[10];  //Ʈ����Ʈ ����
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


	//�θ� ��� ���� ����
	void					SettingRotate(void);

	void					SetShootingDir(_vec3 vDir);

	void					Shooting(void);

	void					SetRotate(_vec3 vRotate);

	void					DestroyCheck(void);
};

#endif