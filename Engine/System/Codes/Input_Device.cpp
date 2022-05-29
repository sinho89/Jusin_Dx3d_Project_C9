#include "Input_Device.h"

USING(Engine)
IMPLEMENT_SINGLETON(CInput_Device)

Engine::CInput_Device::CInput_Device(void)
{

}

Engine::CInput_Device::~CInput_Device(void)
{

}

Engine::_byte Engine::CInput_Device::Get_DIKeyState(_ubyte byKeyID)
{
	return m_byKeyState[byKeyID];
}

Engine::_byte Engine::CInput_Device::Get_DIMouseState(MOUSEKEYSTATE byMouseID)
{
	return m_MouseState.rgbButtons[byMouseID];
}

Engine::_long Engine::CInput_Device::Get_DIMouseMove(MOUSEMOVESTATE byMouseMoveState)
{
	return *(((_long*)(&m_MouseState)) + byMouseMoveState);
}

void Engine::CInput_Device::Set_Acquire(void)
{
	m_pKeyBoard->Acquire();
	m_pMouse->Acquire();
}

HRESULT Engine::CInput_Device::Ready_InputDevice(HINSTANCE hInst, HWND hWnd)
{
	if(FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputSDK, NULL)))
		return E_FAIL;	

	if(FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, NULL)))
		return E_FAIL;

	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	m_pKeyBoard->Acquire();

	if(FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, NULL)))
		return E_FAIL;

	m_pMouse->SetDataFormat(&c_dfDIMouse);
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	m_pMouse->Acquire();

	return S_OK;
}

void Engine::CInput_Device::SetUp_InputState(void)
{
	m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	m_pMouse->GetDeviceState(sizeof(m_MouseState), &m_MouseState);
}

void Engine::CInput_Device::Free(void)
{
	Engine::Safe_Release(m_pKeyBoard);
	Engine::Safe_Release(m_pMouse);
	Engine::Safe_Release(m_pInputSDK);
}

