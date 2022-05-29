#ifndef DarkPollingGrave_h__
#define DarkPollingGrave_h__

#include "Defines.h"
#include "Effect.h"

namespace Engine
{
	class CAnimator;
	class CDynamicMesh;
}


class CDarkSword;
class CWave;

class CDarkPollingGrave
	: public Engine::CEffect
{
private:
	explicit CDarkPollingGrave(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CDarkPollingGrave(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	//===========================================================================
	//Player ���� ���� �Լ�
	const vector<Engine::ANIINFO*>*		 m_pvecAniInfo;
	Engine::CDynamicMesh*				 m_pTargetMesh;
	Engine::CTransform*					 m_pTargetTransform;
	const _bool*						 m_pPlayerAction; //Player�� ������ �޾ƿ��� ����
	//===========================================================================

	//===========================================================================
	//��ų �ʱ⿡ ����� ����Ʈ��================================================
	CWave*								m_pWave;			//�ʱ⿡ Player ������ ���������� ������ ���̺�
	CWave*								m_pWhiteLine[2];	//�ʱ⿡ ��ų ���� ���������� ��� ���̺�
	CWave*								m_pBlackLine;		//Player �ݰ����� ����� ���� ���� ����
	CWave*								m_pBlueFlowerWave;	//�ʱ⿡ Player�� �߽����� ���ܳ��� �Ķ��� �� ����
	//������======================================================================
	CWave*								m_pRuneOutLine;
	CWave*								m_pMainRune;
	CWave*								m_pRedCircle;
	//============================================================================


	//�� �޽�
	//===========================================================================
	CDarkSword*							m_pDarkSword[20];			
	CDarkSword*							m_pDarkOutLineSword[20];	
	CDarkSword*							m_pDarkMiddleLineSword[20];	
	CDarkSword*							m_pCenterSword[20];
	//===========================================================================

	//===========================================================================
	_bool								 m_bSkillCheck; //�ʱ⿡ ��ų�ߵ��� �����Ǵ� ����Ʈ�� ���� ����
	_bool								 m_bBlueFlowerCheck; //BlueFlower ���̺� ���� ����
	//============================================================================

	//=============================================================================
	_bool								 m_bDarkSwordCheck;
	_bool								 m_bSwordTargetCheck;
	_matrix								 PlayerMatrix;
	_vec3								 m_pTargetLook;
	_vec3								 m_vTargetPosition;
	//===========================================================================

	//Center Sword ���� ����=====================================================
	_bool								 m_bCenterSwordCheck; //������Ʈ�� �����Ǵ� ���� �Ǵ� ����
	_bool								 m_bCenterShoot; //CenterSword�� ������ ������ �Ǵ��ϴ� ����

	//Player�� ���� ����� �����ϴ� matrix ����
	_matrix								 CenterMatrix;
	//���� ���� ������� ���� ��� matrix����
	_matrix								 CenterResultMatrix;
	//Inside�� ��������� �����ϴ� matrix ����
	_matrix								 InsideResultMatrix;
	//Middle�� ��������� �����ϴ� matrix ����
	_matrix								 MiddleResultMatrix;
	//Out�� ��������� �����ϴ� matrix����
	_matrix								 OutResultMatrix;


	_bool								 m_bDarkPollingSwordEnd; //�� ���� ����
	//============================================================================

	//�� �޽��� ���û��� ��ǥ ���� ����
	//������� ���� ��ǥ ����
	_vec3								m_vCenterSwordPosition[20];
	_vec3								m_vInsideSwordPosition[20];
	_vec3								m_vMiddleSwordPosition[20];
	_vec3								m_vOutSwordPosition[20];
	_bool								m_bSwordPositionCheck;



	//�� �޽� ���� ������ �ٿ��� ���ڰ� ����
	//���� �ٱ��� �˸޽�
	_float								m_fOutSwordNumber[20];
	//���� ���� �˸޽�
	_float								m_fCenterSwordNumber[20];
	//���� �˸޽�
	_float								m_pInsideSwordNumber[20];

	//�߰� �˸޽�
	_float								m_pMiddleSwordNumber[20];

private:
	HRESULT								Ready_Object(void);

public:
	static CDarkPollingGrave* Create(LPDIRECT3DDEVICE9 pGraphicDev
									, const wstring& wstrName);

private:
	virtual					void  Free(void);
	HRESULT					Add_Component(void);


public:
	void					SkillStart(void);
	void					EndAniCheck(void);
	virtual		HRESULT		Start_ForScene(void);


public:
	//==�ʱ� ��� ����Ʈ �Լ�=======================================================
	void					BlackWaveUpdate(void); //�ʱ⿡ ���������� ���� ���̺�
	void					WhiteLineUpdate(void);
	void					BlackLineUpdate(void);
	void					BlueFlowerUpdate(void);
	//==============================================================================

	void					RuneOutLine(void);
	void					RuneOutLineUpdate(void);

	//���� ���� SwordUpdate Update
	void					CenterSwordUpdate(const _float& fTimeDelta);
	//Inside Sword Update
	void					InsideSwordUpdate(const _float& fTimeDelta);
	//Middle Sword Update
	void					MiddleSwordUpdate(const _float& fTimeDelta);

	//Out Sword Update
	void					OutSwordUpdate(const _float& fTimeDelta);
};

#endif