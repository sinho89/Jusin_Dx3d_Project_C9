#ifndef DynamicMesh_h__
#define DynamicMesh_h__

#include "Mesh.h"

BEGIN(Engine)

class CAnimationCtrl;
class CHierarchyLoader;
class CTexture;
class ENGINE_DLL CDynamicMesh 
	: public CMesh
{
private:
	explicit					 CDynamicMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit					 CDynamicMesh(const CDynamicMesh& rhs);
	virtual 					~CDynamicMesh(void);

public:
	const _matrix*				Get_FrameByName(const char* pFrameName);
	_bool						is_AnimationSetEnd(const _double dMagicNumber = 0.0);
	_uint						Get_MaxAnimationSet(void);
	const char*					Get_AnimationName(const _uint& iIndex);
	_double						Get_TrackPos(void);
	_double						Get_Period(void);
	_double						Get_Period(const _uint& iIndex);
	void						Get_BoneName(vector<const char*>* pBoneList);
	void						Get_BoneName(D3DXFRAME* pFrame, vector<const char*>* pBoneList);
	_bool						IsPlaying_ThisSection(const _double& dStartTime, const _double& dEndTime);

public:
	void						Set_TrackPos(const _double& dPosition);
	void						Set_TrackSpeed(const _float& fSpeed);
	void						Set_AniInfo(const MOTIONINFO* pMotionInfo);
	void						Pause_Animation(const _bool& bIsPlay = false);
public:
	HRESULT						Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName);
	void						Render_Mesh(void);
	void						Render_Mesh(LPD3DXEFFECT pEffect);
	void						Render_BlurMesh(LPD3DXEFFECT pEffect);

	//모션 블러를 위해서 이전 VTF 이미지를 통째로 저장해 주자. 다시 말한다 통째로다. 한 개 두개 안한다. ㅆㅂ
	HRESULT						Save_OldVTFTexture(void);

public: // For.Animation
	_bool						Set_AnimationSet(const _uint& iIdx, const MOTIONINFO* pMotionInfo, _double dMagicNumber);
	void						Set_AnimationSet(const _uint& iIdx);
	void						Play_AnimationSet(const _float& fTimeDelta);
private:
	CHierarchyLoader*			m_pLoader;
	CAnimationCtrl*				m_pAniCtrl;
	D3DXFRAME*					m_pRootFrame;
	//LPD3DXFRAME					m_pRootFrame;
	list<BONEMESH*>				m_MeshContainerList;
	typedef list<BONEMESH*>		MESHCONTAINERLIST;

	list<string>				m_ListBoneName;

	LPDIRECT3DTEXTURE9			m_pVTFTexture;	//행렬 정보를 저장할 텍스쳐.

	vector<LPDIRECT3DTEXTURE9>	m_Vec_OldVTF;		//이전 행렬 정보를 모두 저장할 컨테이너
	_uint						m_iVTFcount;		//이전 행렬 정보 갯수.(텍스쳐 갯수)

	PROJECTID					m_eProjectID;

	_float						m_fTimeDelta;	//모션 블러를 위한 텀.

	_bool						m_bWeaponDrawCheck;
	wstring						m_wstrName;

	//Bardiel 전용
	_bool						m_bBardiel_Weapon_1;
	_bool						m_bBardiel_Weapon_2;
	_bool						m_bBardiel_Armor;

	CTexture*					m_pBardiel_HandGun_Blur_Tex;
	CTexture*					m_pBardiel_Body_Blur_Tex;
	CTexture*					m_pBardiel_Shield_Blur_Tex;

private:
	// 모든 뼈들을 순회하면서 뼈들이 가지고 있는  CombinedTransformationMatrix를 갱신한다.
	void						Update_FrameMatrices(BONE* pFrame, const _matrix* pParentMatrix);
	void						SetUp_FrameMatrixPointer(BONE* pFrame);

public:
	const _float				GetTimeDelta(void) const { return m_fTimeDelta; }
	void						SetTimeDelta(const _float fTime) { m_fTimeDelta = fTime; }
	void						UpdateTimeDelta(const _float fTime);

	const _bool					GetWeaponDrawCheck(void) const { return m_bWeaponDrawCheck; }
	void						SetWeaponDrawCheck(const _bool bWDC) { m_bWeaponDrawCheck = bWDC; }

	const wstring				GetstrName(void) const { return m_wstrName; }
	void						SetstrName(const wstring wstr) { m_wstrName = wstr; }

	const _bool					GetBardiel_Weapon_1_Check(void) const { return m_bBardiel_Weapon_1; }
	void						SetBardiel_Weapon_1_Check(const _bool bWDC) { m_bBardiel_Weapon_1 = bWDC; }

	const _bool					GetBardiel_Weapon_2_Check(void) const { return m_bBardiel_Weapon_2; }
	void						SetBardiel_Weapon_2_Check(const _bool bWDC) { m_bBardiel_Weapon_2 = bWDC; }

	const _bool					GetBardiel_Armor_Check(void) const { return m_bBardiel_Armor; }
	void						SetBardiel_Armor_Check(const _bool bWDC) { m_bBardiel_Armor = bWDC; }

	void						SetBardiel_HandGunBlurTex(CTexture* pTex) { m_pBardiel_HandGun_Blur_Tex = pTex; }
	void						SetBardiel_BodyBlurTex(CTexture* pTex) { m_pBardiel_Body_Blur_Tex = pTex; }
	void						SetBardiel_ShieldBlurTex(CTexture* pTex) { m_pBardiel_Shield_Blur_Tex = pTex; }

public:
	static CDynamicMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName);

public:
	virtual CResource*			Clone(void);

public:
	virtual void				Free(void);
};

END

#endif // DynamicMesh_h__
