#include "..\Public\VIBuffer_Terrain_Dynamic.h"

CVIBuffer_Terrain_Dynamic::CVIBuffer_Terrain_Dynamic(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CVIBuffer(pDevice, pDeviceContext)
{
}

CVIBuffer_Terrain_Dynamic::CVIBuffer_Terrain_Dynamic(const CVIBuffer_Terrain_Dynamic& rhs)
	: CVIBuffer(rhs)
	, m_iNumVerticesX(rhs.m_iNumVerticesX)
	, m_iNumVerticesZ(rhs.m_iNumVerticesZ)
{
}

HRESULT CVIBuffer_Terrain_Dynamic::NativeConstruct_Prototype(const _tchar * pHeightMap)
{

	return S_OK;
}

HRESULT CVIBuffer_Terrain_Dynamic::NativeConstruct_Prototype(const _uint _iNumX, const _uint iNumZ)
{
#pragma region VERTEX_BUFFER

	m_iNumVerticesX = _iNumX;
	m_iNumVerticesZ = iNumZ;


	m_iNumIndicesPerPrimitive = 3;
	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;
	m_iNumVertexBuffers = 1;

	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));

	m_VBDesc.ByteWidth = sizeof(VTXNORTEX) * m_iNumVertices;
	m_VBDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.StructureByteStride = sizeof(VTXNORTEX);
	m_VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	VTXNORTEX*		pVertices = new VTXNORTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXNORTEX) * m_iNumVertices);

	m_pVerticesPos = new _float3[m_iNumVertices];
	ZeroMemory(m_pVerticesPos, sizeof(_float3) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = m_pVerticesPos[iIndex] = _float3((_float)j, 0.f, (_float)i);
			pVertices[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
			pVertices[iIndex].vTexUV = _float2(j / (m_iNumVerticesX - 1.f), i / (m_iNumVerticesZ - 1.f));
		}
	}

	m_pVertices = new VTXNORTEX[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(VTXNORTEX) * m_iNumVertices);

	memcpy(m_pVertices, pVertices, sizeof(VTXNORTEX) * m_iNumVertices);

	ZeroMemory(&m_VBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_VBSubResourceData.pSysMem = pVertices;

#pragma endregion


#pragma region INDEX_BUFFER
	m_iNumPrimitive = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_IBDesc, sizeof(D3D11_BUFFER_DESC));
	m_iIndicesSize = sizeof(FACEINDICES32);
	m_IBDesc.ByteWidth = sizeof(FACEINDICES32) * m_iNumPrimitive;
	m_IBDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_IBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	FACEINDICES32*	pIndices = new FACEINDICES32[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);

	_uint		iNumFace = 0;

	for (_uint i = 0; i < m_iNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; ++j)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			_uint		iIndices[4] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			pIndices[iNumFace]._0 = iIndices[0];
			pIndices[iNumFace]._1 = iIndices[1];
			pIndices[iNumFace]._2 = iIndices[2];

			_vector		vSour, vDest, vNormal;
			vSour = XMLoadFloat3(&m_pVerticesPos[pIndices[iNumFace]._1]) - XMLoadFloat3(&m_pVerticesPos[pIndices[iNumFace]._0]);
			vDest = XMLoadFloat3(&m_pVerticesPos[pIndices[iNumFace]._2]) - XMLoadFloat3(&m_pVerticesPos[pIndices[iNumFace]._1]);
			vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

			XMStoreFloat3(&pVertices[pIndices[iNumFace]._0].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[pIndices[iNumFace]._0].vNormal) + vNormal));
			XMStoreFloat3(&pVertices[pIndices[iNumFace]._1].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[pIndices[iNumFace]._1].vNormal) + vNormal));
			XMStoreFloat3(&pVertices[pIndices[iNumFace]._2].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[pIndices[iNumFace]._2].vNormal) + vNormal));

			++iNumFace;

			pIndices[iNumFace]._0 = iIndices[0];
			pIndices[iNumFace]._1 = iIndices[2];
			pIndices[iNumFace]._2 = iIndices[3];

			vSour = XMLoadFloat3(&m_pVerticesPos[pIndices[iNumFace]._1]) - XMLoadFloat3(&m_pVerticesPos[pIndices[iNumFace]._0]);
			vDest = XMLoadFloat3(&m_pVerticesPos[pIndices[iNumFace]._2]) - XMLoadFloat3(&m_pVerticesPos[pIndices[iNumFace]._1]);
			vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

			XMStoreFloat3(&pVertices[pIndices[iNumFace]._0].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[pIndices[iNumFace]._0].vNormal) + vNormal));
			XMStoreFloat3(&pVertices[pIndices[iNumFace]._1].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[pIndices[iNumFace]._1].vNormal) + vNormal));
			XMStoreFloat3(&pVertices[pIndices[iNumFace]._2].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[pIndices[iNumFace]._2].vNormal) + vNormal));
			++iNumFace;
		}
	}

	ZeroMemory(&m_IBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_IBSubResourceData.pSysMem = pIndices;

	if (FAILED(Create_VertexBuffer()))
		return E_FAIL;

	m_pIndices = new FACEINDICES32[m_iNumPrimitive];
	ZeroMemory(m_pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);
	memcpy(m_pIndices, pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);

	Safe_Delete_Array(pVertices);

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Terrain_Dynamic::NativeConstruct(void * pArg)
{
	return S_OK;
}

void CVIBuffer_Terrain_Dynamic::Add_Height(_float3 _vPickedPos, _float4 _vDampening, _float4 _vValues, _float _fMax, _float _fMin)
{
	VTXNORTEX* pVertices = new VTXNORTEX[m_iNumVertices];
	memcpy(pVertices, m_pVertices, sizeof(VTXNORTEX)*m_iNumVertices);
	//D3D11_MAPPED_SUBRESOURCE mappedResource;
	//ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	//m_pDeviceContext->Map(m_pVB, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedResource);
	//memcpy(pVertices, mappedResource.pData,sizeof(VTXARGB)*m_iNumVertices);
	//m_pDeviceContext->Unmap(m_pVB, 0);

	//1. 모든 정점을 순회 하며, 거리를 재고 값을 누적한다
	for (_uint i = 0; m_iNumVertices > i; ++i)
	{
		//정점까지의 거리
		_vector vVertexPos = XMLoadFloat3(&pVertices[i].vPosition);
		//월드 포지션인데 어쩌피 지형이니까 그냥 하자
		_vector vPickedPos = XMLoadFloat3(&_vPickedPos);
		_vector vDir = vVertexPos - vPickedPos;
		_float fDis = XMVectorGetX(XMVector3Length(vDir));

		//감쇄값을 벗어나면 다음
		if (fDis >= _vDampening.x)
			continue;

		//감쇄값
		_float Att = 1.f / (_vDampening.y + (_vDampening.z * fDis) + (_vDampening.w * fDis * fDis));

		//값 누적

		//1. A채널(높이)
		if (_vValues.x != 0.f)
		{
			pVertices[i].vPosition.y += Att *_vValues.x;

			//최대최소적용
			if (pVertices[i].vPosition.y > _fMax)
				pVertices[i].vPosition.y = _fMax;

			if (pVertices[i].vPosition.y < _fMin)
				pVertices[i].vPosition.y = _fMin;
		}
	}
	memcpy(m_pVertices, pVertices, sizeof(VTXNORTEX)*m_iNumVertices);
	Safe_Delete_Array(pVertices);
}

HRESULT CVIBuffer_Terrain_Dynamic::ReMake_Terrain(_uint iNumVerticesX, _uint iNumVerticesZ)
{
	if (nullptr == m_pIB || nullptr == m_pVB)
		return E_FAIL;

	m_iNumVerticesX = iNumVerticesX;
	m_iNumVerticesZ = iNumVerticesZ;
	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;

	D3D11_MAPPED_SUBRESOURCE tMSR;
	ZeroMemory(&tMSR, sizeof(D3D11_MAPPED_SUBRESOURCE));

	m_pDeviceContext->Map(m_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &tMSR);

	VTXNORTEX* pVernortex = new VTXNORTEX[m_iNumVertices];

	for (_uint i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			pVernortex[iIndex].vPosition = _float3((_float)j, 0.f, (_float)i);
			pVernortex[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
			pVernortex[iIndex].vTexUV = _float2(j / (m_iNumVerticesX - 1.f), i / (m_iNumVerticesZ - 1.f));
		}
	}

	memcpy(tMSR.pData, pVernortex, sizeof(VTXNORTEX) * m_iNumVertices);

	Safe_Delete_Array(pVernortex);
	m_pDeviceContext->Unmap(m_pVB, 0);

	//-------------------------------------------------------------------------------

	m_pDeviceContext->Map(m_pIB, 0, D3D11_MAP_WRITE_DISCARD, 0, &tMSR);

	m_iNumPrimitive = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;

	FACEINDICES32* pIndices = new FACEINDICES32[m_iNumPrimitive];

	_uint		iNumFace = 0;

	for (_uint i = 0; i < m_iNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; ++j)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			_uint		iIndices[4] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			pIndices[iNumFace]._0 = iIndices[0];
			pIndices[iNumFace]._1 = iIndices[1];
			pIndices[iNumFace]._2 = iIndices[2];

			++iNumFace;

			pIndices[iNumFace]._0 = iIndices[0];
			pIndices[iNumFace]._1 = iIndices[2];
			pIndices[iNumFace]._2 = iIndices[3];

			++iNumFace;
		}
	}

	memcpy(tMSR.pData, pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);

	Safe_Delete_Array(pIndices);
	m_pDeviceContext->Unmap(m_pIB, 0);

	return S_OK;
}

