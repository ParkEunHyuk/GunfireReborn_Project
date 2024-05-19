#pragma once

#include "Base.h"

BEGIN(Engine)

class CFrustum_Manager final : public CBase
{
	DECLARE_SINGLETON(CFrustum_Manager)
private:
	CFrustum_Manager();
	virtual ~CFrustum_Manager() = default;
public:
	HRESULT Initialize();
	void Update();
	//HRESULT Transform_ToLocalSpace(class CTransform* pTransform); 지형컬링(CPU상의 정점컬링)

public:
	_bool isIn_WorldSpace(_fvector vPoint, _float fRange);
	//_bool isIn_LocalSpace(_fvector vPoint, _float fRange); 지형컬링(CPU상의 정점컬링)

private:
	_float3				m_vPoint[8];
	_float3				m_vPointInWorld[8];
	//_float3				m_vPointInLocal[8]; 지형컬링(CPU상의 정점컬링)

	_float4				m_PlaneInWorld[6];
	//_float4				m_PlaneInLocal[6]; 지형컬링(CPU상의 정점컬링)

private:
	/* 점 여덟개를 넣어주면 여섯개의 평면을 구해서 리턴해준다. */
	void Compute_Plane(const _float3* pPoints, _float4* pPlanes);


public:
	virtual void Free() override;
};

END