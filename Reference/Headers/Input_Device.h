#ifndef Input_Device_h__
#define Input_Device_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CInput_Device
	: public CBase
{
public:
	DECLARE_SINGLETON(CInput_Device)
public:
	enum MOUSEKEYSTATE {DIM_LB, DIM_RB, DIM_HB, DIM_END};
	enum MOUSEMOVESTATE {DIMS_X, DIMS_Y, DIMS_Z, DIMS_END};
private:
	explicit					CInput_Device(void);
	virtual						~CInput_Device(void);
public:
	_byte 						Get_DIKeyState(_ubyte byKeyID);
	_byte 						Get_DIMouseState(MOUSEKEYSTATE byMouseID);
	_long 						Get_DIMouseMove(MOUSEMOVESTATE byMouseMoveState);
public:
	void						Set_Acquire(void);
public:
	HRESULT						Ready_InputDevice(HINSTANCE hInst, HWND hWnd);
	void						SetUp_InputState(void);
private:
	LPDIRECTINPUT8				m_pInputSDK;
private:
	LPDIRECTINPUTDEVICE8		m_pKeyBoard;
	LPDIRECTINPUTDEVICE8		m_pMouse;
private:
	_byte						m_byKeyState[256];
	DIMOUSESTATE				m_MouseState;
public:
	virtual void				Free(void);
};

END

#endif // Input_Device_h__
