#include "..\Public\VIBuffer_Point_Instance.h"
#include "Shader.h"

CVIBuffer_Point_Instance::CVIBuffer_Point_Instance(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CVIBuffer(pDevice, pDeviceContext)
{

}

CVIBuffer_Point_Instance::CVIBuffer_Point_Instance(const CVIBuffer_Point_Instance & rhs)
	: CVIBuffer(rhs)
	, m_VBInstDesc(rhs.m_VBInstDesc)
	, m_VBInstSubResource(rhs.m_VBInstSubResource)
	, m_iNumInstance(rhs.m_iNumInstance)
{
}

HRESULT CVIBuffer_Point_Instance::NativeConstruct_Prototype(_uint iNumInstance)
{

#pragma region VERTEX_BUFFER
	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));

	m_iNumIndicesPerPrimitive = 1;
	m_iNumVertices = 1;
	m_iNumVertexBuffers = 2;

	m_VBDesc.ByteWidth = sizeof(VTXPOINT) * m_iNumVertices;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.StructureByteStride = sizeof(VTXPOINT);

	VTXPOINT*		pVertices = new VTXPOINT[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXPOINT) * m_iNumVertices);

	pVertices[0].vPosition = _float3(0.0f, 0.0f, 0.f);
	pVertices[0].fPSize = 0.2f;

	ZeroMemory(&m_VBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_VBSubResourceData.pSysMem = pVertices;

	if (FAILED(Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

#pragma endregion


#pragma region INDEX_BUFFER
	m_iNumInstance = iNumInstance;
	m_iNumPrimitive = m_iNumInstance;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;

	ZeroMemory(&m_IBDesc, sizeof(D3D11_BUFFER_DESC));
	m_IBDesc.ByteWidth = sizeof(_ushort) * m_iNumPrimitive;
	m_IBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	_ushort*	pIndices = new _ushort[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumPrimitive);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		pIndices[i] = 0;
	}

	ZeroMemory(&m_IBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_IBSubResourceData.pSysMem = pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion




	return S_OK;
}

HRESULT CVIBuffer_Point_Instance::NativeConstruct(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_ParticleDesc, pArg, sizeof(m_ParticleDesc));

	m_pSpeeds = new _float[m_iNumInstance];

	for (_uint i = 0; i < m_iNumInstance; ++i)
		m_pSpeeds[i] = rand() % _uint(m_ParticleDesc.fMaxSpeed - m_ParticleDesc.fMinSpeed + 1) + m_ParticleDesc.fMinSpeed;


	//원래 코드에선 네이티브컨스트럭트 프로토타입에 이 코드가 있었는데 그렇게 되면 
	//파티클들이 공유하는 현상이 나타난다. 그렇기 때문에 이런식으로 예외처리를 진행해주자.
#pragma region INSTANCE_BUFFER
	ZeroMemory(&m_VBInstDesc, sizeof(D3D11_BUFFER_DESC));

	m_VBInstDesc.ByteWidth = sizeof(VTXMATRIX) * m_iNumInstance;
	m_VBInstDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_VBInstDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBInstDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_VBInstDesc.StructureByteStride = sizeof(VTXMATRIX);

	VTXMATRIX*		pInstanceMatrix = new VTXMATRIX[m_iNumInstance];
	ZeroMemory(pInstanceMatrix, sizeof(VTXMATRIX) * m_iNumInstance);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		pInstanceMatrix[i].vRight = _float4(1.f, 0.f, 0.f, 0.f);
		pInstanceMatrix[i].vUp = _float4(0.f, 1.f, 0.f, 0.f);
		pInstanceMatrix[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
		//pInstanceMatrix[i].vPosition = _float4(rand() % (_uint)m_ParticleDesc.fRange.x, rand() % (_uint)m_ParticleDesc.fRange.y, rand() % (_uint)m_ParticleDesc.fRange.z, 1.f);

		_float RandX, RandY, RandZ;
		RandX = (rand() % ((_uint)m_ParticleDesc.fRange.x * 1000)) - 500;
		RandY = (rand() % ((_uint)m_ParticleDesc.fRange.y * 1000)) - 500;
		RandZ = (rand() % ((_uint)m_ParticleDesc.fRange.z * 1000)) - 500;

		pInstanceMatrix[i].vPosition = _float4(RandX*0.001, RandY*0.001, RandZ*0.001, 1.f);
	}

	ZeroMemory(&m_VBInstSubResource, sizeof(D3D11_SUBRESOURCE_DATA));
	m_VBInstSubResource.pSysMem = pInstanceMatrix;

	if (FAILED(m_pDevice->CreateBuffer(&m_VBInstDesc, &m_VBInstSubResource, &m_pVBInstance)))
		return E_FAIL;

	Safe_Delete_Array(pInstanceMatrix);

#pragma endregion

	Initialize_Particle();

	return S_OK;
}

HRESULT CVIBuffer_Point_Instance::Render(CShader * pShader, _uint iPassIndex)
{
	if (nullptr == m_pDeviceContext)
		return E_FAIL;

	ID3D11Buffer*	pVertexBuffers[] = {
		m_pVB,
		m_pVBInstance
	};

	_uint			iStrides[] = {
		m_VBDesc.StructureByteStride,
		m_VBInstDesc.StructureByteStride
	};

	_uint			iOffsets[] = {
		0,
		0
	};

	m_pDeviceContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iStrides, iOffsets);
	m_pDeviceContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
	m_pDeviceContext->IASetPrimitiveTopology(m_eTopology);

	if (FAILED(pShader->Set_InputLayout(iPassIndex)))
		return E_FAIL;
	if (FAILED(pShader->Apply(iPassIndex)))
		return E_FAIL;

	m_pDeviceContext->DrawIndexedInstanced(1, m_iNumInstance, 0, 0, 0);

	return S_OK;
}

