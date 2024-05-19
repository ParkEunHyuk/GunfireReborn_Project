#pragma once

#include "Client_Defines.h"
#include "UI.h"
#include "UI_HpBar.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUI_Stamina final : public CUI
{
private:
	explicit CUI_Stamina(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUI_Stamina(const CUI_Stamina& rhs);
	virtual ~CUI_Stamina() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int	Tick(_double TimeDelta);
	virtual _int	LateTick(_double TimeDelta);
	virtual HRESULT Render();


private:
	HRESULT			SetUp_Components();
	HRESULT			SetUp_ConstantTable();

private:
	_bool			Once_Trigger();

private:
	CShader*			m_pShaderCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

private:
	_float4x4			m_ProjMatrix;
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;

private:
	CUI_HpBar*			m_pHpBar = nullptr;
	_uint				m_iTextureNumber = 0;

	//기타 외 기능
private:
	_bool					m_bOnce_Switch = false;


public:
	static CUI_Stamina* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END