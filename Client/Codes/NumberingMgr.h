/*!
 * \file NumberingMgr.h
 * \date 2016/12/27 10:00
 *
 * \author ShinHo
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef NumberingMgr_h__
#define NumberingMgr_h__

#include "Defines.h"
#include "Base.h"

namespace Engine
{
}
class CUI;
class CNumbering_Manager
	: public CBase
{
public:
	DECLARE_SINGLETON(CNumbering_Manager)
public:
	enum RESULT_ID { RESULT_KILL, RESULT_SKILL, RESULT_STYLE, RESULT_TECHNIC, 
		RESULT_RANK, RESULT_NANIP, RESULT_PC, RESULT_MENTO, RESULT_BONUS, 
		RESULT_TOTAL, RESULT_GOLD, RESULT_END };
private:
	explicit CNumbering_Manager(void);
	virtual ~CNumbering_Manager(void);

private:
	_uint	m_iComboCnt;
	_uint	m_iBeforeComboCnt;
	
	_short	m_sUnitDigit;
	_short	m_sDoubleDigit;
	_short	m_sTrippleDigit;

	_float	m_fComboResetTime;
	_float	m_fProtecteForTickTime;

	_bool	m_bHit;
	_bool	m_bUpdate;
	// ÄÞº¸
	
	_float	m_fSkill;
	_float	m_fKill;
	_float	m_fMaxKill;
	_float	m_fStyle;
	_float	m_fTechnic;
	_float	m_fMaxCombo;

	_float	m_fRank;
	_float	m_fTotal;
	_float	m_fGold;

private:
	vector<CUI*>	m_vecDigit;
	typedef vector<CUI*>	VECDIGIT;

	CUI*			m_pCombo_Base;
	CUI*			m_pCombo_Effect;
	//// ÄÞº¸

	vector<CUI*>	m_vecResult[RESULT_END];
public:
	HRESULT Ready_Numbering(CUI* pCombo_Base, CUI* pCombo_Effect);
	void	AddVecDigit(CUI* pDigit);
	void	Increase_ComboCount(void);
	void	Update_Combo(const _float& fTimeDelta);
	void	ResetCombo(const _float& fTimeDelta);
	// ÄÞº¸
	void	SetKillCount();
	void	SetSkillCount();

	void	Render_ResultNum();

	// Result Getter
	_vec4	GetResult() { return _vec4(m_fKill, m_fSkill, m_fStyle, m_fTechnic); }
	
private:
	virtual void Free(void);
};


#endif // NumberingMgr_h__