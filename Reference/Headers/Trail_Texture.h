#ifndef Trail_Texture_h__
#define Trail_Texture_h__

#include "VIBuffer.h"

BEGIN(Engine)


class ENGINE_DLL CTrail_Texture : public CVIBuffer
{

private:
	explicit CTrail_Texture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTrail_Texture(const CTrail_Texture& rhs);
	virtual ~CTrail_Texture(void);
public:
	const _vec3* Get_VtxPos(void) const {
		return m_pPosition;}
	_ulong Get_VtxCntX(void) const {
		return m_ih.biWidth;}
	_ulong Get_VtxCntZ(void) const {
		return m_ih.biHeight;}
public:
	void Copy_Indices(Engine::INDEX32* pIndices, const _ulong& dwTriCnt);
public:
	HRESULT Ready_Buffer(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxInterval);
public:
	virtual void Render_Buffer(void);
private:
	HANDLE					m_hFile;
	BITMAPFILEHEADER		m_fh;
	BITMAPINFOHEADER		m_ih;
	_vec3*					m_pPosition;
	_bool					m_isClone;

public:
	static CTrail_Texture* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxInterval);
	virtual CResource* Clone(void);
	virtual void Free(void);


};


END


#endif // Trail_Texture_h__
