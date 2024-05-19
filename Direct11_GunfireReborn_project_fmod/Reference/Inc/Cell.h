#pragma once
#include "Base.h"

BEGIN(Engine)

class CVIBuffer_Triangle;
class CShader;
class CCollider;


class ENGINE_DLL CCell final : public CBase
{
public:
	enum POINTS { POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINES { LINE_AB, LINE_BC, LINE_CA, LINE_END };
private:
	CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CCell() = default;

public:
	_uint Get_Index() {
		return m_iIndex;
	}

	_vector Get_Point(POINTS ePoint) {
		return XMLoadFloat3(&m_vPoints[ePoint]);
	}

	_float3* Get_Points() {
		return m_vPoints;
	}

public:
	void Set_NeighborIndex(LINES eLine, _int iNeighborIndex) {
		m_iNeighbor[eLine] = iNeighborIndex;
	}

public:
	HRESULT NativeConstruct(const _float3* pPoints, _uint iIndex);

#ifdef _DEBUG
public:
	HRESULT Update(_fmatrix WorldMatrix);
	//HRESULT Render(CVIBuffer_Triangle* pVIBuffer, CShader* pShader, _float4 vColor);
	HRESULT	Create_Collider(_uint iLevelIndex, const _tchar* pComponentTag);
	_bool	Check_Collider(_float3* pRayDirInLocal, _float3* pRayPosInLocal, _float3* pPoint);
	void	Move_Collider(_float3* pRayDirInLocal, _float3* pRayPosInLocal, _float3 vPoint);
	void	Move_Collider(_float3* pRayDirInLocal, _float3* pRayPosInLocal, _float fY);
#endif // _DEBUG

public:
	_bool Compare_Points(_fvector vSourPoint, _fvector vDestPoint);
	_bool isIn(_fvector vPosition, _int* pNeighborIndex, _float3* pLineDir = nullptr);

#ifdef _DEBUG
public:
	HRESULT Render(class CVIBuffer_Triangle* pVIBuffer, class CShader* pShader, _float4	vColor);
#endif // _DEBUG


private:
	ID3D11Device*				m_pDevice = nullptr;
	ID3D11DeviceContext*		m_pDeviceContext = nullptr;

#ifdef _DEBUG
private:
	CCollider*		m_pColliderCom[POINT_END] = { nullptr };
#endif // _DEBUG

private:
	_int			m_iNeighbor[LINE_END] = { -1, -1, -1 };
	_float3			m_vPoints[POINT_END];
	_float3			m_vLineDir[LINE_END];
	_uint			m_iIndex = 0;

public:
	static CCell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _float3* pPoints, _uint iIndex);
	virtual void Free() override;
};

END