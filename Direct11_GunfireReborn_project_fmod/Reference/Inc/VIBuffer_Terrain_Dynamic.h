#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain_Dynamic final : public CVIBuffer
{
public:
	CVIBuffer_Terrain_Dynamic(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CVIBuffer_Terrain_Dynamic(const CVIBuffer_Terrain_Dynamic& rhs);
	virtual ~CVIBuffer_Terrain_Dynamic() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pHeightMap);
	virtual HRESULT NativeConstruct_Prototype(const _uint _iNumX, const _uint iNumZ);
	virtual HRESULT NativeConstruct(void* pArg) override;

public:	//높이 변환용 
	void	Add_Height(_float3 _vPickedPos, _float4 _vDampening, _float4 _vValues,_float _fMax,_float _fMin);
	HRESULT	ReMake_Terrain(_uint iNumVerticesX, _uint iNumVerticesZ);
	void	Apply_Data();
	HRESULT	Create_Filter_Image(_tchar* _pFileName);
	HRESULT Load_Image(_tchar* _pFileName);

private:
	_uint			m_iNumVerticesX = 0;
	_uint			m_iNumVerticesZ = 0;

public:
	static CVIBuffer_Terrain_Dynamic* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pHeightMap);
	static CVIBuffer_Terrain_Dynamic* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _uint iNumVerticesX, const _uint iNumVerticesZ);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END