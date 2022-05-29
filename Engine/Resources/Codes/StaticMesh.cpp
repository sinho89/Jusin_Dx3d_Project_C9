#include "StaticMesh.h"
#include "Renderer.h"
#include "Resource_Manager.h"
#include "Export_Utility.h"
#include "Texture.h"

USING(Engine)

Engine::CStaticMesh::CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CMesh(pGraphicDev)
{
	m_eProjectID = CResource_Manager::GetInstance()->GetResourceProjectID();
}

Engine::CStaticMesh::CStaticMesh(const CStaticMesh& rhs)
: Engine::CMesh(rhs)
, m_pAdjacency(rhs.m_pAdjacency)
, m_pSubSetBuffer(rhs.m_pSubSetBuffer)
, m_pSubSets(rhs.m_pSubSets)
, m_dwSubSetCnt(rhs.m_dwSubSetCnt)
, m_pMesh(rhs.m_pMesh)
, m_pOriMesh(rhs.m_pOriMesh)
, m_pVtxPos(rhs.m_pVtxPos)
, m_dwNumVertices(rhs.m_dwNumVertices)
, m_dwStride(rhs.m_dwStride)
, m_pwstrFileName(rhs.m_pwstrFileName)
, m_fTime(0.0f)
{
	m_ppTextures		= new LPDIRECT3DTEXTURE9[rhs.m_dwSubSetCnt];

	m_eProjectID = CResource_Manager::GetInstance()->GetResourceProjectID();

	if (PROJECT_CLIENT == m_eProjectID)
	{
		m_ppNormalTexture = new LPDIRECT3DTEXTURE9[rhs.m_dwSubSetCnt];
		m_ppSpecuralTexture = new LPDIRECT3DTEXTURE9[rhs.m_dwSubSetCnt];
	}	

	for (_ulong i = 0; i < rhs.m_dwSubSetCnt; ++i)
	{
		m_ppTextures[i] = rhs.m_ppTextures[i];
		m_ppTextures[i]->AddRef();

		if (PROJECT_CLIENT == m_eProjectID)
		{
			m_ppNormalTexture[i] = rhs.m_ppNormalTexture[i];
			m_ppNormalTexture[i]->AddRef();

			m_ppSpecuralTexture[i] = rhs.m_ppSpecuralTexture[i];
			m_ppSpecuralTexture[i]->AddRef();
		}
	}

	m_pAdjacency->AddRef();
	m_pSubSetBuffer->AddRef();
	m_pOriMesh->AddRef();
	m_pMesh->AddRef();	
}

Engine::CStaticMesh::~CStaticMesh(void)
{

}

Engine::_ulong Engine::CStaticMesh::Get_NumVertices(void) const
{
	return m_dwNumVertices;
}

Engine::_ulong Engine::CStaticMesh::Get_Stride(void) const
{
	return m_dwStride;
}

const _vec3* Engine::CStaticMesh::Get_VtxPos(void) const
{
	return m_pVtxPos;
}

