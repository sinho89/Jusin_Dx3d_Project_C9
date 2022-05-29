/*!
 * \file CameraMgr.h
 * \date 2016/11/21 22:37
 *
 * \author WINTER
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef CameraMgr_h__
#define CameraMgr_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CCamera;
class ENGINE_DLL CCameraMgr
	: public CBase
{
public:
	DECLARE_SINGLETON(CCameraMgr)

private:
	explicit		CCameraMgr(void);
	virtual			~CCameraMgr(void);

public:
	CCamera*		GetCurrentCamera(void);					//현재 작동하는 카메라를 가져온다.
	CCamera*		GetCamera(WORD eCameraID);				//원하는 카메라를 가져온다.
	void			DefineCurrentCamera(WORD eCameraID);	//현재 작동할 카메라를 지정한다.
	WORD&			GetCurrentCameraID(void);				//현재 작동하는 카메라의 타입을 전달한다.
	void			Set_Shake(const WORD& eCameraID, const _vec3& vShakeDir, const _float& fPower = 0.2f, const _int& iCnt = 5, const _int& iDelay = 1);

public:
	HRESULT			ReadyCamera(CCamera* pCamera, WORD eCameraID);	//카메라를 맵 컨테이너에 넣는다.

public:
	map<WORD, CCamera*>			m_mapCamera;
	typedef map<WORD, CCamera*>	MAPCAMERA;

private:
	CCamera*		m_pCurrentCamera;
	WORD			m_eCameraID;

private:
	virtual void	Free(void);
};

END

#endif // CameraMgr_h__