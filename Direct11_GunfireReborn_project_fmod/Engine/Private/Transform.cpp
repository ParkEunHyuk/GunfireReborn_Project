#include "..\Public\Transform.h"
#include "Shader.h"
#include "Navigation.h"

CTransform::CTransform(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{
}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
	, m_WorldMatrix(rhs.m_WorldMatrix)
{
}

_vector CTransform::Get_Scale(STATE eState) const
{
	return XMVector3Length(XMLoadFloat3((_float3*)&m_WorldMatrix.m[eState][0]));	
}

_vector CTransform::Get_State(STATE eState) const
{
	return XMLoadFloat4((_float4*)&m_WorldMatrix.m[eState][0]);
}

_vector CTransform::Get_Distance(_vector vDescPos)
{
	_vector		vSourcePos = Get_State(CTransform::STATE_POSITION);

	
	return XMVector3Length(vSourcePos- vDescPos);
}

void CTransform::Set_State(STATE eState, _fvector vState)
{
	XMStoreFloat4((_float4*)&m_WorldMatrix.m[eState][0], vState);	
}

void CTransform::Set_TransformDesc(const TRANSFORMDESC & TransformDesc)
{
	m_TransformDesc = TransformDesc;
}

HRESULT CTransform::NativeConstruct_Prototype()
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	return S_OK;
}

HRESULT CTransform::NativeConstruct(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_TransformDesc, pArg, sizeof(TRANSFORMDESC));

	return S_OK;
}

HRESULT CTransform::Bind_OnShader(CShader * pShader, const char * pValueName)
{
	_float4x4		WorldMatrix;

	XMStoreFloat4x4(&WorldMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_WorldMatrix)));

	return pShader->Set_RawValue(pValueName, &WorldMatrix, sizeof(_float4x4));	
}

HRESULT CTransform::Go_Straight(_double TimeDelta, CNavigation* pNavigation)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	vPosition += XMVector3Normalize(vLook) * m_TransformDesc.fSpeedPerSec * (_float)TimeDelta;

	if (nullptr == pNavigation)
	{
		Set_State(CTransform::STATE_POSITION, vPosition);
		return S_OK;
	}

	_float3 vDir;
	XMStoreFloat3(&vDir, XMVector3Normalize(vLook) * m_TransformDesc.fSpeedPerSec * (_float)TimeDelta);

	if (true == pNavigation->Move_OnNavigation(Get_State(CTransform::STATE_POSITION), &vDir))
	{
		Set_State(CTransform::STATE_POSITION, vPosition);
	}
	else
	{
		Set_State(CTransform::STATE_POSITION, Get_State(CTransform::STATE_POSITION) + XMLoadFloat3(&vDir));
	}

	return S_OK;
}

HRESULT CTransform::Go_Right(_double TimeDelta, CNavigation* pNavigation)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	vPosition += XMVector3Normalize(vRight) * m_TransformDesc.fSpeedPerSec * (_float)TimeDelta;

	if (nullptr == pNavigation)
	{
		Set_State(CTransform::STATE_POSITION, vPosition);
		return S_OK;
	}

	_float3 vDir;
	XMStoreFloat3(&vDir, XMVector3Normalize(vRight) * m_TransformDesc.fSpeedPerSec * (_float)TimeDelta);

	if (true == pNavigation->Move_OnNavigation(Get_State(CTransform::STATE_POSITION), &vDir))
	{
		Set_State(CTransform::STATE_POSITION, vPosition);
	}
	else
	{
		Set_State(CTransform::STATE_POSITION, Get_State(CTransform::STATE_POSITION) + XMLoadFloat3(&vDir));
	}

	return S_OK;
}

HRESULT CTransform::Go_Left(_double TimeDelta, CNavigation* pNavigation)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vLeft = Get_State(CTransform::STATE_RIGHT);

	vPosition -= XMVector3Normalize(vLeft) * m_TransformDesc.fSpeedPerSec * (_float)TimeDelta;

	if (nullptr == pNavigation)
	{
		Set_State(CTransform::STATE_POSITION, vPosition);
		return S_OK;
	}

	_float3 vDir;
	XMStoreFloat3(&vDir, XMVector3Normalize(-vLeft) * m_TransformDesc.fSpeedPerSec * (_float)TimeDelta);

	if (true == pNavigation->Move_OnNavigation(Get_State(CTransform::STATE_POSITION), &vDir))
	{
		Set_State(CTransform::STATE_POSITION, vPosition);
	}
	else
	{
		Set_State(CTransform::STATE_POSITION, Get_State(CTransform::STATE_POSITION) + XMLoadFloat3(&vDir));
	}


	return S_OK;
}