HRESULT Engine::CStaticMesh::Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName)
{
	_tchar			szFullPath[128] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	if(FAILED(D3DXLoadMeshFromX(szFullPath, D3DXMESH_MANAGED, m_pGraphicDev, &m_pAdjacency, &m_pSubSetBuffer, NULL, &m_dwSubSetCnt, &m_pOriMesh)))
		return E_FAIL;

	// 메시에 노멀이 없을경우 노멀을 부여한다.
	unsigned long dwFVF = m_pOriMesh->GetFVF();

	if(!(dwFVF & D3DFVF_NORMAL))
	{
		m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pGraphicDev, &m_pMesh);

		D3DXComputeNormals(m_pMesh, (_ulong*)(m_pAdjacency->GetBufferPointer()));
	}
	else
	{
		m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF, m_pGraphicDev, &m_pMesh);

	}

	// 메시에 맞는 사이즈의 바운딩 박스를 생성한다.
	void*			pVertex = NULL;

	m_dwNumVertices = m_pMesh->GetNumVertices();

	m_pVtxPos = new _vec3[m_dwNumVertices];

	m_pMesh->LockVertexBuffer(0, &pVertex);

	LPD3DVERTEXELEMENT9		pDecl = NULL;

	D3DVERTEXELEMENT9		Decl[MAX_FVF_DECL_SIZE];
	ZeroMemory(Decl, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);

	m_pMesh->GetDeclaration(Decl);


	// 어디가 위치정보인지 파악을 하자.

	_ubyte			byOffset = 0;

	for (_ulong i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		if(Decl[i].Usage == D3DDECLUSAGE_POSITION)
		{
			byOffset = (_ubyte)Decl[i].Offset;
			break;
		}
	}

	// 정점의 fvf를 알 때, 정점의 크기를 얻어와준다.
	m_dwStride = D3DXGetFVFVertexSize(dwFVF);	

	for (_ulong i = 0; i < m_dwNumVertices; ++i)
	{
		m_pVtxPos[i] = *((_vec3*)(((_ubyte*)pVertex) + (m_dwStride * i + byOffset)));
	}

	// D3DXBuffer가 담고있는 정보의 가장 앞 주소를 리턴하낟.
	m_pSubSets = (D3DXMATERIAL*)m_pSubSetBuffer->GetBufferPointer();

	m_ppTextures		= new LPDIRECT3DTEXTURE9[m_dwSubSetCnt];

	if (PROJECT_CLIENT == m_eProjectID)
	{
		m_pwstrFileName = new wstring[m_dwSubSetCnt];
		m_ppNormalTexture = new LPDIRECT3DTEXTURE9[m_dwSubSetCnt];
		m_ppSpecuralTexture = new LPDIRECT3DTEXTURE9[m_dwSubSetCnt];
	}

	for (_ulong i = 0; i < m_dwSubSetCnt; ++i)
	{
		_tchar			szFileName[128] = L"";

		lstrcpy(szFullPath, pFilePath);

		MultiByteToWideChar(CP_ACP, 0, m_pSubSets[i].pTextureFilename, strlen(m_pSubSets[i].pTextureFilename)
			, szFileName, 128);

		lstrcat(szFullPath, szFileName);

		if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &m_ppTextures[i])))
		{
			MSG_BOX("StaticMesh's Texture Create Failed");
			return E_FAIL;
		}

		if (PROJECT_CLIENT == m_eProjectID)
		{
			m_pwstrFileName[i] = szFullPath;

			TCHAR* pszNextToken = NULL;
			_tcstok_s(szFileName, _T("."), &pszNextToken);

			TCHAR	szNoramlFileName[128] = L"";
			lstrcpy(szNoramlFileName, szFileName);

			ZeroMemory(szFullPath, sizeof(szFullPath));
			lstrcpy(szFullPath, pFilePath);

			lstrcat(szNoramlFileName, L"_n.dds");
			lstrcat(szFullPath, szNoramlFileName);

			if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &m_ppNormalTexture[i])))
			{
				MSG_BOX("StaticMesh's Normal Texture Create Failed");
				return E_FAIL;
			}

			TCHAR	szSpecularFileName[128] = L"";
			lstrcpy(szSpecularFileName, szFileName);

			ZeroMemory(szFullPath, sizeof(szFullPath));
			lstrcpy(szFullPath, pFilePath);

			lstrcat(szSpecularFileName, L"_spec.dds");
			lstrcat(szFullPath, szSpecularFileName);

			if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &m_ppSpecuralTexture[i])))
			{
				MSG_BOX("StaticMesh's Specular Texture Create Failed");
				return E_FAIL;
			}
		}
	}

	return S_OK;
}

void Engine::CStaticMesh::Render_Mesh(void)
{
	for (_ulong i = 0; i < m_dwSubSetCnt; ++i)
	{
		m_pGraphicDev->SetTexture(0, m_ppTextures[i]);
		m_pMesh->DrawSubset(i);
	}
}

