#ifndef Wave_h__
#define Wave_h__

#include "Defines.h"
#include "Effect.h"


class  CWave
	: public Engine::CEffect

{
private:
	explicit CWave(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CWave(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);
	virtual	void Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);

	enum		 WAVEPATTERN {
		PATTERN_ONE, PATTERN_TWO, PATTERN_THREE, PATTERN_FOUR,
		PATTERN_FIVE, PATTERN_SIX, PATTERN_SEVEN, PATTERN_EIGHT,
		PATTERN_NINE, PATTERN_END
	};

private:
	WAVEPATTERN	 m_ePattern;
	HRESULT		 Ready_Object(void);

private:
	_float		m_fAlphaDecrement;		 //알파값 감소량
	_float		m_fScaleDecrement;		 //스케일 감소량
	_bool		m_bBilBoardCheck;		//빌보드 체크
	_vec3		m_vOriginScale;			//초기설정 크기
	_float		m_fRotateResult;		//회전률
	_bool		m_bRendererCheck;

	_bool		m_bZRotateCheck;

	_float		m_fRotateSpeed;

	_bool		m_bBlurCheck;

	_bool		m_bBillboardRotate;
	 _matrix*	m_matTargetWorld;

public:
	void		SetBlurCheck(_bool bCheck);
	void		SetPattern(WAVEPATTERN Pattern);
	void		SetRotateResult(_float fRotate);
	void		SetZRotateCheck(_bool bCheck);
	void		SetRotateSpeed(_float fSpeed);

	void		SetTargetWorld(_matrix* matTarget);
	void		SetBillboardRotate(_bool bBheck);

public:
	static CWave* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const wstring& wstrName);

private:
	virtual			  void Free(void);
	HRESULT			  Add_Component(void);
	HRESULT			  SetUp_ConstantTable(LPD3DXEFFECT pEffect);


public:
	void			  SetPart(_int nIdx);
	void			  SetParticleTexuture(_int _Number);
	void			  MoveUpdate(const _float& fTimeDelta);
	void			  Update_RePart(void);
	//애니메이션 활성화 / 비활성화
	void			  SetAni(_bool bAni = TRUE);
	_bool			  GetAni(void);

	void			  SetDecrement(_float fAlphaDecrement, _float fScaleDecrement);

	void			  SetBilBoardCheck(_bool bcheck);

	void			  SetOriginScale(_vec3 vScale);

	void			  SetRendererCheck(_bool bCheck);

	void			  SetCheckPattern(void);

	const _bool		  GetBlurCheck(void) const { return m_bBlurCheck; }

};




#endif // Wave_h__
