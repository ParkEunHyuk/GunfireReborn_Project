#pragma once

#include "Base.h"

BEGIN(Engine)

class CPicking final : public CBase
{
	DECLARE_SINGLETON(CPicking)
private:
	CPicking() = default;
	virtual ~CPicking() = default;

public:
	_float3& Get_RayDirInLocal() {
		return m_vRayDirInLocal;
	}

	_float3& Get_RayPosInLocal() {
		return m_vRayPosInLocal;
	}

public:
	HRESULT	NativeConstruct(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, HWND hWnd, _uint _iWinCX, _uint _iWinCY);

public:
	HRESULT	Transform_ToWorldSpace();
	HRESULT	Transform_ToLocalSpace(_float4x4 WorldMatrixInverse);


	_bool isPick(_float3* pLocalPoints, _float3* pOut = nullptr);

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext;
	HWND					m_hWnd;
	_uint					m_iWinCX = 0;
	_uint					m_iWinCY = 0;

private:
	_float3					m_vRayDir;
	_float3					m_vRayPos;

	_float3					m_vRayDirInLocal;
	_float3					m_vRayPosInLocal;

public:
	virtual	void	Free()	override;


};

END