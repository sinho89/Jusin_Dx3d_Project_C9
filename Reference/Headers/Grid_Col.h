#ifndef Grid_Col_h__
#define Grid_Col_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CGrid_Col : public CVIBuffer
{
private:
	explicit CGrid_Col(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGrid_Col(const CGrid_Col& rhs);
	virtual ~CGrid_Col(void);


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
	static CGrid_Col* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxInterval);
	virtual CResource* Clone(void);
	virtual void Free(void);

};

END

#endif // Grid_Col_h__