void CVIBuffer_Point_Instance::Update(_double TimeDelta)
{
	if (nullptr == m_pDeviceContext)
		return;


	PlayParticle(TimeDelta);


}

HRESULT CVIBuffer_Point_Instance::Initialize_Particle()
{

	switch (m_ParticleDesc.ParticleType)
	{
	case PARTICLE_DIAGONAL_DOWN:
	{
		m_pParticleDir = new _float3[m_iNumInstance];

		for (_uint i = 0; i < m_iNumInstance; ++i)
		{
			_int RandDir;
			RandDir = (rand() % 1000) - 500;
			m_pParticleDir[i].x = RandDir;

			m_pParticleDir[i].y = -500;

			RandDir = (rand() % 1000) - 500;
			m_pParticleDir[i].z = RandDir;
		}

		break;
	}
	case PARTICLE_FIRECRACKER:
	{
		m_pParticleDir = new _float3[m_iNumInstance];

		for (_uint i = 0; i < m_iNumInstance; ++i)
		{
			_int RandDir;
			RandDir = (rand() % 1000) - 500;
			m_pParticleDir[i].x = RandDir;

			RandDir = (rand() % 1000) - 500;
			m_pParticleDir[i].y = RandDir;

			RandDir = (rand() % 1000) - 500;
			m_pParticleDir[i].z = RandDir;
		}

		break;
	}
	case PARTICEL_STRAIGHT:
	{
		m_pParticleDir = new _float3[m_iNumInstance];

		_vector vLook = XMLoadFloat3(&m_ParticleDesc.fDirection);

		_float3 Temp = _float3(0.f, 1.f, 0.f);
		_vector vRight = XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&Temp), vLook));
		_vector vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight));

		for (_uint i = 0; i < m_iNumInstance; ++i)
		{
			m_pParticleDir[i] = m_ParticleDesc.fDirection;

			_int RandDir;

			RandDir = (rand() % 2000) - 1000;
			vUp = XMVector3Normalize((vUp)*(RandDir * 0.001));

			RandDir = (rand() % 2000) - 1000;
			vRight = XMVector3Normalize((vRight)*(RandDir * 0.001));

			vLook = vLook * ((rand() % 50) + 20);
			vLook = XMVector3Normalize(vLook + XMVector3Normalize(vUp + vRight));
			//vLook = XMVector3Normalize(vLook * (vUp + vRight)) * 10;

			XMStoreFloat3(&m_pParticleDir[i], vLook);
	
		}

		break;
	}
	default:
		break;
	}
	return S_OK;
}

