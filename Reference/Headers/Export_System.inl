

// For.GraphicDev Instance ---------------------------------------------------------------------------------------------------------------------------------------
// Getter --------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT Engine::Get_GraphicDev(LPDIRECT3DDEVICE9* ppGraphicDev)
{
	return Engine::CGraphicDev::GetInstance()->Get_GraphicDev(ppGraphicDev);
}
// Setter --------------------------------------------------------------------------------------------------------------------------------------------------------
// General -------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT Engine::Ready_GraphicDev(HWND hWnd, Engine::CGraphicDev::WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, Engine::CGraphicDev** ppGraphicDev)
{
	return Engine::CGraphicDev::GetInstance()->Ready_GraphicDev(hWnd, eMode, iSizeX, iSizeY, ppGraphicDev);
}

// For.Timer_Manager Instance ---------------------------------------------------------------------------------------------------------------------------------------
// Getter --------------------------------------------------------------------------------------------------------------------------------------------------------
_float Engine::Get_TimeDelta(const _tchar* pTimerTag)
{
	return Engine::CTimer_Manager::GetInstance()->Get_TimeDelta(pTimerTag);
}
// Setter --------------------------------------------------------------------------------------------------------------------------------------------------------
void Engine::SetUp_TimeDelta(const _tchar* pTimerTag)
{
	Engine::CTimer_Manager::GetInstance()->SetUp_TimeDelta(pTimerTag);
}
// General ----------------------------1---------------------------------------------------------------------------------------------------------------------------
HRESULT Engine::Ready_Timer(const _tchar* pTimerTag)
{
	return Engine::CTimer_Manager::GetInstance()->Ready_Timer(pTimerTag);
}

// For.Frame_Manager Instance ---------------------------------------------------------------------------------------------------------------------------------------
// Getter --------------------------------------------------------------------------------------------------------------------------------------------------------
_bool Engine::isPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta)
{
	return Engine::CFrame_Manager::GetInstance()->isPermit_Call(pFrameTag, fTimeDelta);
}
// Setter --------------------------------------------------------------------------------------------------------------------------------------------------------
// General --------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT Engine::Ready_Frame(const _tchar* pFrameTag, const _float& fCallLimit)
{
	return CFrame_Manager::GetInstance()->Ready_Frame(pFrameTag, fCallLimit);
}

// For.Font_Manager Instance ---------------------------------------------------------------------------------------------------------------------------------------
// Getter --------------------------------------------------------------------------------------------------------------------------------------------------------
// Setter --------------------------------------------------------------------------------------------------------------------------------------------------------
// General --------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT Engine::Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFontPath, const _tchar* pFontTag, const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
	return CFont_Manager::GetInstance()->Ready_Font(pGraphicDev, pFontPath, pFontTag, pFontType, iWidth, iHeight, iWeight);
}

void Engine::Render_Font(const _tchar* pFontTag, const _tchar* pString, const _vec2* pPosition, D3DXCOLOR Color)
{
	CFont_Manager::GetInstance()->Render_Font(pFontTag, pString, pPosition, Color);
}

// For.Input_Device Instance ---------------------------------------------------------------------------------------------------------------------------------------
// Getter --------------------------------------------------------------------------------------------------------------------------------------------------------
_byte Engine::Get_DIKeyState(_ubyte byKeyID)
{
	return CInput_Device::GetInstance()->Get_DIKeyState(byKeyID);
}
_byte Engine::Get_DIMouseState(Engine::CInput_Device::MOUSEKEYSTATE byMouseID)
{
	return CInput_Device::GetInstance()->Get_DIMouseState(byMouseID);
}
_long Engine::Get_DIMouseMove(Engine::CInput_Device::MOUSEMOVESTATE byMouseMoveState)
{
	return CInput_Device::GetInstance()->Get_DIMouseMove(byMouseMoveState);
}
// Setter --------------------------------------------------------------------------------------------------------------------------------------------------------
void Engine::SetUp_InputState(void)
{	
	CInput_Device::GetInstance()->SetUp_InputState();
}
void Engine::Set_Acquire(void)
{
	CInput_Device::GetInstance()->Set_Acquire();
}
// General --------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT Ready_InputDevice(HINSTANCE hInst, HWND hWnd)
{
	return CInput_Device::GetInstance()->Ready_InputDevice(hInst, hWnd);
}

// For.Release System Instance---------------------------------------------------------------------------------------------------------------------------------------
void Engine::Release_System(void)
{
	Engine::CInput_Device::GetInstance()->DestroyInstance();
	Engine::CFont_Manager::GetInstance()->DestroyInstance();
	Engine::CFrame_Manager::GetInstance()->DestroyInstance();
	Engine::CTimer_Manager::GetInstance()->DestroyInstance();
	Engine::CGraphicDev::GetInstance()->DestroyInstance();	
}