void Engine::CStaticMesh::Render_Mesh(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();
		
	for (_ulong i = 0; i < m_dwSubSetCnt; ++i)
	{
		_bool		isAlpha = false;
		_uint		iPassCnt = 0;
		
		if (PROJECT_CLIENT == m_eProjectID)
		{
			if (Engine::MRT_DEFERRED_0 == CRenderer::GetInstance()->GetMRTID())
				iPassCnt = 2;
			else if (Engine::MRT_DEFERRED_1 == CRenderer::GetInstance()->GetMRTID())
				iPassCnt = 0;
			else if (Engine::MRT_DEFERRED_2 == CRenderer::GetInstance()->GetMRTID())
				iPassCnt = 1;
			else if (Engine::MRT_DEFERRED_3 == CRenderer::GetInstance()->GetMRTID())
				iPassCnt = 4;
			else if (Engine::MRT_MINIMAP == CRenderer::GetInstance()->GetMRTID())
				iPassCnt = 3;
			else
				iPassCnt = 0;
		}
		else
		{
			if (isAlpha = Find_Alpha(m_pSubSets[i].pTextureFilename))
				iPassCnt = 1;
		}

		pEffect->BeginPass(iPassCnt);
		
		if (PROJECT_CLIENT == m_eProjectID)
		{			
			//if(isAlpha = Find_Alpha(m_pSubSets[i].pTextureFilename))
			//	iPassCnt = 1;

			//m_pGraphicDev->SetTexture(0, m_ppTextures[i]);
			if (Engine::MRT_DEFERRED_1 == CRenderer::GetInstance()->GetMRTID())
			{
				pEffect->SetTexture("g_BaseTexture", m_ppTextures[i]);
				/*pEffect->SetTexture("g_Normal_Texture", m_ppNormalTexture[i]);
				pEffect->SetTexture("g_Specular_Texture", m_ppSpecuralTexture[i]);*/
			}
			else if (Engine::MRT_DEFERRED_2 == CRenderer::GetInstance()->GetMRTID())
			{
				pEffect->SetTexture("g_Normal_Texture", m_ppNormalTexture[i]);
				pEffect->SetTexture("g_Specular_Texture", m_ppSpecuralTexture[i]);
			}
			else if (Engine::MRT_MINIMAP == CRenderer::GetInstance()->GetMRTID())
			{
				pEffect->SetTexture("g_BaseTexture", m_ppTextures[i]);
			}
			else
			{
				pEffect->SetTexture("g_BaseTexture", m_ppTextures[i]);
			}
		}
		else
		{							
			pEffect->SetTexture("g_BaseTexture", m_ppTextures[i]);
		}

		pEffect->CommitChanges();

		m_pMesh->DrawSubset(i);

		pEffect->EndPass();
	}

	Engine::Safe_Release(pEffect);
}

