#ifndef Engine_Struct_h__
#define Engine_Struct_h__

BEGIN(Engine)

//====하드웨어 스키닝===============================================
typedef struct tagBone : public D3DXFRAME
{
	_matrix		CombinedTransformationMatrix;
}BONE;

typedef struct tagBoneMesh : public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9*			ppTexture;				//Diffuse
	wstring						wstrFileName;			//Diffuse Name

	LPDIRECT3DTEXTURE9*			ppNormalTexture;		//Noraml
	wstring						wstrNormalFileName;		//Noraml Name

	LPDIRECT3DTEXTURE9*			ppSpecularTexture;		//Specular
	wstring						wstrSpecularFileName;	//Specular Name

	ID3DXMesh*					OriginalMesh;		//skinning계산에 사용될 참조 Mesh
	LPD3DXBUFFER				pBoneCombinationBuf;

	_ulong						NumAtributeGroups;	//속성 그룹 수
	D3DXATTRIBUTERANGE*			AttributeTable;		//속성 테이블

	_matrix**					boneMatrixPtrs;		//Bone 변환 행렬들의 포인터
	_matrix*					boneOffsetMats;		//Bone Offset 행렬
	_matrix*					currentBoneMats;	//bone들의 합산된 변환 행렬들

	_ulong						iNumBones;
	_ulong						iPalate;
}BONEMESH;

//====================================================================

//====소프트웨어 스키닝=============================================
typedef struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9*			ppTexture;

	LPD3DXMESH					pOriMesh; // 최초의 상태를 가지고 있는 메시(불변)

	_ulong						dwNumBones;
	// 동적 배열로 할당해서 여러개의 행렬을 보관한다.
	// 애니메이션을 포함한 형태로 렌더링을 할때 최초에 뼈대들이 가지고 있는 행렬
	_matrix*					pFrameOffsetMatrix; 
	// 뼈들이 가지고 있는 CombinedTransformationMatrix이 계속 갱신되기 때문에, 
	// 아에 그 CombinedTransformationMatrix행렬의 주소를 보관해서 매번 갱신된 행렬의 정보를 얻어올거야
	_matrix**					ppFrameCombinedMatrix;

	_matrix*					pRenderingMatrices;
			
}D3DXMESHCONTAINER_DERIVED;

typedef struct D3DXFRAME_DERIVED : public D3DXFRAME
{
	_matrix			CombinedTransformationMatrix;
}D3DXFRAME_DERIVED;
//===================================================================

typedef struct tagIndex16
{
	_ushort			_0, _1, _2;
}INDEX16;

typedef struct tagIndex32
{
	_ulong			_0, _1, _2;
}INDEX32;

typedef struct tagVertex_CubeTexture
{
	_vec3				vPosition; // 위치
	_vec3				vTexUV;	
}VTXCUBETEX;

typedef struct tagVertex_CubeColor
{
	_vec3				vPosition; // 위치
	_ulong				dwColor;
}VTXCUBECOL;


typedef struct tagVertex_Texture
{
	_vec3				vPosition; // 위치
	_vec3				vNormal;
	_vec2				vTexUV; 
}VTXTEX;

typedef struct tagVertex_Color
{
	_vec3				vPosition; // 위치
	_ulong				dwColor; // 색상
}VTXCOL;

typedef struct tagVertex_Screen
{
	_vec4				vPosition;	//위치.
	_vec2				vTexUV;
}VTXSCREEN;

//============== 파티클 =========================================================//
typedef struct tagParticle
{
	D3DXVECTOR3	p;
	DWORD		d;
	float		u, v;


	tagParticle() : p(0, 0, 0), u(0), v(0), d(0xFFFFFFFF){}
	tagParticle(float x, float y, float z,
		float u, float v, DWORD d = 0xFFFFFFFF) : p(x, y, z), u(u), v(v), d(d){}

}VTXPARTICLE;

const DWORD VTXFVF_PARTICLE = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;


typedef struct tagPart
{
	//입자의 운동요소

	_vec3 IntP; //초기위치
	_vec3 IntV; //초기속도
	_vec3 IntA; //초기 가속도


	_vec3 CrnP; //현재 위치
	_vec3 CrnV; //현재 속도
	_vec3 CrnA; //현재 가속도

	_vec3 CrnR; //현재 회전
	_vec3 CrnRv; //현재 회전 속도

	_vec3 CrnS;  //스케일

	_float		fElst; //탄성(Elastic) 계수
	_float		fDamp; //공기 저항 계수

	//입자의 생명요소
	_bool		bLive; //Active (Yes/No)
	_float		fLife; //Particle fLife
	_float       fFade; //Fade Speed
	DWORD		dColor; //Color


	//입자의 표현 요소
	_float		PrsW; //Present Billboard W
	_float		PrsH; //Present Billboard H
	_float		PrsZ; //카메라의 z축과 위치의 내적 값
	_float		PrsImg; //Image Index

}TPART;


