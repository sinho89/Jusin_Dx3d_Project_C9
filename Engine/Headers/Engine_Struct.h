#ifndef Engine_Struct_h__
#define Engine_Struct_h__

BEGIN(Engine)

//====�ϵ���� ��Ű��===============================================
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

	ID3DXMesh*					OriginalMesh;		//skinning��꿡 ���� ���� Mesh
	LPD3DXBUFFER				pBoneCombinationBuf;

	_ulong						NumAtributeGroups;	//�Ӽ� �׷� ��
	D3DXATTRIBUTERANGE*			AttributeTable;		//�Ӽ� ���̺�

	_matrix**					boneMatrixPtrs;		//Bone ��ȯ ��ĵ��� ������
	_matrix*					boneOffsetMats;		//Bone Offset ���
	_matrix*					currentBoneMats;	//bone���� �ջ�� ��ȯ ��ĵ�

	_ulong						iNumBones;
	_ulong						iPalate;
}BONEMESH;

//====================================================================

//====����Ʈ���� ��Ű��=============================================
typedef struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9*			ppTexture;

	LPD3DXMESH					pOriMesh; // ������ ���¸� ������ �ִ� �޽�(�Һ�)

	_ulong						dwNumBones;
	// ���� �迭�� �Ҵ��ؼ� �������� ����� �����Ѵ�.
	// �ִϸ��̼��� ������ ���·� �������� �Ҷ� ���ʿ� ������� ������ �ִ� ���
	_matrix*					pFrameOffsetMatrix; 
	// ������ ������ �ִ� CombinedTransformationMatrix�� ��� ���ŵǱ� ������, 
	// �ƿ� �� CombinedTransformationMatrix����� �ּҸ� �����ؼ� �Ź� ���ŵ� ����� ������ ���ðž�
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
	_vec3				vPosition; // ��ġ
	_vec3				vTexUV;	
}VTXCUBETEX;

typedef struct tagVertex_CubeColor
{
	_vec3				vPosition; // ��ġ
	_ulong				dwColor;
}VTXCUBECOL;


typedef struct tagVertex_Texture
{
	_vec3				vPosition; // ��ġ
	_vec3				vNormal;
	_vec2				vTexUV; 
}VTXTEX;

typedef struct tagVertex_Color
{
	_vec3				vPosition; // ��ġ
	_ulong				dwColor; // ����
}VTXCOL;

typedef struct tagVertex_Screen
{
	_vec4				vPosition;	//��ġ.
	_vec2				vTexUV;
}VTXSCREEN;

//============== ��ƼŬ =========================================================//
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
	//������ ����

	_vec3 IntP; //�ʱ���ġ
	_vec3 IntV; //�ʱ�ӵ�
	_vec3 IntA; //�ʱ� ���ӵ�


	_vec3 CrnP; //���� ��ġ
	_vec3 CrnV; //���� �ӵ�
	_vec3 CrnA; //���� ���ӵ�

	_vec3 CrnR; //���� ȸ��
	_vec3 CrnRv; //���� ȸ�� �ӵ�

	_vec3 CrnS;  //������

	_float		fElst; //ź��(Elastic) ���
	_float		fDamp; //���� ���� ���

	//������ ������
	_bool		bLive; //Active (Yes/No)
	_float		fLife; //Particle fLife
	_float       fFade; //Fade Speed
	DWORD		dColor; //Color


	//������ ǥ�� ���
	_float		PrsW; //Present Billboard W
	_float		PrsH; //Present Billboard H
	_float		PrsZ; //ī�޶��� z��� ��ġ�� ���� ��
	_float		PrsImg; //Image Index

}TPART;


typedef struct tagSpriteInfo
{
	_int				m_iMaxFrame;		//��������Ʈ �ִ밳��
	_float				m_fSpriteSpeed;		//��������Ʈ ���ǵ�
	_int				m_iRow;				//��������Ʈ ���ΰ���
	_int				m_iColumn;			//��������Ʈ �Ѽ��ΰ��� 

}SPRITEINFO;


