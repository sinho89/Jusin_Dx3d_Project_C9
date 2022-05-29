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
	map<wstring, list<Engine::CGameObject*>>* m_pmapObject;		//해당씬의 게임로직 레이어에 오브젝트 리스트
	map<const _uint, Engine::CLight*>* m_pmapPointLight;				//해당씬의 점광원 리스트
	map<const _uint, Engine::CLight*>* m_pmapSpotLight;				//해당씬의 스포트라이트 리스트
	Engine::CCamera* m_pMainCamera;
	Engine::CGameObject* m_pPlayer;
	Engine::CTransform* m_pPlayerTransform;
private:
	HANDLE						m_hThread;
	CRITICAL_SECTION			m_Crt;
	_uint						m_iLoop;
	//PSRWLOCK	m_psrwLock;
private:
	D3DXPLANE	m_Plane[6];													//절두체를 구성하는 면
	_vec3		m_vPoint[8];												//절두체 정점
	_matrix		m_matProj;
	_matrix		m_matView;
	_float		m_fSphereRad;												//플레이어의 위치에서 어느정도 반경인지? 


public:
	_uint	Start_Culling(void);											//업데이트부
	void	Object_Culling(void);											//오브젝트 컬링
	void	PointLight_Culling(void);										//점광원 컬링
	void	SetUp_Frustum(void);											//절두체 정점 초기화
	void	Make_Plane(void);												//절두체를 구성하는 면 만들기
	_bool	IsIn_Frustum(const _vec3* pPosition, const _float& fRadius);	//절두체 안에 들어있는지?
	_bool	IsIn_Sphere(const _vec3* pPosition);							//플레이어의 위치에서 반경안에 들어잇는지?

private:
	virtual void Free(void);
};


#endif // Culling_Manager_h__
