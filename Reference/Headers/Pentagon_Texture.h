#ifndef Pentagon_Texture_h__
#define Pentagon_Texture_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CPentagon_Texture 
	: public CVIBuffer
{
private:
	explicit CPentagon_Texture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPentagon_Texture(const CPentagon_Texture& rhs);
	virtual ~CPentagon_Texture(void);
public:
	HRESULT Ready_Buffer(void);
public:
	_bool	GetVtxMove();
public:
	void	SetVtxMove(_bool bVtxMove);
private:
	_vec3	m_vPos[PENTAGON_END];
	_vec2	m_vUV[PENTAGON_END];
	_vec3	m_vDir[PENTAGON_END];

	_vec3	m_vFighterPos[PENTAGON_END];
	_vec3	m_vHunterPos[PENTAGON_END];
	_vec3	m_vShamanPos[PENTAGON_END];
	_vec3	m_vWitchPos[PENTAGON_END];
	_vec3	m_vMisticPos[PENTAGON_END];


	_bool	m_bVtxMove;
	_bool	m_bVtxMoveAcc;
	_float	m_fUVDistance[PENTAGON_END];
	_float	m_fTime;
	_float	m_fIncreaseTime;

public:
	HRESULT SetPentagon(_uint iIndex = 0, _uint iBeforeIndex = 0);
	void	Pentagon_Update(_uint iIndex, _uint iBeforeIndex, const _float& fTime);
	void	Calculater_FromCenter_Length();
public:
	virtual void Render_Buffer(void);
public:
	static CPentagon_Texture* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CResource* Clone(void);
	virtual void Free(void);
};

END

#endif // Pentagon_Texture_h__
