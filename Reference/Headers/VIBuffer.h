#ifndef VIBuffer_h__
#define VIBuffer_h__

#include "Resource.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer 
	: public CResource
{
protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer(void);
public:
	_ulong Get_TriCnt() const {
		return m_dwTriCnt;}
public:
	void Change_BoxColor(const _ulong& dwColor);
public:
	virtual HRESULT Ready_Buffer(void);
	virtual void Render_Buffer(void) {}

public:
	void			GetVtxInfo(void* pVtxInfo);

	void			SetVtxInfo(void* pVtxInfo);

protected:
	LPDIRECT3DVERTEXBUFFER9				m_pVB;
	_ulong								m_dwVtxCnt;
	_ulong								m_dwVtxFVF;
	_ulong								m_dwVtxSize;
	_bool								m_isCreateVtxBuffer;
protected:
	LPDIRECT3DINDEXBUFFER9				m_pIB;
	_ulong								m_dwIdxSize;
	_ulong								m_dwTriCnt;
	D3DFORMAT							m_IndexFmt;
	_bool								m_isCreateIdxBuffer;

public:
	virtual CResource* Clone(void)PURE;
public:
	virtual void Free(void);
};

END

#endif // VIBuffer_h__
