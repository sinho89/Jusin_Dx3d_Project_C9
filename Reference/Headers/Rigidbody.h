#ifndef Rigidbody_h__
#define Rigidbody_h__

#include "Component.h"

BEGIN(Engine)

class CTransform;
class CCalculator; 

class ENGINE_DLL CRigidbody
	: public Engine::CComponent
{
private:
	explicit CRigidbody(void);
	virtual ~CRigidbody(void);
public://Getter
	_bool Get_Gravity(void);
	_float Get_AccDist(void);
public://Setter
	void Set_Gravity(_bool bGravity);
	void Set_RevisionGravity(const _float& fRevisionPower);
	void Set_Jump(const _float& fForce);
	void Set_AccMotion(const _vec3* pDir, const float& fPower);
public:
	virtual void Update_Component(const _float& fTimeDelta);
private:
	CTransform* m_pTransform;
	CCalculator* m_pCalculator;
private:
	_vec3 m_vVelocity;					//실제로 연산되는 속도
	_vec3 m_vAccMotionVelocity;			//등속도 운동을 위한 변수
	_float m_fMass;						//질량
	_float m_fGravityAcc;				//중력 가속도
	_float m_fGravityDist;				//1프레임당 이동할 거리(중력)
	_float m_fMaxGravityDist;			//중력 누적 거리(제한용도)
	_float m_fRevisionPower;			//중력 보정 변수
	_bool m_bGravity;					//중력을 받고 있는지 아닌지?
private:
	HRESULT Ready_Rigidbody(CTransform* pTransform, CCalculator* pCalculator);
	void SetUp_Gravity(const _float& fTimeDelta);
	void SetUp_AccMotionLine(const _float& fTimeDelta);	//등속도 운동
public:
	static CRigidbody* Create(CTransform* pTransform, CCalculator* pCalculator);
private:
	virtual void Free(void);
};


END

#endif // Rigidbody_h__