typedef struct tagParticleInfo
{
	_int				m_PrtN;				 //��ƼŬ ����
	_int				m_iTextureNumber;	 //�ؽ��� ��ȣ
	_float				m_fTimeAvg;			 //Frame ��� �ð�
	_vec3				m_vStandard;		 //��ƼŬ�� ������ �Ǵ� Position
	_vec3				m_vScale;			 //��ƼŬ�� ũ��
	D3DXCOLOR			m_dColor;			 //��ƼŬ�� ����
	_float				m_fWindPower;		 //��ƼŬ�� �ٶ� ����
	_vec3				m_vWindDir;			 //�ٶ� ���⺤��
	_vec3				m_vTotalWind;		//�ٶ� ����
	_bool				m_bBillBoard;		//������ Ȯ��
	PARTICLE_MOVEMENT	m_eMoveMent;		//��ƼŬ �Ӽ�
	_float				m_fFireArrange;		//����ƼŬ ����
	_vec3				m_vParticleArrange; //��ƼŬ ����
	_int				m_iFade;
	_int				m_iFHeight;			//�� �Ѱ����
	SPRITEINFO			m_tSpriteInfo;		//��������Ʈ
	_int				m_iPath;    		//�����̴� �н�
	bool				m_bRoop;			//������ ���� ����Ʈ���� �ƴ���

	_float				m_fCycloneRadiusInterval; //Cyclone Radius
	_float				m_fCycloneAngleInterval;  //Cyclone Angle
	_float				m_fCycloneHeightInterval; //Cyclone Height


	//==��ü�� ���� ���� ��� ���� ������
	_vec3				m_vParentTranform;        //Transcom ��ġ
	_vec3				m_vParentRotate;	      //Rotate   ��ġ
	_vec3				m_vParentScale;           //Scale    ��ġ
	char				szBoneName[128];          //����     �̸�

}PARTICLEINFO;
//=========================================================================//

typedef struct tagMotionInfo 
{
	_uint iIndex;			//�ִϸ��̼� �ε���
	_float fSpeed;			//�ִϸ��̼� ��� �ӵ�
	_double dStartTime;		//�ִϸ��̼� ���� ��ġ
	_double dEndTime;		//�ִϸ��̼� ������ ��ġ
	_bool bIsLoop;			//�ִϸ��̼� ����?
}MOTIONINFO;

typedef struct tagAnimatorInfo
{
	_uint iNextIndex;		//���� �Ѿ ����
	_double dMagicNumber;	//�������� �Ѿ�� �������� ��ġ?
	_double dCancelTime;	//���� �������� �Ѿ�� ĵ�� ������ �ð�
}ANIMATORINFO;

typedef struct tagMotionEventInfo
{
	_uint iEvent;			//���ۿ� ���� �̺�Ʈ�� �߻���ų����(����, ����Ʈ, �浹ü ��)
	_double dStartTime;		//�̺�Ʈ�� �߻��ؾߵǴ� ����
	_bool bUse;				//�̺�Ʈ�� ��� ����
}MOTIONEVENTINFO;

typedef struct tagAnimationInfo
{
	MOTIONINFO tMotionInfo;					//������ �⺻ ����
	MOTIONEVENTINFO* pMotionEventInfo;		//������ �̺�Ʈ��(�ټ�)
	_bool*	pUse;							//���¸��� ���̺�Ʈ�� ��� ��������?(�ټ�)
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
	COL_INFO eColInfo;		//�浹����(��ġ��,����,�������,�ٿ ��??)
	COL_DIR eColDir;
	_vec3 vDest;			//�浹ü�� ���� �ָ� �ٶ󺸴� ����
	_vec3 vDir;				//�ߵ�ü�� �������ִ¾ְ� �ٶ󺸴º���
	_float fForce;			//�浹 ����
	_float fJumpForce;		//���� ��
	_float fStampForce;		//�����
	WORD dwSkillID;			//��ų enum��
	_int iDamage;			//������
	_bool bCritical;		//ũ��Ƽ�������ƴ���
}COLINFO;

END

#endif // Engine_Sturct_h__
