#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_RectTrail final : public CVIBuffer
{
public:
	CVIBuffer_RectTrail(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CVIBuffer_RectTrail(const CVIBuffer_RectTrail& rhs);
	virtual ~CVIBuffer_RectTrail() = default;

public:
	const _uint& Get_NumVertices() const {
		return m_iNumVertices;
	}

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	VTXTEX*			Vertex_Map();
	void			Vertex_Unmap();

public:
	static CVIBuffer_RectTrail* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END