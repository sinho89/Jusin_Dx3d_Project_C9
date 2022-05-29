/*!
 * \file TextureRenderer.h
 * \date 2016/11/18 20:00
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
#ifndef TextureRenderer_h__
#define TextureRenderer_h__

#include "Engine_Defines.h"

struct __declspec(uuid("{71771540-2017-11cf-ae26-0020afd79767}")) CLSID_TextureRenderer;

BEGIN(Engine)

class CTextureRenderer 
	: public CBaseVideoRenderer
{
private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;

public:
	LONG				m_lVidWidth;	//Video width
	LONG				m_lVidHeight;	//Video Height
	LONG				m_lVidPitch;	//Video Pitch

public:
	LPDIRECT3DTEXTURE9	m_pTexture;
	LPDIRECT3DTEXTURE9	m_pTextureBuffer;
	D3DFORMAT			m_TextureFormat;

public:
	CTextureRenderer(LPUNKNOWN pUnk, HRESULT* phr, LPDIRECT3DDEVICE9 pGraphicDev);
	~CTextureRenderer(void);

public:
	HRESULT				CheckMediaType(const CMediaType* pmt);		//Format acceptable
	HRESULT				SetMediaType(const CMediaType* pmt);		//Video format notification
	HRESULT				DoRenderSample(IMediaSample* pMediaSampler);//New video sample

public:
	void				SetTexture(DWORD dwStage);

public:
	void				ReleaseTexture(void);
};

END

#endif // TextureRenderer_h__