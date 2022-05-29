#ifndef Struct_h__
#define Struct_h__

#include "Export_Engine.h"

typedef struct tagPlayerInfo
{
	CLASSID		m_eClassID;

	_tchar	m_szClassName[128];		// 직업
	_tchar	m_szID[128];			// 아이디

	_uint	m_iLevel;				// 레벨

	_uint	m_iStr; 				// 힘
	_uint	m_iCon; 				// 체력
	_uint	m_iWis; 				// 지혜
	_uint	m_iInt; 				// 지능

	_int	m_iHp;
	_int	m_iMp;
	_int	m_iMaxHp;
	_int	m_iMaxMp;

	_uint	m_iFury;

	_uint	m_iRightAtt;			// 오른손 공격력
	_uint	m_iRightMinAtt;			// 오른손 공격력
	_uint	m_iRightMaxAtt;			// 오른손 공격력
	_uint	m_iLeftAtt;				// 왼손 공격력
	_uint	m_iLeftMinAtt;				// 왼손 공격력
	_uint	m_iLeftMaxAtt;				// 왼손 공격력
	_uint	m_iTotalAtt;			// 총합	공격력
	_uint	m_iTotalMinAtt;			// 총합	공격력
	_uint	m_iTotalMaxAtt;			// 총합	공격력

	_uint	m_iPhysicalDefense;		// 물리방어
	_uint	m_iMegicalDefense;		// 마법방어

	_uint	m_iSlotIndex;			// 캐릭터 슬롯 인덱스

	_float	m_fExp;					// 경험치

	_float	m_fHitAaccuracy;		// 추가명중률
	_float	m_fHitResist;			// 명중저항률
	_float	m_fConcentration;		// 집중률
	_float	m_fGlancingArmor;		// 글랜싱아머
	_float	m_fCritical;			// 크리티컬
	_float	m_fCriticalResist;		// 크리티컬저항
	_float	m_fAttSpeed;			// 공격속도
	_float	m_fMoveSpeed;			// 이동속도
	_float	m_fCastSpeed;			// 캐스팅속도
	_float	m_fLimitDefence;		// 방어한계치

	_float	m_fFireAtt; 			// 화속성 공격력
	_float	m_fSkyAtt;				// 천속성 공격력
	_float	m_fLandAtt; 			// 지속성 공격력
	_float	m_fWaterAtt;			// 수속성 공격력

	_float	m_fFireDef;				// 화속성 방어력
	_float	m_fSkyDef;				// 천속성 방어력
	_float	m_fLandDef;				// 지속성 방어력
	_float	m_fWaterDef;			// 수속성 방어력

	_uint	m_iMoney;				
	_uint	m_iTiredCount;

	_bool	m_bHit;
	_float	m_fHitAngle;
	_vec3	m_vLook;
	_vec3	m_vRight;
	_float	m_fHitDam;

}PLAYER_INFO;

typedef struct tagItemInfo
{
	_tchar	m_szName[128];

	_uint	m_iStr; 				// 힘
	_uint	m_iCon; 				// 체력
	_uint	m_iWis; 				// 지혜
	_uint	m_iInt; 				// 지능

	_uint	m_iRightAtt;			// 오른손 공격력
	_uint	m_iLeftAtt;				// 왼손 공격력

	_uint	m_iHp;
	_uint	m_iMp;

	_float	m_fHitAaccuracy;		// 추가명중률
	_float	m_fHitResist;			// 명중저항률
	_float	m_fConcentration;		// 집중률
	_float	m_fGlancingArmor;		// 글랜싱아머
	_float	m_fCritical;			// 크리티컬
	_float	m_fCriticalResist;		// 크리티컬저항
	_float	m_fAttSpeed;			// 공격속도
	_float	m_fMoveSpeed;			// 이동속도
	_float	m_fCastSpeed;			// 캐스팅속도
	_float	m_fLimitDefence;		// 방어한계치

	_float	m_fFireAtt; 			// 화속성 공격력
	_float	m_fSkyAtt;				// 천속성 공격력
	_float	m_fLandAtt; 			// 지속성 공격력
	_float	m_fWaterAtt;			// 수속성 공격력

	_float	m_fFireDef;				// 화속성 방어력
	_float	m_fSkyDef;				// 천속성 방어력
	_float	m_fLandDef;				// 지속성 방어력
	_float	m_fWaterDef;			// 수속성 방어력

	_uint	m_iSlotIndex;			// 아이템 슬롯 인덱스

	SLOT_ID		m_eSlotType;		// 슬롯 타입
	ITEM_ID		m_eItemType;		// 아이템 타입

	_uint	m_iCost;				// 가격
	_uint	m_iDemandLevel;			// 요구레벨
	_uint	m_iDurabilty;			// 내구도
	_uint	m_iMaxDurabilty;		// 내구도 Max

	_uint	m_iPhysicalDefense;		// 물리방어
	_uint	m_iMegicalDefense;		// 마법방어

	_uint	m_iCount;				// 수량

}ITEM_INFO;

typedef struct tagSkillInfo
{
	SKILL_ID	m_eSkillId;
	SKILL_TYPE	m_eSkillType;

	_uint		m_iLevel;
	_uint		m_iSP;
	_uint		m_iResultData;
	_uint		m_iCoolTime;

	_uint		m_iSlotNum;
	_bool		m_bActive;
	_float		m_fActiveTime;
}SKILL_INFO;

typedef struct tagObjectInfo
{
	_tchar				m_szName[MAX_PATH];
	_tchar				m_szTag[MAX_PATH];
	_vec3				m_vAngle;
	_vec3				m_vScale;
	_vec3				m_vPos;
	Engine::LAYERTYPE	m_eLayerType;
}OBJ_INFO;

typedef struct tagWeaponInfo
{
	char szBoneName[128];				//붙어야 하는 본 이름
	_vec3  vPos;						//붙는 위치
	_vec3  vScale;						//크기
	_vec3  vRotate;						//회전값
}WEAPONINFO;

typedef struct tagMonsterInfo
{
	_int   iMaxHp;
	_int   iHp;
	_int   iAtt;

	MONSTER_ID m_eMonID;
}MON_INFO;

typedef struct tagShaderOptionStateInfo
{
	_bool	bDebugBuffer;
	_bool	bShadowBuffer;
	_bool	bSoftShadow;
	_bool	bCACDShadow;
	_bool	bMotionBlurBuffer;
	_bool	bGaussianBlur;
	_bool	bHDR;
	_bool	bDOF;
	_bool	bSSAO;
	_bool	bRandNormal;
	_bool	bExpFog;
	_bool	bSquareFog;
	_bool	bHeightFog;
	_bool	bLimLight;
	_float  fBlurValue;
	_float	fBrightness;
}SHADEROPTION_INFO;
extern SHADEROPTION_INFO g_ShaderOption_Info;

#endif // Struct_h__
