#ifndef Effect_h__
#define Effect_h__

#include "GameObject.h"
#include "Transform.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"

BEGIN(Engine)

class ENGINE_DLL CEffect
	: public CGameObject
{
protected:
	explicit CEffect(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual		~CEffect(void);


protected:
	CTransform*			m_pTransCom;
	CRenderer*			m_pRendererCom;
	CShader*			m_pShaderCom;
	CTexture*			m_pTextureCom;

protected:
	//파티클 변수
	TPART*				m_PrtD;		//파티클 데이터
	_vec3				m_vcWind;  //바람벡터

	//화면 출력
	_int				m_iVtx;  //Vertex Number
	VTXPARTICLE*		m_pVtx;  //Vertex
	_bool				m_bAni;  //애니메이션 발동 변수
	PARTICLEINFO		m_pParticleInfo;
	_matrix				m_matWorld;
	BOOL				m_bOriginColor;

	const _bool*		m_pPlayerAction; //Player의 동작을 받아오는 변수

	_bool				m_bEffectBlurCheck; //블러 그룹에 넣을지 판단하는 변수

protected:
	virtual HRESULT Ready_Object(void);
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);
	virtual	void Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);

public:
	static CEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev
							, const wstring& wstrName);

public:
	virtual void    Free(void);
	void		    Destroy(void);
	void		    SetAvgTime(_float fTime);// 평균 시간 설정
	static	_int    SortFnc(const TPART* p1, const TPART* p2);//정렬 함수
	PARTICLEINFO*	GetParticleInfo(void);

	void			SetTransCom(_vec3* m_Pos);

	CTransform*		GetTransCom( void );


	void			RandomSpeed(void);


	void			SetPos(const _vec3& vPos);
	void			SetScale(const _vec3& vScale);
	void			SetRot(const _vec3& vRot);

	void			SetAngle(const _vec3* vRot);


	void			SetTranscomInfo(void);

	void			SetBlurRenderCheck(_bool bCheck);

	void			SetBlurVariable(_int iBlurCnt = 8, _float fBlurPower = 1.2f, _float fBlurValue = 128.f, BLURTYPE eBlurType = BLUR_VERTICAL);

	const _bool		GetEffectBlurCheck(void) const { return m_bEffectBlurCheck; }
	void			SetEffectBlurCheck(const _bool bEffectBlurCheck) { m_bEffectBlurCheck = bEffectBlurCheck; }

};

END


#endif // Effect_h__
