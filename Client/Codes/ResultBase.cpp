#include "stdafx.h"
#include "ResultBase.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "StageClear.h"
#include "ReturnStage.h"
#include "ResultTitle.h"
#include "SubTitle.h"
#include "ResultNum.h"
#include "NumberingMgr.h"
#include "Gold.h"
#include "Player.h"
#include "TotalRank.h"
#include "TotalRankBack.h"
#include "Fade_Manager.h"
#include "Upjuk_Ud.h"
#include "Sound.h"

CResultBase::CResultBase(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
	:CUI(pGraphicDev, wstrName, eState, bIsActive)
	, m_pTransCom(NULL)
	, m_pBufferCom(NULL)
	, m_pTextureCom(NULL)
	, m_pRendererCom(NULL)
	, m_pShaderCom(NULL)
	, m_fFadeTime(0.0f)
{

}

CResultBase::~CResultBase(void)
{

}

HRESULT CResultBase::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	CUI::Start_ForScene();

	m_pSound->MyPlaySound(L"ResultStart");

	m_fX = WINSIZEX + 100.f;
	m_fY = (WINSIZEY >> 1) + 50.f;
	m_fTermY = 0.f;

	m_fSizeX = 600.f;
	m_fSizeY = 600.f;
	m_fAlpha = 0.f;
	m_fGold = 0.f;
	m_bRender = false;
	m_bFade = false;
	m_bOnceStart = false;
	m_bAlpha_Increase = false;

	m_fTotalRank = 0.f;

	Engine::CGameObject* pGameObject = CStageClear::Create(m_pGraphicDev, L"StageClear");
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_Result", pGameObject);

	pGameObject = CReturnStage::Create(m_pGraphicDev, L"ReturnStage");
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_Result", pGameObject);

	pGameObject = CResultTitle::Create(m_pGraphicDev, L"ResultTitle_Result", this, true);
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	pGameObject = CResultTitle::Create(m_pGraphicDev, L"ResultTitle_Bonus", this, false);
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	pGameObject = CSubTitle::Create(m_pGraphicDev, L"Kills", 0, 0, 15.f);
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	pGameObject = CSubTitle::Create(m_pGraphicDev, L"Skill", 1, 0, 15.f);
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	pGameObject = CSubTitle::Create(m_pGraphicDev, L"Rank", 2, 0, 15.f);
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	pGameObject = CSubTitle::Create(m_pGraphicDev, L"Mentoring", 3, 0, 15.f);
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	pGameObject = CSubTitle::Create(m_pGraphicDev, L"Nanip", 4, 0, 15.f);
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	pGameObject = CSubTitle::Create(m_pGraphicDev, L"Bonus", 5, 0, 15.f);
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	pGameObject = CSubTitle::Create(m_pGraphicDev, L"Style", 6, 0, 15.f);
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	pGameObject = CSubTitle::Create(m_pGraphicDev, L"Technic", 7, 0, 15.f);
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	pGameObject = CSubTitle::Create(m_pGraphicDev, L"PC", 8, 0, 15.f);
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	pGameObject = CSubTitle::Create(m_pGraphicDev, L"Total", 9, 0, 15.f);
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);


	/// Number : Kill

	pGameObject = CResultNum::Create(m_pGraphicDev, L"Kill_Percent", 10, 2, _vec2(1185.f, 290.f)); // %
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	//float fKill = CNumbering_Manager::GetInstance()->GetResult().x;
	float fKill = 25.f;
	fKill = (fKill / 25.f) * 100.f;
	m_fTotalRank += fKill;
	int iKillOne = int(fKill) % 10;
	pGameObject = CResultNum::Create(m_pGraphicDev, L"Kill_One", iKillOne, 1, _vec2(1155.f, 290.f));
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	if (fKill >= 10)
	{
		if (fKill >= 100)
		{
			fKill = 100.f;
			iKillOne = int(fKill) % 10;
			fKill /= 10;

			pGameObject = CResultNum::Create(m_pGraphicDev, L"Kill_One", iKillOne, 0, _vec2(1130.f, 290.f));
			if (pGameObject == NULL)
				return E_FAIL;
			Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);
		}
		else
		{
			fKill /= 10;

			pGameObject = CResultNum::Create(m_pGraphicDev, L"Kill_One", _uint(fKill), 0, _vec2(1130.f, 290.f));
			if (pGameObject == NULL)
				return E_FAIL;
			Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);
		}
		
	}

	if (fKill >= 10)
	{
		fKill /= 10;

		pGameObject = CResultNum::Create(m_pGraphicDev, L"Kill_One", _uint(fKill), 30, _vec2(1105.f, 290.f));
		if (pGameObject == NULL)
			return E_FAIL;
		Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);
	}

	/////////////////////////////////////////////////////////////////////
	
	/// Number : SKill

	pGameObject = CResultNum::Create(m_pGraphicDev, L"SKill_Percent", 10, 5, _vec2(1185.f, 340.f)); // %
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	//float fSkill = CNumbering_Manager::GetInstance()->GetResult().y;
	float fSkill = _float(rand() % 10) + 90.f;
	fSkill = (fSkill / 100.f) * 100.f;
	m_fTotalRank += fSkill;
	int iSkillOne = int(fSkill) % 10;
	pGameObject = CResultNum::Create(m_pGraphicDev, L"SKill_One", iSkillOne, 4, _vec2(1155.f, 340.f));
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	if (fSkill >= 10)
	{
		fSkill /= 10;

		pGameObject = CResultNum::Create(m_pGraphicDev, L"SKill_One", _uint(fSkill), 3, _vec2(1130.f, 340.f));
		if (pGameObject == NULL)
			return E_FAIL;
		Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);
	}

	/////////////////////////////////////////////////////////////////////
	/// Number : Rank


	pGameObject = CResultNum::Create(m_pGraphicDev, L"Kill_One", 11, 31, _vec2(1105.f, 610.f)); // +
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	pGameObject = CResultNum::Create(m_pGraphicDev, L"SKill_Percent", 10, 8, _vec2(1185.f, 610.f)); // %
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	//float fRank = CNumbering_Manager::GetInstance()->GetResult().y;
	float fRank = 10.f;
	fRank = (fRank /  100.f) * 100.f;
	int iRankOne = int(fRank) % 10;
	pGameObject = CResultNum::Create(m_pGraphicDev, L"SKill_One", iRankOne, 7, _vec2(1155.f, 610.f));
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	if (fRank >= 10)
	{
		fRank /= 10;

		pGameObject = CResultNum::Create(m_pGraphicDev, L"SKill_One", _uint(fRank), 6, _vec2(1130.f, 610.f));
		if (pGameObject == NULL)
			return E_FAIL;
		Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);
	}

	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	/// Number : Mentoring

	pGameObject = CResultNum::Create(m_pGraphicDev, L"SKill_Percent", 10, 11, _vec2(1185.f, 660.f)); // %
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	//float fMento = CNumbering_Manager::GetInstance()->GetResult().y;
	float fMento = 0.f;
	fMento = (fMento / 100.f) * 100.f;
	int iMentoOne = int(fMento) % 10;
	pGameObject = CResultNum::Create(m_pGraphicDev, L"SKill_One", iMentoOne, 10, _vec2(1155.f, 660.f));
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	if (fMento >= 10)
	{
		fMento /= 10;

		pGameObject = CResultNum::Create(m_pGraphicDev, L"SKill_One", _uint(fMento), 9, _vec2(1130.f, 660.f));
		if (pGameObject == NULL)
			return E_FAIL;
		Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);
	}

	/////////////////////////////////////////////////////////////////////
	//////////////////// 2번째 라인 //////////////////////////////////////
	/// Number : 난입

	pGameObject = CResultNum::Create(m_pGraphicDev, L"SKill_Percent", 10, 14, _vec2(1385.f, 610.f)); // %
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	//float fNanip = CNumbering_Manager::GetInstance()->GetResult().y;
	float fNanip = 0.f;
	fNanip = (fNanip / 100.f) * 100.f;
	int iNanip = int(fNanip) % 10;
	pGameObject = CResultNum::Create(m_pGraphicDev, L"SKill_One", iNanip, 13, _vec2(1355.f, 610.f));
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	if (fNanip >= 10)
	{
		fNanip /= 10;

		pGameObject = CResultNum::Create(m_pGraphicDev, L"SKill_One", _uint(fNanip), 12, _vec2(1330.f, 610.f));
		if (pGameObject == NULL)
			return E_FAIL;
		Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);
	}

	/////////////////////////////////////////////////////////////////////
	/// Number : Bonus

	pGameObject = CResultNum::Create(m_pGraphicDev, L"SKill_Percent", 10, 17, _vec2(1385.f, 660.f)); // %
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	//float fBonus = CNumbering_Manager::GetInstance()->GetResult().y;
	float fBonus = 0.f;
	fBonus = (fBonus / 100.f) * 100.f;
	int iBonus = int(fBonus) % 10;
	pGameObject = CResultNum::Create(m_pGraphicDev, L"SKill_One", iBonus, 16, _vec2(1355.f, 660.f));
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	if (fBonus >= 10)
	{
		fBonus /= 10;

		pGameObject = CResultNum::Create(m_pGraphicDev, L"SKill_One", _uint(fBonus), 15, _vec2(1330.f, 660.f));
		if (pGameObject == NULL)
			return E_FAIL;
		Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);
	}

	///////////////////////////////////////////////////////////////////// 3라인
	/// Number : Kill

	pGameObject = CResultNum::Create(m_pGraphicDev, L"Kill_Percent", 10, 20, _vec2(1485.f, 290.f)); // %
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	//float fStyle = CNumbering_Manager::GetInstance()->GetResult().x;
	float fStyle = _float(rand() % 10) + 80.f;
	fStyle = (fStyle / 100.f) * 100.f;
	m_fTotalRank += fStyle;
	int iStyleOne = int(fStyle) % 10;
	pGameObject = CResultNum::Create(m_pGraphicDev, L"Kill_One", iStyleOne, 19, _vec2(1455.f, 290.f));
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	if (fStyle >= 10)
	{
		fStyle /= 10;

		pGameObject = CResultNum::Create(m_pGraphicDev, L"Kill_One", _uint(fStyle), 18, _vec2(1430.f, 290.f));
		if (pGameObject == NULL)
			return E_FAIL;
		Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);
	}

	/////////////////////////////////////////////////////////////////////

	/// Number : Style

	pGameObject = CResultNum::Create(m_pGraphicDev, L"SKill_Percent", 10, 23, _vec2(1485.f, 340.f)); // %
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	//float fTech = CNumbering_Manager::GetInstance()->GetResult().y;
	float fTech = _float(rand() % 10) + 90.f;
	fTech = (fTech / 100.f) * 100.f;
	m_fTotalRank += fTech;
	int iTech = int(fTech) % 10;
	pGameObject = CResultNum::Create(m_pGraphicDev, L"SKill_One", iTech, 22, _vec2(1455.f, 340.f));
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	if (fTech >= 10)
	{
		fTech /= 10;

		pGameObject = CResultNum::Create(m_pGraphicDev, L"SKill_One", _uint(fTech), 21, _vec2(1430.f, 340.f));
		if (pGameObject == NULL)
			return E_FAIL;
		Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);
	}

	/////////////////////////////////////////////////////////////////////

	/// Number : PC

	pGameObject = CResultNum::Create(m_pGraphicDev, L"SKill_Percent", 10, 26, _vec2(1585.f, 610.f)); // %
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	//float fNanip = CNumbering_Manager::GetInstance()->GetResult().y;
	float fPC = 0.f;
	fPC = (fPC / 100.f) * 100.f;
	int iPC = int(fPC) % 10;
	pGameObject = CResultNum::Create(m_pGraphicDev, L"SKill_One", iPC, 25, _vec2(1555.f, 610.f));
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	if (fPC >= 10)
	{
		fPC /= 10;

		pGameObject = CResultNum::Create(m_pGraphicDev, L"SKill_One", _uint(fPC), 24, _vec2(1530.f, 610.f));
		if (pGameObject == NULL)
			return E_FAIL;
		Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);
	}

	/////////////////////////////////////////////////////////////////////
	/// Number : Total

	pGameObject = CResultNum::Create(m_pGraphicDev, L"Kill_One", 11, 32, _vec2(1505.f, 660.f)); // +
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	pGameObject = CResultNum::Create(m_pGraphicDev, L"SKill_Percent", 10, 29, _vec2(1585.f, 660.f)); // %
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	//float fTotal = CNumbering_Manager::GetInstance()->GetResult().y;
	float fTotal = 10.f;
	fTotal = (fTotal / 100.f) * 100.f;
	int iTotal = int(fTotal) % 10;
	pGameObject = CResultNum::Create(m_pGraphicDev, L"SKill_One", iTotal, 28, _vec2(1555.f, 660.f));
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	if (fTotal >= 10)
	{
		fTotal /= 10;

		pGameObject = CResultNum::Create(m_pGraphicDev, L"SKill_One", _uint(fTotal), 27, _vec2(1530.f, 660.f));
		if (pGameObject == NULL)
			return E_FAIL;
		Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);
	}

	///////////////////////////////////////////////////////////////////// 둘째라인


	pGameObject = CGold::Create(m_pGraphicDev, L"Gold");
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);


	//// Gold Count ////

	float fGold = _float(rand() % 1000) + _float(((rand() % 3) + 3) * 1000);
	m_fGold = fGold;
	m_pPlayer->GetPlayerInfo()->m_iMoney += int(m_fGold);
	int iGold = int(fGold) % 10;

	pGameObject = CResultNum::Create(m_pGraphicDev, L"SKill_One", iGold, 36, _vec2(1545.f, 725.f));
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);
	
	fGold /= 10;
	iGold = int(fGold) % 10;

	pGameObject = CResultNum::Create(m_pGraphicDev, L"SKill_One", iGold, 35, _vec2(1520.f, 725.f));
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	fGold /= 10;
	iGold = int(fGold) % 10;

	pGameObject = CResultNum::Create(m_pGraphicDev, L"SKill_One", iGold, 34, _vec2(1495.f, 725.f));
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	fGold /= 10;
	iGold = int(fGold) % 10;

	pGameObject = CResultNum::Create(m_pGraphicDev, L"SKill_One", iGold, 33, _vec2(1470.f, 725.f));
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	///// Result Total

	m_fTotalRank /= 4.f;

	if (m_fTotalRank <= 93.f)
	{
		pGameObject = CTotalRankBack::Create(m_pGraphicDev, L"SKill_One", 0);
		if (pGameObject == NULL)
			return E_FAIL;
		Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

		pGameObject = CTotalRank::Create(m_pGraphicDev, L"SKill_One", 0);
		if (pGameObject == NULL)
			return E_FAIL;
		Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);
	}
	else if(m_fTotalRank > 93.f)
	{

		pGameObject = CTotalRankBack::Create(m_pGraphicDev, L"SKill_One", 1);
		if (pGameObject == NULL)
			return E_FAIL;
		Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

		pGameObject = CTotalRank::Create(m_pGraphicDev, L"SKill_One", 1);
		if (pGameObject == NULL)
			return E_FAIL;
		Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	}

	pGameObject = CUpjuk_Ud::Create(m_pGraphicDev, L"Upjuk_Up", 0);
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	pGameObject = CUpjuk_Ud::Create(m_pGraphicDev, L"Upjuk_Down", 1);
	if (pGameObject == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	pGameObject = CUpjuk_Ud::Create(m_pGraphicDev, L"Upjuk_Up", 0);
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	pGameObject = CUpjuk_Ud::Create(m_pGraphicDev, L"Upjuk_Down", 1);
	Engine::AddObject(Engine::LAYER_UI, L"UI_ResultChar", pGameObject);

	m_pSound->MyPlaySound(L"ResultStart");
	m_fAlphaTime = 0.f;
	m_fTotalRank = 0.f;
	m_bPick = false;

	return S_OK;
}

