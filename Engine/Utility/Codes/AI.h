#ifndef AI_h__
#define AI_h__

#include "Component.h"


BEGIN(Engine)
class CAI_State;
class CTransform;

class ENGINE_DLL CAI
	: public Engine::CComponent
{
private:
	explicit CAI(void);
	virtual ~CAI(void);
public:
	void Set_LookAtDir(_bool bCheck);
	void Set_State(_uint iIndex);
	void Set_State(_uint iIndex, CAI_State* pState);
	void Set_AI_Stop(void);
	void Set_AI_Update(void);
	void Set_StartState(void);
	void Set_StartPos(_uint iIndex, _vec3* pPos);
	int Get_CurState(void);
	CAI_State* Get_State(const _uint& iState);
public:
	virtual void Update_Component(const _float& fTimeDelta);
private:
	map<_uint, CAI_State*> m_mapState;
	typedef map<_uint, CAI_State*> MAPSTATE;

	_uint m_iState;

private:
	_bool m_bAIStop;

private:
	HRESULT Ready_AI(void);

public:
	static CAI* Create(void);
private:
	virtual void Free(void);
};


END

#endif // Animator_h__
