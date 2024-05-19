#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CPipeLine;
END

BEGIN(Client)

class CUI_DotSight final : public CUI
{
private:
	explicit CUI_DotSight(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUI_DotSight(const CUI_DotSight& rhs);
	virtual ~CUI_DotSight() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

	HRESULT	Mouse_Input(_double TimeDelta);

private:
	CShader*			m_pShaderCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

private:
	_float4x4			m_ProjMatrix;


	_float4				m_fRect;

	_double				m_dTimer;

	_uint				m_iFrameKey = 0;


private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();


public:
	static CUI_DotSight* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END