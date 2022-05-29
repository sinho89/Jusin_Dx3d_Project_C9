/*!
 * \file UI.h
 * \date 2016/11/12 16:22
 *
 * \author Shinho
 * Contact: user@company.com
 *
 * \brief  :: UI Top parents
 *
 * TODO: long description
 *
 * \note
*/

#ifndef UI_h__
#define UI_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CUICalculater;
}
class CPlayer;
class CCursor;
class CSound;
class CUI : public Engine::CGameObject
{
protected:
	explicit	CUI(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual		~CUI(void);

public:
	virtual HRESULT Start_ForScene(void);
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);


protected:
	CSound*			m_pSound;
public:
	void		SetSelect(_bool bSelect);
	void		SetIndex(_uint iIndex);
	void		SetBeforeIndex(_uint iIndex);
	void		SetAlpha(_float	fAlpha);
	void		SetSkipScene(_bool bSkipScene);
	void		SetPos(_vec3 vPos);
	void		SetRender(_bool bRender);
	void		SetSortNum(WORD iSortNum);
	void		SetClick(_bool bClick);
	void		SetWindowProcess(_bool bWindowProcess);
	void		SetMove(_bool bMove);
	void		SetAngle(_float fAngle);
	void		SetSheetSelect(_bool bSheetSelect);
	void		SetBlockPick(_bool bBlockPick);
	void		SetWinPos(_vec2 vPos);
	void		SetMousePick(_bool bMousePick);
	void		SetToolTipOn(_bool bToolTipOn);
	void		SetWear(_bool bWear);
	void		SetOut(_bool bOut);
	void		SetSwap(_bool bSwap);
	void		SetFollow(_bool bFollow);
	void		SetAlive(_bool bAlive);
	void		SetSlotLevel(_uint iSlotLevel);
	void		SetMain(_bool bMain);
	void		SetKeyBoard(_bool bKeyBoard);
	void		SetPartition(_bool bPartition);
	void		SetPick(_bool bPick);
	void		SetPartitionMinCnt(_uint iPartitionMinCnt);
	void		SetPartitionMaxCnt(_uint iPartitionMaxCnt);
	void		SetRightWeapon(_bool bRightWeapon);
	void		SetLeftWeapon(_bool bLeftWeapon);
	void		SetFontColor(_vec4 vColor);
	void		SetFade(_bool bFade);
	void		SetFoward(_bool bFoward);
	void		SetSize(_float fX, _float fY){m_fSizeX = fX; m_fSizeY = fY;}
	void		SetAlphaTime(_float fAlphaTime) { m_fAlphaTime = fAlphaTime; }
	void		SetRBPick(_bool bRBPick) { m_bRBPick = bRBPick; }
public:
	_bool		GetSkipScene(void);
	_uint		GetIndex(void);
	_bool		GetSelect(void);
	_float&		GetAlpha(void);
	_float&		GetAngle(void);
	_float*		GetAnglePointer(void);
	_float*		GetfX(void);
	_float*		GetfY(void);
	_bool&		GetAlphaIncrease(void);
	_vec4		GetInfo(void);
	_bool		GetRender(void);
	WORD		GetSortNum(void);
	_bool		GetWindowProcess(void);
	_bool		GetCollision(void);
	_bool		GetClick(void);
	_bool		GetMove(void);
	_bool		GetBarCollision(void);
	_bool		GetSheetSelect(void);
	_bool		GetBlockPick(void);
	_bool		GetMousePick(void);
	_bool		GetToolTipOn(void);
	_bool		GetButtonCol(void);
	_bool		GetWear(void);
	_bool		GetOut(void);
	_bool		GetSwap(void);
	WINDOW_ID	GetWinType(void);
	_bool		GetFollow(void);
	_bool		GetAlive(void);
	_uint		GetSlotLevel(void);
	_bool		GetMain(void);
	_bool		GetKeyBoard(void);
	_bool		GetPick(void);
	_bool		GetPartition(void);
	_uint*		GetPartitionMinCnt(void);
	_uint*		GetPartitionMaxCnt(void);
	_bool		GetRightWeapon(void);
	_bool		GetLeftWeapon(void);
	_bool		GetFade(void);
	_bool		GetFoward(void);
	_float		GetAlphaTime(void) { return m_fAlphaTime; }
	_bool		GetRBPick(void) {	return m_bRBPick; }

protected:	// 알파관련
	_float		m_fAlpha;
	_float		m_fAlphaTime;
	_bool		m_bAlpha_Increase;

protected: 
	_matrix		m_matView, m_matProj;
	_float		m_fX, m_fY;
	_float		m_fSizeX, m_fSizeY;
	_float		m_fTermX, m_fTermY;
	_float		m_fAngle;
	
	WORD		m_wSortNum;
	WINDOW_ID	m_eWinType;

protected:
	_vec2		m_vFontPos;
	_vec2		m_vUvCut;
	_vec4		m_vColor;

protected:
	_bool		m_bCollision;
	_bool		m_bBarCollision;
	_bool		m_bButtonCollision;
	_bool		m_bClick;
	_bool		m_bRClick;
	_bool		m_bDoubleClick;
	_bool		m_bSkipScene;
	_bool		m_bCreate;
	_bool		m_bSelect;
	_bool		m_bRender;
	_bool		m_bMove;
	_bool		m_bWindowProcess;
	_bool		m_bFollow;
	_bool		m_bAlive;
	_bool		m_bRBPick;

	/// Sheet
	_bool		m_bSheetSelect;

	//Block
	_bool		m_bBlockPick;
	_bool		m_bMousePick;
	_bool		m_bToolTipOn;
	_bool		m_bWear;
	_bool		m_bOut;
	_bool		m_bSwap;
	_bool		m_bMain;
	_bool		m_bKeyBoard;
	_bool		m_bPartition;
	_bool		m_bPick;
	_bool		m_bLeftWeapon;
	_bool		m_bRightWeapon;

	// 가리기
	_bool		m_bFade;
	_bool		m_bFoward;


protected:
	_uint		m_iIndex;
	_uint		m_iBeforeIndex;
	_uint		m_iSlotLevel;
	_uint		m_iPartitionMinCnt;
	_uint		m_iPartitionMaxCnt;
	CUI*		m_pCursor;

public:
	void		CheckWindowProcess();
	void		Overlap_Window();

protected:
	Engine::CUICalculater*				m_pCalCom;
	CPlayer*							m_pPlayer;

public:
	virtual void Free(void);
};

#endif // UI_h__