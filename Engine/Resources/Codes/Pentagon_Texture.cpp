#include "Pentagon_Texture.h"

USING(Engine)

Engine::CPentagon_Texture::CPentagon_Texture(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CVIBuffer(pGraphicDev)
{

}
Engine::CPentagon_Texture::CPentagon_Texture(const CPentagon_Texture& rhs)
: Engine::CVIBuffer(rhs)
{

}

Engine::CPentagon_Texture::~CPentagon_Texture(void)
{

}

HRESULT Engine::CPentagon_Texture::Ready_Buffer(void)
{
	m_isCreateVtxBuffer = true;
	m_dwVtxCnt = 7;
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	m_isCreateIdxBuffer = true;
	m_dwTriCnt = 5;
	m_dwIdxSize = sizeof(INDEX16) * m_dwTriCnt;
	m_IndexFmt = D3DFMT_INDEX16;

	if(FAILED(Engine::CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	m_bVtxMove = false;
	m_bVtxMoveAcc = false;
	m_fTime = 0.f;
	SetPentagon();
	// 할당된 메모리에 접근을 하자.
	VTXTEX*				pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = m_vPos[PENTAGON_CENTER];
	pVertex[0].vTexUV =  m_vUV[PENTAGON_CENTER];

	pVertex[1].vPosition = m_vPos[PENTAGON_INT];
	pVertex[1].vTexUV = m_vUV[PENTAGON_INT];

	pVertex[2].vPosition = m_vPos[PENTAGON_CON];
	pVertex[2].vTexUV = m_vUV[PENTAGON_CON];

	pVertex[3].vPosition = m_vPos[PENTAGON_STR];
	pVertex[3].vTexUV = m_vUV[PENTAGON_STR];

	pVertex[4].vPosition = m_vPos[PENTAGON_LEVEL];
	pVertex[4].vTexUV = m_vUV[PENTAGON_LEVEL];

	pVertex[5].vPosition = m_vPos[PENTAGON_WIS];
	pVertex[5].vTexUV = m_vUV[PENTAGON_WIS];

	pVertex[6].vPosition = m_vPos[PENTAGON_INT];
	pVertex[6].vTexUV = m_vUV[PENTAGON_INT];

	m_pVB->Unlock();


	return S_OK;
}
_bool Engine::CPentagon_Texture::GetVtxMove()
{
	return m_bVtxMove;
}

void Engine::CPentagon_Texture::SetVtxMove(_bool bVtxMove)
{
	m_bVtxMove = bVtxMove;
}

HRESULT Engine::CPentagon_Texture::SetPentagon(_uint iIndex /* =0 */, _uint iBeforeIndex /* =0 */)
{
	m_bVtxMove = false;
	m_bVtxMoveAcc = false;
	m_fIncreaseTime = 0.f;
	m_fTime = 0.f;

	m_vPos[PENTAGON_CENTER] = _vec3(0.f, 0.f, 0.f);
	m_vUV[PENTAGON_CENTER]  = _vec2(0.f, 0.f);
	m_vPos[PENTAGON_INT] = _vec3(0.7f, -0.93f, 0.f);
	m_vUV[PENTAGON_INT] = _vec2(1.f, 0.f);
	m_vPos[PENTAGON_CON] = _vec3(-0.7f, -0.93f, 0.f);
	m_vUV[PENTAGON_CON] = _vec2(1.f, 1.f);
	m_vPos[PENTAGON_STR] = _vec3(-1.1f, 0.275f, 0.f);
	m_vUV[PENTAGON_STR] = _vec2(1.f, 0.f);
	m_vPos[PENTAGON_LEVEL] = _vec3(0.f, 1.f, 0.f);
	m_vUV[PENTAGON_LEVEL]  = _vec2(1.f, 1.f);
	m_vPos[PENTAGON_WIS] = _vec3(1.1f, 0.275f, 0.f);
	m_vUV[PENTAGON_WIS] = _vec2(1.f, 0.f);

	Calculater_FromCenter_Length();

	// 파이터
	m_vFighterPos[PENTAGON_CENTER] = m_vPos[PENTAGON_CENTER];

	m_vFighterPos[PENTAGON_INT] = m_vPos[PENTAGON_INT];
	m_vFighterPos[PENTAGON_INT] += m_vDir[PENTAGON_INT] * 0.6f;

	m_vFighterPos[PENTAGON_CON] = m_vPos[PENTAGON_CON]; 

	m_vFighterPos[PENTAGON_STR] = m_vPos[PENTAGON_STR]; 

	m_vFighterPos[PENTAGON_LEVEL] = m_vPos[PENTAGON_LEVEL];
	m_vFighterPos[PENTAGON_LEVEL] += m_vDir[PENTAGON_LEVEL] * 0.5f;

	m_vFighterPos[PENTAGON_WIS] = m_vPos[PENTAGON_WIS];
	m_vFighterPos[PENTAGON_WIS] += m_vDir[PENTAGON_WIS] * 0.9f;

	//헌터
	m_vHunterPos[PENTAGON_CENTER] = m_vPos[PENTAGON_CENTER];

	m_vHunterPos[PENTAGON_INT] = m_vPos[PENTAGON_INT];
	m_vHunterPos[PENTAGON_INT] += m_vDir[PENTAGON_INT] * 0.3f;

	m_vHunterPos[PENTAGON_CON] = m_vPos[PENTAGON_CON];
	m_vHunterPos[PENTAGON_CON] += m_vDir[PENTAGON_CON] * 0.3f; 

	m_vHunterPos[PENTAGON_STR] = m_vPos[PENTAGON_STR]; 
	m_vHunterPos[PENTAGON_STR] += m_vDir[PENTAGON_STR] * 0.3f; 

	m_vHunterPos[PENTAGON_LEVEL] = m_vPos[PENTAGON_LEVEL];

	m_vHunterPos[PENTAGON_WIS] = m_vPos[PENTAGON_WIS];
	m_vHunterPos[PENTAGON_WIS] += m_vDir[PENTAGON_WIS] * 0.7f;

	//샤먼
	m_vShamanPos[PENTAGON_CENTER] = m_vPos[PENTAGON_CENTER];

	m_vShamanPos[PENTAGON_INT] = m_vPos[PENTAGON_INT];
	m_vShamanPos[PENTAGON_INT] += m_vDir[PENTAGON_INT] * 0.3f;

	m_vShamanPos[PENTAGON_CON] = m_vPos[PENTAGON_CON];
	m_vShamanPos[PENTAGON_CON] += m_vDir[PENTAGON_CON] * 0.5f; 

	m_vShamanPos[PENTAGON_STR] = m_vPos[PENTAGON_STR]; 
	m_vShamanPos[PENTAGON_STR] += m_vDir[PENTAGON_STR] * 0.5f; 

	m_vShamanPos[PENTAGON_LEVEL] = m_vPos[PENTAGON_LEVEL];
	m_vShamanPos[PENTAGON_LEVEL] += m_vDir[PENTAGON_LEVEL] * 0.7f; 

	m_vShamanPos[PENTAGON_WIS] = m_vPos[PENTAGON_WIS];

	//위치 블레이드
	m_vWitchPos[PENTAGON_CENTER] = m_vPos[PENTAGON_CENTER];

	m_vWitchPos[PENTAGON_INT] = m_vPos[PENTAGON_INT];
	m_vWitchPos[PENTAGON_INT] += m_vDir[PENTAGON_INT] * 0.5f;

	m_vWitchPos[PENTAGON_CON] = m_vPos[PENTAGON_CON];
	m_vWitchPos[PENTAGON_CON] += m_vDir[PENTAGON_CON] * 0.7f; 

	m_vWitchPos[PENTAGON_STR] = m_vPos[PENTAGON_STR]; 
	m_vWitchPos[PENTAGON_STR] += m_vDir[PENTAGON_STR] * 0.2f; 

	m_vWitchPos[PENTAGON_LEVEL] = m_vPos[PENTAGON_LEVEL];
	m_vWitchPos[PENTAGON_LEVEL] += m_vDir[PENTAGON_LEVEL] * 0.3f; 

	m_vWitchPos[PENTAGON_WIS] = m_vPos[PENTAGON_WIS];
	m_vWitchPos[PENTAGON_WIS] += m_vDir[PENTAGON_WIS] * 0.2f; 

	//미스틱
	m_vMisticPos[PENTAGON_CENTER] = m_vPos[PENTAGON_CENTER];

	m_vMisticPos[PENTAGON_INT] = m_vPos[PENTAGON_INT];
	m_vMisticPos[PENTAGON_INT] += m_vDir[PENTAGON_INT] * 0.6f;

	m_vMisticPos[PENTAGON_CON] = m_vPos[PENTAGON_CON];
	m_vMisticPos[PENTAGON_CON] += m_vDir[PENTAGON_CON] * 0.3f; 

	m_vMisticPos[PENTAGON_STR] = m_vPos[PENTAGON_STR]; 

	m_vMisticPos[PENTAGON_LEVEL] = m_vPos[PENTAGON_LEVEL];
	m_vMisticPos[PENTAGON_LEVEL] += m_vDir[PENTAGON_LEVEL] * 0.5f; 

	m_vMisticPos[PENTAGON_WIS] = m_vPos[PENTAGON_WIS];
	m_vMisticPos[PENTAGON_WIS] += m_vDir[PENTAGON_WIS] * 0.6f; 


	switch(iBeforeIndex)
	{
	case 0: // Fighter
		m_vPos[PENTAGON_INT] = m_vFighterPos[PENTAGON_INT];
		m_vPos[PENTAGON_CON] = m_vFighterPos[PENTAGON_CON];
		m_vPos[PENTAGON_STR] = m_vFighterPos[PENTAGON_STR];
		m_vPos[PENTAGON_LEVEL] = m_vFighterPos[PENTAGON_LEVEL];
		m_vPos[PENTAGON_WIS] = m_vFighterPos[PENTAGON_WIS];
		break;
	case 1: // Hunter
		m_vPos[PENTAGON_INT] = m_vHunterPos[PENTAGON_INT];
		m_vPos[PENTAGON_CON] = m_vHunterPos[PENTAGON_CON];
		m_vPos[PENTAGON_STR] = m_vHunterPos[PENTAGON_STR];
		m_vPos[PENTAGON_LEVEL] = m_vHunterPos[PENTAGON_LEVEL];
		m_vPos[PENTAGON_WIS] = m_vHunterPos[PENTAGON_WIS];
		break;
	case 2: // Shaman
		m_vPos[PENTAGON_INT] = m_vShamanPos[PENTAGON_INT];
		m_vPos[PENTAGON_CON] = m_vShamanPos[PENTAGON_CON];
		m_vPos[PENTAGON_STR] = m_vShamanPos[PENTAGON_STR];
		m_vPos[PENTAGON_LEVEL] = m_vShamanPos[PENTAGON_LEVEL];
		m_vPos[PENTAGON_WIS] = m_vShamanPos[PENTAGON_WIS];
		break;
	case 3: // Witch
		m_vPos[PENTAGON_INT] = m_vWitchPos[PENTAGON_INT];
		m_vPos[PENTAGON_CON] = m_vWitchPos[PENTAGON_CON];
		m_vPos[PENTAGON_STR] = m_vWitchPos[PENTAGON_STR];
		m_vPos[PENTAGON_LEVEL] = m_vWitchPos[PENTAGON_LEVEL];
		m_vPos[PENTAGON_WIS] = m_vWitchPos[PENTAGON_WIS];
		break;
	case 4: // Mistic
		m_vPos[PENTAGON_INT] = m_vMisticPos[PENTAGON_INT];
		m_vPos[PENTAGON_CON] = m_vMisticPos[PENTAGON_CON];
		m_vPos[PENTAGON_STR] = m_vMisticPos[PENTAGON_STR];
		m_vPos[PENTAGON_LEVEL] = m_vMisticPos[PENTAGON_LEVEL];
		m_vPos[PENTAGON_WIS] = m_vMisticPos[PENTAGON_WIS];
		break;
	}

	VTXTEX*				pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = m_vPos[PENTAGON_CENTER];
	pVertex[0].vTexUV =  m_vUV[PENTAGON_CENTER];

	pVertex[1].vPosition = m_vPos[PENTAGON_INT];
	pVertex[1].vTexUV = m_vUV[PENTAGON_INT];

	pVertex[2].vPosition = m_vPos[PENTAGON_CON];
	pVertex[2].vTexUV = m_vUV[PENTAGON_CON];

	pVertex[3].vPosition = m_vPos[PENTAGON_STR];
	pVertex[3].vTexUV = m_vUV[PENTAGON_STR];

	pVertex[4].vPosition = m_vPos[PENTAGON_LEVEL];
	pVertex[4].vTexUV = m_vUV[PENTAGON_LEVEL];

	pVertex[5].vPosition = m_vPos[PENTAGON_WIS];
	pVertex[5].vTexUV = m_vUV[PENTAGON_WIS];

	pVertex[6].vPosition = m_vPos[PENTAGON_INT];
	pVertex[6].vTexUV = m_vUV[PENTAGON_INT];

	m_pVB->Unlock();

	return S_OK;
}

void Engine::CPentagon_Texture::Pentagon_Update(_uint iIndex, _uint iBeforeIndex, const _float& fTime)
{
	if(m_bVtxMoveAcc)
	{
		m_fTime += fTime;
		int iMoveCount = 0;

		for(int i = 1; i < PENTAGON_END; ++i)
		{
			_vec3 vLength;
			
			switch(iIndex)
			{
			case 0:
				vLength = m_vFighterPos[i] - m_vPos[i];
				break;
			case 1:
				vLength = m_vHunterPos[i] - m_vPos[i];
				break;
			case 2:
				vLength = m_vShamanPos[i] - m_vPos[i];
				break;
			case 3:
				vLength = m_vWitchPos[i] - m_vPos[i];
				break;
			case 4:
				vLength = m_vMisticPos[i] - m_vPos[i];
				break;
			}

			_float fDistance = D3DXVec3Length(&vLength);

			if(fDistance <= 0.05)
			{
				switch(iIndex)
				{
				case 0:
					m_vPos[i] = m_vFighterPos[i];
					break;
				case 1:
					m_vPos[i] = m_vHunterPos[i];
					break;
				case 2:
					m_vPos[i] = m_vShamanPos[i];
					break;
				case 3:
					m_vPos[i] = m_vWitchPos[i];
					break;
				case 4:
					m_vPos[i] = m_vMisticPos[i];
					break;
				}

				++iMoveCount;
			}
			else
				m_vPos[i] += m_vDir[i] * m_fTime * 0.1f * -1.f;
		}
		if(iMoveCount == 5)
		{
			m_bVtxMoveAcc = false;
			m_fTime = 0.f;
		}

		iMoveCount = 0;

		VTXTEX*				pVertex = NULL;

		m_pVB->Lock(0, 0, (void**)&pVertex, 0);

		pVertex[0].vPosition = m_vPos[PENTAGON_CENTER];
		pVertex[0].vTexUV =  m_vUV[PENTAGON_CENTER];

		pVertex[1].vPosition = m_vPos[PENTAGON_INT];
		pVertex[1].vTexUV = m_vUV[PENTAGON_INT];

		pVertex[2].vPosition = m_vPos[PENTAGON_CON];
		pVertex[2].vTexUV = m_vUV[PENTAGON_CON];

		pVertex[3].vPosition = m_vPos[PENTAGON_STR];
		pVertex[3].vTexUV = m_vUV[PENTAGON_STR];

		pVertex[4].vPosition = m_vPos[PENTAGON_LEVEL];
		pVertex[4].vTexUV = m_vUV[PENTAGON_LEVEL];

		pVertex[5].vPosition = m_vPos[PENTAGON_WIS];
		pVertex[5].vTexUV = m_vUV[PENTAGON_WIS];

		pVertex[6].vPosition = m_vPos[PENTAGON_INT];
		pVertex[6].vTexUV = m_vUV[PENTAGON_INT];

		m_pVB->Unlock();

	}
	if(m_bVtxMove)
	{
		int iMoveCount = 0;
		m_fTime += fTime;

		for(int i = 1; i < PENTAGON_END; ++i)
		{
			_vec3 vLength = m_vPos[PENTAGON_CENTER] - m_vPos[i];
			_float fDistance = D3DXVec3Length(&vLength);

			if(fDistance <= 0.05)
			{
				m_vPos[i] = m_vPos[PENTAGON_CENTER];
				++iMoveCount;
			}
			else
				m_vPos[i] += m_vDir[i] * m_fTime * 0.1f;
		}

		if(iMoveCount == 5)
		{
			m_bVtxMoveAcc = true;
			m_bVtxMove = false;
			m_fTime = 0.f;
		}
		iMoveCount = 0;
		VTXTEX*				pVertex = NULL;

		m_pVB->Lock(0, 0, (void**)&pVertex, 0);

		pVertex[0].vPosition = m_vPos[PENTAGON_CENTER];
		pVertex[0].vTexUV =  m_vUV[PENTAGON_CENTER];

		pVertex[1].vPosition = m_vPos[PENTAGON_INT];
		pVertex[1].vTexUV = m_vUV[PENTAGON_INT];

		pVertex[2].vPosition = m_vPos[PENTAGON_CON];
		pVertex[2].vTexUV = m_vUV[PENTAGON_CON];

		pVertex[3].vPosition = m_vPos[PENTAGON_STR];
		pVertex[3].vTexUV = m_vUV[PENTAGON_STR];

		pVertex[4].vPosition = m_vPos[PENTAGON_LEVEL];
		pVertex[4].vTexUV = m_vUV[PENTAGON_LEVEL];

		pVertex[5].vPosition = m_vPos[PENTAGON_WIS];
		pVertex[5].vTexUV = m_vUV[PENTAGON_WIS];

		pVertex[6].vPosition = m_vPos[PENTAGON_INT];
		pVertex[6].vTexUV = m_vUV[PENTAGON_INT];

		m_pVB->Unlock();

	}
	
	for(int i = 1; i < PENTAGON_END; ++i)
	{
		_vec3 vLength = m_vPos[PENTAGON_CENTER] - m_vPos[i];
		_float fDistance = D3DXVec3Length(&vLength);

		if(fDistance >= 0.f && fDistance <= 0.33f)
		{
			m_vUV[i].x = 0.33f;
		}
		if(fDistance > 0.33f && fDistance <= 0.66f)
		{
			m_vUV[i].x = 0.66f;
		}
		if(fDistance > 0.66f && fDistance <= 1.f)
		{
			m_vUV[i].x = 1.f;
		}
	}
}

void Engine::CPentagon_Texture::Calculater_FromCenter_Length()
{
	m_vDir[PENTAGON_INT] = (m_vPos[PENTAGON_CENTER] - m_vPos[PENTAGON_INT]);
	D3DXVec3Normalize(&m_vDir[PENTAGON_INT], &m_vDir[PENTAGON_INT]);
	m_vDir[PENTAGON_CON] = (m_vPos[PENTAGON_CENTER] - m_vPos[PENTAGON_CON]);
	D3DXVec3Normalize(&m_vDir[PENTAGON_CON], &m_vDir[PENTAGON_CON]);
	m_vDir[PENTAGON_STR] = (m_vPos[PENTAGON_CENTER] - m_vPos[PENTAGON_STR]);
	D3DXVec3Normalize(&m_vDir[PENTAGON_STR], &m_vDir[PENTAGON_STR]);
	m_vDir[PENTAGON_LEVEL] = (m_vPos[PENTAGON_CENTER] - m_vPos[PENTAGON_LEVEL]);
	D3DXVec3Normalize(&m_vDir[PENTAGON_LEVEL], &m_vDir[PENTAGON_LEVEL]);
	m_vDir[PENTAGON_WIS] = (m_vPos[PENTAGON_CENTER] - m_vPos[PENTAGON_WIS]);
	D3DXVec3Normalize(&m_vDir[PENTAGON_WIS], &m_vDir[PENTAGON_WIS]);

}

void Engine::CPentagon_Texture::Render_Buffer(void)
{
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pGraphicDev->SetFVF(m_dwVtxFVF);
	m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 5);
}

CPentagon_Texture* Engine::CPentagon_Texture::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPentagon_Texture*		pInstance = new CPentagon_Texture(pGraphicDev);

	if(FAILED(pInstance->Ready_Buffer()))
	{
		MSG_BOX("CRect_Texture Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

CResource* Engine::CPentagon_Texture::Clone(void)
{
	return new CPentagon_Texture(*this);
}

void Engine::CPentagon_Texture::Free(void)
{
	Engine::CVIBuffer::Free();



}

