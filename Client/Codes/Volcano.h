#ifndef Volcano_h__
#define Volcano_h__

#include "Defines.h"
#include "Effect.h"

namespace Engine
{
	class CAnimator;
	class CDynamicMesh;
}

class CBardiel;
class CSprite;
class CWave;
class CPhase2_Trail;
class CVolcano
	: public Engine::CEffect
{
private:
	explicit CVolcano(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CVolcano(void);


public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	const vector<Engine::ANIINFO*>*		m_pvecAniInfo;
	Engine::CDynamicMesh*				m_pTargetMesh;
	Engine::CTransform*					m_pTargetTransform;
	Engine::CAnimator*					m_pTargetAnimator;

	CBardiel*							m_pBardiel;

	//Trail
	CPhase2_Trail*						m_pHammerTrail;

	//0 ��ġ�� �ֵθ� �� ����� ����
	CSprite*							m_pDust_0[5];	//��ġ�� ����.
	_double								m_dDust_0Timer;

	//��ġ�� ���� ����� �� ����
	CSprite*							m_pDust_1;		//��ġ��

	//������
	CSprite*							m_pDust_2;		//������


	_bool								m_bFirstSwing_Start;
	_bool								m_bFirstSwing_End;
	_bool								m_bSecondSwing_Start;
	_bool								m_bSecondSwing_End;


	_bool								m_bDust_1;
	_bool								m_bDust_2;

	//�����
	CWave*								m_pHammerWave[3];

	CSprite*							m_pBombFrameSprite[2];


	_bool								m_bHammerWave;
	const _matrix*						m_pWorldMat;		//����
	const _matrix*						m_pHammerEnd;		//�ظ� ��.
	const _matrix*						m_pRightLeg;		//������.

private:
	HRESULT								Ready_Object(void);

public:
	static CVolcano* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const wstring& wstrName);

private:
	virtual					void  Free(void);
	HRESULT					Add_Component(void);

public:
	void					SkillStart(void);
	void					EndAniCheck(void);
	virtual		HRESULT		Start_ForScene(void);

private:
	void					Dust_0_Start(_int iIndex);
	void					Dust_1_Start(void);
	void					Dust_2_Start(void);

	void					HammerWaveStart(void);
	void					HammerWaveUpdate(void);

	void					Dust_0_Update(void);
};

#endif