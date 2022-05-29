#ifndef Transform_h__
#define Transform_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform
	: public CComponent
{
private:
	explicit			CTransform(void);
	virtual 			~CTransform(void);
public:
	const _vec3*		Get_Infomation(INFO eType, _vec3* pInfomation);
	const _vec3*		Get_Scale(_vec3* pScale);
	const _vec3*		Get_OutlineScale(_vec3* pScale);
	const _vec3*		Get_Angle(_vec3* pAngle);
	void				Get_WorldMatrix(_matrix* pWorldMatrix) const;
	const _matrix*		Get_WorldMatrixPointer(void) const;
	const _matrix*		Get_NRotWorldMatrix(_matrix* pWorldMatrix) const;
	const _matrix*		Get_NScaleWorldMatrix(_matrix* pWorldMatrix) const;

public:
	const _vec3 		Get_Now_Position(void)			const { return m_vInfo[INFO_POSITION]; }
	const _vec3 		Get_Now_Look(void)				const { return m_vInfo[INFO_LOOK]; }
	const _vec3			Get_Now_Right(void)				const { return m_vInfo[INFO_RIGHT]; }
	const _vec3			Get_Now_Up(void)				const { return m_vInfo[INFO_UP]; }
	const _vec3			Get_Now_Angle(void)				const { return m_vAngle; }
	const _matrix		Get_Now_ScaleRotMatrix(void)	const { return m_matScaleRot; }
	const _matrix		Get_Now_OutlineMatrix(void)		const { return m_matOutlineWorld; }

public:
	void				Set_Angle(const _vec3* fAngle);
public:
	void 				Set_Transform(LPDIRECT3DDEVICE9 pGraphicDev) const;
	void 				Set_Scale(const _float& fX, const _float& fY, const _float& fZ);
	void 				Set_OutlineScale(const _float& fX, const _float& fY, const _float& fZ);
	void				Add_Scale(const _float& size);
	void				Add_NotYScale(const _float& size);
	void 				Set_Position(const _float& fX, const _float& fY, const _float& fZ);
	void 				Set_Position(const _vec3* pPosition);
	void				Set_AccMotion(const _vec3* pAccMotion);
	void				Set_Gravity(const _float& fPosY);
	void 				Set_ParentMatrix(const _matrix* pParentMatrix);
	void 				Set_WorldMatrix(const _matrix* pWorldMatrix);
public:
	HRESULT				Ready_Transform(void);
	virtual void		Update_Component(const _float& fTimeDelta);
public:
	void 				Move_Position(const _vec3* const pDirection);
	void 				Move_TargetPos(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta);	
	void				Move_Forward(const _float& fSpeed, const _float& fTimeDelta);
	void 				Move_Right(const _vec3* pDir, const _float& fSpeed, const _float& fTimeDelta);
	void 				Move_Left(const _vec3* pDir, const _float& fSpeed, const _float& fTimeDelta);
	void 				Rotation(ROTATION eType, const _float& fAngle);
	void 				Chase_Target(const _vec3* pTargetPos, const _float& fSpeed);
	const _matrix*		Compute_LookAtTarget(const _vec3* pTargetPos);
private:
	_vec3				m_vInfo[INFO_END];
	_vec3				m_vScale;
	_vec3				m_vOutlineScale;
	_vec3				m_vAngle;

	_matrix				m_matWorld;
	_matrix				m_matNRotWorld;
	_matrix				m_matNScaleWorld;
	_matrix				m_matScaleRot;
	_matrix				m_matOutlineWorld;
public:
	static CTransform*	Create(void);
private:
	virtual void		Free(void);
};

END

#endif // Transform_h__
