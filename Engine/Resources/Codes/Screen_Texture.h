/*!
 * \file Screen_Texture.h
 * \date 2016/11/29 8:06
 *
 * \author WINTER
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef Screen_Texture_h__
#define Screen_Texture_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CScreen_Texture
	: public CVIBuffer
{
private:
	explicit				CScreen_Texture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit				CScreen_Texture(const CScreen_Texture& rhs);
	virtual					~CScreen_Texture(void);
public:
	HRESULT					Ready_Buffer(const _uint& iX, const _uint& iY,	const _uint& iSizeX, const _uint& iSizeY);
public:
	virtual void			Render_Buffer(void);
public:
	static CScreen_Texture*	Create(LPDIRECT3DDEVICE9 pGraphicDev, 
		const _uint& iX, 
		const _uint& iY, 
		const _uint& iSizeX, 
		const _uint& iSizeY);
	virtual CResource*		Clone(void);
	virtual void			Free(void);
};

END

#endif // Screen_Texture_h__