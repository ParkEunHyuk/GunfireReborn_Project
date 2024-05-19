#include "..\Public\Cell.h"
#include "VIBuffer_Triangle.h"
#include "Shader.h"

#ifdef _DEBUG
#include "GameInstance.h"
#include "Collider.h"
#endif // _DEBUG

CCell::CCell(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

HRESULT CCell::NativeConstruct(const _float3 * pPoints, _uint iIndex)
{
	memcpy(m_vPoints, pPoints, sizeof(_float3) * POINT_END);

	XMStoreFloat3(&m_vLineDir[LINE_AB], XMLoadFloat3(&m_vPoints[POINT_B]) - XMLoadFloat3(&m_vPoints[POINT_A]));
	XMStoreFloat3(&m_vLineDir[LINE_BC], XMLoadFloat3(&m_vPoints[POINT_C]) - XMLoadFloat3(&m_vPoints[POINT_B]));
	XMStoreFloat3(&m_vLineDir[LINE_CA], XMLoadFloat3(&m_vPoints[POINT_A]) - XMLoadFloat3(&m_vPoints[POINT_C]));

	m_iIndex = iIndex;

	return S_OK;
}


#ifdef _DEBUG

HRESULT CCell::Update(_fmatrix WorldMatrix)
{
	for (_uint i = 0; i < 3; ++i)
	{
		m_pColliderCom[i]->Update_Position(m_vPoints[i]);
		m_pColliderCom[i]->Update_Transform(WorldMatrix);
	}

	return S_OK;
}

//HRESULT CCell::Render(CVIBuffer_Triangle* pVIBuffer, CShader* pShader, _float4 vColor)
//{
//	pVIBuffer->Update(m_vPoints);
//
//	if (FAILED(pShader->Set_RawValue("g_vColor", &vColor, sizeof(vColor))))
//		return E_FAIL;
//
//	if (FAILED(pVIBuffer->Render(pShader, 0)))
//		return E_FAIL;
//
//	for (_uint i = 0; i < 3; ++i)
//	{
//		if (nullptr != m_pColliderCom[i])
//			m_pColliderCom[i]->Render();
//	}
//
//	return S_OK;
//}

HRESULT CCell::Create_Collider(_uint iLevelIndex, const _tchar* pComponentTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	for (_uint i = 0; i < 3; ++i)
	{
		CCollider::COLLIDERTOOLDESC ColliderDesc;
		ColliderDesc.eType = CCollider::TYPE_SPHERE;
		ColliderDesc.vPosition = _float4(m_vPoints[i], 1.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
		m_pColliderCom[i] = dynamic_cast<CCollider*>(pGameInstance->Clone_Component(iLevelIndex, pComponentTag, &ColliderDesc));
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_bool CCell::Check_Collider(_float3 * pRayDirInLocal, _float3 * pRayPosInLocal, _float3* pPoint)
{
	for (_uint i = 0; i < POINT_END; ++i)
	{
		if (true == m_pColliderCom[i]->Check_RayCast(XMLoadFloat3(pRayDirInLocal), XMLoadFloat3(pRayPosInLocal)))
		{
			*pPoint = m_vPoints[i];

			return true;
		}
	}

	return false;
}

void CCell::Move_Collider(_float3 * pRayDirInLocal, _float3 * pRayPosInLocal, _float3 vPoint)
{
	for (_uint i = 0; i < POINT_END; ++i)
	{
		if (true == m_pColliderCom[i]->Check_RayCast(XMLoadFloat3(pRayDirInLocal), XMLoadFloat4(&_float4(*pRayPosInLocal, 1.f))))
		{
			m_vPoints[i].x = vPoint.x;
			m_vPoints[i].z = vPoint.z;

			return;
		}
	}
}

void CCell::Move_Collider(_float3 * pRayDirInLocal, _float3 * pRayPosInLocal, _float fY)
{
	for (_uint i = 0; i < POINT_END; ++i)
	{
		if (true == m_pColliderCom[i]->Check_RayCast(XMLoadFloat3(pRayDirInLocal), XMLoadFloat4(&_float4(*pRayPosInLocal, 1.f))))
		{
			m_vPoints[i].y += fY;

			return;
		}
	}
}

#endif // _DEBUG

_bool CCell::Compare_Points(_fvector vSourPoint, _fvector vDestPoint)
{

	if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vSourPoint))
	{
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vDestPoint))
			return true;
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vDestPoint))
			return true;
	}

	if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vSourPoint))
	{
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vDestPoint))
			return true;
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vDestPoint))
			return true;
	}

	if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vSourPoint))
	{
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vDestPoint))
			return true;
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vDestPoint))
			return true;
	}

	return false;
}

_bool CCell::isIn(_fvector vPosition, _int* pNeighborIndex, _float3* pLineDir)
{
	for (_uint i = 0; i < LINE_END; ++i)
	{
		_vector		vDestDir = XMVector3Normalize(vPosition - XMLoadFloat3(&m_vPoints[i]));
		_vector		vSourDir = XMVector3Normalize(XMLoadFloat4(&_float4(m_vLineDir[i].z * -1.f, 0.0f, m_vLineDir[i].x, 0.f))); //x와y위치를 바꾸고 앞에 음수를 붙이면 수직인 벡터를 구할 수 있다.

		if (0 < XMVectorGetX(XMVector3Dot(vDestDir, vSourDir)))
		{
			if (0 <= m_iNeighbor[i])
				*pNeighborIndex = m_iNeighbor[i];
			else
			{//
				if (nullptr != pLineDir)
					*pLineDir = m_vLineDir[i];
			}

			return false;
		}
	}

	return true;
}

#ifdef _DEBUG
HRESULT CCell::Render(CVIBuffer_Triangle* pVIBuffer, CShader* pShader, _float4	vColor)
{
	pVIBuffer->Update(m_vPoints);

	pShader->Set_RawValue("g_vColor", &vColor, sizeof(_float4));

	if (FAILED(pVIBuffer->Render(pShader, 0)))
		return E_FAIL;

	for (_uint i = 0; i < 3; ++i)
	{
		if (nullptr != m_pColliderCom[i])
			m_pColliderCom[i]->Render();
	}

	return S_OK;
}
#endif // _DEBUG

CCell * CCell::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _float3 * pPoints, _uint iIndex)
{
	CCell*	pInstance = new CCell(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pPoints, iIndex)))
	{
		MSGBOX("Failed to Creating CCell");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCell::Free()
{
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);

#ifdef _DEBUG
	for (auto& pCollider : m_pColliderCom)
		Safe_Release(pCollider);
#endif
}
