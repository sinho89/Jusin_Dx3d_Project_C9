#ifndef Sprite_h__
#define Sprite_h__

#include "Defines.h"
#include "Effect.h"






class CMouseCol;
class CPickingRect;

class  CSprite
	: public Engine::CEffect

{
private:
	explicit CSprite(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CSprite(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	HRESULT		 Ready_Object(void);


public:
	static CSprite* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const wstring& wstrName);

private:
	virtual void Free(void);


	HRESULT			  Add_Component(void);
	HRESULT			  SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	void			  SetPart(_int nIdx);
	void			  SetParticleTexuture(_int _Number);
	void			  MoveUpdate(const _float& fTimeDelta);
	void			  Update_RePart(void);
	void			  SetParticleCount(_int iCount);


	//스프라이트 초기 설정
	void			InitSprite(void);

	//빌보드 체크함수
	void			SetBillBoardCheck(_bool check);

	//Effect 속성 체크함수
	void			SetMoveMent(Engine::PARTICLE_MOVEMENT move);

	//애니메이션 활성화 / 비활성화
	void			SetAni(_bool bAni= TRUE);

	void			SetSpriteSpeed(_float speed);

	//스프라이트 설정
	void			SetSpriteFrame(_int frame, _int select);

	//스프라이트 위치 조정함수
	void			SetStandardPos(_float pos, _int select);

	//Size
	void			SetSize(_float size, _int select);

	//Color
	void			SetColor(_float color, _int select);

	//Load
	void			LoadSpriteInfo(Engine::PARTICLEINFO* info);

	//Roof
	void			SetRoof(_bool roof);

	//AnimationStart
	void			ResetAnimation(void);


	static CSprite* LoadCreate(LPDIRECT3DDEVICE9 pGraphicDev
							, const wstring& wstrName);


private:
	HRESULT		    LoadReady_Object(void);


//맵배치용 변수 및 함수 추가
private:
	Engine::LAYERTYPE		m_eLayerType;
	Engine::VTXTEX*			m_pPickingVtx;
	CPickingRect*			m_pPickingRect;
	CMouseCol*				m_pMouseCol;
	OBJECT_MODE				m_eMode;
	OBJ_INFO				m_tInfo;
	_bool					m_bStop;
	_bool					m_bComplete;
	_long					m_lMouseMove;
	_bool					m_bCheck;
	_bool					m_bCreateRect;


public:
	void						Stop(void);
	void						Complete(void);
	Engine::CTransform*			GetTransCom(void);
	OBJECT_MODE					GetMode(void);

	void			SetMode(OBJECT_MODE eMode);
	void			SetObjectInfo(_vec3* vPos, _vec3* vScale, _vec3* vAngle);
	_bool			GetChecked(void);
	void			CheckTrue(void);
	void			CheckFalse(void);
	OBJ_INFO*		GetObjectInfo(void);
	void			SettingWork(void);



};


#endif // Sprite_h__