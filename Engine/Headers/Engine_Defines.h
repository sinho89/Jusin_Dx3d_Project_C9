#ifndef Engine_Defines_h__
#define Engine_Defines_h__

#pragma warning (disable : 4251)

#include <d3dx9.h>
#include <d3d9.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <map>
#include <vector>
#include <list>
#include <algorithm>
#include <string>
#include <iostream>
#include <tchar.h>
//#include <windows.h>
//#include <synchapi.h>
using namespace std;

//boost
#include <unordered_map.hpp>
#include <timer.hpp>

using namespace boost;

#ifndef __TOOL__
#define __TOOL__
//DirectShow-----------------------------------------------
#include <streams.h>
#pragma comment(lib, "winmm.lib")

#ifdef _DEBUG
#pragma comment(lib, "Strmbasd.lib")
#else
#pragma comment(lib, "Strmbase.lib")
#endif
#pragma comment(lib, "Strmiids.lib")


#endif
#pragma warning(disable : 4099)

//FMOD
#include <io.h>
#include "fmod.h"
#include "fmod.hpp"
#include "fmod_dsp.h"
#include "fmod_errors.h"

//DirectShow-----------------------------------------------
//#include <streams.h>
//#pragma comment(lib, "winmm.lib")
//
//#ifdef _DEBUG
//#pragma comment(lib, "Strmbasd.lib")
//#else
//#pragma comment(lib, "Strmbase.lib")
//#endif
//#pragma comment(lib, "Strmiids.lib")
//
//#pragma warning(disable : 4099)
//DShow End------------------------------------------------

#include "Engine_Enum.h"
#include "Engine_Typedef.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_Function.h"
#include "Engine_Functor.h"

#endif // Defines_h__
