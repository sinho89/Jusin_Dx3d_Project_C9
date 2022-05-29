#ifndef Scene_Logo_h__
#define Scene_Logo_h__

#include "Defines.h"
#include "Scene.h"

namespace Engine
{
	class CCamera;
}

class CMediaUI;
class CLoading;
class CScene_Logo 
	: public Engine::CScene
{
private:
	explicit			CScene_Logo(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual				~CScene_Logo(void);

public:
	virtual HRESULT		Ready_Scene(void);
	virtual HRESULT		Start_ForScene(void);	//�� ���� ���� ���̾ ���� �� �� �ʿ��� �ʱ�ȭ �Լ�.
	virtual void		Update_Scene(const _float& fTimeDelta);
	virtual void		Render_Scene(void);

private:
	HRESULT 			SetUp_DefaultSetting(void);
	HRESULT 			Ready_Layer_GameLogic(void);
	HRESULT 			Ready_Resource(void);

private:
	void				SkipScene(const _float& fTimeDelta);
private:
	_float				m_fSkipTime;

	CMediaUI*			m_pOpenningMedia;	
	//���ҽ� �ε��� ���� �ӽ� �ε�(���߿� ���� ��Ź) : ������
	CLoading*			m_pLoading;

public:
	static CScene_Logo* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void		Free(void);
};

#endif // Scene_Logo_h__
