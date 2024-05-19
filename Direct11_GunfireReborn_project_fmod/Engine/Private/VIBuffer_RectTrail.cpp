#include "..\Public\VIBuffer_RectTrail.h"

CVIBuffer_RectTrail::CVIBuffer_RectTrail(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CVIBuffer(pDevice, pDeviceContext)
{

}

CVIBuffer_RectTrail::CVIBuffer_RectTrail(const CVIBuffer_RectTrail & rhs)
	: CVIBuffer(rhs)
{

}

HRESULT CVIBuffer_RectTrail::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer_RectTrail::NativeConstruct(void * pArg)
{
#pragma region VERTEX_BUFFER
	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));

	m_iNumIndicesPerPrimitive = 3;
	m_iNumVertices = 8;
	m_iNumVertexBuffers = 1;

	m_VBDesc.ByteWidth = sizeof(VTXTEX) * m_iNumVertices;
	m_VBDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.StructureByteStride = sizeof(VTXTEX);
	m_VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	VTXTEX*		pVertices = new VTXTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXTEX) * m_iNumVertices);

	pVertices[0].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[0].vTexUV = _float2(0.f, 1.f);

	pVertices[1].vPosition = _float3(-0.5f, 0.5f, 0.f);
	pVertices[1].vTexUV = _float2(0.f, 0.f);

	pVertices[2].vPosition = _float3(-0.25f, -0.5f, 0.f);
	pVertices[2].vTexUV = _float2(0.333f, 1.f);

	pVertices[3].vPosition = _float3(-0.25f, 0.5f, 0.f);
	pVertices[3].vTexUV = _float2(0.333f, 0.f);

	pVertices[4].vPosition = _float3(0.25f, -0.5f, 0.f);
	pVertices[4].vTexUV = _float2(0.666f, 1.f);

	pVertices[5].vPosition = _float3(0.25f, 0.5f, 0.f);
	pVertices[5].vTexUV = _float2(0.666f, 0.f);

	pVertices[6].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[6].vTexUV = _float2(1.f, 1.f);

	pVertices[7].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[7].vTexUV = _float2(1.f, 0.f);

	ZeroMemory(&m_VBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_VBSubResourceData.pSysMem = pVertices;

	if (FAILED(Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

#pragma endregion


#pragma region INDEX_BUFFER
	m_iNumPrimitive = 6;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	// m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_IBDesc, sizeof(D3D11_BUFFER_DESC));
	m_IBDesc.ByteWidth = sizeof(FACEINDICES16) * m_iNumPrimitive;
	m_IBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	FACEINDICES16*	pIndices = new FACEINDICES16[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES16) * m_iNumPrimitive);

	pIndices[0]._0 = 3; pIndices[0]._1 = 1; pIndices[0]._2 = 0;
	pIndices[1]._0 = 2; pIndices[1]._1 = 3; pIndices[1]._2 = 0;
	pIndices[2]._0 = 5; pIndices[2]._1 = 3; pIndices[2]._2 = 2;
	pIndices[3]._0 = 4; pIndices[3]._1 = 5; pIndices[3]._2 = 2;
	pIndices[4]._0 = 7; pIndices[4]._1 = 5; pIndices[4]._2 = 4;
	pIndices[5]._0 = 6; pIndices[5]._1 = 7; pIndices[5]._2 = 4;

	ZeroMemory(&m_IBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_IBSubResourceData.pSysMem = pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion


	return S_OK;
}

VTXTEX * CVIBuffer_RectTrail::Vertex_Map()
{
	D3D11_MAPPED_SUBRESOURCE SubResource;

	m_pDeviceContext->Map(m_pVB, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	return (VTXTEX*)SubResource.pData;
}

void CVIBuffer_RectTrail::Vertex_Unmap()
{
	m_pDeviceContext->Unmap(m_pVB, 0);
}

CVIBuffer_RectTrail * CVIBuffer_RectTrail::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CVIBuffer_RectTrail* pInstance = new CVIBuffer_RectTrail(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed To Creating CVIBuffer_RectTrail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_RectTrail::Clone(void * pArg)
{
	CVIBuffer_RectTrail*	pInstance = new CVIBuffer_RectTrail(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CVIBuffer_RectTrail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_RectTrail::Free()
{
	__super::Free();

}
