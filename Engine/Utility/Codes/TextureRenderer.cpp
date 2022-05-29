#include "TextureRenderer.h"

Engine::CTextureRenderer::CTextureRenderer(LPUNKNOWN pUnk, HRESULT* phr, LPDIRECT3DDEVICE9 pGraphicDev)
: CBaseVideoRenderer(__uuidof(CLSID_TextureRenderer), NAME("Texture Renderer"), pUnk, phr)
, m_lVidWidth(0)
, m_lVidHeight(0)
, m_lVidPitch(0)
, m_pTexture(NULL)
, m_pGraphicDev(pGraphicDev)
{
	// Store and AddRef the texture for our use.
	ASSERT(phr);
	if (phr)
		*phr = S_OK;
}

Engine::CTextureRenderer::~CTextureRenderer(void)
{
	ReleaseTexture();
}

HRESULT Engine::CTextureRenderer::CheckMediaType(const CMediaType* pmt)
{
	HRESULT hr = E_FAIL;
	VIDEOINFO* pvi = 0;

	CheckPointer(pmt, E_POINTER);

	//REject the connection if this is not a video type
	if(*pmt->FormatType() != FORMAT_VideoInfo)
		return E_INVALIDARG;

	//Only accept RGB24 video
	pvi = (VIDEOINFO*)pmt->Format();

	if(IsEqualGUID(*pmt->Type(), MEDIATYPE_Video) && IsEqualGUID(*pmt->Subtype(), MEDIASUBTYPE_RGB24))
		hr = S_OK;

	return hr;
}

HRESULT Engine::CTextureRenderer::SetMediaType(const CMediaType* pmt)
{
	HRESULT hr = NULL;

	VIDEOINFO* pviBmp;	//Bitmap info header
	pviBmp = (VIDEOINFO*)pmt->Format();

	m_lVidWidth		=	pviBmp->bmiHeader.biWidth;
	m_lVidHeight	=	abs(pviBmp->bmiHeader.biHeight);
	m_lVidPitch		=	(m_lVidWidth * 3 + 3) & ~(3);	// We are forcing RGB24

	//Create the texture that maps to this media type
	hr = m_pGraphicDev->CreateTexture(m_lVidWidth, m_lVidHeight, 1, 0,
		D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &m_pTexture, NULL);

	if(FAILED(hr))
		return hr;

	//텍스쳐 버퍼 생성..
	hr = m_pGraphicDev->CreateTexture(m_lVidWidth, m_lVidHeight, 1, 0,
		D3DFMT_X8R8G8B8, D3DPOOL_MANAGED,
		&m_pTextureBuffer, NULL);

	if(FAILED(hr))
		return hr;

	return S_OK;
}

HRESULT Engine::CTextureRenderer::DoRenderSample(IMediaSample* pMediaSampler)
{
	BYTE  *pBmpBuffer , *pTxtBuffer; // Bitmap buffer , texture buffer
	LONG  lTxtPitch;                // Pitch of bitmap , texture

	BYTE  * pbS = NULL;
	DWORD * pdwS = NULL;
	DWORD * pdwD = NULL;

	CheckPointer(pMediaSampler , E_POINTER);
	CheckPointer(m_pTextureBuffer , E_UNEXPECTED);

	// Get the video bitmap buffer
	pMediaSampler->GetPointer( &pBmpBuffer );

	// Lock the Texture
	D3DLOCKED_RECT d3dlr;

	if (FAILED(m_pTextureBuffer->LockRect(0, &d3dlr , 0, 0)))
		return E_FAIL;

	// Get the texture buffer & pitch
	pTxtBuffer = static_cast<byte *>(d3dlr.pBits);
	lTxtPitch = d3dlr.Pitch;

	// Copy the bits    
	pTxtBuffer += (m_lVidHeight-1) * lTxtPitch;
	for(int y=0; y<m_lVidHeight; y++ )
	{
		BYTE *pBmpBufferOld = pBmpBuffer;
		BYTE *pTxtBufferOld = pTxtBuffer;   
		for (int x = 0; x < m_lVidWidth; x++)
		{
			pTxtBuffer[0] = pBmpBuffer[0];
			pTxtBuffer[1] = pBmpBuffer[1];
			pTxtBuffer[2] = pBmpBuffer[2];
			pTxtBuffer[3] = 0xff;
			pTxtBuffer += 4;
			pBmpBuffer += 3;
		}
		pBmpBuffer = pBmpBufferOld + m_lVidPitch;
		pTxtBuffer = pTxtBufferOld - lTxtPitch;
	}

	// Unlock the Texture
	if (FAILED(m_pTextureBuffer->UnlockRect(0)))
		return E_FAIL;

	LPDIRECT3DTEXTURE9 pTexture = m_pTexture;

	m_pTexture = m_pTextureBuffer;
	m_pTextureBuffer = pTexture;

	return S_OK;
}

void Engine::CTextureRenderer::SetTexture(DWORD dwStage)
{
	if(m_pTexture != NULL)
		m_pGraphicDev->SetTexture(dwStage, m_pTexture);
}

void Engine::CTextureRenderer::ReleaseTexture(void)
{
	m_pTexture->Release();
	m_pTextureBuffer->Release();
}

