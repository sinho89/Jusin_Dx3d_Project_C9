/*!
 * \file Player.h
 * \date 2016/11/12 20:13
 *
 * \author 곽 경훈
 * Contact: user@company.com
 *
 * \brief 
 플레이어 클래스
 *
 * TODO: long description
 *
 * \note
*/
#ifndef Player_h__
#define Player_h__

#include "Defines.h"
#include "GameObject.h"

namespace  Engine
{
	class CTransform;
	class CDynamicMesh;
	class CRenderer;
	class CShader;
	class CAnimator;
	class CCalculator;
	class CRigidbody;
	class CCollider;
	class CCamera;
	class CLayer;
}

class CSound;
class CUI;
class CPlayerCamera;
class CPlayer
	: public Engine::CGameObject
{
protected:
	explicit					CPlayer(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual						~CPlayer(void);
public:
	PLAYER_INFO*				GetPlayerInfo(void);
	const vector<Engine::ANIINFO*>* Get_AniInfo(void) const;
	Engine::CAnimator*			GetAnimatorInfo(void);
	_uint						GetStateNum(void);

public:
	virtual	void				Set_ActionEnd(void)PURE;
	void						SetPlayerInfo(PLAYER_INFO* pPlayerInfo);
	void						Set_SceneType(const SCENE_TYPE& eType);
	void						Camera_MoveFoward(const _float& fTimeDelta);
	void						Camera_MoveReturn(const _float& fTimeDelta);
	void						Set_CameraAction(const _bool& bAction);
public:
	virtual HRESULT				Start_ForScene(void);
	virtual void				Update_Object(const _float& fTimeDelta);
	virtual void				Render_Object(void);
	virtual	void				Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);
protected:
	Engine::CTransform*			m_pTransCom;
	Engine::CDynamicMesh*		m_pMeshCom;
	Engine::CRenderer*			m_pRendererCom;
	Engine::CShader*			m_pShaderCom;
	Engine::CAnimator*			m_pAnimatorCom;
	Engine::CCalculator*		m_pCalculatorCom;
	Engine::CRigidbody*			m_pRigidbodyCom;
	Engine::CCollider*			m_pColliderCom;
	Engine::CCollider*			m_pSphereColCom;
	CSound*						m_pSoundCom;

	Engine::CStaticMesh**		m_ppMapMesh;
	Engine::CLayer*				m_pLogicLayer;
	_int						m_iMapSize;
	_float						m_fFootPrintTime;
	_bool						m_bFootPrintLR;
	
	//발자국
	//_bool						m_bOneTwoStep[2];
	//_int						m_iFootPrintCnt;
	//void						DrawPrintFoot(_int iLeftRight);

protected:
	Engine::CCamera*			m_pLightCam;
	CPlayerCamera*				m_pPlayerCamera;
protected:
	vector<Engine::ANIINFO*>	m_vecAniInfo;
	map<_uint, list<Engine::ANIMATORINFO*>>*	m_pmapNextMotion;
	_uint						m_iAniIdx;
	_uint						m_iPlayerAniState;
	SCENE_TYPE					m_eSceneType;

protected: // 레벨업
	_bool						m_bLevelUp;
	_float						m_fAlpha;
	CUI*						m_pDamParent;
	
	//For. Motion Blur, 이전 위치를 위한 World, View, Projection 행렬을 저장할 행렬 변수들

protected:
	_matrix						m_matOldWorld;
	_matrix						m_matOldView;
	_matrix						m_matOldProj;
protected:
	PLAYER_INFO					m_tPlayerInfo;
	_uint						m_iPlayerSkillState;
	_bool						m_bPlayerMoveState;
protected:
	HRESULT 					LoadAniInfo(const _tchar* pFileName);
	HRESULT						LoadCollider(const _tchar* pFileName);
	void						Compute_CameraLook(const _vec3* pTargetPos);
	void						PlayerInfoUpdate();
	void						LevelUp(void);
public:
	_uint						GetPlayerAniState(void) { return m_iPlayerAniState; }
	_bool						GetPlayerMoveState(void){ return m_bPlayerMoveState; }
protected:
	virtual void				Free(void);
};

#endif // Player_h__