#ifndef CoolTimeMgr_h__
#define CoolTimeMgr_h__


#include "Defines.h"
#include "Base.h"

namespace Engine
{
}
class CUI_Skill;
class CCoolTime_Manager
	: public CBase
{
public:
	DECLARE_SINGLETON(CCoolTime_Manager)
public:
	enum COOLTYPE{ COOL_DASH, COOL_RIDDLE, COOL_BLARE, COOL_UPATT, COOL_TRICK, COOL_PLANIT, 
		COOL_VIPER, COOL_HILLTURN, COOL_SILUET, COOL_SPINING, COOL_SOFTRANDING, COOL_END};
private:
	explicit CCoolTime_Manager(void);
	virtual ~CCoolTime_Manager(void);

private:
	map<COOLTYPE, CUI_Skill*>			m_SkillMap;
	typedef map<COOLTYPE, CUI_Skill*>	SKILLMAP;

public:
	void AddCoolTime(COOLTYPE eType, CUI_Skill* pSkill);
	void StartCoolTime(COOLTYPE eType);
	CUI_Skill* FindCoolTime(COOLTYPE eType);
	bool CheckCoolTime(COOLTYPE eType);
	void Ready_CoolTime(void);

public:
	void UpdateCoolTime(void);
private:
	virtual void Free(void);
};


#endif // CoolTimeMgr_h__
