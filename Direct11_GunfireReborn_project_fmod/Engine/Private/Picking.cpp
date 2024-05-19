#include "..\Public\Picking.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CPicking)

HRESULT CPicking::NativeConstruct(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, HWND hWnd, _uint _iWinCX, _uint _iWinCY)
{
	m_hWnd = hWnd;
	m_pDevice = pDevice;
	m_pDeviceContext = pDeviceContext;
	m_iWinCX = _iWinCX;
	m_iWinCY = _iWinCY;
	Safe_AddRef(pDevice);
	Safe_AddRef(pDeviceContext);

	return S_OK;
}

HRESULT CPicking::Transform_ToWorldSpace()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//����Ʈ�� ���콺 ��ǥ�� ���ϴ���
	POINT	ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);

	//����Ʈ���� �������� ��ȯ��
	_float4	vMousePos;
	vMousePos.x = ptMouse.x / ((_float)m_iWinCX * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -((_float)m_iWinCY * 0.5f) + 1.f;
	vMousePos.z = 0.f;
	vMousePos.w = 1.f;

	//�������� �� �����̽��� ��ȯ��
	_matrix matProj, matProjInverse;
	_vector	vViewPos;
	matProj = pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);
	matProjInverse = XMMatrixInverse(nullptr,matProj);
	vViewPos = XMVector4Transform(XMLoadFloat4(&vMousePos), matProjInverse);

	//���⺤�� ���ϴ��� ���ϴ� �� ������ - ����  = �������κ��� �� ���������� ���ϴ� ���⺤��
	_vector vViewDir = vViewPos - XMVectorSet(0.f,0.f,0.f,1.f);
	XMStoreFloat3(&m_vRayDir, vViewDir);
	m_vRayPos = _float3(0.f, 0.f, 0.f);

	//ī�޶��� ������ ���� ���콺�� ��ġ�� �޶������ϴ� �� �����̽����� ���⺤�� ���ϰ� ����� ��������.

	//�信�� ����� ��ȯ��
	_matrix matView, matViewInverse;
	matView = pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);
	matViewInverse = XMMatrixInverse(nullptr, matView);
	XMStoreFloat3(&m_vRayDir, XMVector3TransformNormal(XMLoadFloat3(&m_vRayDir), matViewInverse));
	XMStoreFloat3(&m_vRayPos, XMVector3TransformCoord(XMLoadFloat3(&m_vRayPos), matViewInverse));

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CPicking::Transform_ToLocalSpace(_float4x4 WorldMatrixInverse)
{
	_matrix matWorldInverse = XMLoadFloat4x4(&WorldMatrixInverse);

	XMStoreFloat3(&m_vRayDirInLocal, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_vRayDir), matWorldInverse)));
	XMStoreFloat3(&m_vRayPosInLocal, XMVector3TransformCoord(XMLoadFloat3(&m_vRayPos), matWorldInverse));



	return S_OK;
}

_bool CPicking::isPick(_float3* pLocalPoints, _float3 * pOut)
{
	_float fDist;
	_vector vRayPos = XMLoadFloat3(&m_vRayPosInLocal);
	_vector vRayDir = XMLoadFloat3(&m_vRayDirInLocal);
	_vector	vLocal1 = XMLoadFloat3(&pLocalPoints[0]);
	_vector vLocal2 = XMLoadFloat3(&pLocalPoints[1]);
	_vector vLocal3 = XMLoadFloat3(&pLocalPoints[2]);

	if (TriangleTests::Intersects(vRayPos, vRayDir, vLocal1, vLocal2, vLocal3, fDist))
	{
		_vector vDir = vRayPos + vRayDir * fDist;
		XMStoreFloat3(pOut, vDir);
		return true;
	}
	else {
		return false;
	}
}

void CPicking::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}
