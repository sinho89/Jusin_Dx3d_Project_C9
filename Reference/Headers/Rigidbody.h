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
	_vec3 m_vVelocity;					//������ ����Ǵ� �ӵ�
	_vec3 m_vAccMotionVelocity;			//��ӵ� ��� ���� ����
	_float m_fMass;						//����
	_float m_fGravityAcc;				//�߷� ���ӵ�
	_float m_fGravityDist;				//1�����Ӵ� �̵��� �Ÿ�(�߷�)
	_float m_fMaxGravityDist;			//�߷� ���� �Ÿ�(���ѿ뵵)
	_float m_fRevisionPower;			//�߷� ���� ����
	_bool m_bGravity;					//�߷��� �ް� �ִ��� �ƴ���?
private:
	HRESULT Ready_Rigidbody(CTransform* pTransform, CCalculator* pCalculator);
	void SetUp_Gravity(const _float& fTimeDelta);
	void SetUp_AccMotionLine(const _float& fTimeDelta);	//��ӵ� �
public:
	static CRigidbody* Create(CTransform* pTransform, CCalculator* pCalculator);
private:
	virtual void Free(void);
};


END

#endif // Rigidbody_h__
