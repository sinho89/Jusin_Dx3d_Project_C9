#ifndef Reflect_h__
#define Reflect_h__


#include "Defines.h"
#include "Effect.h"


namespace Engine
{
	class CTexture;
}

class  CReflect
	: public Engine::CEffect

{
private:
	explicit CReflect(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CReflect(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	HRESULT		 Ready_Object(void);

private:
	Engine::CTexture*		m_pNormalTextureCom;
	_vec2					m_vDisturbResult; //±³¶õ°ª
	_float					m_fReflectionIndex;//±¼Àý·ü


	_int					m_iTextureNumber; //Base Texture Number;
	_int					m_iNormalTextureNumber;
	_int					m_iShaderPath;


public:
	static CReflect* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const wstring& wstrName);

private:
	virtual			  void Free(void);
	HRESULT			  Add_Component(void);
	HRESULT			  SetUp_ConstantTable(LPD3DXEFFECT pEffect);


public:
	void			  SetPart(_int nIdx);
	void			  MoveUpdate(const _float& fTimeDelta);
	void			  Update_RePart(void);
	void			  SetAni(void);
	void			  SetReflectIndex(_float fReflect);

	void			  SetTextureNumber(_int iNumber);
	void			  SetNormalTextureNumber(_int iNumber);
	void			  SetShaderPath(_int iPath);
	void			  SetAni(_bool bAni);
	_bool			  GetAni(void);
	void			  MinusReflectionIndex(_float fResult);
	_float			  GetReflectionIndex(void);
};


#endif