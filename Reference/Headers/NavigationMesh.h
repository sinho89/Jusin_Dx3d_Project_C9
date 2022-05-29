#ifndef NavigationMesh_h__
#define NavigationMesh_h__

#include "Mesh.h"

BEGIN(Engine)

// 삼각형들을 여러개 가지고 있을꺼야.
class CCell;
class ENGINE_DLL CNavigationMesh : public CMesh
{
private:
	explicit CNavigationMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNavigationMesh(const CNavigationMesh& rhs);
	virtual ~CNavigationMesh(void);

public:
	void		Set_CurrentCellIndex(const _ulong& dwCurrentIndex);
	_ulong		Get_CurrentCellIndex(_vec3* pPos);
	_ulong		Get_MaxCellIndex(void);
public:
	HRESULT		Ready_NavigationMesh(const _tchar* pFilePath, const _tchar* pFileName);
	void		Render_NaviCell(void);
	_vec3		Move_OnNaviMesh(const _vec3* pTargetPos, const _vec3* pTargetDir, _float fSpeedRate
								, _ulong dwIdx, const _float& fTimeDelta);
	_vec3		Move_OnNaviMesh_Tool( const _vec3* pTargetPos, const _vec3* pTargetDir,
											_float fSpeedRate, _ulong dwIdx);
	_bool		Jump_Check_NaviSide( const _vec3* pTargetPos, _ulong dwIdx );

	void		Add_Cell(const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC, NAVI_TYPE eType);
	vector<Engine::CCell*>*			GetVECCELL(void);

public:			//길찾기하는 함수들..
	void		FindPath(_vec3* pStartPos, _vec3* pEndPos);

	_vec3		AStarMove(_vec3* pPos);
	_vec3*		GetAStarDir(void);


private:
	void        Release_Path(void);		//에이스타용 맵 비우는것..
	_bool		CheckList(_ulong dwIdx);
	void		Add_Node(NODE* pParent, _float fGCost, _float fHCost, _ulong dwIdx, _vec3* pPos);
private:
	HANDLE						m_hThread;
	CRITICAL_SECTION			m_Crt;
	_bool						m_isFinish;

private:	
	vector<CCell*>					m_vecCell;
	typedef vector<CCell*>			VECCELL;

private:
	multimap<float, Engine::NODE*>	m_OpenMap;
	list<_vec3>						m_BestList;

	CCell*							m_pNextCell;
	_vec3							m_vAstarDir;

public: // 이컴포넌트를 사용하는 객체가 존재하는 셀의 인덱스를 보관.
	_ulong							m_dwMaxIdx;      //네비셀 벡터의 최대인덱스
	_ulong							m_dwCurrentIdx;	//MoveOnNaviMesh함수에서 쓰는 현재셀인덱스
	_ulong							m_dwEndIdx;		//시작하는 셀의 인덱스
	_ulong							m_dwTargetIdx;	//목표 셀의 인덱스
	_ulong							m_dwIdx;		//에이스타용 현재 인덱스..

	_int							m_iCount;
public:
	HRESULT Link_Cell(void);
public:
	static CNavigationMesh* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName);
	virtual CResource* Clone(void);
	virtual void Free(void);
};

END


#endif // NavigationMesh_h__
