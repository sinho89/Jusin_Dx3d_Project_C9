#ifndef UIEffect_h__
#define UIEffect_h__


#include "Defines.h"
#include "Effect.h"

//UI Sprite를 위한 예시 클래스
class CUI;
class CUIEffect
	: public Engine::CEffect
{
private:
	explicit CUIEffect(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CUIEffect(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	HRESULT		 Ready_Object(void);

private:
	_matrix		m_matView, m_matProj;
	_float		m_fX, m_fY;
	_float		m_fSizeX, m_fSizeY;
	_float		m_fTermX, m_fTermY;
	_float		m_fAngle;

	//===============스프라이트 관련 변수==========================//
private:
	_int		m_iMaxFrame;  //스프라이트의 총개수
	_int		m_iRow; //스프라이트 가로 개수
	_int		m_iColumn; //세로 개수
	_float		m_fSpriteSpeed; //스프라이트 속도


private:	// 알파관련
	_float		m_fAlpha;
	_float		m_fAlphaTime;
	_bool		m_bAlpha_Increase;
	_bool		m_bRender;
	_bool		m_bAlpha;

private:
	CUI*	m_pParent;
	_uint	m_iImgType;
	_vec2	m_vScale;
	_float	m_fTime;
	_vec3	m_vCount;

public:
	void SetParent(CUI* pParent) { m_pParent = pParent; }
	void SetImgType(_uint iImgType) { m_iImgType = iImgType; }
	void SetScale(_float fX, _float fY) { m_fSizeX = fX; m_fSizeY = fY; }
	void SetTime(_float fTime) { m_fTime = fTime; }
	void SetCount(_int iRow, _int iColumn, _int iMaxFrame) {	
		m_iRow = iRow; 
		m_iColumn = iColumn;
		m_iMaxFrame = iMaxFrame;
	}
	void SetXY (_float fX, _float fY) { m_fX = fX; m_fY = fY; }
	void SetAlpha(_bool bAlpha) { m_bAlpha = bAlpha; }

public:
	static CUIEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const wstring& wstrName);
	static CUIEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const wstring& wstrName, CUI* pParent, _uint iImgType, _float fSizeX, _float fSizeY
		, _float fTime, _int iRow, _int iColumn, _int iMaxFrame, _float fX = 0.f, _float fY = 0.f, _bool bAlpha = false);

private:
	virtual void	  Free(void);

	HRESULT			  Add_Component(void);
	HRESULT			  SetUp_ConstantTable(LPD3DXEFFECT pEffect);


public:
	void			  Update_RePart(void);
	void			  SpriteUpdate(const _float& fTimeDelta);
	void			  SetPart(void);


};


#endif // UIEffect_h__
