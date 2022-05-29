#include "stdafx.h"
#include "DarkPollingWave.h"
#include "Export_Engine.h"
#include "PlayerCamera.h"


CDarkPollingWave::CDarkPollingWave(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
: CEffect(pGraphicDev, wstrName)
{
	m_bAni = false;
}

CDarkPollingWave::~CDarkPollingWave(void)
{
}

void CDarkPollingWave::Update_Object(const _float & fTimeDelta)
{
}

void CDarkPollingWave::Render_Object(void)
{
}

HRESULT CDarkPollingWave::Ready_Object(void)
{
	return E_NOTIMPL;
}

CDarkPollingWave * CDarkPollingWave::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	return nullptr;
}

void CDarkPollingWave::Free(void)
{
}

HRESULT CDarkPollingWave::Add_Component(void)
{
	return E_NOTIMPL;
}

HRESULT CDarkPollingWave::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	return E_NOTIMPL;
}

void CDarkPollingWave::SetPart(_int nIdx)
{
}

void CDarkPollingWave::MoveUpdate(const _float & fTimeDelta)
{
}

void CDarkPollingWave::Update_RePart(void)
{
}

void CDarkPollingWave::SetAni(_bool bAni)
{
}

_bool CDarkPollingWave::GetAni(void)
{
	return _bool();
}
