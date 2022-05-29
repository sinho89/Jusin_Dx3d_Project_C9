#include "stdafx.h"
#include "UI.h"
#include "Player.h"
#include "Cursor.h"

CUI::CUI(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
:Engine::CGameObject(pGraphicDev, wstrName, eState, bIsActive)
, m_fAlpha(0.f)
, m_fAlphaTime(0.f)
, m_bAlpha_Increase(false)
, m_bCollision(false)
, m_bClick(false)
, m_bDoubleClick(false)
, m_bSkipScene(false)
, m_bCreate(false)
, m_bSelect(false)
, m_bRender(true)
, m_bMove(false)
, m_bBarCollision(false)
, m_bButtonCollision(false)
, m_bWindowProcess(false)
, m_bSheetSelect(false)
, m_bBlockPick(false)
, m_bMousePick(false)
, m_bToolTipOn(false)
, m_bKeyBoard(false)
, m_bWear(false)
, m_bMain(false)
, m_vFontPos(0.f, 0.f)
, m_vUvCut(0.f, 0.f)
, m_pCalCom(NULL)
, m_iIndex(0)
, m_iBeforeIndex(0)
, m_fTermX(0.f)
, m_fTermY(0.f)
, m_fAngle(0.f)
, m_wSortNum(0)
, m_pPlayer(NULL)
, m_pCursor(NULL)
, m_eWinType(WINDOW_END)
, m_bFollow(false)
, m_bAlive(true)
, m_iSlotLevel(1)
, m_bPartition(false)
, m_iPartitionMinCnt(0)
, m_iPartitionMaxCnt(0)
, m_bPick(false)
, m_bOut(false)
, m_bSwap(false)
, m_bLeftWeapon(false)
, m_bRightWeapon(false)
, m_bFade(false)
, m_vColor(1.f, 1.f, 1.f, 1.f)
, m_bFoward(false)
, m_bRBPick(false)
, m_bRClick(false)
, m_pSound(NULL)
{
	D3DXMatrixIdentity(&m_matProj);
	D3DXMatrixIdentity(&m_matView);

	if (Engine::GetSceneID() == Engine::SCENE_LOADING)
		m_bIsActive = false;

}
CUI::~CUI(void)
{

}
HRESULT CUI::Start_ForScene(void)
{
	m_pPlayer = ((CPlayer*)(Engine::Find(L"Player")));
	::Safe_Release(m_pPlayer);
	return S_OK;
}

void CUI::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);
}

void CUI::Render_Object(void)
{

}

void CUI::SetSelect(_bool bSelect)
{
	m_bSelect = bSelect;
}
void CUI::SetIndex(_uint iIndex)
{
	m_iIndex = iIndex;
}
void CUI::SetSkipScene(_bool bSkipScene)
{
	m_bSkipScene = bSkipScene;
}
void CUI::SetAlpha(_float fAlpha)
{
	m_fAlpha = fAlpha;
}
void CUI::SetBeforeIndex(_uint iIndex)
{
	m_iBeforeIndex = iIndex;
}
void CUI::SetRender(_bool bRender)
{
	m_bRender = bRender;
}

void CUI::SetSortNum(WORD iSortNum)
{
	m_wSortNum = iSortNum;
}

void CUI::SetClick(_bool bClick)
{
	m_bClick = bClick;
}

void CUI::SetMove(_bool bMove)
{
	m_bMove = bMove;
}
void CUI::SetAngle(_float fAngle)
{
	m_fAngle = fAngle;
}
void CUI::SetSheetSelect(_bool bSheetSelect)
{
	m_bSheetSelect = bSheetSelect;
}
void CUI::SetWinPos(_vec2 vPos)
{
	m_fX = vPos.x;
	m_fY = vPos.y;
}
void CUI::SetBlockPick(_bool bBlockPick)
{
	m_bBlockPick = bBlockPick;
}
void CUI::SetMousePick(_bool bMousePick)
{
	m_bMousePick = bMousePick;
}
void CUI::SetToolTipOn(_bool bToolTipOn)
{
	m_bToolTipOn = bToolTipOn;
}
void CUI::SetWear(_bool bWear)
{
	m_bWear = bWear;
}
void CUI::SetSlotLevel(_uint iSlotLevel)
{
	m_iSlotLevel = iSlotLevel;
}
void CUI::SetFollow(_bool bFollow)
{
	m_bFollow = bFollow;
}

void CUI::SetAlive(_bool bAlive)
{
	m_bAlive = bAlive;
}
void CUI::SetMain(_bool bMain)
{
	m_bMain = bMain;
}
void CUI::SetKeyBoard(_bool bKeyBoard)
{
	m_bKeyBoard = bKeyBoard;
}
void CUI::SetPartition(_bool bPartition)
{
	m_bPartition = bPartition;
}
void CUI::SetPartitionMinCnt(_uint iPartitionMinCnt)
{
	m_iPartitionMinCnt = iPartitionMinCnt;
}

void CUI::SetPartitionMaxCnt(_uint iPartitionMaxCnt)
{
	m_iPartitionMaxCnt = iPartitionMaxCnt;
}
void CUI::SetPick(_bool bPick)
{
	m_bPick = bPick;
}
void CUI::SetOut(_bool bOut)
{
	m_bOut = bOut;
}

