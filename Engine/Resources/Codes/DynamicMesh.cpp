#include "DynamicMesh.h"
#include "HierarchyLoader.h"
#include "AnimationCtrl.h"
#include "Renderer.h"
#include "Resource_Manager.h"
#include "Export_Utility.h"
#include "Texture.h"

USING(Engine)

Engine::CDynamicMesh::CDynamicMesh(LPDIRECT3DDEVICE9 pGraphicDev)
: CMesh(pGraphicDev)
, m_pLoader(NULL)
, m_pRootFrame(NULL)
, m_pAniCtrl(NULL)
, m_pVTFTexture(NULL)
, m_iVTFcount(0)
, m_fTimeDelta(0.0f)
, m_bWeaponDrawCheck(FALSE)
, m_bBardiel_Weapon_1(FALSE)
, m_bBardiel_Weapon_2(FALSE)
, m_bBardiel_Armor(FALSE)
, m_wstrName(L"")
{
	m_eProjectID = CResource_Manager::GetInstance()->GetResourceProjectID();
}

Engine::CDynamicMesh::CDynamicMesh(const CDynamicMesh& rhs)
: CMesh(rhs)
, m_pRootFrame(rhs.m_pRootFrame)
, m_MeshContainerList(rhs.m_MeshContainerList)
, m_pLoader(rhs.m_pLoader)
, m_pVTFTexture(rhs.m_pVTFTexture)
, m_iVTFcount(0)
, m_fTimeDelta(0.0f)
, m_bWeaponDrawCheck(FALSE)
, m_bBardiel_Weapon_1(FALSE)
, m_bBardiel_Weapon_2(FALSE)
, m_bBardiel_Armor(FALSE)
, m_wstrName(L"")
{
	m_pAniCtrl = CAnimationCtrl::Create(*(rhs.m_pAniCtrl));
	m_eProjectID = CResource_Manager::GetInstance()->GetResourceProjectID();
}

Engine::CDynamicMesh::~CDynamicMesh(void)
{

}

const _matrix* Engine::CDynamicMesh::Get_FrameByName(const char* pFrameName)
{
	BONE* pFrame = (BONE*)D3DXFrameFind(m_pRootFrame, pFrameName);
	
	return &pFrame->CombinedTransformationMatrix;
}

Engine::_bool Engine::CDynamicMesh::is_AnimationSetEnd(const _double dMagicNumber)
{
	return m_pAniCtrl->is_AnimationSetEnd(dMagicNumber);
}

_uint Engine::CDynamicMesh::Get_MaxAnimationSet(void)
{
	return m_pAniCtrl->Get_MaxAnimationSet();
}

const char* Engine::CDynamicMesh::Get_AnimationName(const _uint& iIndex)
{
	return m_pAniCtrl->Get_AnimationName(iIndex);
}

_double Engine::CDynamicMesh::Get_TrackPos(void)
{
	return m_pAniCtrl->Get_TrackPos();
}

_double Engine::CDynamicMesh::Get_Period(void)
{
	return m_pAniCtrl->Get_Period();
}

_double Engine::CDynamicMesh::Get_Period(const _uint& iIndex)
{
	return m_pAniCtrl->Get_Period(iIndex);
}

void Engine::CDynamicMesh::Get_BoneName(vector<const char*>* pBoneList)
{
	if(pBoneList == NULL)
		return;

	Get_BoneName(m_pRootFrame, pBoneList);
}

void Engine::CDynamicMesh::Get_BoneName(D3DXFRAME* pFrame, vector<const char*>* pBoneList)
{
	if(pFrame != NULL)
		pBoneList->push_back(pFrame->Name);

	if(pFrame->pFrameFirstChild != NULL)
		Get_BoneName(pFrame->pFrameFirstChild, pBoneList);
	if(pFrame->pFrameSibling != NULL)
		Get_BoneName(pFrame->pFrameSibling, pBoneList);
}

_bool Engine::CDynamicMesh::IsPlaying_ThisSection(const _double & dStartTime, const _double & dEndTime)
{
	return m_pAniCtrl->IsPlaying_ThisSection(dStartTime, dEndTime);
}

void Engine::CDynamicMesh::Set_TrackPos(const _double& dPosition)
{
	m_pAniCtrl->Set_TrackPos(dPosition);
}