HRESULT CTransform::Go_Backward(_double TimeDelta, CNavigation* pNavigation)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	vPosition -= XMVector3Normalize(vLook) * m_TransformDesc.fSpeedPerSec * (_float)TimeDelta;

	if (nullptr == pNavigation)
	{
		Set_State(CTransform::STATE_POSITION, vPosition);
		return S_OK;
	}

	_float3 vDir;
	XMStoreFloat3(&vDir, XMVector3Normalize(-vLook) * m_TransformDesc.fSpeedPerSec * (_float)TimeDelta);

	if (true == pNavigation->Move_OnNavigation(Get_State(CTransform::STATE_POSITION), &vDir))
	{
		Set_State(CTransform::STATE_POSITION, vPosition);
	}
	else
	{
		Set_State(CTransform::STATE_POSITION, Get_State(CTransform::STATE_POSITION) + XMLoadFloat3(&vDir));
	}


	return S_OK;
}

HRESULT CTransform::Go_Up(_double TimeDelta)
{
	_vector vPosition = Get_State(CTransform::STATE_POSITION);
	_vector vUp = Get_State(CTransform::STATE_UP);

	vPosition += XMVector3Normalize(vUp) * (TimeDelta );

	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_Down(_double TimeDelta)
{
	_vector vPosition = Get_State(CTransform::STATE_POSITION);
	_vector vUp = Get_State(CTransform::STATE_UP);

	vPosition -= XMVector3Normalize(vUp) * (TimeDelta);

	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Turn(_fvector vAxis, _double TimeDelta)
{
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_vector		vUp = Get_State(CTransform::STATE_UP);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	// vRight = XMVectorSetW(vRight, 0.f);

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, m_TransformDesc.fRotationPerSec * (_float)TimeDelta);

	vRight = XMVector4Transform(vRight, RotationMatrix);
	vUp = XMVector4Transform(vUp, RotationMatrix);
	vLook = XMVector4Transform(vLook, RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);	

	return S_OK;
}

HRESULT CTransform::Rotation(_fvector vAxis, _float fRadian)
{
	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(Get_State(CTransform::STATE_RIGHT)));
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(Get_State(CTransform::STATE_UP)));
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVectorGetX(XMVector3Length(Get_State(CTransform::STATE_LOOK)));

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

	vRight = XMVector4Transform(vRight, RotationMatrix);
	vUp = XMVector4Transform(vUp, RotationMatrix);
	vLook = XMVector4Transform(vLook, RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

	return S_OK;
}

HRESULT CTransform::Scaled(_fvector vScale)
{
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_vector		vUp = Get_State(CTransform::STATE_UP);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);


	vRight = XMVector3Normalize(vRight) * XMVectorGetX(vScale);
	vUp = XMVector3Normalize(vUp)  * XMVectorGetY(vScale);
	vLook = XMVector3Normalize(vLook)  * XMVectorGetZ(vScale);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

	return S_OK;
}

HRESULT CTransform::Scaling(_uint Second, _double Scale, _double TimeDelta)
{
	//현재 Scaling은 1회용임 m_dFrameTime를 0으로 만들어주지 않았기 때문임. 나중에 불값이든 뭐든 해서 m_dFrameTime를 0으로 만들어줘야함
	_vector	vRight = Get_Scale(CTransform::STATE_RIGHT);
	_vector	vUp = Get_Scale(CTransform::STATE_UP);
	_vector	vLook = Get_Scale(CTransform::STATE_LOOK);

	m_dFrameTime += TimeDelta;

	_double TempScaleTime = 60 * Second;

	_float RealSize = Scale / TempScaleTime;

	if(m_dFrameTime < Second)
	{
		vRight += XMLoadFloat(&RealSize);
		vUp += XMLoadFloat(&RealSize);
		vLook += XMLoadFloat(&RealSize);
	}

	Scaled(XMVectorSet(XMVectorGetX(vRight), XMVectorGetX(vUp), XMVectorGetX(vLook), 0.0f));

	return S_OK;
}

