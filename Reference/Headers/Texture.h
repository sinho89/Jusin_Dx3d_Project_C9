#ifndef Texture_h__
#define Texture_h__

#include "Resource.h"

BEGIN(Engine)

class ENGINE_DLL CTexture 
	: public CResource
{
protected:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture(void);
public:
	void Set_Texture(const _uint& iIndex = 0);
	void Set_Texture(LPD3DXEFFECT pEffect, const char* pShaderName, const _uint& iIndex = 0);
public:
	virtual HRESULT Ready_Texture(const _tchar* pPath, TEXTURETYPE eType, const _uint& iCnt);
private:
	vector<IDirect3DBaseTexture9*>				m_vecTexture;
	typedef vector<IDirect3DBaseTexture9*>		VECTEXTURE;
public:
	virtual CResource* Clone(void);
	static CTexture* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, TEXTURETYPE eType, const _uint& iCnt = 1);
public:
	virtual void Free(void);
};

END

#endif // Texture_h__
