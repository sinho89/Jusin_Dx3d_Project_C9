#ifndef Mouse_h__
#define Mouse_h__

#include "Engine_Defines.h"
#include "Export_Engine.h"
#include "Defines.h"

class CMouse
{
public:
	static D3DXVECTOR3	m_vNowPos;
	static D3DXVECTOR3	m_vOldPos;

public:
	void Update()
	{
		m_vOldPos = m_vNowPos;
		m_vNowPos = GetMousePos();
	}
public:
	static float Get_DIMouseMove(Engine::ROTATION eType)
	{
		float fValue;
		if(eType == Engine::ROT_X)
		{
			fValue = m_vNowPos.x - m_vOldPos.x;
			return fValue;
		}
		else if(eType == Engine::ROT_Y)
		{
			fValue = m_vNowPos.y - m_vOldPos.y;
			return fValue;
		}
		return 0;
	}

	static D3DXVECTOR3 GetMousePos(void)
	{
		D3DXVECTOR3 vPos;

		POINT	ptMouse;
		GetCursorPos(&ptMouse);
		//ScreenToClient(g_hWnd, &ptMouse);
		//마우스사라지는함수
		//ShowCursor(false);

		vPos.x = (float)ptMouse.x;
		vPos.y = (float)ptMouse.y;
		vPos.z = 0.f;

		return vPos;
	}
public:
	CMouse(void);
	~CMouse(void);
};

#endif