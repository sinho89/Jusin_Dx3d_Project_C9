#ifndef Defines_h__
#define Defines_h__

#include "../Default/stdafx.h"
using namespace std;

#include "Typedef.h"
#include "Constant.h"
#include "Macro.h"
#include "Function.h"
#include "Enum.h"
#include "Struct.h"


extern HINSTANCE g_hInst;
extern HWND  g_hWnd;
extern _bool g_bFocus;
extern _bool g_bSetAquire;
extern _bool g_bDebugRender;

//Mouse(for MapTool)
extern _vec3 g_vMouse;		//툴내 마우스
extern _vec3 g_vViewMouse;	//마우스와 메쉬충돌 포지션용
extern _vec3 g_vCamEye;		//카메라 아이

// Input Id
extern wstring g_wstrID;
extern bool	g_bInputID;

// Input PassWord
extern wstring g_wstrPW;
extern bool	g_bInputPW;

// Input NickName
extern wstring g_wstrNick;
extern bool g_bInputNick;


// Input SceneEnter
extern _bool g_bEnterToTown;
extern _bool g_bEnterToIce;
extern _bool g_bEnterToLoadingTown;
extern _bool g_bEnterToLoadingIce;
extern _bool g_bEnterToLoadingFire;
extern _bool g_bStageResource;

//Hanuman Dead
extern _bool g_bHanumanDead;
extern _bool g_bBardielDead;

extern _int	g_iPointLightIndex;

//Sound Value
extern _float g_fBGMValue;
extern _float g_fEffectSoundValue;

#endif // Defines_h__
