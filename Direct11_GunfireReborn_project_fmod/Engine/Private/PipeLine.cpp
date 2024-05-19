#include "..\Public\PipeLine.h"

IMPLEMENT_SINGLETON(CPipeLine)

CPipeLine::CPipeLine()
{
}

HRESULT CPipeLine::Set_Transform(TRANSFORMSTATETYPE eStateType, _fmatrix TransformMatrix)
{
	XMStoreFloat4x4(&m_TransformMatrix[eStateType], TransformMatrix);

	return S_OK;
}

_float4x4 CPipeLine::Get_TransformFloat4x4_TP(TRANSFORMSTATETYPE eStateType)
{
	_float4x4	TransposeMatrix;

	XMStoreFloat4x4(&TransposeMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformMatrix[eStateType])));

	return TransposeMatrix;
}

void CPipeLine::Update()
{
	_matrix			ViewMatrix = XMLoadFloat4x4(&m_TransformMatrix[D3DTS_VIEW]);

	ViewMatrix = XMMatrixInverse(nullptr, ViewMatrix);

	XMStoreFloat4(&m_vCamPosition, ViewMatrix.r[3]);
}

void CPipeLine::Free()
{
}
