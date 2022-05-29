#ifndef GameObject_h__
#define GameObject_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CComponent;

class ENGINE_DLL CGameObject 
	: public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const OBJECTSTATE& eState = STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CGameObject(void);

public:	//Getter
	template <typename T>
	CComponent*			GetComponent(COMPONENTID eID);
	_float				Get_ViewZ(void) const;
	OBJECTSTATE&		GetState(void);
	_bool				GetActive(void);
	_bool				GetLight(void);
	wstring&			GetName(void);
	_bool				GetStatic(void);
	_bool				GetLoading(void);

	const _uint			GetBlurPass(void) const { return m_iBlurPass; }
	const _int			GetBlurCnt(void) const { return m_iBlurCnt; }
	const _float		GetBlurPower(void) const { return m_fBlurPower; }
	const _float		GetBlurValue(void) const { return m_fBlurValue; }
	const BLURTYPE		GetBlurType(void) const { return m_eBlurType; }
	const _bool			GetOutlineCheck(void) const { return m_bOutlineCheck; }
	const _vec3			GetOutlineColor(void) const { return m_bOutlineColor; }
	const wstring		GetObjectName(void) const { return m_wstrName; }

	void				SetBlurPass(const _uint iBlurPass) { m_iBlurPass = iBlurPass; }
	void				SetBlurCnt(const _int iBlurCnt) { m_iBlurCnt = iBlurCnt; }
	void				SetBlurPower(const _float fBlurPower) { m_fBlurPower = fBlurPower; }
	void				SetBlurValue(const _float fBlurValue) { m_fBlurValue = fBlurValue; }
	void				SetBlurType(const BLURTYPE eBlurType) { m_eBlurType = eBlurType; }
	void				SetOutlineCheck(const _bool bOutlineCheck) { m_bOutlineCheck = bOutlineCheck; }
	void				SetOutlineColor(const _vec3 vColor) { m_bOutlineColor = vColor; }

	void				PlusBlurPower(const _float fBlurPower) { m_fBlurPower += fBlurPower; }

public: //Setter
	void				SetState(const OBJECTSTATE& eState);
	void				SetActive(const _bool& bIsActive);

public:
	virtual HRESULT		Start_ForScene(void);	//씬 생성 전에 레이어에 접근 할 필요가 있을때 초기화 함수.
	virtual void		Update_Object(const _float& fTimeDelta);
	virtual void		Render_Object(void);
	virtual	void		Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);	//최적화를 위함이다.

protected:
	LPDIRECT3DDEVICE9	m_pGraphicDev;

protected:
	wstring				m_wstrName;				//객체의 고유 이름
	OBJECTSTATE			m_eState;				//오브젝트 상태 (죽었는지 살았는지)
	_bool				m_bIsActive;			//객체의 활성화 상태
	_bool				m_bIsStatic;			//씬 이동 할때 지울건지 말건지의 판단
	_float				m_fViewZ;
	_bool				m_bLight;				// UI 빛활성화관련변수
	_bool				m_bLoading;				// Loading 씬 객체 제어 변수
	_bool				m_bOnceStart;			// Start For Scene 한번 호출되게 막는 용도(알아서 원하는위치에 막아요) 초기값 false
	_bool				m_bOutlineDraw;			//외곽선을 그릴 놈?

	_uint				m_iBlurPass;			//Effect Shader의 Pass
	_int				m_iBlurCnt;				//블러할 횟수
	_float				m_fBlurPower;			//블러의 POWER
	_float				m_fBlurValue;			//흔들어 줄 정도?
	BLURTYPE			m_eBlurType;			//블러의 타입

	_bool				m_bOutlineCheck;		//외곽선을 그릴 것인가 말 것인가?
	_vec3				m_bOutlineColor;		//외곽선 컬러.

protected:
	void				Compute_ViewZ(const _vec3* pPosition);

protected:
	map<const _tchar*, CComponent*>				m_mapComponent[COMID_END];
	typedef map<const _tchar*, CComponent*>		MAPCOMPONENT;

public:
	virtual void Free(void);
};

template <typename T>
CComponent* Engine::CGameObject::GetComponent(COMPONENTID eID)
{
	MAPCOMPONENT::iterator iter = m_mapComponent[eID].begin();
	MAPCOMPONENT::iterator iter_end = m_mapComponent[eID].end();

	for(iter; iter != iter_end; iter++)
	{
		if(dynamic_cast<T*>((iter->second)))
		{
			iter->second->AddRef();
			return iter->second;
		}
	}
	return NULL;
}

END

#endif // GameObject_h__