void Engine::CDynamicMesh::Set_TrackSpeed(const _float& fSpeed)
{
	m_pAniCtrl->Set_TrackSpeed(fSpeed);
}

void Engine::CDynamicMesh::Set_AniInfo(const MOTIONINFO* pMotionInfo)
{
	m_pAniCtrl->Set_AniInfo(pMotionInfo);
}

void Engine::CDynamicMesh::Pause_Animation(const _bool & bIsPlay)
{
	m_pAniCtrl->Pause_Animation(bIsPlay);
}

HRESULT Engine::CDynamicMesh::Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName)
{
	_tchar			szFullPath[128] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	m_pLoader = CHierarchyLoader::Create(m_pGraphicDev, pFilePath);
	if(NULL == m_pLoader)
		return E_FAIL;		


	LPD3DXANIMATIONCONTROLLER			pAniCtrl = NULL;
	
	if(FAILED(D3DXLoadMeshHierarchyFromX(szFullPath, 
										 D3DXMESH_MANAGED, 
										 m_pGraphicDev, 
										 m_pLoader, 
										 NULL, 
										 &m_pRootFrame, 
										 &pAniCtrl)))
		return E_FAIL;

	m_pAniCtrl = CAnimationCtrl::Create(pAniCtrl);
	if(NULL == m_pLoader)
		return E_FAIL;	

	Engine::Safe_Release(pAniCtrl);

	_matrix				matTmp;	

	Update_FrameMatrices((BONE*)m_pRootFrame, &matTmp);
	SetUp_FrameMatrixPointer((BONE*)m_pRootFrame);

	HRESULT hr = D3DXCreateTexture(m_pGraphicDev
									, 48
									, 48
									, 1
									, 0
									, D3DFMT_A32B32G32R32F
									, D3DPOOL_MANAGED
									, &m_pVTFTexture);
	FAILED_CHECK(hr);

	
	//m_ListBoneName;
	//int i=0; 

	m_bWeaponDrawCheck = FALSE;
	m_wstrName = L"";

	return S_OK;
}

void Engine::CDynamicMesh::Render_Mesh(void)
{
	MESHCONTAINERLIST::iterator	iter = m_MeshContainerList.begin();
	MESHCONTAINERLIST::iterator	iter_end = m_MeshContainerList.end();

	for (; iter != iter_end; ++iter)
	{
		BONEMESH*	pMeshContainer = (*iter);

		for (_ulong i = 0; i < pMeshContainer->iNumBones; ++i)
			pMeshContainer->currentBoneMats[i] = pMeshContainer->boneOffsetMats[i] * *pMeshContainer->boneMatrixPtrs[i];


		void*		pSrcVertices = NULL;
		void*		pDestVertices = NULL;

		pMeshContainer->OriginalMesh->LockVertexBuffer(0, &pSrcVertices);
		pMeshContainer->MeshData.pMesh->LockVertexBuffer(0, &pDestVertices);

		pMeshContainer->pSkinInfo->UpdateSkinnedMesh(pMeshContainer->currentBoneMats, NULL, pSrcVertices, pDestVertices);

		for (_ulong i = 0; i < pMeshContainer->NumMaterials; ++i)
		{
			m_pGraphicDev->SetTexture(0, pMeshContainer->ppTexture[i]);
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}		

		pMeshContainer->OriginalMesh->UnlockVertexBuffer();
		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();
	}
}

