#include "..\Public\VIBuffer.h"
#include "Shader.h"
#include "Picking.h"

CVIBuffer::CVIBuffer(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{

}

CVIBuffer::CVIBuffer(const CVIBuffer & rhs)
	: CComponent(rhs)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_VBDesc(rhs.m_VBDesc)
	, m_VBSubResourceData(rhs.m_VBSubResourceData)
	, m_iNumVertices(rhs.m_iNumVertices)
	, m_iNumVertexBuffers(rhs.m_iNumVertexBuffers)
	, m_IBDesc(rhs.m_IBDesc)
	, m_IBSubResourceData(rhs.m_IBSubResourceData)
	, m_iNumPrimitive(rhs.m_iNumPrimitive)
	, m_iNumIndicesPerPrimitive(rhs.m_iNumIndicesPerPrimitive)
	, m_eIndexFormat(rhs.m_eIndexFormat)
	, m_eTopology(rhs.m_eTopology)
	, m_pIndices(rhs.m_pIndices)
	, m_iIndicesSize(rhs.m_iIndicesSize)
	, m_pVertices(rhs.m_pVertices)
	, m_pVerticesPos(rhs.m_pVerticesPos)
{
	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);

	
}

HRESULT CVIBuffer::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer::NativeConstruct(void * pArg)
{
	return S_OK;
}

_bool CVIBuffer::Pick_Dynamic(const _float4x4 & WorldMatrixInverse, _float3 * pOut)
{
	if (m_VBDesc.Usage != D3D11_USAGE_DYNAMIC)
	{
		MSGBOX("Picked_Faild : UsageError")
			return false;
	}

	CPicking* pPicking = GET_INSTANCE(CPicking);

	// 1. 마우스Ray 를 로컬스페이스로 변환시켜줌
	pPicking->Transform_ToLocalSpace(WorldMatrixInverse);

	_uint iIndexByte = 0;

	//2. 인덱스 포맷 형식에 따라 바이트 분류(인덱스를 포인터연산으로 접근)
	if (m_eIndexFormat == DXGI_FORMAT_R16_UINT)
		iIndexByte = 2;
	else
		iIndexByte = 4;

	////## map
	//D3D11_MAPPED_SUBRESOURCE mappedResourceVB;
	//ZeroMemory(&mappedResourceVB, sizeof(D3D11_MAPPED_SUBRESOURCE));
	//m_pDeviceContext->Map(m_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResourceVB);

	//3. 폴리곤을 순회 하며 충돌체크
	_bool isPick = false;
	for (_uint i = 0; i < m_iNumPrimitive; ++i) // 폴리곤을 전부 순회
	{
		_uint iIndices[3] = { 0 };

		for (_uint j = 0; j < 3; ++j)
			memcpy(&iIndices[j], (((_byte*)m_pIndices + m_iIndicesSize * i) + iIndexByte * j), iIndexByte);

		_float3 vPoint[3] = {
			*(_float3*)(((_byte*)m_pVertices) + m_VBDesc.StructureByteStride * iIndices[0]),
			*(_float3*)(((_byte*)m_pVertices) + m_VBDesc.StructureByteStride * iIndices[1]),
			*(_float3*)(((_byte*)m_pVertices) + m_VBDesc.StructureByteStride * iIndices[2])
		};

		/*_float3 vPoint[3] = {
			*(_float3*)(((_byte*)mappedResourceVB.pData) + m_VBDesc.StructureByteStride * iIndices[0]),
			*(_float3*)(((_byte*)mappedResourceVB.pData) + m_VBDesc.StructureByteStride * iIndices[1]),
			*(_float3*)(((_byte*)mappedResourceVB.pData) + m_VBDesc.StructureByteStride * iIndices[2])
		};*/

		if (isPick = pPicking->isPick(vPoint, pOut))
		{
			// Picking 한 점을 다시 월드매트릭스를 곱해서 월드스페이스로 변환 후 pOut 으로 보내줌
			_matrix WorldMatrix;
			WorldMatrix = XMMatrixInverse(nullptr, XMLoadFloat4x4(&WorldMatrixInverse));
			XMStoreFloat3(pOut, XMVector3TransformCoord(XMLoadFloat3(pOut), WorldMatrix));
			break;
		}
	}

	////## Unmap
	//m_pDeviceContext->Unmap(m_pVB, 0);


	RELEASE_INSTANCE(CPicking);

	return isPick;
}

HRESULT CVIBuffer::Render(CShader* pShader, _uint iPassIndex)
{
	if (nullptr == m_pDeviceContext)
		return E_FAIL;

	ID3D11Buffer*	pVertexBuffers[] = {
		m_pVB
	};

	_uint			iStrides[] = {
		m_VBDesc.StructureByteStride
	};

	_uint			iOffsets[] = {
		0, 
	};

	m_pDeviceContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iStrides, iOffsets);
	m_pDeviceContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
	m_pDeviceContext->IASetPrimitiveTopology(m_eTopology);

	if (FAILED(pShader->Set_InputLayout(iPassIndex)))
		return E_FAIL;
	if (FAILED(pShader->Apply(iPassIndex)))
		return E_FAIL;

	m_pDeviceContext->DrawIndexed(m_iNumIndicesPerPrimitive * m_iNumPrimitive, 0, 0);

	return S_OK;
}


HRESULT CVIBuffer::Create_VertexBuffer()
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	//CreateBuffer를 통해 버퍼가 만들어진다.
	return m_pDevice->CreateBuffer(&m_VBDesc, &m_VBSubResourceData, &m_pVB);	
}

HRESULT CVIBuffer::Create_IndexBuffer()
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	return m_pDevice->CreateBuffer(&m_IBDesc, &m_IBSubResourceData, &m_pIB);
}

void CVIBuffer::Free()
{
	__super::Free();

	if (false == m_isCloned)
	{
		Safe_Delete_Array(m_pVerticesPos);
		Safe_Delete_Array(m_pIndices);
		Safe_Delete_Array(m_pVertices);
	}

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
}