HRESULT CTransform::Scaling_Small(_uint Second, _double Scale, _double TimeDelta)
{
	//현재 Scaling은 1회용임 m_dFrameTime를 0으로 만들어주지 않았기 때문임. 나중에 불값이든 뭐든 해서 m_dFrameTime를 0으로 만들어줘야함
	_vector	vRight = Get_Scale(CTransform::STATE_RIGHT);
	_vector	vUp = Get_Scale(CTransform::STATE_UP);
	_vector	vLook = Get_Scale(CTransform::STATE_LOOK);

	m_dFrameTime += TimeDelta;

	_double TempScaleTime = 60 * Second;

	_float RealSize = (Scale / TempScaleTime) * -1;

	if (m_dFrameTime < Second)
	{
		vRight += XMLoadFloat(&RealSize);
		vUp += XMLoadFloat(&RealSize);
		vLook += XMLoadFloat(&RealSize);
	}

	Scaled(XMVectorSet(XMVectorGetX(vRight), XMVectorGetX(vUp), XMVectorGetX(vLook), 0.0f));
	return S_OK;
}

HRESULT CTransform::Chase(_fvector vTargetPos, _double TimeDelta)
{
	return S_OK;
}

HRESULT CTransform::LookAt(_fvector vTargetPos, _double TimeDelta)
{
	_vector vPos = Get_State(CTransform::STATE_POSITION);

	_vector vLook = XMVector3Normalize((vTargetPos - vPos)) *XMVectorGetZ(Get_Scale(CTransform::STATE_LOOK));

	_float3 Temp = _float3(0, 1, 0);
	_vector vRight = XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&Temp), vLook)) *XMVectorGetX(Get_Scale(CTransform::STATE_RIGHT));

	_vector vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight)) * XMVectorGetY(Get_Scale(CTransform::STATE_UP));

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

	return S_OK;
}

HRESULT CTransform::LookAtNOTAxisY(_vector vTargetPos, _double TimeDelta)
{
	_vector vTempPos = Get_State(CTransform::STATE_POSITION);
	_vector vTarPos = vTargetPos;

	vTempPos = XMVectorSetY(vTempPos,0.f);
	vTarPos = XMVectorSetY(vTarPos, 0.f);

	_vector vLook = vTarPos - vTempPos;

	vLook = XMVector3Normalize(vLook);

	_vector vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f,0.f,0.f), vLook));

	_vector vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight));

	Set_State(CTransform::STATE_RIGHT, vRight *XMVectorGetX(Get_Scale(CTransform::STATE_RIGHT)));
	Set_State(CTransform::STATE_UP, vUp * XMVectorGetY(Get_Scale(CTransform::STATE_UP)));
	Set_State(CTransform::STATE_LOOK, vLook * XMVectorGetZ(Get_Scale(CTransform::STATE_LOOK)));

	//_vector vPos = Get_State(CTransform::STATE_POSITION);

	//vPos += vLook;

	//LookAt(vPos,0.f);


	//_vector vPos = Get_State(CTransform::STATE_POSITION);

	//_vector vLook = XMVector3Normalize(XMVectorSetY(vTargetPos - vPos, 0.f)) *XMVectorGetZ(Get_Scale(CTransform::STATE_LOOK));
	//

	//_float3 Temp = _float3(0, 1, 0);
	//_vector vRight = XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&Temp), vLook)) *XMVectorGetX(Get_Scale(CTransform::STATE_RIGHT));

	//_vector vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight)) * XMVectorGetY(Get_Scale(CTransform::STATE_UP));

	//Set_State(CTransform::STATE_RIGHT, vRight);
	//Set_State(CTransform::STATE_UP, vUp);
	//Set_State(CTransform::STATE_LOOK, vLook);

	return S_OK;
}

//_bool CTransform::Go_Interpolation_Straight(_double TimeDelta)
//{
//	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
//
//	if (false == m_bMovingSwitch)
//	{
//		XMStoreFloat3(&m_fRight,Get_State(CTransform::STATE_RIGHT));
//
//		m_bMovingSwitch = true;
//	}
//
//	_vector Straight = XMVector3Cross(XMLoadFloat3(&m_fRight), XMLoadFloat3(&_float3(0.f, 1.f, 0.f)));
//
//	//_float		fTempSpeedPerSec = m_TransformDesc.fSpeedPerSec * 50;
//
//	m_dMovingTime += TimeDelta;
//
//	if (m_dMovingTime > 0.5)
//	{
//		m_dMovingTime = 0;
//		m_bMovingSwitch = false;
//		return false;
//	}
//	else {
//		//fTempSpeedPerSec = fTempSpeedPerSec;
//		//XMVectorSet
//		vPosition += XMVector3Normalize(Straight) *  m_TransformDesc.fSpeedPerSec*2 * (_float)TimeDelta;
//		Set_State(CTransform::STATE_POSITION, vPosition);
//	}
//
//
//	return true;
//}
CTransform * CTransform::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CTransform*	pInstance = new CTransform(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTransform::Clone(void * pArg)
{
	CTransform*	pInstance = new CTransform(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free()
{
	__super::Free();

}