void Engine::CDynamicMesh::Render_Mesh(LPD3DXEFFECT pEffect)
{
	MESHCONTAINERLIST::iterator	iter = m_MeshContainerList.begin();
	MESHCONTAINERLIST::iterator	iter_end = m_MeshContainerList.end();
		
	for (; iter != iter_end; ++iter)
	{
		BONEMESH*	pMeshContainer = (*iter);

		if(NULL != pMeshContainer->pSkinInfo)
		{
			//bone Combination Table Get
			LPD3DXBONECOMBINATION pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());

			//모든 속성 그룹에 대해-
			for(_int i=0; i<(_int)pMeshContainer->NumAtributeGroups; ++i)
			{
				_uint  iIndex = 0;	//텍스쳐의 인덱스를 변수.

				//Bone Combination Table로부터 bone들의 ID를 얻고, Matrix Palete를 구성
				for(_ulong iPaletteEntry = 0; iPaletteEntry < pMeshContainer->iNumBones; ++iPaletteEntry)
				{
					_int iMatIndex = pBoneComb[i].BoneId[iPaletteEntry];
					if(UINT_MAX != iMatIndex)
					{
						D3DXMatrixMultiply(&pMeshContainer->currentBoneMats[iPaletteEntry],
							&pMeshContainer->boneOffsetMats[iMatIndex],
							pMeshContainer->boneMatrixPtrs[iMatIndex]);

						D3DLOCKED_RECT		LockRect;
						m_pVTFTexture->LockRect(0, &LockRect, NULL, 0);

						_vec4 pVec4 = _vec4(0.0f, 0.0f, 0.0f, 0.0f);

						memcpy(&pVec4, &pMeshContainer->currentBoneMats[iPaletteEntry].m[0][0], sizeof(_vec4));
						((_vec4*)LockRect.pBits)[iIndex] = pVec4;
						iIndex += 1;

						memcpy(&pVec4, &pMeshContainer->currentBoneMats[iPaletteEntry].m[1][0], sizeof(_vec4));
						((_vec4*)LockRect.pBits)[iIndex] = pVec4;
						iIndex += 1;

						memcpy(&pVec4, &pMeshContainer->currentBoneMats[iPaletteEntry].m[2][0], sizeof(_vec4));
						((_vec4*)LockRect.pBits)[iIndex] = pVec4;
						iIndex += 1;

						memcpy(&pVec4, &pMeshContainer->currentBoneMats[iPaletteEntry].m[3][0], sizeof(_vec4));
						((_vec4*)LockRect.pBits)[iIndex] = pVec4;
						iIndex += 1;

						m_pVTFTexture->UnlockRect(0);
					}
				}

				pEffect->AddRef();

				//pEffect->SetInt("numFrameInfluences", pMeshContainer->iPalate);
				//pEffect->SetMatrixArray("MatrixPalette", pMeshContainer->currentBoneMats, pMeshContainer->iNumBones);

				_int mtrlIndex = pBoneComb[i].AttribId;
				m_pGraphicDev->SetMaterial(&(pMeshContainer->pMaterials[mtrlIndex].MatD3D));
				//m_pGraphicDev->SetTexture(0, pMeshContainer->ppTexture[mtrlIndex]);

				//pEffect->SetInt("iVTF_Index", pMeshContainer->iNumBones);
				pEffect->SetTexture("g_VTF_Texture", m_pVTFTexture);

				if(PROJECT_CLIENT == m_eProjectID)
				{
					if (Engine::MRT_DEFERRED_1 == CRenderer::GetInstance()->GetMRTID())
					{
						pEffect->SetTexture("g_BaseTexture", pMeshContainer->ppTexture[i]);				//Diffuse Texture
						pEffect->SetTexture("g_Normal_Texture", pMeshContainer->ppNormalTexture[i]);

						if (pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/Select_Character/WitchBlade/pw0028_C_hel_hair.dds" ||
							pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/Select_Character/Mistic/ps0022_C_hair.dds" ||
							pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/Select_Character/Fighter/pk0028_C_hair.dds" ||
							pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/Select_Character/Hunter/ph0028_C_hel_hair.dds" ||
							pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/Select_Character/Shaman/pm0003_hair.dds" ||
							pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/Player_WitchBlade/pw0028_C_hel_hair.dds" ||
							pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/Player_Mistic/ps0022_C_hair.dds" ||
							pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/HanumanBoss/m10002.dds" ||
							pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/Monster/Knole_Commander/m00600.dds" ||
							pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/Monster/Knole_Commander/m00601.dds")
						{
							m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x0000020);
							m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
							m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
							//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
							//m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
							//m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
						}

						if (L"Mesh_Knole_Commander" == m_wstrName)
						{
							if (m_bWeaponDrawCheck)
							{
								if (pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/Monster/Knole_Commander/m00600_1Swo_2.dds")
								{
									continue;
								}
							}
							else
							{
								if (pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/Monster/Knole_Commander/m00600_1Swo_1.dds")
								{
									continue;
								}
							}
						}
						else if (L"Mesh_Bardiel" == m_wstrName)
						{
							if (!m_bBardiel_Armor)
							{
								if (pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/BardielBoss/m13602.dds")
								{
									continue;
								}
							}

							if (!m_bBardiel_Weapon_1)
							{
								if (pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/BardielBoss/m13603.dds")
								{
									continue;
								}
							}

							if (!m_bBardiel_Weapon_2)
							{
								if (pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/BardielBoss/m13604.dds")
								{
									continue;
								}
							}							
						}						
					}
					else if (Engine::MRT_DEFERRED_2 == CRenderer::GetInstance()->GetMRTID())
					{
						pEffect->SetTexture("g_Normal_Texture", pMeshContainer->ppNormalTexture[i]);	//Noraml Texture
						pEffect->SetTexture("g_Specular_Texture", pMeshContainer->ppSpecularTexture[i]);//Specular Texture

						if (pMeshContainer->wstrNormalFileName == L"../Bin/Resource/Mesh/Dynamic/Select_Character/WitchBlade/pw0028_C_hel_hair_n.dds" ||
							pMeshContainer->wstrNormalFileName == L"../Bin/Resource/Mesh/Dynamic/Select_Character/Mistic/ps0022_C_hair_n.dds" ||
							pMeshContainer->wstrNormalFileName == L"../Bin/Resource/Mesh/Dynamic/Select_Character/Fighter/pk0028_C_hair_n.dds" ||
							pMeshContainer->wstrNormalFileName == L"../Bin/Resource/Mesh/Dynamic/Select_Character/Hunter/ph0028_C_hel_hair_n.dds" ||
							pMeshContainer->wstrNormalFileName == L"../Bin/Resource/Mesh/Dynamic/Select_Character/Shaman/pm0003_hair_n.dds" ||
							pMeshContainer->wstrNormalFileName == L"../Bin/Resource/Mesh/Dynamic/Player_WitchBlade/pw0028_C_hel_hair_n.dds" ||
							pMeshContainer->wstrNormalFileName == L"../Bin/Resource/Mesh/Dynamic/Player_Mistic/ps0022_C_hair_n.dds" ||
							pMeshContainer->wstrNormalFileName == L"../Bin/Resource/Mesh/Dynamic/HanumanBoss/m10002_n.dds" ||
							pMeshContainer->wstrNormalFileName == L"../Bin/Resource/Mesh/Dynamic/Monster/Knole_Commander/m00600_n.dds" ||
							pMeshContainer->wstrNormalFileName == L"../Bin/Resource/Mesh/Dynamic/Monster/Knole_Commander/m00601_n.dds")
						{
							m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x0000020);
							m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
							m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
							pEffect->SetBool("g_Alphatest", true);
							pEffect->SetTexture("g_Albedo_Texture", pMeshContainer->ppTexture[i]);
							//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
							//m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
							//m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
						}

						
						if (L"Mesh_Knole_Commander" == m_wstrName)
						{
							if (m_bWeaponDrawCheck)
							{
								if (pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/Monster/Knole_Commander/m00600_1Swo_2.dds")
								{
									continue;
								}
							}
							else
							{
								if (pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/Monster/Knole_Commander/m00600_1Swo_1.dds")
								{
									continue;
								}
							}
						}
						else if (L"Mesh_Bardiel" == m_wstrName)
						{
							if (!m_bBardiel_Armor)
							{
								if (pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/BardielBoss/m13602.dds")
								{
									continue;
								}
							}

							if (!m_bBardiel_Weapon_1)
							{
								if (pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/BardielBoss/m13603.dds")
								{
									continue;
								}
							}

							if (!m_bBardiel_Weapon_2)
							{
								if (pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/BardielBoss/m13604.dds")
								{
									continue;
								}
							}
						}						
					}
					else if (Engine::MRT_DEFERRED_4 == CRenderer::GetInstance()->GetMRTID())
					{
						pEffect->SetTexture("g_BaseTexture", pMeshContainer->ppTexture[i]);
					}
					else if (Engine::MRT_POSTEFFECT == CRenderer::GetInstance()->GetMRTID())
					{
						if (pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/HanumanBoss/m10002.dds")
						{
							continue;
						}
					}
					else if (Engine::MRT_POSTEFFECT_BLUR == CRenderer::GetInstance()->GetMRTID())
					{
						if (pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/HanumanBoss/m10002.dds")
						{
							continue;
						}

						//Bardiel-----------------
						if (L"Mesh_Bardiel" == m_wstrName)
						{
							if (m_bBardiel_Armor)
							{
								if (pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/BardielBoss/m13602.dds")
								{
									m_pBardiel_Shield_Blur_Tex->Set_Texture(pEffect, "g_BlurTexture");
								}
							}
							else
							{
								if (pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/BardielBoss/m13602.dds")
								{
									continue;
								}

								if (pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/BardielBoss/m13601.dds")
								{
									m_pBardiel_Body_Blur_Tex->Set_Texture(pEffect, "g_BlurTexture");
								}
							}

							if (m_bBardiel_Weapon_1)
							{
								if (pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/BardielBoss/m13603.dds")
								{
									m_pBardiel_HandGun_Blur_Tex->Set_Texture(pEffect, "g_BlurTexture");
								}
							}
							else
							{
								if (pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/BardielBoss/m13603.dds")
								{
									continue;
								}
							}

							if (pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/BardielBoss/m13604.dds")
							{
								continue;
							}
						}						
						//------------------------
					}
					else if (Engine::MRT_DRAW_DYNAMIC_SHADOWMAP == CRenderer::GetInstance()->GetMRTID())
					{
						if (L"Mesh_Bardiel" == m_wstrName)
						{
							if (!m_bBardiel_Armor)
							{
								if (pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/BardielBoss/m13602.dds")
								{
									continue;
								}
							}

							if (!m_bBardiel_Weapon_1)
							{
								if (pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/BardielBoss/m13603.dds")
								{
									continue;
								}
							}

							if (!m_bBardiel_Weapon_2)
							{
								if (pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/BardielBoss/m13604.dds")
								{
									continue;
								}
							}
						}
					}
					else
					{
						pEffect->SetTexture("g_BaseTexture", pMeshContainer->ppTexture[i]);				//Diffuse Texture
						pEffect->SetTexture("g_Normal_Texture", pMeshContainer->ppNormalTexture[i]);	//Noraml Texture
						pEffect->SetTexture("g_Specular_Texture", pMeshContainer->ppSpecularTexture[i]);//Specular Texture
					}
				}
				else
				{
					pEffect->SetTexture("g_BaseTexture", pMeshContainer->ppTexture[i]);
				}
				
				pEffect->CommitChanges();

				pMeshContainer->MeshData.pMesh->DrawSubset(i);


				if (pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/Select_Character/WitchBlade/pw0028_C_hel_hair.dds" ||
					pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/Select_Character/Mistic/ps0022_C_hair.dds" ||
					pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/Select_Character/Fighter/pk0028_C_hair.dds" ||
					pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/Select_Character/Hunter/ph0028_C_hel_hair.dds" ||
					pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/Select_Character/Shaman/pm0003_hair.dds" ||
					pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/Player_WitchBlade/pw0028_C_hel_hair.dds" ||
					pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/Player_Mistic/ps0022_C_hair.dds" ||
					pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/HanumanBoss/m10002.dds" ||
					pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/Monster/Knole_Commander/m00600.dds" ||
					pMeshContainer->wstrFileName == L"../Bin/Resource/Mesh/Dynamic/Monster/Knole_Commander/m00601.dds" ||
					pMeshContainer->wstrNormalFileName == L"../Bin/Resource/Mesh/Dynamic/Select_Character/WitchBlade/pw0028_C_hel_hair_n.dds" ||
					pMeshContainer->wstrNormalFileName == L"../Bin/Resource/Mesh/Dynamic/Select_Character/Mistic/ps0022_C_hair_n.dds" ||
					pMeshContainer->wstrNormalFileName == L"../Bin/Resource/Mesh/Dynamic/Select_Character/Fighter/pk0028_C_hair_n.dds" ||
					pMeshContainer->wstrNormalFileName == L"../Bin/Resource/Mesh/Dynamic/Select_Character/Hunter/ph0028_C_hel_hair_n.dds" ||
					pMeshContainer->wstrNormalFileName == L"../Bin/Resource/Mesh/Dynamic/Select_Character/Shaman/pm0003_hair_n.dds" ||
					pMeshContainer->wstrNormalFileName == L"../Bin/Resource/Mesh/Dynamic/Player_WitchBlade/pw0028_C_hel_hair_n.dds" ||
					pMeshContainer->wstrNormalFileName == L"../Bin/Resource/Mesh/Dynamic/Player_Mistic/ps0022_C_hair_n.dds" ||
					pMeshContainer->wstrNormalFileName == L"../Bin/Resource/Mesh/Dynamic/HanumanBoss/m10002_n.dds" ||
					pMeshContainer->wstrNormalFileName == L"../Bin/Resource/Mesh/Dynamic/Monster/Knole_Commander/m00600_n.dds" ||
					pMeshContainer->wstrNormalFileName == L"../Bin/Resource/Mesh/Dynamic/Monster/Knole_Commander/m00601_n.dds")
				{
					m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x0000020);
					m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
					m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
					pEffect->SetBool("g_Alphatest", false);
					//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
				}

				Engine::Safe_Release(pEffect);
			}
		}
	}
}

void Engine::CDynamicMesh::Render_BlurMesh(LPD3DXEFFECT pEffect)
{
	MESHCONTAINERLIST::iterator	iter = m_MeshContainerList.begin();
	MESHCONTAINERLIST::iterator	iter_end = m_MeshContainerList.end();

	m_iVTFcount = 0;

	for (; iter != iter_end; ++iter)
	{
		BONEMESH*	pMeshContainer = (*iter);

		if (NULL != pMeshContainer->pSkinInfo)
		{
			//bone Combination Table Get
			LPD3DXBONECOMBINATION pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());

			//모든 속성 그룹에 대해-
			for (_int i = 0; i<(_int)pMeshContainer->NumAtributeGroups; ++i)
			{
				_uint  iIndex = 0;	//텍스쳐의 인덱스를 변수.

									//Bone Combination Table로부터 bone	 ID를 얻고, Matrix Palete를 구성
				for (_ulong iPaletteEntry = 0; iPaletteEntry < pMeshContainer->iNumBones; ++iPaletteEntry)
				{
					_int iMatIndex = pBoneComb[i].BoneId[iPaletteEntry];
					if (UINT_MAX != iMatIndex)
					{
						D3DXMatrixMultiply(&pMeshContainer->currentBoneMats[iPaletteEntry],
							&pMeshContainer->boneOffsetMats[iMatIndex],
							pMeshContainer->boneMatrixPtrs[iMatIndex]);

						D3DLOCKED_RECT		LockRect;
						m_pVTFTexture->LockRect(0, &LockRect, NULL, 0);

						_vec4 pVec4 = _vec4(0.0f, 0.0f, 0.0f, 0.0f);

						memcpy(&pVec4, &pMeshContainer->currentBoneMats[iPaletteEntry].m[0][0], sizeof(_vec4));
						((_vec4*)LockRect.pBits)[iIndex] = pVec4;
						iIndex += 1;

						memcpy(&pVec4, &pMeshContainer->currentBoneMats[iPaletteEntry].m[1][0], sizeof(_vec4));
						((_vec4*)LockRect.pBits)[iIndex] = pVec4;
						iIndex += 1;

						memcpy(&pVec4, &pMeshContainer->currentBoneMats[iPaletteEntry].m[2][0], sizeof(_vec4));
						((_vec4*)LockRect.pBits)[iIndex] = pVec4;
						iIndex += 1;

						memcpy(&pVec4, &pMeshContainer->currentBoneMats[iPaletteEntry].m[3][0], sizeof(_vec4));
						((_vec4*)LockRect.pBits)[iIndex] = pVec4;
						iIndex += 1;

						m_pVTFTexture->UnlockRect(0);
					}
				}

				pEffect->AddRef();

				_int mtrlIndex = pBoneComb[i].AttribId;
				m_pGraphicDev->SetMaterial(&(pMeshContainer->pMaterials[mtrlIndex].MatD3D));

				pEffect->SetTexture("g_VTF_Texture", m_pVTFTexture);

				//pEffect->SetTexture("g_BaseTexture", pMeshContainer->ppTexture[i]);				//Diffuse Texture
				//pEffect->SetTexture("g_Normal_Texture", pMeshContainer->ppNormalTexture[i]);	//Noraml Texture
				//pEffect->SetTexture("g_Specular_Texture", pMeshContainer->ppSpecularTexture[i]);//Specular Texture

				if (0 < m_Vec_OldVTF.size())
					pEffect->SetTexture("g_VTF_OldTexture", m_Vec_OldVTF[m_iVTFcount++]);

				pEffect->CommitChanges();

				pMeshContainer->MeshData.pMesh->DrawSubset(i);

				Engine::Safe_Release(pEffect);
			}
		}
	}

	if (0.07f <= m_fTimeDelta)
	{
		if (FAILED(Save_OldVTFTexture()))
		{
			MSG_BOX("Faild Save VTF Old Texture");
			return;
		}

		m_fTimeDelta = 0.0f;
	}
}

HRESULT Engine::CDynamicMesh::Save_OldVTFTexture(void)
{
	for (_uint i = 0; i<m_Vec_OldVTF.size(); ++i)
	{
		Engine::Safe_Release(m_Vec_OldVTF[i]);
	}
	m_Vec_OldVTF.clear();

	MESHCONTAINERLIST::iterator	iter = m_MeshContainerList.begin();
	MESHCONTAINERLIST::iterator	iter_end = m_MeshContainerList.end();

	for (; iter != iter_end; ++iter)
	{
		BONEMESH*	pMeshContainer = (*iter);

		if (NULL != pMeshContainer->pSkinInfo)
		{
			//bone Combination Table Get
			LPD3DXBONECOMBINATION pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());

			//모든 속성 그룹에 대해-
			for (_int i = 0; i<(_int)pMeshContainer->NumAtributeGroups; ++i)
			{
				LPDIRECT3DTEXTURE9	pVTFTexture;
				HRESULT hr = D3DXCreateTexture(m_pGraphicDev
					, 48
					, 48
					, 1
					, 0
					, D3DFMT_A32B32G32R32F
					, D3DPOOL_MANAGED
					, &pVTFTexture);
				FAILED_CHECK(hr);

				_uint  iIndex = 0;	//텍스쳐의 인덱스를 변수.

									//Bone Combination Table로부터 bone	 ID를 얻고, Matrix Palete를 구성
				for (_ulong iPaletteEntry = 0; iPaletteEntry < pMeshContainer->iNumBones; ++iPaletteEntry)
				{
					_int iMatIndex = pBoneComb[i].BoneId[iPaletteEntry];
					if (UINT_MAX != iMatIndex)
					{
						D3DXMatrixMultiply(&pMeshContainer->currentBoneMats[iPaletteEntry],
							&pMeshContainer->boneOffsetMats[iMatIndex],
							pMeshContainer->boneMatrixPtrs[iMatIndex]);

						D3DLOCKED_RECT		LockRect;
						pVTFTexture->LockRect(0, &LockRect, NULL, 0);

						_vec4 pVec4 = _vec4(0.0f, 0.0f, 0.0f, 0.0f);

						memcpy(&pVec4, &pMeshContainer->currentBoneMats[iPaletteEntry].m[0][0], sizeof(_vec4));
						((_vec4*)LockRect.pBits)[iIndex] = pVec4;
						iIndex += 1;

						memcpy(&pVec4, &pMeshContainer->currentBoneMats[iPaletteEntry].m[1][0], sizeof(_vec4));
						((_vec4*)LockRect.pBits)[iIndex] = pVec4;
						iIndex += 1;

						memcpy(&pVec4, &pMeshContainer->currentBoneMats[iPaletteEntry].m[2][0], sizeof(_vec4));
						((_vec4*)LockRect.pBits)[iIndex] = pVec4;
						iIndex += 1;

						memcpy(&pVec4, &pMeshContainer->currentBoneMats[iPaletteEntry].m[3][0], sizeof(_vec4));
						((_vec4*)LockRect.pBits)[iIndex] = pVec4;
						iIndex += 1;

						pVTFTexture->UnlockRect(0);
					}
				}

				m_Vec_OldVTF.push_back(pVTFTexture);
			}
		}
	}

	return S_OK;
}

_bool Engine::CDynamicMesh::Set_AnimationSet(const _uint& iIdx, const MOTIONINFO* pMotionInfo, _double dMagicNumber)
{
	return m_pAniCtrl->Set_AnimationSet(iIdx, pMotionInfo, dMagicNumber);
}

void Engine::CDynamicMesh::Set_AnimationSet(const _uint& iIdx)
{
	m_pAniCtrl->Set_AnimationSet(iIdx);
}

void Engine::CDynamicMesh::Play_AnimationSet(const _float& fTimeDelta)
{
	m_pAniCtrl->Play_Animation(fTimeDelta);

	_matrix				matTmp;	
	D3DXMatrixIdentity(&matTmp);

	Update_FrameMatrices((BONE*)m_pRootFrame, &matTmp);
}

void Engine::CDynamicMesh::Update_FrameMatrices(BONE* pFrame, const _matrix* pParentMatrix)
{
	if(NULL == pFrame)	
		return;

	pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix * *pParentMatrix;

	if(NULL != pFrame->pFrameSibling)
		Update_FrameMatrices(((BONE*)pFrame->pFrameSibling), pParentMatrix);

	if(NULL != pFrame->pFrameFirstChild)
		Update_FrameMatrices(((BONE*)pFrame->pFrameFirstChild), &pFrame->CombinedTransformationMatrix);
}

void Engine::CDynamicMesh::SetUp_FrameMatrixPointer(BONE* pFrame)
{
	if(NULL != pFrame->pMeshContainer)
	{
		BONEMESH*	pMeshContainer = (BONEMESH*)pFrame->pMeshContainer;

		//Skinded Mesh를 가진 Bone들에 대해, Matrix Pointer를 설정
		if(NULL != pMeshContainer->pSkinInfo)
		{
			//Mesh와 link된 bone들의 수를 얻자.
			pMeshContainer->iNumBones = pMeshContainer->pSkinInfo->GetNumBones();
			//bone의 수만큼 Pointer 배열을 만들자.
			pMeshContainer->boneMatrixPtrs = new _matrix*[pMeshContainer->iNumBones];

			for (_ulong i = 0; i < pMeshContainer->iNumBones; ++i)
			{
				//링크된 본을 이름으로 찾자
				BONE* b = (BONE*)D3DXFrameFind(m_pRootFrame, pMeshContainer->pSkinInfo->GetBoneName(i));

				//m_ListBoneName.push_back(pMeshContainer->pSkinInfo->GetBoneName(i));

				//그 본의 결합된 변환 행렬의 포인터를 배열에 저장
				if(NULL != b)
					pMeshContainer->boneMatrixPtrs[i] = &b->CombinedTransformationMatrix;
				else
					pMeshContainer->boneMatrixPtrs[i] = NULL;
			}
		}

		m_MeshContainerList.push_back(pMeshContainer);
	}

	if(NULL != pFrame->pFrameSibling)
		SetUp_FrameMatrixPointer((BONE*)pFrame->pFrameSibling);

	if(NULL != pFrame->pFrameFirstChild)
		SetUp_FrameMatrixPointer((BONE*)pFrame->pFrameFirstChild);
}

void Engine::CDynamicMesh::UpdateTimeDelta(const _float fTime)
{
	m_fTimeDelta += fTime;
}

Engine::CDynamicMesh* Engine::CDynamicMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName)
{
	CDynamicMesh*		pInstance = new CDynamicMesh(pGraphicDev);

	if(FAILED(pInstance->Ready_Meshes(pFilePath, pFileName)))
	{
		MSG_BOX("CDynamicMesh Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

CResource* Engine::CDynamicMesh::Clone(void)
{
	return new CDynamicMesh(*this);
}

void Engine::CDynamicMesh::Free(void)
{
	D3DXSaveTextureToFile(L"../bin/SaveTexture.bmp", D3DXIFF_BMP, m_pVTFTexture, NULL);
	
	Engine::CMesh::Free();

	Engine::Safe_Release(m_pAniCtrl);
	
	if(false == m_bClone)
	{
		m_pLoader->DestroyFrame(m_pRootFrame);
		Engine::Safe_Release(m_pLoader);	
		Engine::Safe_Release(m_pVTFTexture);

		for (_uint i = 0; i<m_Vec_OldVTF.size(); ++i)
		{
			Engine::Safe_Release(m_Vec_OldVTF[i]);
		}
		m_Vec_OldVTF.clear();
	}
	
	m_MeshContainerList.clear();
}