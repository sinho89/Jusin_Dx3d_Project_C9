#ifndef Cell_h__
#define Cell_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CLine;
class CRect_Texture;
class ENGINE_DLL CCell : public CBase
{
public:
	enum POINT {POINT_A, POINT_B, POINT_C, POINT_END};
	enum NEIGHBOR {NEIGHBOR_AB, NEIGHBOR_BC, NEIGHBOR_CA, NEIGHBOR_END};
	enum LINE {LINE_AB, LINE_BC, LINE_CA, LINE_END};
	enum COMPARE {COMPARE_MOVE, COMPARE_NOMOVE};
private:
	explicit CCell(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CCell(void);

public:
	bool				m_bChoosedPoint[POINT_END];
	bool				m_bMovePoint[POINT_END];

public:
	void					SetMovePoint(D3DXVECTOR3* vPoint);
	void					ChoicePoint(void);
	void					SetType(NAVI_TYPE eType);
	void					SetFirstType(void);
	LINE					GetLineType(const _vec3* pEndPos, _ulong* pCellIndex);
	NAVI_TYPE				GetType(void);
	NAVIINFO*				GetNaviInfo(void);
	_vec3					GetCellCenter(void);
	_vec3*					GetLineCenter(void);

public:
	const _vec3* Get_Point(POINT eType) const;
	CCell* Get_Neighbor(NEIGHBOR eType) const;
	const _ulong* Get_Index(void);
public:
	void Set_Neighbor(NEIGHBOR eType, CCell* pNeighbor);
	
public:
	HRESULT Ready_Cell(const _ulong& dwIndex, const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC, NAVI_TYPE eType);
	_bool Compare_Point(const _vec3* pPointA, const _vec3* pPointB, CCell* pCell);
	COMPARE Compare(const _vec3* pEndPos, _ulong* pCellIndex);
	void Render_Cell();

public:
	_ulong				GetIndex(void);
	_float*				GetArrivalCost(void);

private:
	_vec3				m_vPoint[POINT_END];
	_vec3               m_vLineCenter[LINE_END];	//라인의 중점...
	_vec3				m_vCellCenter;				//셀의 중점..
	_float				m_fArrivalCost[LINE_END];	//셀에서 라인의 중점까지의 거리
	CCell*				m_pNeighbor[NEIGHBOR_END];
	CLine*				m_pLine[LINE_END];
	_ulong				m_dwIndex;
	NAVI_TYPE			m_eType;
	NAVI_TYPE			m_eFirstType;
	NAVIINFO			m_tInfo;
private:
	CRect_Texture*		m_pIndexTex;

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	LPD3DXLINE			m_pD3DXLine;


public:
	static CCell* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwIndex, const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC, NAVI_TYPE eType);
	virtual void Free(void);	
};

END

#endif // Cell_h__
