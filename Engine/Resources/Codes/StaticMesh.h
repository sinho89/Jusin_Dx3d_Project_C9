#ifndef StaticMesh_h__
#define StaticMesh_h__

#include "Mesh.h"

BEGIN(Engine)

class CTexture;
class ENGINE_DLL CStaticMesh : public CMesh
{
private:
	explicit					CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit					CStaticMesh(const CStaticMesh& rhs);
	virtual						~CStaticMesh(void);

public:
	_ulong						Get_NumVertices(void) const;
	_ulong						Get_Stride(void) const;
	const _vec3*				Get_VtxPos(void) const;
	LPD3DXMESH*					Get_Mesh(void);

public:
	HRESULT						Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName);
	void						Render_Mesh(void);
	void						Render_Mesh(LPD3DXEFFECT pEffect);
	void						Render_Mesh(LPD3DXEFFECT pEffect, const _uint iPassIdx);

private:
	LPD3DXMESH					m_pOriMesh;	
	LPD3DXMESH					m_pMesh;
	LPD3DXBUFFER				m_pAdjacency;
	LPD3DXBUFFER				m_pSubSetBuffer;
	D3DXMATERIAL*				m_pSubSets;
	_ulong						m_dwSubSetCnt;
	_ulong						m_dwNumVertices;
	_ulong						m_dwStride;
	_vec3*						m_pVtxPos;

	LPDIRECT3DTEXTURE9*			m_ppTextures;
	LPDIRECT3DTEXTURE9*			m_ppNormalTexture;
	LPDIRECT3DTEXTURE9*			m_ppSpecuralTexture;

	wstring*					m_pwstrFileName;

	PROJECTID					m_eProjectID;

	_float						m_fTime;

	CTexture*					m_pReflectNormalTex;

private:
	_bool						Find_Alpha(const char* pFileName);

public:
	static CStaticMesh*			Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName);
	
public:
	void						PlusTime(_float fTime) { m_fTime += fTime; }
	void						SetReflectNormalTex(CTexture* pTex) { m_pReflectNormalTex = pTex; }

public:
	virtual CResource*			Clone(void);

public:
	virtual void				Free(void);

};

END

#endif // StaticMesh_h__
