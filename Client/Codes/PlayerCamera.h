/*!
 * \file PlayerCamera.h
 * \date 2016/11/13 12:24
 *
 * \author 곽 경훈
 * Contact: user@company.com
 *
 * \brief 
 플레이어 전용 카메라.
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
	void					FixMouse(void);		//카메라 고정
	void					Set_CtrlKey_Check(void);
	void					Set_SkillKey_Check(void);
	void					Set_InvenKey_Check(void);
	void					Set_StateKey_Check(void);
	void					Set_ESCKey_Check(void);

	void					Set_MainSlot_KeyCheck(const _float& fTimeDelta); // 메인슬롯 단축키 관련 함수
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
	D3DXQUATERNION			m_Quaternion;		//쿼터니언
	_matrix					m_matWorld;			//카메라 월드 행렬
	_vec3					m_vAngle;			//카메라 각
	_float					m_fCamSpeed;		//카메라 속도
	_float					m_fSkillCoolTime;
	_bool					m_bUsingSkill;
	_float					m_fInvSpeed;
	_bool					m_bActionUI;
	
private:
	CPlayer*				m_pPlayer;			//플레이어
	Engine::CTransform*		m_pPlayerTrans;
	Engine::CTransform*		m_pActionTarget;
	CSound*					m_pSoundCom;
private:
	map<_int, LPD3DXMESH*> m_mapMesh;
	typedef map<_int, LPD3DXMESH*> MAPMESH;
	_uint m_iMeshSize;
private:
	CCursor*				m_pCursor;			//마우스 커서
	CUI*					m_pSkill;			//스킬창
	CUI*					m_pInven;			//인벤창
	CUI*					m_pState;			//정보창
	CUI*					m_pOption;			//옵션창
	CUI*					m_pMain;			//메인슬롯창
	CUI*					m_pPartition;		//분할창
	CUI*					m_pEsc;				//ESC창
	CUI*					m_pSystem;			//옵션창
	CUI*					m_pDunGeon;			//던전창

	CUI*					m_pWeaponShop;			//상점창
	CUI*					m_pArmorShop;			//상점창
	CUI*					m_pAccShop;			//상점창
	CUI*					m_pPotionShop;			//상점창
	CUI*					m_pBuy;			//구매창

	
private:
	_vec3					m_vCamDir;			//카메라 방향
	_float					m_fEpslnY;			//조정을 위한 Y값
	//_float					m_fGap;				//카메라와 플레이어와의 거리.
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