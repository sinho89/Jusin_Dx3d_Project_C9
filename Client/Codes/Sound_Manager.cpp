#include "stdafx.h"
#include "Sound_Manager.h"

IMPLEMENT_SINGLETON(CSound_Manager)

CSound_Manager::CSound_Manager(void)
: m_pSystem(NULL)
, m_iVersion(0)
{
}

CSound_Manager::~CSound_Manager(void)
{
}

FMOD_SYSTEM * CSound_Manager::GetSystem(void)
{
	return m_pSystem;
}

FMOD_SOUND * CSound_Manager::GetSound(const wstring & wstrKey)
{
	MAPSOUND::iterator iter = m_mapSound.find(wstrKey);

	if (iter == m_mapSound.end())
		return NULL;

	return iter->second;
}

_bool CSound_Manager::ErrorCheck(FMOD_RESULT Result)
{
	if (Result != FMOD_OK)
		return true;

	return false;
}

void CSound_Manager::Ready_SoundManager(void)
{
	m_Result = FMOD_System_Create(&m_pSystem);
	ErrorCheck(m_Result);

	m_Result = FMOD_System_GetVersion(m_pSystem, &m_iVersion);
	ErrorCheck(m_Result);

	m_Result = FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL);
	ErrorCheck(m_Result);

	LoadSoundFile();
}

void CSound_Manager::LoadSoundFile(void)
{
	_finddata_t		fd;

	long handle;
	int iResult = 1;

	handle = _findfirst("../bin/Sound/*.*", &fd);

	if (handle == -1)
		return;

	while (iResult != -1)
	{
		TCHAR szName[256];

		ZeroMemory(szName, sizeof(szName));		//매크로 함수 : memset

		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED,
			fd.name, strlen(fd.name), szName, 256);
		//멀티 바이트를 유니코드로 변경해주는 함수.

		TCHAR* pName = new TCHAR[256];
		ZeroMemory(pName, sizeof(TCHAR) * 256);
		lstrcpy(pName, szName);					//유니코드 문자열 복사.

		char szFullName[256];

		strcpy_s(szFullName, "../bin/Sound/");
		strcat_s(szFullName, fd.name);

		FMOD_SOUND* pSound;

		m_Result = FMOD_System_CreateSound(m_pSystem, szFullName,
			FMOD_HARDWARE, 0, &pSound);
		if (m_Result == FMOD_OK)
		{
			//사운드파일 정상적인 완성!
			wstring wstrName = pName;
			m_mapSound.insert(make_pair(wstrName, pSound));
			
		}

		delete[] pName;

		iResult = _findnext(handle, &fd);


	}

	_findclose(handle);

	m_Result = FMOD_System_Update(m_pSystem);
	ErrorCheck(m_Result);
}

void CSound_Manager::Free(void)
{
	MAPSOUND::iterator iter = m_mapSound.begin();
	MAPSOUND::iterator iter_end = m_mapSound.end();

	for (iter; iter != iter_end; iter++)
	{
		m_Result = FMOD_Sound_Release(iter->second);
		ErrorCheck(m_Result);
	}

	m_mapSound.clear();

	m_Result = FMOD_System_Close(m_pSystem);
	ErrorCheck(m_Result);

	m_Result = FMOD_System_Release(m_pSystem);
	ErrorCheck(m_Result);
}