void CUI::SetSwap(_bool bSwap)
{
	m_bSwap = bSwap;
}
void CUI::SetRightWeapon(_bool bRightWeapon)
{
	m_bRightWeapon = bRightWeapon;
}

void CUI::SetLeftWeapon(_bool bLeftWeapon)
{
	m_bLeftWeapon = bLeftWeapon;
}
_bool CUI::GetSkipScene(void)
{
	return m_bSkipScene;
}

_uint CUI::GetIndex(void)
{
	return m_iIndex;
}

_bool CUI::GetSelect(void)
{
	return m_bSelect;
}

_float& CUI::GetAlpha(void)
{
	return m_fAlpha;
}

_bool& CUI::GetAlphaIncrease(void)
{
	return m_bAlpha_Increase;
}

_vec4 CUI::GetInfo(void)
{
	_vec4	vInfo = _vec4(m_fX, m_fY, m_fSizeX, m_fSizeY);
	return	vInfo;
}

_bool CUI::GetRender(void)
{
	return m_bRender;
}

WORD CUI::GetSortNum(void)
{
	return m_wSortNum;
}
_bool CUI::GetWindowProcess(void)
{
	return m_bWindowProcess;
}
_bool CUI::GetCollision(void)
{
	return m_bCollision;
}
_bool CUI::GetClick(void)
{
	return m_bClick;
}

_bool CUI::GetMove(void)
{
	return m_bMove;
}
_bool CUI::GetBarCollision(void)
{
	return m_bBarCollision;
}
_float& CUI::GetAngle(void)
{
	return m_fAngle;
}

_float* CUI::GetAnglePointer(void)
{
	return &m_fAngle;
}
_bool CUI::GetSheetSelect(void)
{
	return m_bSheetSelect;
}

_bool CUI::GetBlockPick(void)
{
	return m_bBlockPick;
}
_float* CUI::GetfX(void)
{
	return &m_fX;
}

_float* CUI::GetfY(void)
{
	return &m_fY;
}
_bool CUI::GetMousePick(void)
{
	return m_bMousePick;
}

_bool CUI::GetToolTipOn(void)
{
	return m_bToolTipOn;
}
_bool CUI::GetWear(void)
{
	return m_bWear;
}

_bool CUI::GetButtonCol(void)
{
	return m_bButtonCollision;
}
WINDOW_ID CUI::GetWinType(void)
{
	return m_eWinType;
}

_bool CUI::GetFollow(void)
{
	return m_bFollow;
}

_bool CUI::GetAlive(void)
{
	return m_bAlive;
}
_uint CUI::GetSlotLevel(void)
{
	return m_iSlotLevel;
}

_bool CUI::GetMain(void)
{
	return m_bMain;
}

_bool CUI::GetKeyBoard(void)
{
	return m_bKeyBoard; 
}

_bool CUI::GetPartition(void)
{
	return m_bPartition;
}
_uint* CUI::GetPartitionMinCnt(void)
{
	return &m_iPartitionMinCnt;
}
_uint* CUI::GetPartitionMaxCnt(void)
{
	return &m_iPartitionMaxCnt;
}
_bool CUI::GetPick(void)
{
	return m_bPick;
}

_bool CUI::GetOut(void)
{
	return m_bOut;
}

_bool CUI::GetSwap(void)
{
	return m_bSwap;
}
_bool CUI::GetRightWeapon(void)
{
	return m_bRightWeapon;
}

_bool CUI::GetLeftWeapon(void)
{
	return m_bLeftWeapon;
}
void CUI::CheckWindowProcess()
{
	list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"UI_Window")->begin();
	list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"UI_Window")->end();

	for(; iter != iter_end; ++iter)
	{
		if(((CUI*)(*iter))->GetWindowProcess()
			&& ((CUI*)(*iter))->GetCollision())
		{
			m_bCollision = false;
			return;
		}
	}
}	

void CUI::Overlap_Window()
{
	list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"UI_Window")->begin();
	list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"UI_Window")->end();

	int iIndex = 0;

	for(; iter != iter_end; ++iter)
	{
		if(((CUI*)(*iter))->GetBarCollision())
			++iIndex;
	}

	if(iIndex >= 2)
	{
		iter = Engine::FindList(L"UI_Window")->begin();

		for(; iter != iter_end; ++iter)
		{
			if(!((CUI*)(*iter))->GetSortNum() && ((CUI*)(*iter))->GetBarCollision())
			{
				((CUI*)(*iter))->SetClick(false);
				((CUI*)(*iter))->SetSelect(false);
				((CUI*)(*iter))->SetWindowProcess(false);
			}
		}
	}
}
void CUI::SetWindowProcess(_bool bWindowProcess)
{
	m_bWindowProcess = bWindowProcess;
}

void CUI::Free(void)
{
	Engine::CGameObject::Free();
}

void CUI::SetFontColor(_vec4 vColor)
{
	m_vColor = vColor;
}

void CUI::SetFade(_bool bFade)
{
	m_bFade = bFade;
}

_bool CUI::GetFade(void)
{
	return m_bFade;
}

void CUI::SetFoward(_bool bFoward)
{
	m_bFoward = bFoward;
}

_bool CUI::GetFoward(void)
{
	return m_bFoward;
}