void CVIBuffer_Terrain_Dynamic::Apply_Data()
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	m_pDeviceContext->Map(m_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, m_pVertices, sizeof(VTXNORTEX)*m_iNumVertices);
	m_pDeviceContext->Unmap(m_pVB, 0);
}

HRESULT CVIBuffer_Terrain_Dynamic::Create_Filter_Image(_tchar * _pFileName)
{
	ID3D11Texture2D*			pTexture;
	D3D11_TEXTURE2D_DESC		TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = m_iNumVerticesX;
	TextureDesc.Height = m_iNumVerticesZ;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.Usage = D3D11_USAGE_DYNAMIC;
	TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA			SubResourceData;
	ZeroMemory(&SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	_ulong*		pPixel = new _ulong[m_iNumVerticesX * m_iNumVerticesZ];
	ZeroMemory(pPixel, sizeof(_ulong) * m_iNumVerticesX * m_iNumVerticesZ);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	m_pDeviceContext->Map(m_pVB, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedResource);

	//1. 모든 정점을 순회 하며, 거리를 재고 값을 누적한다
	VTXNORTEX* pVertices = (VTXNORTEX*)mappedResource.pData;

	//2. 파일을 저장할 경로를완성한다
	_tchar		szDefaultPath[] = TEXT("");
	_tchar		szHeightFullPath[MAX_PATH] = TEXT("");
	_tchar		szFilterFullPath[MAX_PATH] = TEXT("");

	wcscpy_s(szHeightFullPath, szDefaultPath);
	wcscat_s(szHeightFullPath, _pFileName);
	wcscat_s(szHeightFullPath, TEXT(".bin"));

	wcscpy_s(szFilterFullPath, szDefaultPath);
	wcscat_s(szFilterFullPath, _pFileName);
	wcscat_s(szFilterFullPath, TEXT(".png"));

	//3. 높이데이터 저장
	HANDLE hFile = CreateFile(
		szHeightFullPath,
		GENERIC_WRITE,
		0,
		0,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		nullptr);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MSGBOX("INVALID_HANDLE_VALUE");
		return E_FAIL;
	}

	DWORD dwBytes = 0;
	//1. 먼저 가로 지형 크기저장
	WriteFile(
		hFile,
		&m_iNumVerticesX,
		sizeof(_float),
		&dwBytes,
		nullptr);

	WriteFile(
		hFile,
		&m_iNumVerticesZ,
		sizeof(_float),
		&dwBytes,
		nullptr);

	for (_uint i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint iIndex = (i)* m_iNumVerticesX + (j);

			// 1. 높이저장

			WriteFile(
				hFile,
				&pVertices[iIndex].vPosition.y,
				sizeof(_float),
				&dwBytes,
				nullptr);

			//WriteFile(
			//	hFile,
			//	&pVertices[iIndex].vRGB,
			//	sizeof(_float3),
			//	&dwBytes,
			//	nullptr);

			//// 2.필터값 저장
			//pPixel[iIndex] = D3D11COLOR_ARGB(0, (_uint)(pVertices[iIndex].vRGB.x*255.f), (_uint)(pVertices[iIndex].vRGB.y*255.f), (_uint)(pVertices[iIndex].vRGB.z*255.f));

		}
	}

	m_pDeviceContext->Unmap(m_pVB, 0);

	CloseHandle(hFile);

	//필터
	SubResourceData.pSysMem = pPixel;
	SubResourceData.SysMemPitch = sizeof(_ulong) * m_iNumVerticesX;
	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, &SubResourceData, &pTexture)))
		return E_FAIL;
	if (FAILED(SaveWICTextureToFile(m_pDeviceContext, pTexture, GUID_ContainerFormatPng, szFilterFullPath, &GUID_WICPixelFormat32bppBGRA)))
		return E_FAIL;
	Safe_Release(pTexture);
	return S_OK;
}

