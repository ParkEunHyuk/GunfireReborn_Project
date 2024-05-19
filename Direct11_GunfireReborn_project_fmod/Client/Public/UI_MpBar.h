#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUI_MpBar final : public CUI
{
private:
	explicit CUI_MpBar(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUI_MpBar(const CUI_MpBar& rhs);
	virtual ~CUI_MpBar() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();


	_bool	Use_Mp(_float _fMpBarPercent); //Mp를 사용할 수 있는지 없는지를 체크

public:
	void	SetUp_MpBar(_float _fMpBarPercent); //사용한 마나량을 바꾸기 위한 함수

private:
	CShader*			m_pShaderCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

private:
	_float4x4			m_ProjMatrix;

	_float4				m_fRect;
	_float4				m_fTempRect;


private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();


public:
	static CUI_MpBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END