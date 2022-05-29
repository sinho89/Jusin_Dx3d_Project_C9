#ifndef Struct_h__
#define Struct_h__

#include "Export_Engine.h"

typedef struct tagPlayerInfo
{
	CLASSID		m_eClassID;

	_tchar	m_szClassName[128];		// ����
	_tchar	m_szID[128];			// ���̵�

	_uint	m_iLevel;				// ����

	_uint	m_iStr; 				// ��
	_uint	m_iCon; 				// ü��
	_uint	m_iWis; 				// ����
	_uint	m_iInt; 				// ����

	_int	m_iHp;
	_int	m_iMp;
	_int	m_iMaxHp;
	_int	m_iMaxMp;

	_uint	m_iFury;

	_uint	m_iRightAtt;			// ������ ���ݷ�
	_uint	m_iRightMinAtt;			// ������ ���ݷ�
	_uint	m_iRightMaxAtt;			// ������ ���ݷ�
	_uint	m_iLeftAtt;				// �޼� ���ݷ�
	_uint	m_iLeftMinAtt;				// �޼� ���ݷ�
	_uint	m_iLeftMaxAtt;				// �޼� ���ݷ�
	_uint	m_iTotalAtt;			// ����	���ݷ�
	_uint	m_iTotalMinAtt;			// ����	���ݷ�
	_uint	m_iTotalMaxAtt;			// ����	���ݷ�

	_uint	m_iPhysicalDefense;		// �������
	_uint	m_iMegicalDefense;		// �������

	_uint	m_iSlotIndex;			// ĳ���� ���� �ε���

	_float	m_fExp;					// ����ġ

	_float	m_fHitAaccuracy;		// �߰����߷�
	_float	m_fHitResist;			// �������׷�
	_float	m_fConcentration;		// ���߷�
	_float	m_fGlancingArmor;		// �۷��̾Ƹ�
	_float	m_fCritical;			// ũ��Ƽ��
	_float	m_fCriticalResist;		// ũ��Ƽ������
	_float	m_fAttSpeed;			// ���ݼӵ�
	_float	m_fMoveSpeed;			// �̵��ӵ�
	_float	m_fCastSpeed;			// ĳ���üӵ�
	_float	m_fLimitDefence;		// ����Ѱ�ġ

	_float	m_fFireAtt; 			// ȭ�Ӽ� ���ݷ�
	_float	m_fSkyAtt;				// õ�Ӽ� ���ݷ�
	_float	m_fLandAtt; 			// ���Ӽ� ���ݷ�
	_float	m_fWaterAtt;			// ���Ӽ� ���ݷ�

	_float	m_fFireDef;				// ȭ�Ӽ� ����
	_float	m_fSkyDef;				// õ�Ӽ� ����
	_float	m_fLandDef;				// ���Ӽ� ����
	_float	m_fWaterDef;			// ���Ӽ� ����

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

	_uint	m_iStr; 				// ��
	_uint	m_iCon; 				// ü��
	_uint	m_iWis; 				// ����
	_uint	m_iInt; 				// ����

	_uint	m_iRightAtt;			// ������ ���ݷ�
	_uint	m_iLeftAtt;				// �޼� ���ݷ�

	_uint	m_iHp;
	_uint	m_iMp;

	_float	m_fHitAaccuracy;		// �߰����߷�
	_float	m_fHitResist;			// �������׷�
	_float	m_fConcentration;		// ���߷�
	_float	m_fGlancingArmor;		// �۷��̾Ƹ�
	_float	m_fCritical;			// ũ��Ƽ��
	_float	m_fCriticalResist;		// ũ��Ƽ������
	_float	m_fAttSpeed;			// ���ݼӵ�
	_float	m_fMoveSpeed;			// �̵��ӵ�
	_float	m_fCastSpeed;			// ĳ���üӵ�
	_float	m_fLimitDefence;		// ����Ѱ�ġ

	_float	m_fFireAtt; 			// ȭ�Ӽ� ���ݷ�
	_float	m_fSkyAtt;				// õ�Ӽ� ���ݷ�
	_float	m_fLandAtt; 			// ���Ӽ� ���ݷ�
	_float	m_fWaterAtt;			// ���Ӽ� ���ݷ�

	_float	m_fFireDef;				// ȭ�Ӽ� ����
	_float	m_fSkyDef;				// õ�Ӽ� ����
	_float	m_fLandDef;				// ���Ӽ� ����
	_float	m_fWaterDef;			// ���Ӽ� ����

	_uint	m_iSlotIndex;			// ������ ���� �ε���

	SLOT_ID		m_eSlotType;		// ���� Ÿ��
	ITEM_ID		m_eItemType;		// ������ Ÿ��

	_uint	m_iCost;				// ����
	_uint	m_iDemandLevel;			// �䱸����
	_uint	m_iDurabilty;			// ������
	_uint	m_iMaxDurabilty;		// ������ Max

	_uint	m_iPhysicalDefense;		// �������
	_uint	m_iMegicalDefense;		// �������

	_uint	m_iCount;				// ����

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
	char szBoneName[128];				//�پ�� �ϴ� �� �̸�
	_vec3  vPos;						//�ٴ� ��ġ
	_vec3  vScale;						//ũ��
	_vec3  vRotate;						//ȸ����
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
