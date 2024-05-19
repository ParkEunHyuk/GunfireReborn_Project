#include "..\Public\Frustum_Manager.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CFrustum_Manager)

CFrustum_Manager::CFrustum_Manager()
{

}

HRESULT CFrustum_Manager::Initialize()
{
	m_vPoint[0] = _float3(-1.f, 1.f, 0.f);
	m_vPoint[1] = _float3(1.f, 1.f, 0.f);
	m_vPoint[2] = _float3(1.f, -1.f, 0.f);
	m_vPoint[3] = _float3(-1.f, -1.f, 0.f);

	m_vPoint[4] = _float3(-1.f, 1.f, 1.f);
	m_vPoint[5] = _float3(1.f, 1.f, 1.f);
	m_vPoint[6] = _float3(1.f, -1.f, 1.f);
	m_vPoint[7] = _float3(-1.f, -1.f, 1.f);

	return S_OK;
}

void CFrustum_Manager::Update()
{
	CPipeLine*		pPipeLine = GET_INSTANCE(CPipeLine);

	/* 2차원 투영스페이스 상의 정의된 절두체의 여덟개 점을 월드로 옮긴다. */
	/*  * 투영행렬의 역 * 뷰행렬의 역 */
	_matrix		ProjMatrixInv, ViewMatrixInv;

	ProjMatrixInv = pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);
	ProjMatrixInv = XMMatrixInverse(nullptr, ProjMatrixInv);

	ViewMatrixInv = pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);
	ViewMatrixInv = XMMatrixInverse(nullptr, ViewMatrixInv);

	/* 뷰스페이스로 변환한다. */
	for (_uint i = 0; i < 8; ++i)
	{
		_vector			vPoint;

		vPoint = XMVector3TransformCoord(XMLoadFloat3(&m_vPoint[i]), ProjMatrixInv);
		vPoint = XMVector3TransformCoord(vPoint, ViewMatrixInv);

		XMStoreFloat3(&m_vPointInWorld[i], vPoint);
	}

	Compute_Plane(m_vPointInWorld, m_PlaneInWorld);

	RELEASE_INSTANCE(CPipeLine);
}

//HRESULT CFrustum_Manager::Transform_ToLocalSpace(CTransform * pTransform) //지형컬링(CPU상의 정점컬링)
//{
//	for (_uint i = 0; i < 8; ++i)
//	{
//		_vector			vPoint;
//
//		vPoint = XMVector3TransformCoord(XMLoadFloat3(&m_vPointInWorld[i]), pTransform->Get_WorldMatrixInverse());
//
//		XMStoreFloat3(&m_vPointInLocal[i], vPoint);
//	}
//
//	Compute_Plane(m_vPointInLocal, m_PlaneInLocal);
//
//	return S_OK;
//}

_bool CFrustum_Manager::isIn_WorldSpace(_fvector vPoint, _float fRange)
{
	/*a b c d
	x y z 1
	ax + by + cz + d*/

	for (_uint i = 0; i < 6; ++i)
	{
		/* 평면방정식의 해를 구한다. */
		if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_PlaneInWorld[i]), vPoint)) > fRange)
			return false;
	}
	return true;
}

//_bool CFrustum_Manager::isIn_LocalSpace(_fvector vPoint, _float fRange) //지형컬링(CPU상의 정점컬링)
//{
//	for (_uint i = 0; i < 6; ++i)
//	{
//		/* 평면방정식의 해를 구한다. */
//		if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_PlaneInLocal[i]), vPoint)) > fRange)
//			return false;
//	}
//	return true;
//}

void CFrustum_Manager::Compute_Plane(const _float3 * pPoints, _float4* pPlanes)
{
	/* +x */
	XMStoreFloat4(&pPlanes[0], XMPlaneFromPoints(XMLoadFloat3(&pPoints[1]), XMLoadFloat3(&pPoints[5]), XMLoadFloat3(&pPoints[6])));
	/* -x */
	XMStoreFloat4(&pPlanes[1], XMPlaneFromPoints(XMLoadFloat3(&pPoints[4]), XMLoadFloat3(&pPoints[0]), XMLoadFloat3(&pPoints[3])));
	/* +y */
	XMStoreFloat4(&pPlanes[2], XMPlaneFromPoints(XMLoadFloat3(&pPoints[4]), XMLoadFloat3(&pPoints[5]), XMLoadFloat3(&pPoints[1])));
	/* -y */
	XMStoreFloat4(&pPlanes[3], XMPlaneFromPoints(XMLoadFloat3(&pPoints[6]), XMLoadFloat3(&pPoints[7]), XMLoadFloat3(&pPoints[3])));
	/* +z */
	XMStoreFloat4(&pPlanes[4], XMPlaneFromPoints(XMLoadFloat3(&pPoints[5]), XMLoadFloat3(&pPoints[4]), XMLoadFloat3(&pPoints[7])));
	/* -z */
	XMStoreFloat4(&pPlanes[5], XMPlaneFromPoints(XMLoadFloat3(&pPoints[0]), XMLoadFloat3(&pPoints[1]), XMLoadFloat3(&pPoints[2])));
}

void CFrustum_Manager::Free()
{

}