HRESULT CResultBase::Start_ForScene(void)
{
	return S_OK;
}

void CResultBase::Update_Object(const _float& fTimeDelta)
{
	if (!m_bFade)
	{
		m_fFadeTime += fTimeDelta;
		m_fX -= m_fFadeTime * 50.f;
		if (m_fX <= WINSIZEX - 300.f)
		{
			m_fX = WINSIZEX - 300.f;
			m_bFade = true;
		}
	}
	else
	{
		m_fAlphaTime += fTimeDelta;

		if (m_fAlphaTime >= 12.f)
		{
			if (!m_bPick)
			{
				m_bPick = true;
				m_fAlphaTime = 0.f;
				CFade_Manager::GetInstance()->FadeReset();
				CFade_Manager::GetInstance()->CreateSelectItem(CFade_Manager::FADE_CLEAR_GETITEM);
				CFade_Manager::GetInstance()->FadeStart(0.f, 0.05f, CFade_Manager::FADE_CLEAR_GETITEM);
				CFade_Manager::GetInstance()->SetResultFading(true);
			}
			
		}
	}

	m_fAlpha += 0.05f;
	if (m_fAlpha >= 0.8f)
		m_fAlpha = 0.8f;

	CUI::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);

	D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);

}

void CResultBase::Render_Object(void)
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (NULL == pEffect)
		return;

	_uint			iPassCnt;

	_matrix				matWorld, matView;
	_matrix				matOldView, matOldProj;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matOldProj);

	matWorld._11 = m_fSizeX;
	matWorld._22 = m_fSizeY;
	matWorld._33 = 1.f;
	matWorld._41 = m_fX - WINSIZEX * 0.5f;
	matWorld._42 = -m_fY + WINSIZEY * 0.5f;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &m_matProj);

	pEffect->SetFloat("g_fAlpha", m_fAlpha);

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture");

	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);

}


HRESULT CResultBase::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.Buffer Component
	m_pBufferCom = (Engine::CRect_Texture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_RcTex");
	pComponent = (Engine::CComponent*)m_pBufferCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));

	// For.Texture Component 
	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"ResultBase");
	pComponent = (Engine::CComponent*)m_pTextureCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));

	// For.Renderer Component
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	if (NULL == pComponent)
		return E_FAIL;
	pComponent->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));

	pComponent = m_pShaderCom = (Engine::CShader*)Engine::Clone_Prototype(L"Shader_UI");
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	// Sound
	pComponent = m_pSound = CSound::Create(NULL);
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Sound", pComponent));
	m_pSound->Set_Sound(L"ResultStart", L"ResultStart.wav");

	return S_OK;
}


CResultBase* CResultBase::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CResultBase*		pInstance = new CResultBase(pGraphicDev, wstrName, eState, bIsActive);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}


void CResultBase::Free(void)
{
	CUI::Free();
}
