/*!
 * \file MediaObj.h
 * \date 2016/11/18 19:05
 *
 * \author 곽 경훈
 * Contact: user@company.com
 *
 * \brief 
 박 건우 2016년 11월 18일 19시 05분 게이 선언.
 *
 * TODO: long description
 *
 * \note
*/
#ifndef MediaObj_h__
#define MediaObj_h__

#include "Engine_Defines.h"
#include "atlbase.h"

BEGIN(Engine)

class CTextureRenderer;
class ENGINE_DLL CMediaObj
{
public:
	CComPtr<IGraphBuilder>  m_pGB;          // GraphBuilder
	CComPtr<IMediaControl>  m_pMC;          // Media Control
	CComPtr<IMediaPosition> m_pMP;          // Media Position
	CComPtr<IMediaEvent>    m_pME;          // Media Event
	CComPtr<IBasicAudio>	m_pBA;
	CComPtr<IBaseFilter>    m_pRenderer;    // our custom renderer
	CTextureRenderer*		pCTR;			// TextureRenderer 

private:
	_bool					m_bPause;

public:
	HRESULT					Initialize(LPDIRECT3DDEVICE9 pGraphicDev, _tchar* wFileName);
	void					Release(void);

public:
	void					SetTexMovie(_ulong dwStage);
	void					SetTime(_double time){ m_pMP->put_CurrentPosition(time); }

public:
	double					GetStopTime(void);
	double					GetCurrentPosition(void) const;
	LPDIRECT3DTEXTURE9		GetTexture(void);

	long					GetVidWidth(void);
	long					GetVidHeight(void);

public:
	BOOL					Play(void);
	BOOL					Pause(void);
	void					Loop(void);
	void					SetSound(_long _lVolume);
	void					Setbalance(_long _lBalance);

public:
	CMediaObj(void);
	~CMediaObj(void);
};

END

#endif // MediaObj_h__