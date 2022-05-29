#ifndef UI_CoolEff_h__
#define UI_CoolEff_h__


#include "Defines.h"
#include "Effect.h"

//UI Sprite를 위한 예시 클래스
class CUI_Skill;
class CUI_CoolEff
	: public Engine::CEffect
{
private:
	explicit CUI_CoolEff(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CUI_CoolEff(void);

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
	_float		m_fFadeTime;
	_bool		m_bAlpha_Increase;
	_bool		m_bRender;

private:
	CUI_Skill*	m_pParent;
	SKILL_ID	m_eSkillId;

public:
	void SetParent(CUI_Skill* pParent) { m_pParent = pParent; }
	void SetRender(_bool bRender) { m_bRender = bRender; }
	void SetFadeTime(_float fTime) { m_fFadeTime = fTime; }
	void SetSkillId(SKILL_ID eID) { m_eSkillId = eID; }

public:
	static CUI_CoolEff* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const wstring& wstrName);
	static CUI_CoolEff* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const wstring& wstrName, CUI_Skill* pParent, _float fFadeTime, SKILL_ID eID);

private:
	virtual void	  Free(void);

	HRESULT			  Add_Component(void);
	HRESULT			  SetUp_ConstantTable(LPD3DXEFFECT pEffect);


public:
	void			  Update_RePart(void);
	void			  SpriteUpdate(const _float& fTimeDelta);
	void			  SetPart(void);


};

#endif // UI_CoolEff_h__
