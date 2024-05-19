#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{

private:
	CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;

public:
	HRESULT SetUp_OnShader(class CShader* pShader, const char* pValueName, _uint iTextureIndex = 0);
	HRESULT	SetUp_OnShader_AutoFrame(class CShader* pShader, const char* pValueName, _double DeltaTime,_uint iMaxTexture ,_float* pOutFrameCount = nullptr);

public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pTexturePath, _uint iNumTextures);
	virtual HRESULT NativeConstruct(void* pArg);

private:
	vector<ID3D11ShaderResourceView*>				m_Textures;
	typedef vector<ID3D11ShaderResourceView*>		TEXTURES;

	_uint						m_iNumMaxTexture = 0;
	_double						m_dFrameTime = 0;
	_double						m_dFramePerSec = 6.0;

public:
	static CTexture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pTextureFilePath, _uint iNumTextures = 1);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END