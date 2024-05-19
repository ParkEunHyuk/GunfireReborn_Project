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
	//HRESULT Transform_ToLocalSpace(class CTransform* pTransform); �����ø�(CPU���� �����ø�)

public:
	_bool isIn_WorldSpace(_fvector vPoint, _float fRange);
	//_bool isIn_LocalSpace(_fvector vPoint, _float fRange); �����ø�(CPU���� �����ø�)

private:
	_float3				m_vPoint[8];
	_float3				m_vPointInWorld[8];
	//_float3				m_vPointInLocal[8]; �����ø�(CPU���� �����ø�)

	_float4				m_PlaneInWorld[6];
	//_float4				m_PlaneInLocal[6]; �����ø�(CPU���� �����ø�)

private:
	/* �� �������� �־��ָ� �������� ����� ���ؼ� �������ش�. */
	void Compute_Plane(const _float3* pPoints, _float4* pPlanes);


public:
	virtual void Free() override;
};

END