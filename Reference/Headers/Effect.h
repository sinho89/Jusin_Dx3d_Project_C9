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
	//��ƼŬ ����
	TPART*				m_PrtD;		//��ƼŬ ������
	_vec3				m_vcWind;  //�ٶ�����

	//ȭ�� ���
	_int				m_iVtx;  //Vertex Number
	VTXPARTICLE*		m_pVtx;  //Vertex
	_bool				m_bAni;  //�ִϸ��̼� �ߵ� ����
	PARTICLEINFO		m_pParticleInfo;
	_matrix				m_matWorld;
	BOOL				m_bOriginColor;

	const _bool*		m_pPlayerAction; //Player�� ������ �޾ƿ��� ����

	_bool				m_bEffectBlurCheck; //�� �׷쿡 ������ �Ǵ��ϴ� ����

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
	void		    SetAvgTime(_float fTime);// ��� �ð� ����
	static	_int    SortFnc(const TPART* p1, const TPART* p2);//���� �Լ�
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