typedef struct tagSpriteInfo
{
	_int				m_iMaxFrame;		//스프라이트 최대개수
	_float				m_fSpriteSpeed;		//스프라이트 스피드
	_int				m_iRow;				//스프라이트 가로개수
	_int				m_iColumn;			//스프라이트 총세로개수 

}SPRITEINFO;


typedef struct tagParticleInfo
{
	_int				m_PrtN;				 //파티클 개수
	_int				m_iTextureNumber;	 //텍스쳐 번호
	_float				m_fTimeAvg;			 //Frame 평균 시간
	_vec3				m_vStandard;		 //파티클의 기준이 되는 Position
	_vec3				m_vScale;			 //파티클의 크기
	D3DXCOLOR			m_dColor;			 //파티클의 색상
	_float				m_fWindPower;		 //파티클의 바람 세기
	_vec3				m_vWindDir;			 //바람 방향벡터
	_vec3				m_vTotalWind;		//바람 벡터
	_bool				m_bBillBoard;		//빌보드 확인
	PARTICLE_MOVEMENT	m_eMoveMent;		//파티클 속성
	_float				m_fFireArrange;		//불파티클 범위
	_vec3				m_vParticleArrange; //파티클 범위
	_int				m_iFade;
	_int				m_iFHeight;			//불 한계높이
	SPRITEINFO			m_tSpriteInfo;		//스프라이트
	_int				m_iPath;    		//색깔세이더 패스
	bool				m_bRoop;			//루프를 도는 이펙트인지 아닌지

	_float				m_fCycloneRadiusInterval; //Cyclone Radius
	_float				m_fCycloneAngleInterval;  //Cyclone Angle
	_float				m_fCycloneHeightInterval; //Cyclone Height


	//==객체의 본에 붙일 경우등에 사용될 변수들
	_vec3				m_vParentTranform;        //Transcom 위치
	_vec3				m_vParentRotate;	      //Rotate   위치
	_vec3				m_vParentScale;           //Scale    위치
	char				szBoneName[128];          //본의     이름

}PARTICLEINFO;
//=========================================================================//

typedef struct tagMotionInfo 
{
	_uint iIndex;			//애니메이션 인덱스
	_float fSpeed;			//애니메이션 재생 속도
	_double dStartTime;		//애니메이션 시작 위치
	_double dEndTime;		//애니메이션 끝나는 위치
	_bool bIsLoop;			//애니메이션 연속?
}MOTIONINFO;

typedef struct tagAnimatorInfo
{
	_uint iNextIndex;		//다음 넘어갈 동작
	_double dMagicNumber;	//다음으로 넘어갈때 선형보간 수치?
	_double dCancelTime;	//다음 동작으로 넘어갈때 캔슬 가능한 시간
}ANIMATORINFO;

typedef struct tagMotionEventInfo
{
	_uint iEvent;			//동작에 무슨 이벤트를 발생시킬건지(사운드, 이펙트, 충돌체 등)
	_double dStartTime;		//이벤트가 발생해야되는 시점
	_bool bUse;				//이벤트의 사용 여부
}MOTIONEVENTINFO;

typedef struct tagAnimationInfo
{
	MOTIONINFO tMotionInfo;					//동작의 기본 정보
	MOTIONEVENTINFO* pMotionEventInfo;		//가능한 이벤트들(다수)
	_bool*	pUse;							//상태마다 이이벤트를 사용 가능한지?(다수)
}ANIINFO;

typedef struct tagNaviCell
{
	D3DXVECTOR3				vPoint[3];
	Engine::NAVI_TYPE		m_eType;
}NAVIINFO;

typedef struct tagAStarNode
{
	_vec3				vPos;
	_ulong				dwIdx;
	_float				fFCost;
	_float				fGCost;
	_float				fHCost;
	bool				bIsClosed;
	tagAStarNode*		pParent;

	tagAStarNode(void)
	{
		dwIdx		= 0;
		fFCost		= 0.f;
		fGCost		= 0.f;
		fHCost		= 0.f;
		bIsClosed	= false;
		pParent		= NULL;
	}
}NODE;

typedef struct tagCollisionInfo
{
	COL_INFO eColInfo;		//충돌정보(밀치기,띄우기,내려찍기,바운스 등??)
	COL_DIR eColDir;
	_vec3 vDest;			//충돌체가 맞은 애를 바라보는 벡터
	_vec3 vDir;				//중돌체를 가지고있는애가 바라보는벡터
	_float fForce;			//충돌 세기
	_float fJumpForce;		//띄우는 힘
	_float fStampForce;		//찍는힘
	WORD dwSkillID;			//스킬 enum값
	_int iDamage;			//데미지
	_bool bCritical;		//크리티컬인지아닌지
}COLINFO;

END

#endif // Engine_Sturct_h__
