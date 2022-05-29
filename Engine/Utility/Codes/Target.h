#ifndef Target_h__
#define Target_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CScreen_Texture;
class ENGINE_DLL CTarget
	: public CBase
{
private:
	explicit				CTarget(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual					~CTarget(void);

public:
	HRESULT					Ready_Target(const _uint& iSizeX,	//Ÿ�� ����
										const _uint& iSizeY,
										D3DFORMAT Format,
										D3DXCOLOR Color,
										RENDERTARGETID eRTID);

	HRESULT					Ready_DebugBuffer(const _uint& iX,
											const _uint& iY,
											const _uint& iSizeX,
											const _uint& iSizeY);	//����׸� ���� ���۵� ȭ�� ���� ����

	void					Render_DubugBuffer(void);	//���۵� ȭ�� ���ۿ� �׸��� �׸���.
	void					Clear_RenderTarget(void);	//�ش� Ÿ���� Ŭ���� �Ѵ�.(�� Ÿ�ٿ� ���ϴ� ����� Ŭ���� �ϱ� �����̴�.)
	void					SetUp_OnGraphicDev(const _ulong& dwIndex);	//����۸� �����ϰ� ����Ÿ���� ��ġ�� �ø���
	void					Release_OnGraphicDev(const _ulong& dwIndex);	//������ ����۸� �ٽ� ��ġ�� �ø���.
	HRESULT					SetUp_OnShader(LPD3DXEFFECT pEffect, const char* pConstantName);

	//Clear �Լ��� ��ġ�� �ö�� ��� Ÿ�ٵ��� ���� �ʱ�ȭ �Ѵ�.
private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;

	LPDIRECT3DTEXTURE9		m_pTargetTexture;	//���� Ÿ�� �ؽ���
	LPDIRECT3DSURFACE9		m_pTargetSurface;	//���� Ÿ�� ���ǽ�

	LPDIRECT3DSURFACE9		m_pOldSurface;		//���� �� ����.

	D3DXCOLOR				m_Color;			//���� Ÿ���� Ŭ���� �÷�.

	RENDERTARGETID			m_eRenderTargetID;	//���� Ÿ�� ���̵�.

private:
	CScreen_Texture*		m_pScreenTexture;	//�׸� ��.

public:
	static CTarget*			Create(LPDIRECT3DDEVICE9 pGraphicDev,
									const _uint& iSizeX,
									const _uint& iSizeY,
									D3DFORMAT Format,
									D3DXCOLOR Color,
									RENDERTARGETID eRTID);

public:
	LPDIRECT3DTEXTURE9		GetTargetTexture(void) { return m_pTargetTexture; }
	RENDERTARGETID&			GetRenderTargetID(void) { return m_eRenderTargetID; }

private:
	virtual void			Free(void);
};


END

#endif