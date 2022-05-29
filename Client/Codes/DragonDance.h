#ifndef DragonDance_h__
#define DragonDance_h__

#include "Defines.h"
#include "Effect.h"

class CRing;
class CCherryBlossom;
class CPledgeRing;
class CWave;

class CDragonDance
	: public Engine::CEffect
{
private:
	explicit CDragonDance(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual  ~CDragonDance(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);


private:
	const vector<Engine::ANIINFO*>*		m_pvecAniInfo;
	Engine::CDynamicMesh*				m_pTargetMesh;
	Engine::CTransform*					m_pTargetTransform;

	CPledgeRing*						 m_pRightLongTrail;	 //������ ūTrail �ֵθ���
	CPledgeRing*						 m_pRightSmallTrail; //������ ����Trail �ֵθ���
	CPledgeRing*						 m_pLeftLongTrail;   //�޼� ūTrail �ֵθ���
	CPledgeRing*						 m_pLeftSmallTrail;  //�޼� ���� Trail �ֵθ���


	//��ų ������ ���ؼ� �ѹ��� ȣ��ǰ� �ϱ����ؼ�  ���� �����
	_bool								 m_bRightTrailCheck;
	_bool								 m_bLeftTrailCheck;

	//���̴������ؽ��Ŀ� ���İ��� �־��ִ� ���� UI ������ ó�� �Ǿ�����
	_float								 m_fRightLongGage;
	_float								 m_fRightSmallGage;

	_float								 m_fLeftLongGage;
	_float								 m_fLeftSmallGage;

	Engine::CAnimator*					m_pTargetAnimator;

private:
	HRESULT								Ready_Object(void);

public:
	static CDragonDance* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);

private:
	virtual		void		Free(void);
	HRESULT					Add_Component(void);

public:
	void					SkillStart(void);
	void					EndAniCheck(void);
	virtual		HRESULT		Start_ForScene(void);


	//Right Trail ���� �Լ�===========================================================
	void					AllRightTrailInit(void); //�ʱ� ���� �Լ�
	void					RightTrailStart(void);   //��ų�ߵ��� �ʱ�ȭ �Լ�
	void					RightTrailUpdate(const _float & fTimeDelta);
	void					RightTrailSmallUpdate(const _float& fTImeDelta);

	//================================================================================

	//Left Trail ���� �Լ�============================================================
	void					AllLeftTrailInit(void);
	void					LeftTrailStart(void);
	void					LeftTrailUpdate(const _float& fTimeDelta);
	void					LeftTrailSmallUpdate(const _float& fTImeDelta);
	//================================================================================

};



#endif //DragonDance_h__