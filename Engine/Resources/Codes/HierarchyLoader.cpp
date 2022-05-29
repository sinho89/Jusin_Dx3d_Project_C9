#include "HierarchyLoader.h"
#include "Resource_Manager.h"

USING(Engine)

Engine::CHierarchyLoader::CHierarchyLoader(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath)
: m_pGraphicDev(pGraphicDev)
, m_pPath(pPath)
{
	m_pGraphicDev->AddRef();
}

Engine::CHierarchyLoader::~CHierarchyLoader(void)
{

}

STDMETHODIMP Engine::CHierarchyLoader::Ready_Loader(void)
{
	m_eProjectID = CResource_Manager::GetInstance()->GetResourceProjectID();

	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::CreateFrame(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	/*D3DXFRAME_DERIVED*				pNewFrame = new D3DXFRAME_DERIVED;
	ZeroMemory(pNewFrame, sizeof(D3DXFRAME_DERIVED));

	Allocate_Name(&pNewFrame->Name, Name);

	pNewFrame->CombinedTransformationMatrix = *D3DXMatrixIdentity(&pNewFrame->TransformationMatrix);

	*ppNewFrame = pNewFrame;*/

	BONE*	newBone = new BONE;
	memset(newBone, NULL, sizeof(BONE));

	//name복사
	if(NULL != Name)
		Allocate_Name(&newBone->Name, Name);

	//변환 행렬들의 초기화
	D3DXMatrixIdentity(&newBone->TransformationMatrix);
	D3DXMatrixIdentity(&newBone->CombinedTransformationMatrix);

	//생성된 Bone을 Return
	*ppNewFrame = (D3DXFRAME*)newBone;

	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::CreateMeshContainer(THIS_ LPCSTR Name
														   , CONST D3DXMESHDATA *pMeshData
														   , CONST D3DXMATERIAL *pMaterials
														   , CONST D3DXEFFECTINSTANCE *pEffectInstances
														   , DWORD NumMaterials
														   , CONST DWORD *pAdjacency
														   , LPD3DXSKININFO pSkinInfo
														   , LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	HRESULT		hr = NULL;

	BONEMESH*			pNewMeshContainer = new BONEMESH;
	ZeroMemory(pNewMeshContainer, sizeof(BONEMESH));

	Allocate_Name(&pNewMeshContainer->Name, Name);	

	pNewMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	LPD3DXMESH			pMesh = pMeshData->pMesh;

	//인접 정보
	_ulong dwNumFaces = pMesh->GetNumFaces();

	pNewMeshContainer->pAdjacency = new _ulong[dwNumFaces * 3];
	memcpy(pNewMeshContainer->pAdjacency, pAdjacency, sizeof(_ulong) * dwNumFaces * 3);

	_ulong dwFVF = pMesh->GetFVF();

	/*pNewMeshContainer->MeshData.pMesh = pMeshData->pMesh;*/

	//노말 벡터
	if(!(dwFVF & D3DFVF_NORMAL))
	{
		hr = pMesh->CloneMeshFVF(pMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pGraphicDev, &pNewMeshContainer->MeshData.pMesh);
		FAILED_CHECK(hr);

		D3DXComputeNormals(pNewMeshContainer->MeshData.pMesh, pNewMeshContainer->pAdjacency);
	}
	else
	{
		hr = pMesh->CloneMeshFVF(pMesh->GetOptions(), dwFVF, m_pGraphicDev, &pNewMeshContainer->MeshData.pMesh);
		FAILED_CHECK(hr);
	}

	//재질 정보
	pNewMeshContainer->NumMaterials = (NumMaterials == 0 ? 1 : NumMaterials);

	pNewMeshContainer->pMaterials = new D3DXMATERIAL[pNewMeshContainer->NumMaterials];	
	ZeroMemory(pNewMeshContainer->pMaterials, sizeof(D3DXMATERIAL) * pNewMeshContainer->NumMaterials);

	pNewMeshContainer->ppTexture = new LPDIRECT3DTEXTURE9[pNewMeshContainer->NumMaterials];
	ZeroMemory(pNewMeshContainer->ppTexture, sizeof(LPDIRECT3DTEXTURE9) * pNewMeshContainer->NumMaterials);

	if (PROJECT_CLIENT == m_eProjectID)
	{
		pNewMeshContainer->ppNormalTexture = new LPDIRECT3DTEXTURE9[pNewMeshContainer->NumMaterials];
		ZeroMemory(pNewMeshContainer->ppNormalTexture, sizeof(LPDIRECT3DTEXTURE9) * pNewMeshContainer->NumMaterials);

		pNewMeshContainer->ppSpecularTexture = new LPDIRECT3DTEXTURE9[pNewMeshContainer->NumMaterials];
		ZeroMemory(pNewMeshContainer->ppSpecularTexture, sizeof(LPDIRECT3DTEXTURE9) * pNewMeshContainer->NumMaterials);
	}

	if(0 != NumMaterials)
	{
		memcpy(pNewMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * pNewMeshContainer->NumMaterials);

		for (_ulong i = 0; i < pNewMeshContainer->NumMaterials; ++i)		
		{
			if(pNewMeshContainer->pMaterials[i].pTextureFilename == NULL)
				continue;

			TCHAR			szFullPath[MAX_PATH] = L"";
			TCHAR			szFileName[128] = L"";

			MultiByteToWideChar(CP_ACP, 0, pNewMeshContainer->pMaterials[i].pTextureFilename, strlen(pNewMeshContainer->pMaterials[i].pTextureFilename)
				, szFileName, 128);

			lstrcpy(szFullPath, m_pPath);
			lstrcat(szFullPath, szFileName);

			pNewMeshContainer->wstrFileName = szFullPath;

			if(FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &pNewMeshContainer->ppTexture[i])))
			{
				MSG_BOX("DynamicMesh`s Texture Create Failed");
				return E_FAIL;
			}

			if (PROJECT_CLIENT == m_eProjectID)
			{
				TCHAR*	pszNextToken = NULL;
				_tcstok_s(szFileName, _T("."), &pszNextToken);

				TCHAR	szNoramlFileName[128] = L"";

				lstrcpy(szNoramlFileName, szFileName);
				ZeroMemory(szFullPath, sizeof(szFullPath));

				lstrcpy(szFullPath, m_pPath);

				lstrcat(szNoramlFileName, L"_n.dds");
				lstrcat(szFullPath, szNoramlFileName);

				pNewMeshContainer->wstrNormalFileName = szFullPath;

				if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &pNewMeshContainer->ppNormalTexture[i])))
				{
					MSG_BOX("DyanamicMesh's Normal Texture Create Failed");
					return E_FAIL;
				}

				TCHAR	szSpecularFileName[128] = L"";

				lstrcpy(szSpecularFileName, szFileName);
				ZeroMemory(szFullPath, sizeof(szFullPath));

				lstrcpy(szFullPath, m_pPath);

				lstrcat(szSpecularFileName, L"_spec.dds");
				lstrcat(szFullPath, szSpecularFileName);

				pNewMeshContainer->wstrSpecularFileName = szFullPath;

				if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &pNewMeshContainer->ppSpecularTexture[i])))
				{
					MSG_BOX("DyanamicMesh's Specular Texture Create Failed");
					return E_FAIL;
				}
			}
		}
	}
	else
	{
		pNewMeshContainer->pMaterials[0].MatD3D.Diffuse = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pNewMeshContainer->pMaterials[0].MatD3D.Specular = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pNewMeshContainer->pMaterials[0].MatD3D.Ambient = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pNewMeshContainer->pMaterials[0].MatD3D.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pNewMeshContainer->pMaterials[0].MatD3D.Power = 0.f;

		pNewMeshContainer->ppTexture[0] = NULL;	
	}

	//스킨 정보
	if(NULL != pSkinInfo)
	{
		pNewMeshContainer->pSkinInfo = pSkinInfo;
		pNewMeshContainer->pSkinInfo->AddRef();

		//pNewMeshContainer->MeshData.pMesh->CloneMeshFVF(pNewMeshContainer->MeshData.pMesh->GetOptions(), pNewMeshContainer->MeshData.pMesh->GetFVF(), m_pGraphicDev, &pNewMeshContainer->pOriMesh);

		// 이 메시의 영향을 미치는 뼈대의 갯수를 얻어온다.
		pNewMeshContainer->iNumBones = pNewMeshContainer->pSkinInfo->GetNumBones();

		pSkinInfo->ConvertToIndexedBlendedMesh(pMeshData->pMesh,
												D3DXMESH_MANAGED | D3DXMESH_WRITEONLY,
												pNewMeshContainer->iNumBones,
												pNewMeshContainer->pAdjacency,
												NULL,
												NULL,
												NULL,
												&pNewMeshContainer->iPalate,
												&pNewMeshContainer->NumAtributeGroups,
												&pNewMeshContainer->pBoneCombinationBuf,
												&pNewMeshContainer->MeshData.pMesh);

		pNewMeshContainer->MeshData.pMesh->GetAttributeTable(NULL, &pNewMeshContainer->NumAtributeGroups);
		pNewMeshContainer->AttributeTable = new D3DXATTRIBUTERANGE[pNewMeshContainer->NumAtributeGroups];
		pNewMeshContainer->MeshData.pMesh->GetAttributeTable(pNewMeshContainer->AttributeTable, NULL);

		pNewMeshContainer->boneOffsetMats = new _matrix[pNewMeshContainer->iNumBones];
		ZeroMemory(pNewMeshContainer->boneOffsetMats, sizeof(_matrix) * pNewMeshContainer->iNumBones);

		/*pNewMeshContainer->boneMatrixPtrs = new _matrix*[pNewMeshContainer->iNumBones];
		ZeroMemory(pNewMeshContainer->boneMatrixPtrs, sizeof(_matrix*) * pNewMeshContainer->iNumBones);*/

		pNewMeshContainer->currentBoneMats = new _matrix[pNewMeshContainer->iNumBones];
		ZeroMemory(pNewMeshContainer->currentBoneMats, sizeof(_matrix) * pNewMeshContainer->iNumBones);

		for	(_ulong i = 0; i < pNewMeshContainer->iNumBones; ++i)		
			pNewMeshContainer->boneOffsetMats[i] = *pNewMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i);
	}
	else
	{
		pNewMeshContainer->pSkinInfo = NULL;
		pNewMeshContainer->boneOffsetMats = NULL;
		pNewMeshContainer->OriginalMesh = NULL;
	}
	
	*ppNewMeshContainer = pNewMeshContainer;

	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
{
	Engine::Safe_Delete_Array(pFrameToFree->Name);

	if(NULL != pFrameToFree->pMeshContainer)
		DestroyMeshContainer(pFrameToFree->pMeshContainer);

	if(NULL != pFrameToFree->pFrameSibling)
		DestroyFrame(pFrameToFree->pFrameSibling);

	if(NULL != pFrameToFree->pFrameFirstChild)
		DestroyFrame(pFrameToFree->pFrameFirstChild);

	Engine::Safe_Delete(pFrameToFree);

	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	BONEMESH*		pMeshContainer = (BONEMESH*)pMeshContainerToFree;

	for (_ulong i = 0; i < pMeshContainer->NumMaterials; ++i)
	{
		Engine::Safe_Release(pMeshContainer->ppTexture[i]);

		if (PROJECT_CLIENT == m_eProjectID)
		{
			Engine::Safe_Release(pMeshContainer->ppNormalTexture[i]);
			Engine::Safe_Release(pMeshContainer->ppSpecularTexture[i]);
		}
	}

	Engine::Safe_Delete_Array(pMeshContainer->ppTexture);

	if (PROJECT_CLIENT == m_eProjectID)
	{
		Engine::Safe_Delete_Array(pMeshContainer->ppNormalTexture);
		Engine::Safe_Delete_Array(pMeshContainer->ppSpecularTexture);
	}

	Engine::Safe_Release(pMeshContainer->OriginalMesh);

	Engine::Safe_Delete_Array(pMeshContainer->boneOffsetMats);
	Engine::Safe_Delete_Array(pMeshContainer->boneMatrixPtrs);
	Engine::Safe_Delete_Array(pMeshContainer->currentBoneMats);

	Engine::Safe_Delete_Array(pMeshContainer->Name);
	Engine::Safe_Release(pMeshContainer->MeshData.pMesh);
	Engine::Safe_Delete_Array(pMeshContainer->pMaterials);
	Engine::Safe_Delete_Array(pMeshContainer->pAdjacency);
	Engine::Safe_Release(pMeshContainer->pSkinInfo);

	Engine::Safe_Delete_Array(pMeshContainer->AttributeTable);
	Engine::Safe_Release(pMeshContainer->pBoneCombinationBuf);	

	Engine::Safe_Delete_Array(pMeshContainer->pMaterials);

	Engine::Safe_Delete(pMeshContainer);	

	return S_OK;
}

void Engine::CHierarchyLoader::Allocate_Name(char** ppName, const char* pFrameName)
{
	if(NULL == pFrameName)
		return;

	_uint		iLength = strlen(pFrameName);

	*ppName = new char[iLength + 1];

	strcpy_s(*ppName, iLength + 1 ,pFrameName);
}

CHierarchyLoader* Engine::CHierarchyLoader::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath)
{
	CHierarchyLoader*		pInstance = new CHierarchyLoader(pGraphicDev, pPath);

	if(FAILED(pInstance->Ready_Loader()))
	{
		MSG_BOX("CHierarchyLoader Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;

}

_ulong Engine::CHierarchyLoader::Release(void)
{
	Engine::Safe_Release(m_pGraphicDev);

	delete this;

	return 0;
}



