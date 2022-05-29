#ifndef WITCH_FIGHT__
#define WITCH_FIGHT__

#include "Witch_State.h"

namespace Engine
{
	class CDynamicMesh;
	class CTransform;
	class CLight;
}

class CWitchBladeWeapon;

class CUI_Skill;
class CUI;
class CWitch_Fight
	: public CWitch_State
{
public:
	enum COLCNT { COL1, COL2, COL3, COL4, COL5, COL6, COL_END };
private:
	explicit CWitch_Fight(_uint iAniIndex);
	virtual ~CWitch_Fight(void);
public:
	void SetActive(void);
	void Set_Skill(const CUI_Skill* pSkill);
	void Set_Move(const _bool& bMove);
	void Set_ActionEnd(void);
	void Set_PointLight(Engine::CLight* pLight);

public:
	void Get_Move(_bool* bMove);
public:
	virtual void Start_State(void);
	virtual void Update_State(const _float& fTimeDelta);
	virtual void End_State(void);
private:
	Engine::COLINFO m_tColInfo;
	_vec3	m_vLook;
	_bool	m_bCanMove;
	_bool	m_bCanJump;
	_float  m_fDarkGrave;
	_float  m_fDarkDelay;
	_float	m_fSpiralDeley;
	_float  m_fSpinSpeed;
	_bool	m_bAddCol[COL_END];
private:
	_bool	m_bCameraAction;
	_bool	m_bCameraIntro;
	_float	m_fFadeTime;

private:
	_int	m_iSoundCnt;

private:
	Engine::CLight* m_pPointLight;
private:
	HRESULT Ready_State(Engine::CDynamicMesh* pMesh, map<_uint, list<Engine::ANIMATORINFO*>>* pmapNextMotion,
		CWitchBlade* pPlayer, CWitchBladeWeapon* pLeftSword, CWitchBladeWeapon* pRightSword);
private:
	void ActionCheck(void);
	void MoveCheck(const _float& fTimeDelta);
	void JumpCheck(const _float& fTimeDelta);
	void AttackCheck(const _float& fTimeDelta);
	void CommandSkillCheck(const _float& fTimeDelta);
	void ActionSkillCheck(const _float& fTimeDelta);
	void DemageCheck(const _float& fTimeDelta);

	void CheckCameraAction(const _float& fTimeDelta);
private:
	_int GetDamage(_bool* pCritical);
public:
	static CWitch_Fight* Create(Engine::CDynamicMesh* pMesh, map<_uint, list<Engine::ANIMATORINFO*>>* pmapNextMotion,
		CWitchBlade* pPlayer, _uint iAniIndex, CWitchBladeWeapon* pLeftSword, CWitchBladeWeapon* pRightSword);
private:
	virtual void Free(void);
};


#endif // !WITCH_FIGHT__
