/*!
 * \file PlayerCamera.h
 * \date 2016/11/13 12:24
 *
 * \author �� ����
 * Contact: user@company.com
 *
 * \brief 
 �÷��̾� ���� ī�޶�.
 *
 * TODO: long description
 *
 * \note
*/
#ifndef PlayerCamera_h__
#define PlayerCamera_h__

#include "Defines.h"
#include "Camera.h"

namespace Engine
{
	class CTransform;
}
class CUI;
class CCursor;
class CUI_Skill;
class CUI_Item;
class CPlayer;
class CSound;

class CPlayerCamera
	: public Engine::CCamera
{
private:
	explicit				CPlayerCamera(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
public:
	virtual					~CPlayerCamera(void);
public:
	const _vec3*			Get_Pos(void);
	_bool					Get_Move(void);
	void					MoveFoward(const _float& fTimeDelta);
	void					MoveReturn(const _float& fTimeDelta);
public:
	void					Set_Move(void);
	void					Set_Action(const _bool& bAction);
	void					Set_ActionEnd(void);
	void					SetPlayBGM(const wstring& wstrKey);
public:
	HRESULT					InitCamera(void);

public:
	void					Loading_Update(const _float& fTimeDelta);
	virtual HRESULT			Start_ForScene(void);
	virtual void			Update_Object(const _float& fTimeDelta);

private:
	HRESULT					Ready_Camera(void);
	void					CheckCollision(void);
	void					MouseMove(const _float& fTimeDelta);
	void					FixMouse(void);		//ī�޶� ����
	void					Set_CtrlKey_Check(void);
	void					Set_SkillKey_Check(void);
	void					Set_InvenKey_Check(void);
	void					Set_StateKey_Check(void);
	void					Set_ESCKey_Check(void);

	void					Set_MainSlot_KeyCheck(const _float& fTimeDelta); // ���ν��� ����Ű ���� �Լ�
	void					Set_MainSlot_Skill(CUI_Skill* pSkill, _uint iIndex);
	void					Set_MainSlot_Item(CUI_Item* pItem, _uint iIndex, vector<CUI*>* pVecMainSlot);

	void					Set_CtrlKey_UIRender(void);
	void					Set_SkillKey_UIRender(void);
	void					Set_InvenKey_UIRender(void);
	void					Set_StateKey_UIRender(void);
	void					Set_ESCKey_UIRender(void);

	void					Set_Mouse_Active_Check(void);
public:
	void					Reset_CashingUI(void);
	void					Reset_Scene_Change(void);

public:
	const _vec3				GetCamAngle(void) const { return m_vAngle; }

public:
	static CPlayerCamera*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
private:
	D3DXQUATERNION			m_Quaternion;		//���ʹϾ�
	_matrix					m_matWorld;			//ī�޶� ���� ���
	_vec3					m_vAngle;			//ī�޶� ��
	_float					m_fCamSpeed;		//ī�޶� �ӵ�
	_float					m_fSkillCoolTime;
	_bool					m_bUsingSkill;
	_float					m_fInvSpeed;
	_bool					m_bActionUI;
	
private:
	CPlayer*				m_pPlayer;			//�÷��̾�
	Engine::CTransform*		m_pPlayerTrans;
	Engine::CTransform*		m_pActionTarget;
	CSound*					m_pSoundCom;
private:
	map<_int, LPD3DXMESH*> m_mapMesh;
	typedef map<_int, LPD3DXMESH*> MAPMESH;
	_uint m_iMeshSize;
private:
	CCursor*				m_pCursor;			//���콺 Ŀ��
	CUI*					m_pSkill;			//��ųâ
	CUI*					m_pInven;			//�κ�â
	CUI*					m_pState;			//����â
	CUI*					m_pOption;			//�ɼ�â
	CUI*					m_pMain;			//���ν���â
	CUI*					m_pPartition;		//����â
	CUI*					m_pEsc;				//ESCâ
	CUI*					m_pSystem;			//�ɼ�â
	CUI*					m_pDunGeon;			//����â

	CUI*					m_pWeaponShop;			//����â
	CUI*					m_pArmorShop;			//����â
	CUI*					m_pAccShop;			//����â
	CUI*					m_pPotionShop;			//����â
	CUI*					m_pBuy;			//����â

	
private:
	_vec3					m_vCamDir;			//ī�޶� ����
	_float					m_fEpslnY;			//������ ���� Y��
	//_float					m_fGap;				//ī�޶�� �÷��̾���� �Ÿ�.
	_float					m_fOldGap;
	_float					m_fFadeTime;
	_bool					m_bIsMove;
	_bool					m_bIsAction;
	_bool					m_bActionEnd;
	_bool					m_bPush[UI_KEY_END];
	_bool					m_bSlotPush[18];

	// Action Angle
	_float					m_fActionAngleForHanuman;

	// FadeEnd
	_bool					m_bFadeOut;
	_bool					m_bFadeIn;
	_float					m_fFadeInTime;


private:
	void					RotateYaw(void);
	void					RotatePitch(void);
	void					RotateRoll(void);
	void					RotateSlerp(const D3DXQUATERNION* pOriQuat, const D3DXQUATERNION* pDestQuat, const _float& fSpeed);
private:
	void					CameraAction(const _float& fTimeDelta);
	void					FireAction(const _float& fTimeDelta);
private:
	virtual	void			Free(void);
};

#endif // PlayerCamera_h__