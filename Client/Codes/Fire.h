#ifndef Fire_h__
#define Fire_h__


#include "Defines.h"
#include "Effect.h"

class CFire
	: public Engine::CEffect
{
private:
	explicit		CFire(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual			~CFire(void);

public:
	virtual void	Update_Object(const _float& fTimeDelta);
	virtual void	Render_Object(void);

private:
	HRESULT			Ready_Object(void);


public:
	static CFire*	Create(LPDIRECT3DDEVICE9 pGraphicDev
						, const wstring& wstrName);


private:
	virtual			void Free(void);
	HRESULT			Add_Component(void);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	void			SetPart(_int nIdx);
	void			MoveUpdate(const _float& fTimeDelta);
	void			Update_RePart(void);


	static CFire*	LoadCreate(LPDIRECT3DDEVICE9 pGraphicDev
								, const wstring& wstrName);


	void			AddInitSetPart(_vec3* pos);

	void			SetInitAnimation(void);


	
	vector<_vec3>*	GetVecPos(void);

private:
	HRESULT		    LoadReady_Object(void);
	_int			m_iCount;
	vector<_vec3>	m_vecPos;

};





#endif // Fire_h__
