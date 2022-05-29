#ifndef Culling_Manager_h__
#define Culling_Manager_h__

#include "Defines.h"
#include "Base.h"

namespace Engine
{
	class CCameara;
	class CGameObject;
	class CLight;
}

class CCulling_Manager
	: public CBase
{
public:
	DECLARE_SINGLETON(CCulling_Manager)
private:
	explicit CCulling_Manager(void);
	virtual ~CCulling_Manager(void);
public:
	CRITICAL_SECTION* Get_CriticalSection(void);
	_uint Get_IsLoop(void);
public:
	void	StopThread(void);
public:
	HRESULT Ready_Culling(map<wstring, list<Engine::CGameObject*>>* pmapObject, const _float& fSphereRad);
public:
	static unsigned int APIENTRY Thread_Main(void* pArg);
private:
	map<wstring, list<Engine::CGameObject*>>* m_pmapObject;		//�ش���� ���ӷ��� ���̾ ������Ʈ ����Ʈ
	map<const _uint, Engine::CLight*>* m_pmapPointLight;				//�ش���� ������ ����Ʈ
	map<const _uint, Engine::CLight*>* m_pmapSpotLight;				//�ش���� ����Ʈ����Ʈ ����Ʈ
	Engine::CCamera* m_pMainCamera;
	Engine::CGameObject* m_pPlayer;
	Engine::CTransform* m_pPlayerTransform;
private:
	HANDLE						m_hThread;
	CRITICAL_SECTION			m_Crt;
	_uint						m_iLoop;
	//PSRWLOCK	m_psrwLock;
private:
	D3DXPLANE	m_Plane[6];													//����ü�� �����ϴ� ��
	_vec3		m_vPoint[8];												//����ü ����
	_matrix		m_matProj;
	_matrix		m_matView;
	_float		m_fSphereRad;												//�÷��̾��� ��ġ���� ������� �ݰ�����? 


public:
	_uint	Start_Culling(void);											//������Ʈ��
	void	Object_Culling(void);											//������Ʈ �ø�
	void	PointLight_Culling(void);										//������ �ø�
	void	SetUp_Frustum(void);											//����ü ���� �ʱ�ȭ
	void	Make_Plane(void);												//����ü�� �����ϴ� �� �����
	_bool	IsIn_Frustum(const _vec3* pPosition, const _float& fRadius);	//����ü �ȿ� ����ִ���?
	_bool	IsIn_Sphere(const _vec3* pPosition);							//�÷��̾��� ��ġ���� �ݰ�ȿ� ����մ���?

private:
	virtual void Free(void);
};


#endif // Culling_Manager_h__
