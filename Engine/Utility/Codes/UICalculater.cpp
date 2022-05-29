#include "UICalculater.h"

USING(Engine)

Engine::CUICalculater::CUICalculater(void)
:m_vFontPos(0.f, 0.f)
{

}

Engine::CUICalculater::~CUICalculater(void)
{

}

HRESULT Engine::CUICalculater::Ready_Transform(void)
{
	return S_OK;
}

_vec2 Engine::CUICalculater::CunvertFontPos(const _vec3& vPos
											, _uint WINSIZEX
											, _uint WINSIZEY
											, _float fMoveX
											, _float fMoveY)
{
	m_vFontPos.x = (vPos.x + 1.f) * (WINSIZEX >> 1) + fMoveX;
	m_vFontPos.y = (vPos.y - 1.f) * (WINSIZEY >> 1) * -1 + fMoveY;

	return m_vFontPos;
}

bool Engine::CUICalculater::Collision_ByMouse(const _uint& WINSIZEX, const _uint& WINSIZEY
											  , _float m_fX, _float m_fY, _float m_fSizeX, _float m_fSizeY, const POINT& ptMouse)
{
	RECT		rcUI = {long(m_fX - m_fSizeX * 0.5f)
		, long(m_fY - m_fSizeY * 0.5f)
		, long(m_fX + m_fSizeX * 0.5f)
		, long(m_fY + m_fSizeY * 0.5f)};

	if(PtInRect(&rcUI, ptMouse))
		return true;

	return false;
}

bool Engine::CUICalculater::Move_Window_UI(const _uint& WINSIZEX, const _uint& WINSIZEY , _float m_fX, _float m_fY, _float m_fSizeX, _float m_fSizeY, const POINT& ptMouse)
{
	RECT		rcUI = {long(m_fX - m_fSizeX * 0.5f)
		, long(m_fY - m_fSizeY * 0.5f)
		, long(m_fX + m_fSizeX * 0.5f)
		, long((m_fY - m_fSizeY * 0.5f) + 30.f)};

	if(PtInRect(&rcUI, ptMouse))
		return true;

	return false;
}
CUICalculater* Engine::CUICalculater::Create(void)
{
	CUICalculater*		pInstance = new CUICalculater;

	if(FAILED(pInstance->Ready_Transform()))
	{
		MSG_BOX("CUICalculater Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void Engine::CUICalculater::Free(void)
{

}

_vec3 Engine::CUICalculater::WorldToScreenPos(const _matrix* pmatWorld, const _matrix* pmatView, const _matrix* pmatProj, _uint iWinCX, _uint iWINCY)
{
	_matrix matWVP = (*pmatWorld) * (*pmatView) * (*pmatProj);

	_vec4	vWorldToScreen(0, 0, 0, 1);
	D3DXVec4Transform(&vWorldToScreen, &vWorldToScreen, &matWVP);

	float f_WorldToScreenX = vWorldToScreen.x / vWorldToScreen.w;
	float f_WorldToScreenY = vWorldToScreen.y / vWorldToScreen.w;
	float f_WorldToScreenZ = vWorldToScreen.z / vWorldToScreen.w;

	float f_ScreenNormalX = (f_WorldToScreenX + 1) * 0.5f;
	float f_ScreenNormalY = (f_WorldToScreenY + 1) * 0.5f;
	f_ScreenNormalY = 1.0f - f_ScreenNormalY;

	float fX = f_ScreenNormalX * iWinCX;
	float fY = f_ScreenNormalY * iWINCY;

	_vec3 vResult = _vec3(fX, fY, f_WorldToScreenZ);

	return vResult;
}

