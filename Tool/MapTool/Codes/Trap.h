#ifndef	TRAP_h__
#define	TRAP_h__

#include "Defines.h"
#include "GameObject.h"

//¸ÊÅø
namespace Engine
{
	class CTransform;
	class CRenderer;
	class CStaticMesh;
	class CDynamicMesh;
	class CShader;

	class CAnimator;
	class CCalculator;
	class CRigidbody;
}

class CMouseCol;
class CPickingRect;
//class CSphere_Tool;

class CTrap
	:public Engine::CGameObject
{
public:
	enum TRAP_ANI { STAND_IDLE, MOVE };

private:
	explicit CTrap(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CTrap(void);
public:
	virtual HRESULT				Start_ForScene(void);
	virtual void				Update_Object(const _float& fTimeDelta);
	virtual void				Render_Object(void);

public:
	void						Stop(void);
	void						Complete(void);
	Engine::CTransform*			GetTransCom(void);
	OBJECT_MODE					GetMode(void);


public:
	void			SetMode(OBJECT_MODE eMode);
	void			SetObjectInfo(_vec3* vPos, _vec3* vScale, _vec3* vAngle);
	_bool			GetChecked(void);
	void			CheckTrue(void);
	void			CheckFalse(void);
	OBJ_INFO*		GetObjectInfo(void);

private:
	Engine::CResource_Manager* m_pResourceMgr;
private:
	Engine::CTransform*			m_pTransCom;
	Engine::CRenderer*			m_pRendererCom;
	Engine::CShader*			m_pShaderCom;
	//Engine::CStaticMesh*		m_pMeshCom;
	Engine::CDynamicMesh*		m_pMeshCom;

	Engine::CAnimator*			m_pAnimatorCom;
	Engine::CCalculator*		m_pCalculatorCom;
	Engine::CRigidbody*			m_pRigidbodyCom;
	Engine::CTransform*			m_pTargetTransCom;

	Engine::LAYERTYPE			m_eLayerType;
	Engine::VTXTEX*				m_pPickingVtx;

	//CSphere_Tool*			m_pSphere;
	CPickingRect*			m_pPickingRect;
	CMouseCol*				m_pMouseCol;
	OBJECT_MODE				m_eMode;
	OBJ_INFO				m_tInfo;

private:
	vector<Engine::ANIINFO*>	m_vecAniInfo;
	map<_uint, list<Engine::ANIMATORINFO*>>*	m_pmapNextMotion;


private:
	_bool					m_bStop;
	_bool					m_bComplete;
	_long					m_lMouseMove;
	_float					m_fAlpha;
	_int					m_iAniIdx;
	_int					m_iMonsterAniState;
	_vec3					m_vStartPos;

private:
	_bool					m_bCheck;
	_bool					m_bCreateRect;
	_bool					m_bCubeRayCol;


private:
	HRESULT		Ready_Object(void);
	HRESULT		Add_Component(void);
	HRESULT		Set_ConstantTable(LPD3DXEFFECT pEffect);

private:
	HRESULT 	LoadAniInfo(const _tchar* pFileName);
	void		SettingWork(void);

public:
	static CTrap* Create(LPDIRECT3DDEVICE9 pGraphicDev
		,const wstring& wstrName);
private:
	virtual void Free(void);
};


#endif // NPC_h__