HRESULT CVIBuffer_Terrain_Dynamic::Load_Image(_tchar * _pFileName)
{
	//1. 파일을 저장할 경로를완성한다
	_tchar		szDefaultPath[] = TEXT("../Bin/Save/");
	_tchar		szHeightFullPath[MAX_PATH] = TEXT("");
	_tchar		szFilterFullPath[MAX_PATH] = TEXT("");

	wcscpy_s(szHeightFullPath, szDefaultPath);
	wcscat_s(szHeightFullPath, _pFileName);
	wcscat_s(szHeightFullPath, TEXT(".bin"));

	HANDLE		hFile = CreateFile(
		szHeightFullPath,
		GENERIC_READ,
		0,
		nullptr,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		0);
	if (0 == hFile)
		return E_FAIL;
	_float	fFileNumX = 0.f;
	_float	fFileNumXZ = 0.f;
	_ulong	dwByte = 0;
	ReadFile(hFile, &fFileNumX, sizeof(_float), &dwByte, nullptr);
	ReadFile(hFile, &fFileNumXZ, sizeof(_float), &dwByte, nullptr);


	//2. 정점들의 값을 세팅한다
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	m_pDeviceContext->Map(m_pVB, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedResource);

	//1. 모든 정점을 순회 하며, 거리를 재고 값을 누적한다
	VTXNORTEX* pVertices = new VTXNORTEX[m_iNumVertices];
	memcpy(pVertices, mappedResource.pData, sizeof(VTXNORTEX)*m_iNumVertices);

	for (_uint i = 0; i < m_iNumVerticesX; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesZ; ++j)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;
			_float		fHeight = 0.f;
			ReadFile(hFile, &fHeight, sizeof(_float), &dwByte, nullptr);
			pVertices[iIndex].vPosition = _float3(j, fHeight, i);
		}
	}


	memcpy(mappedResource.pData, pVertices, sizeof(VTXNORTEX)*m_iNumVertices);
	Safe_Delete_Array(pVertices);
	m_pDeviceContext->Unmap(m_pVB, 0);
	CloseHandle(hFile);

	return S_OK;
}

CVIBuffer_Terrain_Dynamic * CVIBuffer_Terrain_Dynamic::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar * pHeightMap)
{
	CVIBuffer_Terrain_Dynamic*	pInstance = new CVIBuffer_Terrain_Dynamic(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(pHeightMap)))
	{
		MSGBOX("Failed to Created CVIBuffer_Terrain_Dynamic");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CVIBuffer_Terrain_Dynamic * CVIBuffer_Terrain_Dynamic::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _uint iNumVerticesX, const _uint iNumVerticesZ)
{
	CVIBuffer_Terrain_Dynamic*	pInstance = new CVIBuffer_Terrain_Dynamic(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(iNumVerticesX, iNumVerticesZ)))
	{
		MSGBOX("Failed to Created CVIBuffer_Terrain_Dynamic");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CVIBuffer_Terrain_Dynamic::Clone(void * pArg)
{
	CVIBuffer_Terrain_Dynamic*	pInstance = new CVIBuffer_Terrain_Dynamic(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CVIBuffer_Terrain_Dynamic");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Terrain_Dynamic::Free()
{
	__super::Free();

}
