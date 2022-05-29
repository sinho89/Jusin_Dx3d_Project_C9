#ifndef TrickyRabbit_h__
#define TrickyRabbit_h__

#include "Defines.h"
#include "Effect.h"

namespace Engine
{
	class CAnimator;
	class CDynamicMesh;
	class CAnimator;
}

class CWave;
class CTrickyRabbitTrail;

class CTrickyRabbit
	: public Engine::CEffect
{
private:
	explicit CTrickyRabbit(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CTrickyRabbit(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	const vector<Engine::ANIINFO*>*		 m_pvecAniInfo;
	Engine::CDynamicMesh*				 m_pTargetMesh;
	Engine::CTransform*					 m_pTargetTransform;
	Engine::CAnimator*					 m_pTargetAnimator;
	Engine::CTransform*					 pTargetSwordTransform;
	CTrickyRabbitTrail*					 m_pTrailLeft;
	CTrickyRabbitTrail*					 m_pTrailRight;

	_bool								 m_bTrailLeft;
	_bool								 m_bTrailRight;
	_bool								 m_bKickCheck;
	CWave*								 m_pKickImpact;

private:
	HRESULT								Ready_Object(void);

public:
	static CTrickyRabbit* Create(LPDIRECT3DDEVICE9 pGraphicDev
								, const wstring& wstrName);

private:
	//�˳����� ����ϴ� �˱� �ؽ���
	CWave*					m_pTrailWave;
	_bool					m_bTrailWaveCheck;

	//�ʱ⵿�ۿ� ������ ����
	CWave*					m_pGroundWave;
	_bool					m_bGroundWaveCheck;




private:
	virtual					void  Free(void);
	HRESULT					Add_Component(void);

public:
	void					SkillStart(void);
	void					EndAniCheck(void);
	virtual		HRESULT		Start_ForScene(void);

public:
	//TrailWave Update ���� �Լ�
	void					TrailWaveUpdate(void);
	//GroundWave Update ���� �Լ�
	void					GroundWaveUpdate(void);

	//GroundWave �ʱ� Setting �Լ�
	void					GroundWaveStart(void);


	//KickImpact Start
	void					KickImpactStart(void);

	//KickImpact Update
	void					KickImpactUpdate(void);
};


#endif