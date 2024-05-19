#include "..\Public\Navigation.h"
#include "Cell.h"
#include "Shader.h"
#include "PipeLine.h"
#include "Transform.h"
#include "VIBuffer_Triangle.h"

CNavigation::CNavigation(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{

}

CNavigation::CNavigation(const CNavigation & rhs)
	: CComponent(rhs)
	, m_Cells(rhs.m_Cells)
#ifdef _DEBUG
	, m_pVIBuffer(rhs.m_pVIBuffer)
	, m_pShader(rhs.m_pShader)
#endif
{
	for (auto& pCell : m_Cells)
		Safe_AddRef(pCell);
#ifdef _DEBUG
	Safe_AddRef(m_pShader);
	Safe_AddRef(m_pVIBuffer);
#endif
}

HRESULT CNavigation::NativeConstruct_Prototype(const _tchar * pNaviDataFilePath)
{
	_ulong			dwByte = 0;
	HANDLE			hFile = CreateFile(pNaviDataFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	_float3			vPoints[3];

	while (true)
	{
		ReadFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

		if (0 == dwByte)
			break;

		CCell*		pCell = CCell::Create(m_pDevice, m_pDeviceContext, vPoints, m_Cells.size());
		if (nullptr == pCell)
		{
			MSGBOX("Cell 생성 실패");
			return E_FAIL;
		}

		m_Cells.push_back(pCell);
	}

	CloseHandle(hFile);

	if (FAILED(SetUp_Neighbor()))
		return E_FAIL;

#ifdef _DEBUG
	m_pVIBuffer = CVIBuffer_Triangle::Create(m_pDevice, m_pDeviceContext);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	m_pShader = CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_Navigation.hlsl"), VTXCOL_DECLARATION::Elements, VTXCOL_DECLARATION::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;
#endif // _DEBUG

	return S_OK;
}

HRESULT CNavigation::NativeConstruct(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	memcpy(&m_NaviDesc, pArg, sizeof(NAVIDESC));

	Safe_AddRef(m_NaviDesc.pParent);

	return S_OK;
}

_bool CNavigation::Move_OnNavigation(_fvector vPosition, _float3* pDir)
{
	_int		iNeighborIndex = -1;
	_float3		vLineDir;
	_vector		vPos = vPosition + XMLoadFloat3(pDir);

	/* _fvector vPosition : 움직이고 난 후의 위치다. */
	if (false == m_Cells[m_NaviDesc.iCurrentIndex]->isIn(vPos, &iNeighborIndex, &vLineDir))
	{
		/* 나간쪽에 이웃이 있다라면. */
		if (0 <= iNeighborIndex)
		{
			while (true)
			{
				_int	iCurrentNeighborIndex = -1;

				if (true == m_Cells[iNeighborIndex]->isIn(vPos, &iCurrentNeighborIndex, &vLineDir))
				{
					m_NaviDesc.iCurrentIndex = iNeighborIndex;
					break;
				}
				else
				{
					if (-1 == iCurrentNeighborIndex)
					{
						_vector vNormal = XMVector3Normalize(XMVectorSet(vLineDir.z * -1.f, 0.f, vLineDir.x, 0.f));

						XMStoreFloat3(pDir, XMLoadFloat3(pDir) - vNormal * XMVectorGetX(XMVector3Dot(XMLoadFloat3(pDir), vNormal)));

						if (false == m_Cells[m_NaviDesc.iCurrentIndex]->isIn(vPosition + XMLoadFloat3(pDir), &iNeighborIndex))
						{
							if (0 <= iNeighborIndex)
								m_NaviDesc.iCurrentIndex = iNeighborIndex;
							else
								*pDir = _float3(0.f, 0.f, 0.f); //오류 발견 일단 주석처리하겠음
						}

						return false;
					}
				}

				iNeighborIndex = iCurrentNeighborIndex;
			}

			return true;
		}

		/* 이웃이 없는 쪽으로 나갔다면. 슬라이딩 벡터*/
		else
		{
			_vector vNormal = XMVector3Normalize(XMVectorSet(vLineDir.z * -1.f, 0.f, vLineDir.x, 0.f));

			XMStoreFloat3(pDir, XMLoadFloat3(pDir) - vNormal * XMVectorGetX(XMVector3Dot(XMLoadFloat3(pDir), vNormal)));

			if (false == m_Cells[m_NaviDesc.iCurrentIndex]->isIn(vPosition + XMLoadFloat3(pDir), &iNeighborIndex))
			{
				if (0 <= iNeighborIndex)
					m_NaviDesc.iCurrentIndex = iNeighborIndex;
				else
					*pDir = _float3(0.f, 0.f, 0.f); //오류 발견 일단 주석처리하겠음
			}

			return false;
		}
	}
	else
		return true;
}

_vector CNavigation::Get_Height(_fvector vPos)
{
	_vector vPosition = vPos;
	_float3* pPoints = m_Cells[m_NaviDesc.iCurrentIndex]->Get_Points();

	_vector vPlane = XMPlaneFromPoints(XMLoadFloat3(&pPoints[0]), XMLoadFloat3(&pPoints[1]),
		XMLoadFloat3(&pPoints[2])); //점 3개를 넣어주면 평면의 abcd를 구해준다.

									//y = (-ax - cz - d) / b;
	_float fY = (-XMVectorGetX(vPlane) * XMVectorGetX(vPosition)
		- XMVectorGetZ(vPlane) * XMVectorGetZ(vPosition)
		- XMVectorGetW(vPlane)) / XMVectorGetY(vPlane);

	return XMVectorSetY(vPosition, fY);
}

HRESULT CNavigation::Find_CellIndex(CTransform * _pTransform)
{
	_uint index;

	for (auto& pCell : m_Cells)
	{
		if (nullptr != pCell)
		{
			index = pCell->Get_Index();
			_int	iNeighborIndex = -1;
			_vector vPosition = _pTransform->Get_State(CTransform::STATE_POSITION);
			_float3 vLineDir = { 0.f, 0.f, 0.f };
			if (m_Cells[index]->isIn(vPosition, &iNeighborIndex, &vLineDir))
			{
				m_NaviDesc.iCurrentIndex = index;
			}
		}
	}

	return S_OK;
}

#ifdef _DEBUG
HRESULT CNavigation::Render()
{
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	CPipeLine*		pPipeLine = GET_INSTANCE(CPipeLine);

	_float4x4		WorldMatrix;

	if (nullptr == m_NaviDesc.pParent)
		XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());
	else
		XMStoreFloat4x4(&WorldMatrix, XMMatrixTranspose(m_NaviDesc.pParent->Get_WorldMatrix()));

	m_pShader->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4));

	m_pShader->Set_RawValue("g_ViewMatrix", &pPipeLine->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ProjMatrix", &pPipeLine->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4));

	_uint		iIndex = 0;

	if (-1 == m_NaviDesc.iCurrentIndex)
	{
		for (auto& pCell : m_Cells)
		{
			if (nullptr != pCell)
			{
				pCell->Render(m_pVIBuffer, m_pShader, iIndex++ == m_NaviDesc.iCurrentIndex ? _float4(1.f, 0.f, 0.f, 1.f) : _float4(1.f, 1.f, 1.f, 1.f));
			}
		}
	}
	else
	{
		m_Cells[m_NaviDesc.iCurrentIndex]->Render(m_pVIBuffer, m_pShader, _float4(1.f, 0.f, 0.f, 1.f));

	}


	RELEASE_INSTANCE(CPipeLine);

	return S_OK;
}
#endif
HRESULT CNavigation::SetUp_Neighbor()
{
	for (auto& pSourCell : m_Cells)
	{
		for (auto& pDestCell : m_Cells)
		{
			if (pSourCell == pDestCell)
				continue;

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_A), pSourCell->Get_Point(CCell::POINT_B)))
			{
				pSourCell->Set_NeighborIndex(CCell::LINE_AB, pDestCell->Get_Index());
			}

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_B), pSourCell->Get_Point(CCell::POINT_C)))
			{
				pSourCell->Set_NeighborIndex(CCell::LINE_BC, pDestCell->Get_Index());
			}

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_C), pSourCell->Get_Point(CCell::POINT_A)))
			{
				pSourCell->Set_NeighborIndex(CCell::LINE_CA, pDestCell->Get_Index());

			}
		}
	}

	return S_OK;
}

CNavigation * CNavigation::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar * pNaviDataFilePath)
{
	CNavigation*	pInstance = new CNavigation(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(pNaviDataFilePath)))
	{
		MSGBOX("Failed to Creating CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CNavigation::Clone(void * pArg)
{
	CNavigation*	pInstance = new CNavigation(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNavigation::Free()
{
	__super::Free();

	Safe_Release(m_NaviDesc.pParent);

	for (auto& pCell : m_Cells)
		Safe_Release(pCell);

	m_Cells.clear();
#ifdef _DEBUG
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pShader);
#endif

}