void CStaticMesh::Render_Mesh(LPD3DXEFFECT pEffect, const _uint iPassIdx)
{
	pEffect->AddRef();

	for (_ulong i = 0; i < m_dwSubSetCnt; ++i)
	{
		switch (iPassIdx)
		{
		case 0:	//MRT_DEFERRED_1
			pEffect->SetTexture("g_BaseTexture", m_ppTextures[i]);
			pEffect->SetTexture("g_Normal_Texture", m_ppNormalTexture[i]);

			if (Engine::SCENE_STAGE_FIRE == Engine::GetSceneID())
			{
				if (L"../Bin/Resource/Mesh/Static/Volsland_UT/C9_Stage_2th_VolslandUT_Terrain_Lava_01.dds" == m_pwstrFileName[i] ||
					L"../Bin/Resource/Mesh/Static/Volsland_UT/c9_bd_dun_ta_ak.dds" == m_pwstrFileName[i])
				{
					pEffect->SetBool("m_gUVAnimation", true);
					pEffect->SetFloat("m_gTime", m_fTime);
					m_pReflectNormalTex->Set_Texture(pEffect, "g_RandNormal_Texture", 0);
				}
			}

			break;
		case 1:	//MRT_DEFERRED_2
			pEffect->SetTexture("g_Normal_Texture", m_ppNormalTexture[i]);
			pEffect->SetTexture("g_Specular_Texture", m_ppSpecuralTexture[i]);

			if (Engine::SCENE_STAGE_FIRE == Engine::GetSceneID())
			{
				if (L"../Bin/Resource/Mesh/Static/Volsland_UT/C9_Stage_2th_VolslandUT_Terrain_Lava_01.dds" == m_pwstrFileName[i] ||
					L"../Bin/Resource/Mesh/Static/Volsland_UT/c9_bd_dun_ta_ak.dds" == m_pwstrFileName[i])
				{
					pEffect->SetBool("m_gUVAnimation", true);
					pEffect->SetFloat("m_gTime", m_fTime);
					m_pReflectNormalTex->Set_Texture(pEffect, "g_RandNormal_Texture", 0);
				}
			}
			break;
		case 3:	//MRT_MINIMAP
			pEffect->SetTexture("g_BaseTexture", m_ppTextures[i]);
			break;
		default:
			break;
		}

		pEffect->CommitChanges();
		m_pMesh->DrawSubset(i);		

		if (Engine::SCENE_STAGE_FIRE == Engine::GetSceneID())
		{
			if (L"../Bin/Resource/Mesh/Static/Volsland_UT/C9_Stage_2th_VolslandUT_Terrain_Lava_01.dds" == m_pwstrFileName[i] ||
				L"../Bin/Resource/Mesh/Static/Volsland_UT/c9_bd_dun_ta_ak.dds" == m_pwstrFileName[i])
			{
				pEffect->SetBool("m_gUVAnimation", false);
			}
		}
	}

	Engine::Safe_Release(pEffect);
}

_bool Engine::CStaticMesh::Find_Alpha(const char* pFileName)
{
	_uint iLength = strlen(pFileName);

	for (_uint i = 0; i < iLength + 1; ++i)
	{
		if(pFileName[i] == '.')
		{
			if(pFileName[i - 1] == 'A')
				return true;
		}
	}
	return false;
}

CStaticMesh* Engine::CStaticMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName)
{
	CStaticMesh*		pInstance = new CStaticMesh(pGraphicDev);

	if(FAILED(pInstance->Ready_Meshes(pFilePath, pFileName)))
	{
		MSG_BOX("CStaticMesh Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

CResource* Engine::CStaticMesh::Clone(void)
{
	return new CStaticMesh(*this);
}

void Engine::CStaticMesh::Free(void)
{	
	Engine::CMesh::Free();

	for (_ulong i = 0; i < m_dwSubSetCnt; ++i)
	{
		Engine::Safe_Release(m_ppTextures[i]);

		if (PROJECT_CLIENT == m_eProjectID)
		{
			Engine::Safe_Release(m_ppNormalTexture[i]);
			Engine::Safe_Release(m_ppSpecuralTexture[i]);
		}
	}

	_ulong dwRefCnt = 0;
	dwRefCnt = Safe_Release(m_pOriMesh);
	dwRefCnt = Safe_Release(m_pMesh);

	if(false == m_bClone)
		Safe_Delete_Array(m_pVtxPos);

	Safe_Release(m_pAdjacency);
	Safe_Release(m_pSubSetBuffer);
	Safe_Delete_Array(m_ppTextures);

	if (PROJECT_CLIENT == m_eProjectID)
	{
		Safe_Delete_Array(m_ppNormalTexture);
		Safe_Delete_Array(m_ppSpecuralTexture);
	}
}

LPD3DXMESH* Engine::CStaticMesh::Get_Mesh( void )
{
	return &m_pMesh;
}