HRESULT CVIBuffer_Point_Instance::PlayParticle(_double TimeDelta)
{
	switch (m_ParticleDesc.ParticleType)
	{
	case PARTICLE_DIAGONAL_DOWN:
	{
		D3D11_MAPPED_SUBRESOURCE		SubResource;

		m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

		for (_uint i = 0; i < m_iNumInstance; ++i)
		{
			_vector		vPosition = XMLoadFloat4(&(((VTXMATRIX*)SubResource.pData)[i].vPosition));

			vPosition += XMVector3Normalize(XMLoadFloat3(&m_pParticleDir[i])) * m_pSpeeds[i] * TimeDelta;

			if (XMVectorGetY(vPosition) < -10.f)
				vPosition = XMVectorSetY(vPosition, 0.f);

			if (XMVectorGetX(vPosition) < -10.f
				|| XMVectorGetX(vPosition) > 10.f)
			{
				vPosition = XMVectorSetX(vPosition, (rand() % ((_uint)m_ParticleDesc.fRange.x * 1000))*0.001f);
			}

			if (XMVectorGetZ(vPosition) < -10.f
				|| XMVectorGetZ(vPosition) > 10.f)
			{
				vPosition = XMVectorSetZ(vPosition, (rand() % ((_uint)m_ParticleDesc.fRange.z * 1000))*0.001f);
			}



			XMStoreFloat4(&(((VTXMATRIX*)SubResource.pData)[i].vPosition), vPosition);
		}



		m_pDeviceContext->Unmap(m_pVBInstance, 0);
		break;
	}
	case PARTICLE_DOWN:
	{
		D3D11_MAPPED_SUBRESOURCE		SubResource;

		m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

		for (_uint i = 0; i < m_iNumInstance; ++i)
		{
			_vector		vPosition = XMLoadFloat4(&(((VTXMATRIX*)SubResource.pData)[i].vPosition));

			vPosition += XMVector3Normalize(XMLoadFloat3(&_float3(0.f, -1.f, 0.f))) * m_pSpeeds[i] * TimeDelta;

			//if (XMVectorGetY(vPosition) < -10.f)
			//	vPosition = XMVectorSetY(vPosition, 0.f);
			if (XMVectorGetY(vPosition) < -10.f)
			{
				_float TempX = (rand() % ((_uint)m_ParticleDesc.fRange.x * 1000)) * 0.001f;
				_float TempZ = (rand() % ((_uint)m_ParticleDesc.fRange.z * 1000)) * 0.001f;
				vPosition = XMVectorSet(TempX, 0, TempZ, 1.f);
			}

			XMStoreFloat4(&(((VTXMATRIX*)SubResource.pData)[i].vPosition), vPosition);
		}



		m_pDeviceContext->Unmap(m_pVBInstance, 0);
		break;
	}
	case PARTICLE_UP:
	{
		D3D11_MAPPED_SUBRESOURCE		SubResource;

		m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

		for (_uint i = 0; i < m_iNumInstance; ++i)
		{
			_vector		vPosition = XMLoadFloat4(&(((VTXMATRIX*)SubResource.pData)[i].vPosition));

			vPosition += XMVector3Normalize(XMLoadFloat3(&_float3(0.f, 1.f, 0.f))) * m_pSpeeds[i] * TimeDelta;

			if (XMVectorGetY(vPosition) > 10.f)
			{
				_float TempX = (rand() % ((_uint)m_ParticleDesc.fRange.x * 1000)) * 0.001f;
				_float TempZ = (rand() % ((_uint)m_ParticleDesc.fRange.z * 1000)) * 0.001f;
				vPosition = XMVectorSet(TempX, 0, TempZ, 1.f);
			}

			XMStoreFloat4(&(((VTXMATRIX*)SubResource.pData)[i].vPosition), vPosition);
		}



		m_pDeviceContext->Unmap(m_pVBInstance, 0);

		break;
	}
	case PARTICLE_FIRECRACKER:
	{
		D3D11_MAPPED_SUBRESOURCE		SubResource;

		m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

		for (_uint i = 0; i < m_iNumInstance; ++i)
		{
			_vector		vPosition = XMLoadFloat4(&(((VTXMATRIX*)SubResource.pData)[i].vPosition));

			vPosition += XMVector3Normalize(XMLoadFloat3(&m_pParticleDir[i])) * m_pSpeeds[i] * TimeDelta;

			if (XMVectorGetX(vPosition) < -15.f
				|| XMVectorGetX(vPosition) > 15.f)
			{
				//vPosition = XMVectorSetX(vPosition, 0.f);
				vPosition = XMVectorSet(0.f,0.f,0.f,1.f);
			}

			if (XMVectorGetY(vPosition) < -15.f
				|| XMVectorGetY(vPosition) > 15.f)
			{
				//vPosition = XMVectorSetY(vPosition, 0.f);
				vPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);
			}

			if (XMVectorGetZ(vPosition) < -15.f
				|| XMVectorGetZ(vPosition) > 15.f)
			{
				//vPosition = XMVectorSetZ(vPosition, 0.f);
				vPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);
			}

			XMStoreFloat4(&(((VTXMATRIX*)SubResource.pData)[i].vPosition), vPosition);
		}



		m_pDeviceContext->Unmap(m_pVBInstance, 0);
		break;
	}
	case PARTICEL_STRAIGHT:
	{
		D3D11_MAPPED_SUBRESOURCE		SubResource;

		m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

		for (_uint i = 0; i < m_iNumInstance; ++i)
		{
			_vector		vPosition = XMLoadFloat4(&(((VTXMATRIX*)SubResource.pData)[i].vPosition));

			vPosition += XMVector3Normalize(XMLoadFloat3(&m_pParticleDir[i])) * m_pSpeeds[i] * TimeDelta;

			if (XMVectorGetX(vPosition) < -20.f
				|| XMVectorGetX(vPosition) > 20.f)
			{
				vPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);
			}

			if (XMVectorGetY(vPosition) < -20.f
				|| XMVectorGetY(vPosition) > 20.f)
			{
				vPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);
			}

			if (XMVectorGetZ(vPosition) < -20.f
				|| XMVectorGetZ(vPosition) > 20.f)
			{
				vPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);
			}

			XMStoreFloat4(&(((VTXMATRIX*)SubResource.pData)[i].vPosition), vPosition);
		}



		m_pDeviceContext->Unmap(m_pVBInstance, 0);
		break;
	}

	}
	return S_OK;
}

CVIBuffer_Point_Instance * CVIBuffer_Point_Instance::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iNumInstance)
{
	CVIBuffer_Point_Instance*	pInstance = new CVIBuffer_Point_Instance(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(iNumInstance)))
	{
		MSGBOX("Failed to Created CVIBuffer_Point_Instance");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CVIBuffer_Point_Instance::Clone(void * pArg)
{
	CVIBuffer_Point_Instance*	pInstance = new CVIBuffer_Point_Instance(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CVIBuffer_Point_Instance");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Point_Instance::Free()
{
	__super::Free();

	Safe_Delete_Array(m_pSpeeds);
	Safe_Delete_Array(m_pParticleDir);


	Safe_Release(m_pVBInstance);

}
