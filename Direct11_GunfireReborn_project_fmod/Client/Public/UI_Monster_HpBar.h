#pragma once

#include "Client_Defines.h"
#include "UI.h"
#include "LandObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUI_Monster_HpBar final : public CUI
{
private:
	explicit CUI_Monster_HpBar(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUI_Monster_HpBar(const CUI_Monster_HpBar& rhs);
	virtual ~CUI_Monster_HpBar() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

	HRESULT	SetUp_Pos(_double TimeDelta);
	void SetUp_Hp();


private:
	CShader*			m_pShaderCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

private:
	_float4x4			m_ViewMatrix;
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;

	_float				m_fTickHp = 0;
	_float				m_fDecreaseHp = 0;

	CLandObject::INFODESC m_Monster_Info;


	CLandObject*		m_pMonsterObject = nullptr;
	class CTransform*	m_pMonsterTransform = nullptr;
	class CTransform*	m_pPlayerTransform = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();


public:
	static CUI_Monster_HpBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END