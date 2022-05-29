#ifndef GameObject_h__
#define GameObject_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CComponent;

class ENGINE_DLL CGameObject 
	: public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const OBJECTSTATE& eState = STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CGameObject(void);

public:	//Getter
	template <typename T>
	CComponent*			GetComponent(COMPONENTID eID);
	_float				Get_ViewZ(void) const;
	OBJECTSTATE&		GetState(void);
	_bool				GetActive(void);
	_bool				GetLight(void);
	wstring&			GetName(void);
	_bool				GetStatic(void);
	_bool				GetLoading(void);

	const _uint			GetBlurPass(void) const { return m_iBlurPass; }
	const _int			GetBlurCnt(void) const { return m_iBlurCnt; }
	const _float		GetBlurPower(void) const { return m_fBlurPower; }
	const _float		GetBlurValue(void) const { return m_fBlurValue; }
	const BLURTYPE		GetBlurType(void) const { return m_eBlurType; }
	const _bool			GetOutlineCheck(void) const { return m_bOutlineCheck; }
	const _vec3			GetOutlineColor(void) const { return m_bOutlineColor; }
	const wstring		GetObjectName(void) const { return m_wstrName; }

	void				SetBlurPass(const _uint iBlurPass) { m_iBlurPass = iBlurPass; }
	void				SetBlurCnt(const _int iBlurCnt) { m_iBlurCnt = iBlurCnt; }
	void				SetBlurPower(const _float fBlurPower) { m_fBlurPower = fBlurPower; }
	void				SetBlurValue(const _float fBlurValue) { m_fBlurValue = fBlurValue; }
	void				SetBlurType(const BLURTYPE eBlurType) { m_eBlurType = eBlurType; }
	void				SetOutlineCheck(const _bool bOutlineCheck) { m_bOutlineCheck = bOutlineCheck; }
	void				SetOutlineColor(const _vec3 vColor) { m_bOutlineColor = vColor; }

	void				PlusBlurPower(const _float fBlurPower) { m_fBlurPower += fBlurPower; }

public: //Setter
	void				SetState(const OBJECTSTATE& eState);
	void				SetActive(const _bool& bIsActive);

public:
	virtual HRESULT		Start_ForScene(void);	//�� ���� ���� ���̾ ���� �� �ʿ䰡 ������ �ʱ�ȭ �Լ�.
	virtual void		Update_Object(const _float& fTimeDelta);
	virtual void		Render_Object(void);
	virtual	void		Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);	//����ȭ�� �����̴�.

protected:
	LPDIRECT3DDEVICE9	m_pGraphicDev;

protected:
	wstring				m_wstrName;				//��ü�� ���� �̸�
	OBJECTSTATE			m_eState;				//������Ʈ ���� (�׾����� ��Ҵ���)
	_bool				m_bIsActive;			//��ü�� Ȱ��ȭ ����
	_bool				m_bIsStatic;			//�� �̵� �Ҷ� ������� �������� �Ǵ�
	_float				m_fViewZ;
	_bool				m_bLight;				// UI ��Ȱ��ȭ���ú���
	_bool				m_bLoading;				// Loading �� ��ü ���� ����
	_bool				m_bOnceStart;			// Start For Scene �ѹ� ȣ��ǰ� ���� �뵵(�˾Ƽ� ���ϴ���ġ�� ���ƿ�) �ʱⰪ false
	_bool				m_bOutlineDraw;			//�ܰ����� �׸� ��?

	_uint				m_iBlurPass;			//Effect Shader�� Pass
	_int				m_iBlurCnt;				//���� Ƚ��
	_float				m_fBlurPower;			//���� POWER
	_float				m_fBlurValue;			//���� �� ����?
	BLURTYPE			m_eBlurType;			//���� Ÿ��

	_bool				m_bOutlineCheck;		//�ܰ����� �׸� ���ΰ� �� ���ΰ�?
	_vec3				m_bOutlineColor;		//�ܰ��� �÷�.

protected:
	void				Compute_ViewZ(const _vec3* pPosition);

protected:
	map<const _tchar*, CComponent*>				m_mapComponent[COMID_END];
	typedef map<const _tchar*, CComponent*>		MAPCOMPONENT;

public:
	virtual void Free(void);
};

template <typename T>
CComponent* Engine::CGameObject::GetComponent(COMPONENTID eID)
{
	MAPCOMPONENT::iterator iter = m_mapComponent[eID].begin();
	MAPCOMPONENT::iterator iter_end = m_mapComponent[eID].end();

	for(iter; iter != iter_end; iter++)
	{
		if(dynamic_cast<T*>((iter->second)))
		{
			iter->second->AddRef();
			return iter->second;
		}
	}
	return NULL;
}

END

#endif // GameObject_h__
